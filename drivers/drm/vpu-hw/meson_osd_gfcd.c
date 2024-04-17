// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include "meson_vpu_pipeline.h"
#include "meson_vpu_reg.h"
#include "meson_osd_afbc.h"
#include "meson_osd_gfcd.h"

static struct gfcd_reg_s gfcd_reg[2] = {
	{
		GFCD_MIF_MODE_SEL,
		GFCD_MIF_HEAD_BADDR,
		GFCD_MIF_BODY_BADDR,
		GFCD_FRM_BGN,
		GFCD_FRM_END,
		GFCD_TOP_CTRL,
		GFCD_AFRC_CTRL,
		GFCD_AFBC_CTRL,
		GFCD_FRM_SIZE,
		GFCD_MIF_HEAD_CTRL0,
		GFCD_MIF_BODY_CTRL,
	},
	{
		GFCD_MIF_MODE_SEL_S1,
		GFCD_MIF_HEAD_BADDR_S1,
		GFCD_MIF_BODY_BADDR_S1,
		GFCD_FRM_BGN_S1,
		GFCD_FRM_END_S1,
		GFCD_TOP_CTRL_S1,
		GFCD_AFRC_CTRL_S1,
		GFCD_AFBC_CTRL_S1,
		GFCD_FRM_SIZE_S1,
		GFCD_MIF_HEAD_CTRL0_S1,
		GFCD_MIF_BODY_CTRL_S1,
	},
};

static int gfcd_check_state(struct meson_vpu_block *vblk,
				 struct meson_vpu_block_state *state,
				 struct meson_vpu_pipeline_state *mvps)
{
	return 0;
}

static void gfcd_set_state(struct meson_vpu_block *vblk,
				struct meson_vpu_block_state *state,
				struct meson_vpu_block_state *old_state)
{
	struct meson_vpu_gfcd *gfcd;
	struct meson_vpu_pipeline *pipeline;
	struct meson_vpu_pipeline_state *mvps;
	struct meson_vpu_sub_pipeline_state *mvsps;
	struct meson_vpu_osd_layer_info *plane_info;
	struct rdma_reg_ops *reg_ops;
	struct gfcd_reg_s *reg;
	const struct meson_drm_format_info *info;
	u64 header_addr;
	bool reverse_x, reverse_y, is_afrc, gfcd_en = 0;
	u8 tile_mode, blk_mode = 0, alpha_replace = 0;
	int i;

	gfcd = to_gfcd_block(vblk);
	pipeline = gfcd->base.pipeline;
	mvps = priv_to_pipeline_state(pipeline->obj.state);
	mvsps = &mvps->sub_states[0];
	reg_ops = state->sub->reg_ops;

	for (i = 0; i < gfcd->num_surface; i++) {
		plane_info = &mvps->plane_info[i];
		reg = &gfcd->reg[i];
		gfcd_en = (plane_info->process_unit == GFCD_AFBC ||
			plane_info->process_unit == GFCD_AFRC);
		if (plane_info->enable && gfcd_en) {
			reverse_x = (plane_info->rotation & DRM_MODE_REFLECT_X) ? 1 : 0;
			reverse_y = (plane_info->rotation & DRM_MODE_REFLECT_Y) ? 1 : 0;

			header_addr = plane_info->phy_addr;
			is_afrc = (plane_info->process_unit == GFCD_AFRC) ? 1 : 0;

			info = __meson_drm_gfcd_format_info(plane_info->pixel_format);
			if (info) {
				alpha_replace = info->alpha_replace;
				if (is_afrc)
					blk_mode = info->gfcd_hw_blkmode_afrc;
				else
					blk_mode = info->gfcd_hw_blkmode_afbc;
			}

			tile_mode = !!(plane_info->afbc_inter_format & TILED_HEADER_EN);

			// MIF
			/*reverse config*/
			reg_ops->rdma_write_reg_bits(reg->gfcd_mif_mode_sel, reverse_x, 5, 1);
			reg_ops->rdma_write_reg_bits(reg->gfcd_mif_mode_sel, reverse_y, 4, 1);

			/* set header addr */
			reg_ops->rdma_write_reg(reg->gfcd_mif_head_baddr, header_addr >> 4);

			/* set body addr for afrc*/
			if (is_afrc)
				reg_ops->rdma_write_reg(reg->gfcd_mif_body_baddr, header_addr >> 4);

			// TOP
			reg_ops->rdma_write_reg(reg->gfcd_frm_bgn,
						plane_info->src_y << 16 | plane_info->src_x);
			reg_ops->rdma_write_reg(reg->gfcd_frm_end,
						(plane_info->src_y + plane_info->src_h - 1) << 16 |
					(plane_info->src_x + plane_info->src_w - 1));

			/*0:afbc 1:afrc*/
			reg_ops->rdma_write_reg_bits(reg->gfcd_top_ctrl, is_afrc, 0, 1);

			reg_ops->rdma_write_reg_bits(reg->gfcd_top_ctrl, 1, 4, 1);
			reg_ops->rdma_write_reg_bits(reg->gfcd_top_ctrl, 0, 16, 2);
			reg_ops->rdma_write_reg_bits(reg->gfcd_top_ctrl, alpha_replace, 28, 1);
			if (alpha_replace)
				reg_ops->rdma_write_reg_bits(reg->gfcd_top_ctrl, 0xff, 20, 8);
			else
				reg_ops->rdma_write_reg_bits(reg->gfcd_top_ctrl, 0, 20, 8);

			if (is_afrc) {
				/*0:config from reg 1:config from header*/
				reg_ops->rdma_write_reg_bits(reg->gfcd_afrc_ctrl, 0, 0, 1);

				/*0:16byte 1:24byte 2:32byte*/
				reg_ops->rdma_write_reg_bits(reg->gfcd_afrc_ctrl,
					plane_info->afrc_cu_bits, 8, 2);

				reg_ops->rdma_write_reg_bits(reg->gfcd_afrc_ctrl, 1, 16, 3);
				reg_ops->rdma_write_reg_bits(reg->gfcd_afrc_ctrl, blk_mode, 20, 5);
			} else {
				/*4'd0: RGBA8888 4'd1: RGBA1010102 4'd2: RGB888 4'd3: RGBA10101010*/
				reg_ops->rdma_write_reg_bits(reg->gfcd_afbc_ctrl,
						blk_mode, 0, 4);
				reg_ops->rdma_write_reg_bits(reg->gfcd_afbc_ctrl, 0, 4, 1);
			}

			reg_ops->rdma_write_reg_bits(reg->gfcd_frm_size, plane_info->fb_h, 0, 13);
			reg_ops->rdma_write_reg_bits(reg->gfcd_frm_size, plane_info->fb_w, 16, 13);

			if (!is_afrc)
				reg_ops->rdma_write_reg_bits(reg->gfcd_mif_head_ctrl0,
						tile_mode, 4, 2);
			else
				reg_ops->rdma_write_reg_bits(reg->gfcd_mif_body_ctrl,
						tile_mode, 0, 2);
			MESON_DRM_BLOCK("is_afrc=%d, afrc_cu_bits=%d blk_mode=%d tile_mode=%d.\n",
				is_afrc, plane_info->afrc_cu_bits, blk_mode, tile_mode);
		} else {
			reg_ops->rdma_write_reg_bits(reg->gfcd_top_ctrl, 0, 4, 1);
		}
		MESON_DRM_BLOCK("osd%d, enable=%d, gfcd_en=%d.\n", plane_info->plane_index,
			plane_info->enable, gfcd_en);
	}

	MESON_DRM_BLOCK("%s set_state called.\n", vblk->name);
}

static void gfcd_hw_enable(struct meson_vpu_block *vblk, struct meson_vpu_block_state *state)
{
}

static void gfcd_hw_disable(struct meson_vpu_block *vblk, struct meson_vpu_block_state *state)
{
	struct meson_vpu_gfcd *gfcd = to_gfcd_block(vblk);

	MESON_DRM_BLOCK("%s disable called.\n", gfcd->base.name);
}

static void gfcd_dump_register(struct drm_printer *p,
							struct meson_vpu_block *vblk)
{
	u32 value = 0, reg_addr;
	struct meson_vpu_gfcd *gfcd;
	struct gfcd_reg_s *reg;
	char buff[16];
	int i;

	gfcd = to_gfcd_block(vblk);

	for (i = 0; i < gfcd->num_surface; i++) {
		reg = &gfcd->reg[i];
		snprintf(buff, 8, "osd%d", i + 1);
		reg_addr = reg->gfcd_mif_mode_sel;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_mif_mode_sel", reg_addr, value);

		reg_addr = reg->gfcd_mif_head_baddr;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_mif_head_baddr", reg_addr, value);

		reg_addr = reg->gfcd_mif_body_baddr;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_mif_body_baddr", reg_addr, value);

		reg_addr = reg->gfcd_frm_bgn;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_frm_bgn", reg_addr, value);

		reg_addr = reg->gfcd_frm_end;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_frm_end", reg_addr, value);

		reg_addr = reg->gfcd_top_ctrl;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_top_ctrl", reg_addr, value);

		reg_addr = reg->gfcd_afrc_ctrl;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_afrc_ctrl", reg_addr, value);

		reg_addr = reg->gfcd_afbc_ctrl;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_afbc_ctrl", reg_addr, value);

		reg_addr = reg->gfcd_frm_size;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_frm_size", reg_addr, value);

		reg_addr = reg->gfcd_mif_head_ctrl0;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_mif_head_ctrl0", reg_addr, value);

		reg_addr = reg->gfcd_mif_body_ctrl;
		value = meson_drm_read_reg(reg_addr);
		drm_printf(p, "%s_%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n", buff,
				  "gfcd_mif_body_ctrl", reg_addr, value);
	}

	reg_addr = OSD_PATH_MISC_CTRL;
	value = meson_drm_read_reg(reg_addr);
	drm_printf(p, "%-35s\t\taddr: 0x%04X\tvalue: 0x%08X\n",
			  "OSD_PATH_MISC_CTRL", reg_addr, value);
}

static void gfcd_hw_init(struct meson_vpu_block *vblk)
{
	struct meson_vpu_gfcd *gfcd = to_gfcd_block(vblk);

	gfcd->reg = &gfcd_reg[0];
	gfcd->num_surface = 2;
	DRM_DEBUG("%s hw init.\n", vblk->name);
}

static void gfcd_hw_fini(struct meson_vpu_block *vblk)
{
}

struct meson_vpu_block_ops gfcd_ops = {
	.check_state = gfcd_check_state,
	.update_state = gfcd_set_state,
	.enable = gfcd_hw_enable,
	.disable = gfcd_hw_disable,
	.dump_register = gfcd_dump_register,
	.init = gfcd_hw_init,
	.fini = gfcd_hw_fini,
};
