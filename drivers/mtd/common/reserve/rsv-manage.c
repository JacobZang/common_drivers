// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>
//#include <linux/mtd/nand_ecc.h>
#include <linux/amlogic/aml_rsv.h>
#include <linux/amlogic/aml_mtd_nand.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

char *aml_nand_get_rsv_cmdline(void);

/* protect flag inside */
static int rsv_protect = 1;
static struct meson_rsv_block_t rsv_blk_cnt[NAND_RSV_END_INDEX] = {
	{"rsv_block_num", DEFAULT_NAND_RSV_BLOCK_NUM, 0, NAND_RSV_INDEX},
	{"rsv_gap_block_num", DEFAULT_NAND_GAP_BLOCK_NUM, 0, NAND_GAP_INDEX},
	{"rsv_bbt_block_num", DEFAULT_NAND_BBT_BLOCK_NUM, 0, NAND_BBT_INDEX},
	{"rsv_env_block_num", DEFAULT_NAND_ENV_BLOCK_NUM, 0, NAND_ENV_INDEX},
	{"rsv_key_block_num", DEFAULT_NAND_KEY_BLOCK_NUM, 0, NAND_KEY_INDEX},
	{"rsv_dtb_block_num", DEFAULT_NAND_DTB_BLOCK_NUM, 0, NAND_DTB_INDEX},
	{"rsv_ddr_block_num", DEFAULT_NAND_DDR_BLOCK_NUM, 0, NAND_DDR_INDEX},
};

u32 meson_rsv_get_block_cnt(enum meson_rsv_blk_cnt index)
{
	return rsv_blk_cnt[index].block_cnt;
}
EXPORT_SYMBOL(meson_rsv_get_block_cnt);

int meson_rsv_prase_parameter_from_dtb(struct mtd_info *mtd)
{
	struct device_node *node = mtd->dev.parent->of_node;
	u32 i, ret = 0;

	for (i = 0; i < NAND_RSV_END_INDEX; i++) {
		ret = of_property_read_u32(node, rsv_blk_cnt[i].para_rsv_name,
					   &rsv_blk_cnt[i].block_cnt);
		if (ret) {
			pr_err("%s %d,please config para item %s in dts\n",
				__func__, __LINE__, rsv_blk_cnt[i].para_rsv_name);
			return ret;
		}
	}

	return ret;
}
EXPORT_SYMBOL(meson_rsv_prase_parameter_from_dtb);

int meson_rsv_prase_parameter_from_cmdline(struct mtd_info *mtd)
{
	u32 i, start, end, size;
	char *p;

	p = aml_nand_get_rsv_cmdline();
	if (!p)
		goto error;

	p = strchr(p, ':');
	if (!p)
		goto error;

	p++;
	for (i = 0; i < NAND_RSV_END_INDEX; i++) {
		start = memparse(p, NULL);
		p = strchr(p, '@');
		if (!p)
			goto error;
		p++;
		end = memparse(p, NULL);
		p = strchr(p, '@');
		if (!p)
			goto error;
		p++;
		size = memparse(p, NULL);
		rsv_blk_cnt[i].block_cnt = (end - start) / mtd->erasesize;
		rsv_blk_cnt[i].size = size;
		p = strchr(p, ',');
		if (!p)
			goto error;
		p++;
	}

	return 0;
error:
	pr_err("%s %d: rsv cmdline is invalid!\n", __func__, __LINE__);
	return -EINVAL;
}
EXPORT_SYMBOL(meson_rsv_prase_parameter_from_cmdline);

int meson_rsv_prase_parameter_init(struct mtd_info *mtd)
{
	struct device_node *node = mtd->dev.parent->of_node;
	u32 ret = 0;

	ret = of_property_read_bool(node, "rsv_dts_config");
	if (ret) {
		pr_err("%s %d: init rsv from dts!\n", __func__, __LINE__);
		return meson_rsv_prase_parameter_from_dtb(mtd);
	}

	ret = of_property_read_bool(node, "rsv_env_config");
	if (ret) {
		pr_err("%s %d: init rsv from cmdline!\n", __func__, __LINE__);
		return meson_rsv_prase_parameter_from_cmdline(mtd);
	}

	/* default rsv */
	return ret;
}
EXPORT_SYMBOL(meson_rsv_prase_parameter_init);

static inline void _aml_rsv_disprotect(void)
{
	rsv_protect = 0;
}

static inline void _aml_rsv_protect(void)
{
	rsv_protect = 1;
}

static inline int _aml_rsv_isprotect(void)
{
	return rsv_protect;
}

struct meson_rsv_handler_t *rsv_handler;

static struct free_node_t *get_free_node(struct meson_rsv_info_t *rsv_info)
{
	struct meson_rsv_handler_t *handler = rsv_info->handler;
	unsigned long index;

	pr_debug("%s %d: bitmap=%llx\n", __func__, __LINE__,
		handler->freenodebitmask);

	index = find_first_zero_bit((void *)&handler->freenodebitmask,
			rsv_blk_cnt[NAND_RSV_INDEX].block_cnt);
	if (index >= rsv_blk_cnt[NAND_RSV_INDEX].block_cnt) {
		pr_info("%s %d: index is greater than max! error",
			__func__, __LINE__);
		return NULL;
	}
	WARN_ON(test_and_set_bit(index, (void *)&handler->freenodebitmask));

	pr_debug("%s %d: bitmap=%llx\n", __func__, __LINE__,
		handler->freenodebitmask);

	return handler->free_node[index];
}

static void release_free_node(struct meson_rsv_info_t *rsv_info,
			      struct free_node_t *free_node)
{
	struct meson_rsv_handler_t *handler = rsv_info->handler;
	unsigned int index_save = free_node->index;

	pr_info("%s %d: bitmap=%llx\n", __func__, __LINE__,
		handler->freenodebitmask);

	if (index_save > rsv_blk_cnt[NAND_RSV_INDEX].block_cnt) {
		pr_info("%s %d: index=%d is greater than max! error",
			__func__, __LINE__, index_save);
		return;
	}

	WARN_ON(!test_and_clear_bit(index_save,
				    (void *)&handler->freenodebitmask));

	/*memset zero to protect from dead-loop*/
	memset(free_node, 0, sizeof(struct free_node_t));
	free_node->index = index_save;
	pr_info("%s %d: bitmap=%llx\n", __func__, __LINE__,
		handler->freenodebitmask);
}

int meson_rsv_erase_protect(struct meson_rsv_handler_t *handler,
			    unsigned int block_addr)
{
	if (!_aml_rsv_isprotect())
		return 0;

	if (handler->bbt && handler->bbt->valid)
		if (block_addr >= handler->bbt->start_block &&
		    block_addr < handler->bbt->end_block)
			return -1;/*need skip bbt blocks*/

	if (handler->key && handler->key->valid)
		if (block_addr >= handler->key->start_block &&
		    block_addr < handler->key->end_block)
			return -1; /*need skip key blocks*/

	return 0;
}

int meson_free_rsv_info(struct meson_rsv_info_t *rsv_info)
{
	struct mtd_info *mtd;
	struct erase_info erase_info;
	struct free_node_t *tmp_node, *next_node = NULL;
	struct meson_rsv_ops *rsv_ops = &rsv_handler->rsv_ops;
	int error = 0;
	loff_t addr = 0;

	if (!rsv_info) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	mtd = rsv_info->mtd;
	pr_info("free %s:\n", rsv_info->name);

	if (rsv_info->valid) {
		addr = rsv_info->valid_node->phy_blk_addr;
		addr *= mtd->erasesize;
		memset(&erase_info,
		       0, sizeof(struct erase_info));
		erase_info.addr = addr;
		erase_info.len = mtd->erasesize;
		_aml_rsv_disprotect();
		error = rsv_ops->_erase(mtd, &erase_info); //nand_erase
		_aml_rsv_protect();
		pr_info("erasing valid info block: %llx\n", addr);
		rsv_info->valid_node->phy_blk_addr = -1;
		rsv_info->valid_node->ec = -1;
		rsv_info->valid_node->phy_page_addr = 0;
		rsv_info->valid_node->timestamp = 0;
		rsv_info->valid_node->status = 0;
		rsv_info->valid = 0;
	}
	tmp_node = rsv_info->free_node;
	while (tmp_node) {
		next_node = tmp_node->next;
		release_free_node(rsv_info, tmp_node);
		tmp_node = next_node;
	}
	rsv_info->free_node = NULL;

	return error;
}

int meson_rsv_write(struct meson_rsv_info_t *rsv_info, u_char *buf)
{
	struct mtd_info *mtd;
	struct oobinfo_t oobinfo;
	struct mtd_oob_ops oob_ops;
	struct meson_rsv_ops *rsv_ops = &rsv_handler->rsv_ops;
	size_t length = 0;
	loff_t offset;
	int ret = 0;

	if (!rsv_info) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	mtd = rsv_info->mtd;
	offset = rsv_info->valid_node->phy_blk_addr;
	offset *= mtd->erasesize;
	offset += rsv_info->valid_node->phy_page_addr * (loff_t)mtd->writesize;
	pr_info("%s:%d,save info to %llx\n", __func__, __LINE__, offset);

	memcpy(oobinfo.name, rsv_info->name, 4);
	oobinfo.ec = rsv_info->valid_node->ec;
	oobinfo.timestamp = rsv_info->valid_node->timestamp;
	while (length < rsv_info->size) {
		oob_ops.mode = MTD_OPS_AUTO_OOB;
		oob_ops.len = min_t(u32, mtd->writesize,
				    (rsv_info->size - length));
		oob_ops.ooblen = sizeof(struct oobinfo_t);
		oob_ops.ooboffs = 0;
		oob_ops.datbuf = buf + length;
		oob_ops.oobbuf = (u8 *)&oobinfo;

		ret = rsv_ops->_write_oob(mtd, offset, &oob_ops); //nand_write_oob
		if (ret) {
			pr_info("blk check good but write failed: %llx, %d\n",
				(u64)offset, ret);
			return 1;
		}
		offset += mtd->writesize;
		length += oob_ops.len;
	}
	return ret;
}

int meson_rsv_save(struct meson_rsv_info_t *rsv_info, u_char *buf)
{
	struct mtd_info *mtd;
	struct free_node_t *free_node, *temp_node;
	struct erase_info erase_info;
	struct meson_rsv_ops *rsv_ops = &rsv_handler->rsv_ops;
	int ret = 0, i = 1, pages_per_blk;
	loff_t offset = 0;

	if (!rsv_info) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	mtd = rsv_info->mtd;
	pages_per_blk = mtd->erasesize / mtd->writesize;
	/*solve power off and ecc error*/
	if (rsv_info->valid_node->status & POWER_ABNORMAL_FLAG ||
	    rsv_info->valid_node->status & ECC_ABNORMAL_FLAG)
		rsv_info->valid_node->phy_page_addr = pages_per_blk;

	if (mtd->writesize < rsv_info->size)
		i = (rsv_info->size + mtd->writesize - 1) / mtd->writesize;
	pr_info("%s:%d, %s: valid=%d, pages=%d\n", __func__, __LINE__,
		rsv_info->name, rsv_info->valid, i);
RE_SEARCH:
	if (rsv_info->valid) {
		rsv_info->valid_node->phy_page_addr += i;

		if ((rsv_info->valid_node->phy_page_addr + i) > pages_per_blk) {
			if ((rsv_info->valid_node->phy_page_addr - i) ==
				 pages_per_blk) {
				offset = rsv_info->valid_node->phy_blk_addr;
				offset *= mtd->erasesize;
				memset(&erase_info,
				       0, sizeof(struct erase_info));

				erase_info.addr = offset;
				erase_info.len = mtd->erasesize;
				_aml_rsv_disprotect();
				ret = rsv_ops->_erase(mtd, &erase_info);
				_aml_rsv_protect();
				if (ret) {
					pr_info("rsv free blk erase failed %d\n",
						ret);
					mtd->_block_markbad(mtd, offset);
				}
				rsv_info->valid_node->ec++;
				pr_info("---erase bad rsv block:%llx\n",
					offset);
			}
			/* free_node = kzalloc(sizeof(struct free_node_t), */
			/* GFP_KERNEL); */
			free_node = get_free_node(rsv_info);
			if (!free_node)
				return -ENOMEM;

			free_node->phy_blk_addr =
				rsv_info->valid_node->phy_blk_addr;
			free_node->ec = rsv_info->valid_node->ec;
			temp_node = rsv_info->free_node;
			while (temp_node->next)
				temp_node = temp_node->next;

			temp_node->next = free_node;

			temp_node = rsv_info->free_node;
			rsv_info->valid_node->phy_blk_addr =
				temp_node->phy_blk_addr;
			rsv_info->valid_node->phy_page_addr = 0;
			rsv_info->valid_node->ec = temp_node->ec;
			rsv_info->valid_node->timestamp += 1;
			rsv_info->free_node = temp_node->next;
			release_free_node(rsv_info, temp_node);
		}
	} else {
		temp_node = rsv_info->free_node;
		rsv_info->valid_node->phy_blk_addr = temp_node->phy_blk_addr;
		rsv_info->valid_node->phy_page_addr = 0;
		rsv_info->valid_node->ec = temp_node->ec;
		rsv_info->valid_node->timestamp += 1;
		rsv_info->free_node = temp_node->next;
		release_free_node(rsv_info, temp_node);
	}

	offset = rsv_info->valid_node->phy_blk_addr;
	offset *= mtd->erasesize;
	offset += rsv_info->valid_node->phy_page_addr * (loff_t)mtd->writesize;

	if (rsv_info->valid_node->phy_page_addr == 0) {
		ret = rsv_ops->_block_isbad(mtd, offset);
		if (ret) {
			/*
			 *bad block here, need fix it
			 *because of info_blk list may be include bad block,
			 *so we need check it and done here. if don't,
			 *some bad blocks may be erase here
			 *and env will lost or too much ecc error
			 **/
			pr_info("have bad block in info_blk list!!!!\n");
			rsv_info->valid_node->phy_page_addr =
				pages_per_blk - i;
			goto RE_SEARCH;
		}

		memset(&erase_info, 0, sizeof(struct erase_info));
		erase_info.addr = offset;
		erase_info.len = mtd->erasesize;
		_aml_rsv_disprotect();
		ret = rsv_ops->_erase(mtd, &erase_info);
		_aml_rsv_protect();
		if (ret) {
			pr_info("env free blk erase failed %d\n", ret);
			mtd->_block_markbad(mtd, offset);
			return ret;
		}
		rsv_info->valid_node->ec++;
	}
	ret = meson_rsv_write(rsv_info, buf);
	if (ret) {
		pr_info("update nand rsv FAILED!\n");
		return 1;
	}
	rsv_info->valid = 1;
	/* clear status when write successfully*/
	rsv_info->valid_node->status = 0;
	return ret;
}

int meson_rsv_scan(struct meson_rsv_info_t *rsv_info)
{
	struct mtd_info *mtd;
	struct mtd_oob_ops oob_ops;
	struct oobinfo_t oobinfo;
	struct free_node_t *free_node, *temp_node;
	struct meson_rsv_ops *rsv_ops = &rsv_handler->rsv_ops;
	u64 offset;
	u32 start, end;
	int ret = 0, error, rsv_status, i, k;

	u8 scan_status;
	u8 good_addr[256] = {0};
	u32 page_num, pages_per_blk;
	u32  remainder;

	if (!rsv_info) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	mtd = rsv_info->mtd;
RE_RSV_INFO_EXT:
	start = rsv_info->start_block;
	end = rsv_info->end_block;
	pr_debug("%s: info size=0x%x max_scan_blk=%d, start_blk=%d\n",
		rsv_info->name, rsv_info->size,
		end, start);

	do {
		offset = mtd->erasesize;
		offset *= start;
		scan_status = 0;
RE_RSV_INFO:
	oob_ops.mode = MTD_OPS_AUTO_OOB;
	oob_ops.len = 0;
	oob_ops.ooblen = sizeof(struct oobinfo_t);
	oob_ops.ooboffs = 0;
	oob_ops.datbuf = NULL;
	oob_ops.oobbuf = (u8 *)&oobinfo;

	memset((u8 *)&oobinfo, 0, sizeof(struct oobinfo_t));

	error = rsv_ops->_read_oob(mtd, offset, &oob_ops);
	if (error != 0 && error != -EUCLEAN) {
		pr_info("blk check good but read failed: %llx, %d\n",
			(u64)offset, error);
		offset += rsv_info->size;
		div_u64_rem(offset, mtd->erasesize, &remainder);
		if (scan_status++ > 6 || !remainder) {
			pr_info("ECC error, scan ONE block exit\n");
			scan_status = 0;
			continue;
		}
		goto RE_RSV_INFO;
	}

	rsv_info->init = 1;
	rsv_info->valid_node->status = 0;
	if (!memcmp(oobinfo.name, rsv_info->name, 4)) {
		rsv_info->valid = 1;
		if (rsv_info->valid_node->phy_blk_addr >= 0) {
			free_node = get_free_node(rsv_info);
			if (!free_node)
				return -ENOMEM;

			free_node->dirty_flag = 1;
		if (oobinfo.timestamp > rsv_info->valid_node->timestamp) {
			free_node->phy_blk_addr =
				rsv_info->valid_node->phy_blk_addr;
			free_node->ec =
			rsv_info->valid_node->ec;
			rsv_info->valid_node->phy_blk_addr =
				start;
			rsv_info->valid_node->phy_page_addr = 0;
			rsv_info->valid_node->ec = oobinfo.ec;
			rsv_info->valid_node->timestamp =
				oobinfo.timestamp;
		} else {
			free_node->phy_blk_addr = start;
			free_node->ec = oobinfo.ec;
		}
			if (!rsv_info->free_node) {
				rsv_info->free_node = free_node;
			} else {
				temp_node = rsv_info->free_node;
				while (temp_node->next)
					temp_node = temp_node->next;

				temp_node->next = free_node;
			}
		} else {
			rsv_info->valid_node->phy_blk_addr =
				start;
			rsv_info->valid_node->phy_page_addr = 0;
			rsv_info->valid_node->ec = oobinfo.ec;
			rsv_info->valid_node->timestamp =
				oobinfo.timestamp;
		}
	} else {
		free_node = get_free_node(rsv_info);
		if (!free_node)
			return -ENOMEM;
		free_node->phy_blk_addr = start;
		free_node->ec = oobinfo.ec;
		if (!rsv_info->free_node) {
			rsv_info->free_node = free_node;
		} else {
			temp_node = rsv_info->free_node;
			while (temp_node->next)
				temp_node = temp_node->next;

			temp_node->next = free_node;
		}
	}

	} while ((++start) < end);

	pr_debug("%s : phy_blk_addr=%d, ec=%d, phy_page_addr=%d, timestamp=%d\n",
		rsv_info->name,
		rsv_info->valid_node->phy_blk_addr,
		rsv_info->valid_node->ec,
		rsv_info->valid_node->phy_page_addr,
		rsv_info->valid_node->timestamp);
	pr_debug("%s free list:\n", rsv_info->name);
	temp_node = rsv_info->free_node;
	while (temp_node) {
		pr_debug("blockN=%d, ec=%d, dirty_flag=%d\n",
			temp_node->phy_blk_addr,
			temp_node->ec,
			temp_node->dirty_flag);
		temp_node = temp_node->next;
	}

	/*second stage*/
	pages_per_blk = 1 << (mtd->erasesize_shift - mtd->writesize_shift);
	page_num = rsv_info->size >> mtd->writesize_shift;
	if (page_num == 0)
		page_num++;

	pr_debug("%s %d: page_num=%d\n", __func__, __LINE__, page_num);

	if (rsv_info->valid == 1) {
		oob_ops.mode = MTD_OPS_AUTO_OOB;
		oob_ops.len = 0;
		oob_ops.ooblen = sizeof(struct oobinfo_t);
		oob_ops.ooboffs = 0;
		oob_ops.datbuf = NULL;
		oob_ops.oobbuf = (u8 *)&oobinfo;

	for (i = 0; i < pages_per_blk; i++) {
		memset((u8 *)&oobinfo, 0, oob_ops.ooblen);

		offset = rsv_info->valid_node->phy_blk_addr;
		offset *= mtd->erasesize;
		offset += i * (u64)mtd->writesize;
		error = rsv_ops->_read_oob(mtd, offset, &oob_ops);
		if (error != 0 && error != -EUCLEAN) {
			pr_info("blk good but read failed:%llx,%d\n",
				(u64)offset, error);
			rsv_info->valid_node->status |= ECC_ABNORMAL_FLAG;
			ret = -1;
			continue;
		}

		if (!memcmp(oobinfo.name, rsv_info->name, 4)) {
			good_addr[i] = 1;
			rsv_info->valid_node->phy_page_addr = i;
		} else {
			break;
		}
	}
	}

	if (mtd->writesize < rsv_info->size &&
	    rsv_info->valid == 1) {
		i = rsv_info->valid_node->phy_page_addr;
		if (((i + 1) % page_num) != 0) {
			ret = -1;
			rsv_info->valid_node->status |= POWER_ABNORMAL_FLAG;
			pr_info("find %s incomplete\n", rsv_info->name);
		}
		if (ret == -1) {
			for (i = 0; i < (pages_per_blk / page_num); i++) {
				rsv_status = 0;
				for (k = 0; k < page_num; k++) {
					if (!good_addr[k + i * page_num]) {
						rsv_status = 1;
						break;
					}
				}
				if (!rsv_status) {
					pr_info("find %d page ok\n",
						i * page_num);
				rsv_info->valid_node->phy_page_addr =
						k + i * page_num - 1;
					ret = 0;
				}
			}
		}
		if (ret == -1) {
			rsv_info->valid_node->status = 0;
			meson_free_rsv_info(rsv_info);
			goto RE_RSV_INFO_EXT;
		}
		i = (rsv_info->size + mtd->writesize - 1) / mtd->writesize;
		rsv_info->valid_node->phy_page_addr -= (i - 1);
	}

	if (rsv_info->valid != 1)
		ret = -1;
	offset = rsv_info->valid_node->phy_blk_addr;
	offset *= mtd->erasesize;
	offset += rsv_info->valid_node->phy_page_addr * (u64)mtd->writesize;
	pr_info("%s valid addr: %llx\n", rsv_info->name, (u64)offset);
	return ret;
}
EXPORT_SYMBOL(meson_rsv_scan);

int meson_rsv_read(struct meson_rsv_info_t *rsv_info, u_char *buf)
{
	struct mtd_info *mtd;
	struct oobinfo_t oobinfo;
	struct mtd_oob_ops oob_ops;
	struct meson_rsv_ops *rsv_ops = &rsv_handler->rsv_ops;
	size_t length = 0;
	loff_t offset;
	int ret = 0;

	if (!rsv_info->valid) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	mtd = rsv_info->mtd;
READ_RSV_AGAIN:
	offset = rsv_info->valid_node->phy_blk_addr;
	offset *= mtd->erasesize;
	offset += rsv_info->valid_node->phy_page_addr * (loff_t)mtd->writesize;
	pr_info("%s:%d,read info %s from %llx\n", __func__, __LINE__,
		 rsv_info->name, offset);

	while (length < rsv_info->size) {
		oob_ops.mode = MTD_OPS_AUTO_OOB;
		oob_ops.len = min_t(u32, mtd->writesize,
				    (rsv_info->size - length));
		oob_ops.ooblen = sizeof(struct oobinfo_t);
		oob_ops.ooboffs = 0;
		oob_ops.datbuf = buf + length;
		oob_ops.oobbuf = (u8 *)&oobinfo;

		memset((u8 *)&oobinfo, 0, oob_ops.ooblen);

		ret = rsv_ops->_read_oob(mtd, offset, &oob_ops);
		if (ret != 0 && ret != -EUCLEAN) {
			pr_info("blk good but read failed: %llx, %d\n",
				(u64)offset, ret);
			ret = meson_rsv_scan(rsv_info);
			if (ret == -1)
				return 1;
			goto READ_RSV_AGAIN;
		}

		if (memcmp(oobinfo.name, rsv_info->name, 4))
			pr_info("invalid nand info %s magic: %llx\n",
				rsv_info->name, (u64)offset);

		offset += mtd->writesize;
		length += oob_ops.len;
	}
	return ret;
}
EXPORT_SYMBOL(meson_rsv_read);

int meson_rsv_check(struct meson_rsv_info_t *rsv_info)
{
	int ret = 0;

	if (!rsv_info) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}

	ret = meson_rsv_scan(rsv_info);
	if (ret)
		pr_info("%s %d %s info check failed ret %d\n",
			__func__, __LINE__, rsv_info->name, ret);
	if (!rsv_info->valid) {
		pr_info("%s %d no %s info exist! ",
			__func__, __LINE__, rsv_info->name);
		pr_info("size=0x%x, phy_blk_addr=%d, ec=%d, phy_page_addr=%d, timestamp=%d\n",
			rsv_info->size,
			rsv_info->valid_node->phy_blk_addr,
			rsv_info->valid_node->ec,
			rsv_info->valid_node->phy_page_addr,
			rsv_info->valid_node->timestamp);
		ret = 1;
	}
	return ret;
}
EXPORT_SYMBOL(meson_rsv_check);

int meson_rsv_bbt_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->bbt) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->bbt->size;
	temp = vzalloc(len);
	ret = meson_rsv_read(rsv_handler->bbt, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%zx bytes from bbt, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	vfree(temp);
	return ret;
}

int meson_rsv_key_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->key) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->key->size;
	temp = vzalloc(len);
	ret = meson_rsv_read(rsv_handler->key, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%zx bytes from key, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	vfree(temp);
	return ret;
}

int meson_rsv_env_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->env) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->env->size;
	temp = vzalloc(len);
	ret = meson_rsv_read(rsv_handler->env, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%zx bytes from env, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	vfree(temp);
	return ret;
}

int meson_rsv_dtb_read(u_char *dest, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->dtb) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!dest || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, dest, size);
		return 1;
	}
	len = rsv_handler->dtb->size;
	temp = vzalloc(len);
	ret = meson_rsv_read(rsv_handler->dtb, temp);
	memcpy(dest, temp, len > size ? size : len);
	pr_info("%s %d read 0x%zx bytes from dtb, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	vfree(temp);
	return ret;
}

int meson_rsv_bbt_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->bbt) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->bbt->size;
	temp = vzalloc(len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->bbt, temp);
	pr_info("%s %d write 0x%zx bytes to bbt, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	vfree(temp);
	return ret;
}
EXPORT_SYMBOL(meson_rsv_bbt_write);

int meson_rsv_key_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->key) {
		pr_info("%s %d rsv info not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->key->size;
	temp = vzalloc(len);
	memcpy(temp, source, len > size ? size : len);
	pr_debug("%s %d write size 0x%zx bytes,want len: 0x%zx to key\n",
		 __func__, __LINE__, len, size);

	ret = meson_rsv_save(rsv_handler->key, temp);
	pr_info("%s %d write key, ret %d\n",
		__func__, __LINE__, ret);
	vfree(temp);
	return ret;
}

int meson_rsv_env_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->env) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->env->size;
	temp = vzalloc(len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->env, temp);
	pr_info("%s %d write 0x%zx bytes to env, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	vfree(temp);
	return ret;
}

int meson_rsv_dtb_write(u_char *source, size_t size)
{
	u_char *temp;
	size_t len;
	int ret;

	if (!rsv_handler ||
	    !rsv_handler->dtb) {
		pr_info("%s %d rsv info has not inited yet!\n",
			__func__, __LINE__);
		return 1;
	}
	if (!source || size == 0) {
		pr_info("%s %d parameter error %p %zd\n",
			__func__, __LINE__, source, size);
		return 1;
	}
	len = rsv_handler->dtb->size;
	temp = vzalloc(len);
	memcpy(temp, source, len > size ? size : len);
	ret = meson_rsv_save(rsv_handler->dtb, temp);
	pr_info("%s %d write 0x%zx bytes to dtb, ret %d\n",
		__func__, __LINE__, len > size ? size : len, ret);
	vfree(temp);
	return ret;
}

int meson_rsv_init(struct mtd_info *mtd,
		   struct meson_rsv_handler_t *handler)
{
	int i, ret = 0;
	unsigned int env_size = 0;
	u32 pages_per_blk_shift, start, vernier;

	meson_rsv_prase_parameter_init(mtd);
	pages_per_blk_shift = mtd->erasesize_shift - mtd->writesize_shift;
	start = BOOT_TOTAL_PAGES >> pages_per_blk_shift;
	start += rsv_blk_cnt[NAND_GAP_INDEX].block_cnt;
	vernier = start;

	handler->freenodebitmask = 0;
	for (i = 0; i < rsv_blk_cnt[NAND_RSV_INDEX].block_cnt; i++) {
		handler->free_node[i] =
			kzalloc(sizeof(struct free_node_t), GFP_KERNEL);
		if (!handler->free_node[i]) {
			ret = -ENOMEM;
			goto error;
		}

		handler->free_node[i]->index = i;
	}

	/*bbt info init*/
	handler->bbt =
		kzalloc(sizeof(*handler->bbt), GFP_KERNEL);
	if (!handler->bbt) {
		ret = -ENOMEM;
		goto error;
	}

	handler->bbt->valid_node =
		kzalloc(sizeof(*handler->bbt->valid_node), GFP_KERNEL);
	if (!handler->bbt->valid_node) {
		ret = -ENOMEM;
		goto error;
	}

	handler->bbt->mtd = mtd;
	handler->bbt->start_block = vernier;
	handler->bbt->end_block =
		vernier + rsv_blk_cnt[NAND_BBT_INDEX].block_cnt;

	handler->bbt->valid_node->phy_blk_addr = -1;

	handler->bbt->size = mtd->size >> mtd->erasesize_shift;
	handler->bbt->handler = handler;
	handler->bbt->read = meson_rsv_bbt_read;
	handler->bbt->write = meson_rsv_bbt_write;
	memcpy(handler->bbt->name, BBT_NAND_MAGIC, 4);
	vernier += rsv_blk_cnt[NAND_BBT_INDEX].block_cnt;

	/*env info init*/
	if (rsv_blk_cnt[NAND_ENV_INDEX].block_cnt) {
		handler->env =
			kzalloc(sizeof(*handler->env), GFP_KERNEL);
		if (!handler->env) {
			ret = -ENOMEM;
			goto error;
		}

		handler->env->valid_node =
			kzalloc(sizeof(struct valid_node_t), GFP_KERNEL);
		if (!handler->env->valid_node) {
			ret = -ENOMEM;
			goto error;
		}
		handler->env->mtd = mtd;
		handler->env->start_block = vernier;
		handler->env->end_block =
			vernier + rsv_blk_cnt[NAND_ENV_INDEX].block_cnt;
		handler->env->valid_node->phy_blk_addr = -1;
		if (!of_property_read_u32(mtd_get_of_node(mtd), "env_size", &env_size))
			handler->env->size = env_size;
		else if (rsv_blk_cnt[NAND_ENV_INDEX].size)
			handler->env->size = rsv_blk_cnt[NAND_ENV_INDEX].size;
		else
			handler->env->size = CONFIG_ENV_SIZE;
		handler->env->handler = handler;
		handler->env->read = meson_rsv_env_read;
		handler->env->write = meson_rsv_env_write;
		memcpy(handler->env->name, ENV_NAND_MAGIC, 4);
		vernier += rsv_blk_cnt[NAND_ENV_INDEX].block_cnt;
	}

	if (rsv_blk_cnt[NAND_KEY_INDEX].block_cnt) {
		handler->key =
			kzalloc(sizeof(*handler->key), GFP_KERNEL);
		if (!handler->key) {
			ret = -ENOMEM;
			goto error;
		}

		handler->key->valid_node =
			kzalloc(sizeof(struct valid_node_t), GFP_KERNEL);
		if (!handler->key->valid_node) {
			ret = -ENOMEM;
			goto error;
		}

		handler->key->mtd = mtd;
		handler->key->start_block = vernier;
		handler->key->end_block =
			vernier + rsv_blk_cnt[NAND_KEY_INDEX].block_cnt;
		handler->key->valid_node->phy_blk_addr = -1;
		handler->key->size = 0;
		handler->key->handler = handler;
		handler->key->read = meson_rsv_key_read;
		handler->key->write = meson_rsv_key_write;
		memcpy(handler->key->name, KEY_NAND_MAGIC, 4);
		vernier += rsv_blk_cnt[NAND_KEY_INDEX].block_cnt;
		if (rsv_blk_cnt[NAND_KEY_INDEX].size) {
			handler->key->size = rsv_blk_cnt[NAND_KEY_INDEX].size;
		} else {
			if (mtd->erasesize < 0x40000)
				handler->key->size = mtd->erasesize >> 2;
			else
				handler->key->size = 0x40000;
		}
	}

	if (rsv_blk_cnt[NAND_DTB_INDEX].block_cnt) {
		handler->dtb =
			kzalloc(sizeof(*handler->dtb), GFP_KERNEL);
		if (!handler->dtb) {
			ret = -ENOMEM;
			goto error;
		}

		handler->dtb->valid_node =
			kzalloc(sizeof(struct valid_node_t), GFP_KERNEL);
		if (!handler->dtb->valid_node) {
			ret = -ENOMEM;
			goto error;
		}
		handler->dtb->mtd = mtd;
		handler->dtb->start_block = vernier;
		handler->dtb->end_block =
			vernier + rsv_blk_cnt[NAND_DTB_INDEX].block_cnt;
		handler->dtb->valid_node->phy_blk_addr = -1;
		handler->dtb->size = 0;
		handler->dtb->handler = handler;
		handler->dtb->read = meson_rsv_dtb_read;
		handler->dtb->write = meson_rsv_dtb_write;
		memcpy(handler->dtb->name, DTB_NAND_MAGIC, 4);
		vernier += rsv_blk_cnt[NAND_DTB_INDEX].block_cnt;
		if (rsv_blk_cnt[NAND_DTB_INDEX].size) {
			handler->dtb->size = rsv_blk_cnt[NAND_DTB_INDEX].size;
		} else {
			if (mtd->erasesize < 0x40000)
				handler->dtb->size = mtd->erasesize >> 1;
			else
				handler->dtb->size = 0x40000;
		}
	}

	if ((vernier - start) > rsv_blk_cnt[NAND_RSV_INDEX].block_cnt) {
		pr_info("ERROR: total blk number is over the limit\n");
		ret = -ENOMEM;
		goto error;
	}

	pr_info("bbt_start=%d end=%d size=0x%x\n", handler->bbt->start_block,
			handler->bbt->end_block,
			handler->bbt->size);
	if (rsv_blk_cnt[NAND_ENV_INDEX].block_cnt) {
		pr_info("env_start=%d end=%d size=0x%x\n", handler->env->start_block,
				handler->env->end_block,
				handler->env->size);
		meson_rsv_register_cdev(handler->env, ENV_CDEV_NAME);
	}
	if (rsv_blk_cnt[NAND_KEY_INDEX].block_cnt) {
		pr_info("key_start=%d end=%d size=0x%x\n", handler->key->start_block,
				handler->key->end_block,
				handler->key->size);
		meson_rsv_register_unifykey(handler->key);
	}
	if (rsv_blk_cnt[NAND_DTB_INDEX].block_cnt) {
		pr_info("dtb_start=%d end=%d size=0x%x\n", handler->dtb->start_block,
				handler->dtb->end_block,
				handler->dtb->size);
		meson_rsv_register_cdev(handler->dtb, DTB_CDEV_NAME);
	}

	rsv_handler = handler;
	return ret;
error:
	for (i = 0; i < rsv_blk_cnt[NAND_RSV_INDEX].block_cnt; i++) {
		kfree(handler->free_node[i]);
		handler->free_node[i] = NULL;
	}

	if (handler->bbt) {
		if (handler->bbt->valid_node)
			kfree(handler->bbt->valid_node);
		kfree(handler->bbt);
		handler->bbt = NULL;
	}

	if (handler->env) {
		if (handler->env->valid_node)
			kfree(handler->env->valid_node);
		kfree(handler->env);
		handler->env = NULL;
	}

	if (handler->key) {
		if (handler->key->valid_node)
			kfree(handler->key->valid_node);
		kfree(handler->key);
		handler->key = NULL;
	}

	if (handler->dtb) {
		if (handler->dtb->valid_node)
			kfree(handler->dtb->valid_node);
		kfree(handler->dtb);
		handler->dtb = NULL;
	}

	return ret;
}
EXPORT_SYMBOL(meson_rsv_init);

