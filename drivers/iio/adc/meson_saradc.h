/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/iio/iio.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/regulator/consumer.h>

enum meson_sar_adc_vref_sel {
	CALIB_VOL_AS_VREF = 0,
	VDDA_AS_VREF = 1,
};

enum meson_sar_adc_test_input_sel {
	TEST_MUX_VSS = 0x0,
	TEST_MUX_VDD_DIV4 = 0x1,
	TEST_MUX_VDD_DIV2 = 0x2,
	TEST_MUX_VDD_MUL3_DIV4 = 0x3,
	TEST_MUX_VDD = 0x4,
	TEST_MUX_CHANN_INPUT = 0x7,
};

enum meson_sar_adc_sampling_mode {
	SINGLE_MODE,
	PERIOD_MODE,
	MAX_MODE,
};

struct meson_sar_adc_diff_ops {
	int (*extra_init)(struct iio_dev *indio_dev);
	void (*set_test_input)(struct iio_dev *indio_dev,
			       enum meson_sar_adc_test_input_sel sel);
	int (*read_fifo)(struct iio_dev *indio_dev,
			 const struct iio_chan_spec *chan, bool chk_channel);
	void (*enable_chnl)(struct iio_dev *indio_dev, bool en);
	int (*read_chnl)(struct iio_dev *indio_dev,
			 const struct iio_chan_spec *chan);
	void (*set_bandgap)(struct iio_dev *indio_dev, bool on_off);
	void (*select_temp)(struct iio_dev *indio_dev,
			    const struct iio_chan_spec *chan);
	void (*init_ch)(struct iio_dev *indio_dev,
			const struct iio_chan_spec *chan);
	void (*enable_decim_filter)(struct iio_dev *indio_dev,
				    const struct iio_chan_spec *chan, bool en);
	int (*tuning_clock)(struct iio_dev *indio_dev,
			    enum meson_sar_adc_sampling_mode mode);
};

struct meson_sar_adc_calib {
	enum meson_sar_adc_test_input_sel test_upper;
	enum meson_sar_adc_test_input_sel test_lower;
	int test_channel;
};

/* struct meson_sar_adc_param - describe the differences of different platform
 *
 * @has_bl30_integration: g12a and later SoCs don not use saradc to sample
 * internal temp sensor in bl30
 *
 * @resolution: gxl and later: 12bit; others(gxtvbb etc): 10bit
 *
 * @vref_is_optional: txlx and later SoCs support VDDA or Calibration Voltage
 * as vref, but others support only Calibration Voltage
 *
 * @disable_ring_counter: gxl and later SoCs write 1 to disable continuous ring
 * counter, but others write 0
 *
 * @has_chnl_regs: enable period sampling mode when the SoCs contain chnl regs
 *
 * @vrefp_select: g12a and later SoCs must write 0, others SoC write 1
 *
 * @vcm_select: g12a and later SoCs must write 0, others SoC write 1
 *
 * @adc_eoc: g12a and later SoCs must write 1, gxlx3/txhd2 must write 0
 *
 * @calib_enable: enable sw calibration, TXL and before SoCs must write true,
 * other SoCs are optional.
 */
struct meson_sar_adc_param {
	u8					has_bl30_integration;
	unsigned long				clock_rate;
	u32                                     bandgap_reg;
	u32					bandgap_en_mask;
	unsigned int				resolution;
	const struct regmap_config		*regmap_config;
	u8					temperature_trimming_bits;
	unsigned int				temperature_multiplier;
	unsigned int				temperature_divider;
	const struct meson_sar_adc_diff_ops	*dops;
	const struct iio_chan_spec		*channels;
	u8					num_channels;
	u8					vref_is_optional;
	u8					disable_ring_counter;
	u8					has_chnl_regs;
	u8					vrefp_select;
	u8					vcm_select;
	u8					adc_eoc;
	const struct meson_sar_adc_calib	*calib;
};

struct meson_sar_adc_priv {
	struct regmap				*regmap;
	struct regulator			*vref;
	const struct meson_sar_adc_param	*param;
	struct clk				*clkin;
	struct clk				*core_clk;
	struct clk				*adc_sel_clk;
	struct clk				*adc_clk;
	struct clk_gate				clk_gate;
	struct clk				*adc_div_clk;
	struct clk_divider			clk_div;
	int					calibbias;
	int					calibscale;
	u8					temperature_sensor_calibrated;
	u8					temperature_sensor_coefficient;
	u16					temperature_sensor_adc_val;
	int					delay_per_tick;
	int					ticks_per_period;
	int					active_channel_cnt;
	u8					*datum_buf;
	u8					test_input_sel;
	u32					continuous_sample_count;
	struct completion			done;
	u32					*continuous_sample_buffer;
	u8					calib_ready;
};

extern const struct meson_sar_adc_param meson_sar_adc_meson8_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_meson8b_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_gxbb_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_gxl_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_txlx_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_g12a_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_c2_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_txhd2_param __initconst;
extern const struct meson_sar_adc_param meson_sar_adc_s7_param __initconst;
