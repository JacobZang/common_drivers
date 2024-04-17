// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/device.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/pm_opp.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/cpumask.h>
#include <linux/mutex.h>
#include <linux/of_platform.h>
#include <linux/topology.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/regulator/driver.h>
#include "opp.h"
#include "aml_cpufreq_v2.h"
#include <linux/energy_model.h>
#include <linux/cpu_cooling.h>
#include <linux/thermal.h>
#include <linux/arm-smccc.h>
#include <linux/proc_fs.h>
#include <linux/amlogic/gki_module.h>

static LIST_HEAD(cluster_list);
static DEFINE_MUTEX(cluster_list_lock);
static DEFINE_MUTEX(cpufreq_target_lock);

static unsigned int freqmax[CLUSTER_MAX];
static int freqmax0_param_v2(char *buff)
{
	if (!buff)
		return -EINVAL;
	/*Example: freqmax0=1800000*/
	if (kstrtoint(buff, 0, &freqmax[0]))
		return -EINVAL;

	return 0;
}

static int freqmax1_param_v2(char *buff)
{
	if (!buff)
		return -EINVAL;
	/*Example: freqmax1=1800000*/
	if (kstrtoint(buff, 0, &freqmax[1]))
		return -EINVAL;

	return 0;
}

__setup("freqmax0=", freqmax0_param_v2);
__setup("freqmax1=", freqmax1_param_v2);

static struct cluster_data *find_cluster_data_by_cpu(int cpu)
{
	struct cluster_data *pos, *ret = NULL;

	mutex_lock(&cluster_list_lock);
	list_for_each_entry(pos, &cluster_list, node) {
		if (cpumask_test_cpu(cpu, pos->cpus)) {
			ret = pos;
			break;
		}
	}
	mutex_unlock(&cluster_list_lock);

	return ret;
}

static int aml_cpufreq_verify(struct cpufreq_policy_data *pd)
{
	int ret;
	unsigned int maxfreq = pd->cpu ? freqmax[1] : freqmax[0];

	ret = cpufreq_generic_frequency_table_verify(pd);
	if (ret)
		return ret;

	if (maxfreq && maxfreq >= pd->min && maxfreq < pd->max) {
		pr_info("policy%d: set policy->max to %u from bootargs\n", pd->cpu, maxfreq);
		pd->max = maxfreq;
	}
	return ret;
}

/* find the dsu opp state:
 * i=-1: follow cpu freq volt
 * i=0: use dsu_opp_table[0]; i=1: use dsu_opp_table[1]; ......
 */
static void get_suggested_dsuopp_by_cpuopp(struct cluster_data *data,
	struct opp_node *dsuopp, struct opp_node *cpuopp)
{
	int cnt = data->dsu_opp_cnt, i;
	struct cpudsu_threshold_node *table = data->dsu_opp_table;

	if (!data->dsuclk)
		return;
	for (i = cnt; i > 0; i--)
		if (cpuopp->rate > table[i - 1].cpurate)
			break;
	i--;

	if (i < 0) {
		dsuopp->rate = cpuopp->rate;
		dsuopp->volt = cpuopp->volt;
	} else {
		dsuopp->rate = table[i].dsurate;
		dsuopp->volt = table[i].dsuvolt;
	}
}

static void aml_dsu_freq_volt_set(struct cluster_data *cdata,
	struct opp_node *dsuopp, enum cpufreq_scaling_mode mode)
{
	if (!cdata->dsuclk)
		return;
	if (cdata->dsureg && mode == SCALING_UP)
		regulator_set_voltage(cdata->dsureg, dsuopp->volt, MAX_VOLTAGE);

	if (cdata->dsuclk_shared)
		clk_set_min_rate(cdata->dsuclk, dsuopp->rate * 1000);
	clk_set_rate(cdata->dsuclk, dsuopp->rate * 1000);

	if (cdata->dsureg && mode == SCALING_DOWN)
		regulator_set_voltage(cdata->dsureg, dsuopp->volt, MAX_VOLTAGE);
}

static int aml_dvfs_algorithm_default(struct cluster_data *data,
					unsigned int index)
{
	struct opp_node oppold, *oppnew;
	struct opp_node dsuopp;
	int ret = 0;

	oppold.rate = clk_get_rate(data->cpuclk) / 1000;
	oppold.volt = data->cpureg ? regulator_get_voltage(data->cpureg) : 0;
	oppnew = &data->opp_table[index];
	pr_debug("[cluster%d]scaling from[%lu %lu] to [%lu %lu]\n", data->clusterid, oppold.rate,
		oppold.volt, oppnew->rate, oppnew->volt);

	get_suggested_dsuopp_by_cpuopp(data, &dsuopp, oppnew);

	if (oppnew->rate > oppold.rate) {
		if (data->cpureg && regulator_set_voltage(data->cpureg, oppnew->volt,
			MAX_VOLTAGE)) {
			pr_err("scale voltage up to %luuv failed. ret=%d\n", oppnew->volt, ret);
			goto out;
		}
		aml_dsu_freq_volt_set(data, &dsuopp, SCALING_UP);
	}

	ret = clk_set_rate(data->cpuclk, oppnew->rate * 1000);
	if (ret) {
		if (oppnew->rate > oppold.rate) {
			get_suggested_dsuopp_by_cpuopp(data, &dsuopp, &oppold);
			aml_dsu_freq_volt_set(data, &dsuopp, SCALING_DOWN);
			if (data->cpureg)
				regulator_set_voltage(data->cpureg, oppold.volt, MAX_VOLTAGE);
		}
		goto out;
	}
	if (oppnew->rate < oppold.rate) {
		aml_dsu_freq_volt_set(data, &dsuopp, SCALING_DOWN);
		if (data->cpureg && regulator_set_voltage(data->cpureg, oppnew->volt, MAX_VOLTAGE))
			goto out;
	}
	pr_debug("[cluster%d]cpuget[%lu %d],dsuget[%lu %d]\n",
		data->clusterid, clk_get_rate(data->cpuclk) / 1000,
		data->cpureg ? regulator_get_voltage(data->cpureg) : 0,
		clk_get_rate(data->dsuclk) / 1000, data->dsureg ?
		regulator_get_voltage(data->dsureg) : 0);
out:
	return ret;
}

static int aml_cpufreq_set_target(struct cpufreq_policy *policy,
				    unsigned int index)
{
	struct cluster_data *data = policy->driver_data;
	int ret;

	mutex_lock(&cpufreq_target_lock);
	ret = aml_dvfs_algorithm_default(data, index);
	mutex_unlock(&cpufreq_target_lock);

	return ret;
}

static unsigned int aml_cpufreq_get_rate(unsigned int cpu)
{
	struct cpufreq_policy policy;
	struct cluster_data *data;
	u32 rate = 0;

	if (!cpufreq_get_policy(&policy, cpu)) {
		data = policy.driver_data;
		rate = clk_get_rate(data->cpuclk) / 1000;
		pr_debug("%s: cpu: %d, cluster: %d, freq: %u\n",
			 __func__, cpu, data->clusterid, rate);
	}
	return rate;
}

static void free_clock(struct clk *clock)
{
	if (!IS_ERR_OR_NULL(clock))
		clk_put(clock);
}

static void free_regulator(struct regulator *reg)
{
	if (!IS_ERR_OR_NULL(reg))
		devm_regulator_put(reg);
}

static bool aml_setup_clock(struct cluster_data *cdata)
{
	bool ret = false;

	cdata->cpuclk = of_clk_get_by_name(cdata->np, CPUCLK);
	if (PTR_ERR_OR_ZERO(cdata->cpuclk))
		goto out;
	cdata->dsuclk = of_clk_get_by_name(cdata->np, DSUCLK);
	if (PTR_ERR_OR_ZERO(cdata->dsuclk))
		cdata->dsuclk = NULL;
	if (!cdata->dsuclk && of_property_count_strings(cdata->np, "clock-names") == 2)
		ret = false;
	else
		ret = true;

	if (cdata->dsuclk)
		cdata->dsuclk_shared = of_property_read_bool(cdata->np, "dsuclk_shared");
out:
	return ret;
}

static bool aml_setup_regulator(struct cluster_data *cdata)
{
	bool ret = false;
	char regname[25] = {0};

	cdata->skip_volt_scaling = of_property_read_bool(cdata->np, "skip_volt_scaling");
	if (cdata->skip_volt_scaling) {
		cdata->cpureg = NULL;
		cdata->dsureg = NULL;
		ret = true;
		goto out;
	}
	sprintf(regname, "cluster%d-cpu", cdata->clusterid);
	cdata->cpureg = devm_regulator_get_optional(cdata->dev, regname);
	if (PTR_ERR_OR_ZERO(cdata->cpureg)) {
		pr_debug("%s:failed to get %s regulator, ret=%ld\n", __func__, regname,
		       PTR_ERR(cdata->cpureg));
		cdata->cpureg = NULL;
		goto out;
	}
	sprintf(regname, "cluster%d-dsu", cdata->clusterid);
	cdata->dsureg = devm_regulator_get_optional(cdata->dev, regname);
	if (PTR_ERR_OR_ZERO(cdata->dsureg)) {
		pr_debug("%s:failed to get %s regulator, ret=%ld\n", __func__, regname,
		       PTR_ERR(cdata->dsureg));
		cdata->dsureg = NULL;
		sprintf(regname, "cluster%d-dsu-supply", cdata->clusterid);
		if (!cdata->dsureg && of_find_property(cdata->dev->of_node, regname, NULL))
			goto out;
	}
	ret = true;
out:
	return ret;
}

static unsigned int get_cpufreq_table_index(u64 function_id,
					    u64 arg0, u64 arg1, u64 arg2)
{
	struct arm_smccc_res res;

	arm_smccc_smc((unsigned long)function_id,
		      (unsigned long)arg0,
		      (unsigned long)arg1,
		      (unsigned long)arg2,
		      0, 0, 0, 0, &res);
	return res.a0;
}

static int get_opp_index_of_policy_exit(struct cluster_data *data)
{
	struct opp_node *opp_table = data->opp_table;
	int i;

	for (i = 0; i < data->opp_cnt; i++) {
		if (opp_table[i].volt != opp_table[0].volt)
			break;
	}

	return i > 0 ? i - 1 : i;
}

static bool aml_setup_opptable(struct cluster_data *data)
{
	struct device_node *np;
	struct device *cpudev;
	struct opp_table *opptable;
	struct dev_pm_opp *opp;
	bool ret = false;
	int table_index = 0, i = 0, cpu;
	u32 rate, rate1, volt;
	char dsutable_name[20] = {0};

	np = data->np;
	data->pdvfs_enabled = of_property_read_bool(np, "pdvfs_enabled");
	if (data->pdvfs_enabled)
		table_index = get_cpufreq_table_index(GET_DVFS_TABLE_INDEX,
					      data->clusterid, 0, 0);
	data->table_index = table_index;

	/*one of the cluster cpus may be removed from device tree for isolation,
	 *so we need to get opp-table from other cpus of the cluster
	 */
	for_each_cpu(cpu, data->cpus) {
		cpudev = get_cpu_device(cpu);
		if (dev_pm_opp_of_add_table_indexed(cpudev, table_index))
			continue;
		else
			break;
	}

	if (dev_pm_opp_init_cpufreq_table(cpudev, &data->freq_table)) {
		pr_err("%s:failed to init cpufreq table,cluster:%d\n", __func__, data->clusterid);
		goto out;
	}
	data->opp_cnt = dev_pm_opp_get_opp_count(cpudev);
	data->opp_table = kmalloc_array(data->opp_cnt, sizeof(*data->opp_table), GFP_KERNEL);
	opptable = dev_pm_opp_get_opp_table(cpudev);
	if (!data->opp_table || !opptable)
		goto out;
	mutex_lock(&opptable->lock);
	list_for_each_entry(opp, &opptable->opp_list, node) {
		if (!opp->available)
			continue;
		pr_debug("%lu %lu\n", opp->rate, opp->supplies[0].u_volt);
		data->opp_table[i].rate = opp->rate / 1000;
		data->opp_table[i].volt = opp->supplies[0].u_volt;
		i++;
	}
	dev_pm_opp_put_opp_table(opptable);
	mutex_unlock(&opptable->lock);
	data->exit_index = get_opp_index_of_policy_exit(data);

	if (!data->dsuclk)
		return true;

	sprintf(dsutable_name, "dsu-opp-table%d", data->table_index);
	data->dsu_opp_cnt = of_property_count_u32_elems(np, dsutable_name) / 3;
	if (data->dsu_opp_cnt <= 0)
		goto out;
	data->dsu_opp_table = kmalloc_array(data->dsu_opp_cnt,
		sizeof(*data->dsu_opp_table), GFP_KERNEL);
	if (IS_ERR(data->dsu_opp_table)) {
		pr_err("failed to alloc dsu_opp_table.\n");
		data->dsu_opp_table = NULL;
		goto out;
	}
	for (i = 0; i < data->dsu_opp_cnt; i++) {
		of_property_read_u32_index(np, dsutable_name, 3 * i, &rate);
		of_property_read_u32_index(np, dsutable_name, 3 * i + 1, &rate1);
		of_property_read_u32_index(np, dsutable_name, 3 * i + 2, &volt);
		data->dsu_opp_table[i].cpurate = rate;
		data->dsu_opp_table[i].dsurate = rate1;
		data->dsu_opp_table[i].dsuvolt = volt;
	}

	return true;
out:
	return ret;
}

static int opptable_show(struct seq_file *m, void *v)
{
	struct cluster_data *data = m->private;
	int i;

	seq_printf(m, "pdvfs_en: %d\n", data->pdvfs_enabled);
	seq_printf(m, "table_index: %d\n", data->table_index);

	seq_puts(m, "opp table:\n");
	for (i = 0; i < data->opp_cnt; i++)
		seq_printf(m, "%lu %lu\n", data->opp_table[i].rate, data->opp_table[i].volt);
	seq_puts(m, "dsu_opp table:\n");
	for (i = 0; i < data->dsu_opp_cnt; i++)
		seq_printf(m, "%d %d %d\n", data->dsu_opp_table[i].cpurate,
			data->dsu_opp_table[i].dsurate, data->dsu_opp_table[i].dsuvolt);
	return 0;
}

static int aml_opptable_open(struct inode *inode, struct file *file)
{
	return single_open(file, opptable_show, PDE_DATA(inode));
}

static const struct proc_ops aml_opptable_fops = {
	.proc_open = aml_opptable_open,
	.proc_read = seq_read,
	.proc_release = single_release,
};

static void aml_create_proc_files(struct cluster_data *data)
{
	char policy_name[10] = {0};

	snprintf(policy_name, sizeof(policy_name), "policy%u", cpumask_first(data->cpus));
	data->proccur = proc_mkdir(policy_name, data->procroot);
	if (data->proccur)
		proc_create_data("opp-table", 0444, data->proccur,
			&aml_opptable_fops, data);
}

static void aml_validate_cpu_voltage(struct cluster_data *data)
{
	int cnt = data->opp_cnt;
	int dsu_cnt = data->dsu_opp_cnt;
	unsigned long max = data->opp_table[cnt - 1].volt;
	unsigned long min = data->opp_table[0].volt;
	int cpuvolt_old, dsuvolt_old;

	if (data->skip_volt_scaling)
		return;
	cpuvolt_old = regulator_get_voltage(data->cpureg);
	if (cpuvolt_old > max)
		cpuvolt_old = max;
	else if (cpuvolt_old < min)
		cpuvolt_old = min;

	regulator_set_voltage(data->cpureg, cpuvolt_old, MAX_VOLTAGE);

	if (!data->dsureg)
		return;
	dsuvolt_old = regulator_get_voltage(data->dsureg);
	if (dsuvolt_old > data->dsu_opp_table[dsu_cnt - 1].dsuvolt)
		dsuvolt_old = data->dsu_opp_table[dsu_cnt - 1].dsuvolt;
	else if (dsuvolt_old < min)
		dsuvolt_old = min;

	regulator_set_voltage(data->dsureg, dsuvolt_old, MAX_VOLTAGE);
}

static void aml_setup_policy_rest(struct cluster_data *data)
{
	struct device_node *np = data->np;
	struct cpufreq_policy *policy = data->policy;

	if (of_property_read_u32(np, "clock-latency", &policy->cpuinfo.transition_latency))
		policy->cpuinfo.transition_latency = CPUFREQ_ETERNAL;
	if (of_property_read_u32(np, "suspend-freq", &policy->suspend_freq))
		policy->suspend_freq = data->opp_table[data->opp_cnt - 1].rate;

	policy->cur = clk_get_rate(data->cpuclk) / 1000;
	cpumask_copy(policy->cpus, data->cpus);
}

static int aml_cpufreq_init(struct cpufreq_policy *policy)
{
	struct cluster_data *data = find_cluster_data_by_cpu(policy->cpu);

	data->policy = policy;
	policy->driver_data = data;
	policy->freq_table = data->freq_table;

	aml_create_proc_files(data);
	aml_setup_policy_rest(data);
	pr_info("cpufreq v2 policy%d init succeed.\n", cpumask_first(data->cpus));

	return 0;
}

static void destroy_aml_cpufreq_proc_files(struct cpufreq_policy *policy)
{
	char policy_name[10] = {0};
	struct cluster_data *data = policy->driver_data;

	snprintf(policy_name, sizeof(policy_name), "policy%u", cpumask_first(data->cpus));
	remove_proc_entry("opp-table", data->proccur);
	remove_proc_entry(policy_name, data->procroot);
}

static int aml_cpufreq_exit(struct cpufreq_policy *policy)
{
	struct cluster_data *data = policy->driver_data;

	if (!data->suspended)
		aml_cpufreq_set_target(policy, data->exit_index);
	destroy_aml_cpufreq_proc_files(policy);

	return 0;
}

static int aml_cpufreq_suspend(struct cpufreq_policy *policy)
{
	struct cluster_data *data = policy->driver_data;

	data->suspended = true;
	if (policy->cdev)
		dev_set_uevent_suppress(&policy->cdev->device, true);

	return cpufreq_generic_suspend(policy);
}

static int aml_cpufreq_resume(struct cpufreq_policy *policy)
{
	struct cluster_data *data = policy->driver_data;

	data->suspended = false;
	if (policy->cdev)
		dev_set_uevent_suppress(&policy->cdev->device, false);

	return cpufreq_generic_suspend(policy);
}

static struct cpufreq_driver aml_cpufreq_driver = {
	.name			= "aml-cpufreq",
	.flags			= CPUFREQ_IS_COOLING_DEV |
					CPUFREQ_HAVE_GOVERNOR_PER_POLICY |
					CPUFREQ_NEED_INITIAL_FREQ_CHECK,
	.verify			= aml_cpufreq_verify,
	.target_index	= aml_cpufreq_set_target,
	.get			= aml_cpufreq_get_rate,
	.init			= aml_cpufreq_init,
	.exit			= aml_cpufreq_exit,
	.attr			= cpufreq_generic_attr,
	.suspend		= aml_cpufreq_suspend,
	.resume			= aml_cpufreq_resume,
	.register_em	= cpufreq_register_em_with_opp,
};

static int aml_cpuclk_ready(struct device_node *np)
{
	int ret;
	struct clk *cpuclk;

	cpuclk = of_clk_get_by_name(np, CPUCLK);
	ret = PTR_ERR_OR_ZERO(cpuclk);
	if (ret == -EPROBE_DEFER)
		pr_err("cpu clock not ready, retry!\n");
	else if (ret)
		pr_err("failed to get clock:%d!\n", ret);
	else
		clk_put(cpuclk);

	return ret;
}

static int aml_cpureg_ready(struct device *dev, struct device_node *np, int clusterid)
{
	int ret;
	struct regulator *cpureg;
	char cpureg_name[15] = {0};

	if (of_property_read_bool(np, "skip_volt_scaling"))
		return 0;
	sprintf(cpureg_name, "cluster%d-cpu", clusterid);
	cpureg = devm_regulator_get_optional(dev, cpureg_name);
	ret = PTR_ERR_OR_ZERO(cpureg);
	if (ret == -EPROBE_DEFER)
		pr_err("cpu regulator not ready, retry!\n");
	else if (ret)
		pr_err("failed to get cpu regulator:%d!\n", ret);
	else
		devm_regulator_put(cpureg);

	return ret;
}

static int aml_cpufreq_probe(struct platform_device *pdev)
{
	struct proc_dir_entry *root = NULL;
	struct device_node *cluster_np;
	struct cluster_data *clusterdata;
	char cluster_name[10] = {0};
	int ret = -1, i = 0, j, count;
	struct property *prop;
	const __be32 *cur;

	while (1) {
		sprintf(cluster_name, "cluster%d", i);
		cluster_np = of_get_child_by_name(pdev->dev.of_node, cluster_name);
		if (!cluster_np)
			break;

		ret = aml_cpuclk_ready(cluster_np);
		if (ret)
			return ret;

		ret = aml_cpureg_ready(&pdev->dev, cluster_np, i);
		if (ret)
			return ret;
		i++;
	}
	count = i;
	if (count == 0)
		goto out;

	clusterdata = devm_kmalloc(&pdev->dev, sizeof(*clusterdata) * count, GFP_KERNEL);
	if (!clusterdata)
		goto out;

	root = proc_mkdir("aml_cpufreq", NULL);
	if (!root)
		goto out;

	for (i = 0; i < count; i++) {
		sprintf(cluster_name, "cluster%d", i);
		cluster_np = of_get_child_by_name(pdev->dev.of_node, cluster_name);

		clusterdata[i].clusterid = i;
		clusterdata[i].np = cluster_np;
		clusterdata[i].dev = &pdev->dev;
		clusterdata[i].procroot = root;
		/*setup cluster related cpus*/
		of_property_for_each_u32(cluster_np, "cluster_cores", prop, cur, j) {
			cpumask_set_cpu(j, clusterdata[i].cpus);
			pr_info("cpu%d->cluster%d\n", j, clusterdata[i].clusterid);
		}

		if (!aml_setup_clock(&clusterdata[i]))
			goto free;

		if (!aml_setup_regulator(&clusterdata[i]))
			goto free;

		if (!aml_setup_opptable(&clusterdata[i]))
			goto free;

		aml_validate_cpu_voltage(&clusterdata[i]);

		mutex_lock(&cluster_list_lock);
		list_add_tail(&clusterdata[i].node, &cluster_list);
		mutex_unlock(&cluster_list_lock);
	}

	ret = cpufreq_register_driver(&aml_cpufreq_driver);
	if (ret)
		pr_err("%s: Failed to register platform driver, err: %d\n", __func__, ret);
	else
		goto out;
free:
	for (i = 0; i < count; i++) {
		kfree(clusterdata[i].opp_table);
		kfree(clusterdata[i].dsu_opp_table);
		free_regulator(clusterdata[i].cpureg);
		free_regulator(clusterdata[i].dsureg);
		free_clock(clusterdata[i].cpuclk);
		free_clock(clusterdata[i].dsuclk);
	}
	kfree(clusterdata);
	proc_remove(root);
out:
	return ret;
}

static int aml_cpufreq_remove(struct platform_device *pdev)
{
	return cpufreq_unregister_driver(&aml_cpufreq_driver);
}

static const struct of_device_id aml_cpufreq_dt_match[] = {
	{	.compatible = "amlogic, aml-cpufreq-v2",
	},
	{},
};

static struct platform_driver aml_cpufreq_platdrv = {
	.driver = {
		.name	= "cpufreq-aml",
		.owner  = THIS_MODULE,
		.of_match_table = aml_cpufreq_dt_match,
	},
	.probe		= aml_cpufreq_probe,
	.remove		= aml_cpufreq_remove,
};

int __init aml_cpufreq_v2_init(void)
{
	return platform_driver_register(&aml_cpufreq_platdrv);
}

void __exit aml_cpufreq_v2_exit(void)
{
	platform_driver_unregister(&aml_cpufreq_platdrv);
}
