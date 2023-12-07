// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * drivers/amlogic/media/video_sink/video_safa.c
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/amlogic/major.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/ctype.h>
#include <linux/amlogic/media/vfm/vframe.h>
#include <linux/amlogic/media/utils/amstream.h>
#include <linux/amlogic/media/vout/vout_notify.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/clk.h>
#include <linux/arm-smccc.h>
#include <linux/debugfs.h>
#include <linux/sched.h>
#include "video_priv.h"
#include "video_reg.h"
#include "video_common.h"

#if defined(CONFIG_AMLOGIC_MEDIA_ENHANCEMENT_VECM)
#include <linux/amlogic/media/amvecm/amvecm.h>
#endif
#include <linux/amlogic/media/utils/vdec_reg.h>

#include <linux/amlogic/media/registers/register.h>
#include <linux/uaccess.h>
#include <linux/amlogic/media/utils/amports_config.h>
#include <linux/amlogic/media/vpu/vpu.h>
#include "videolog.h"

#include <linux/amlogic/media/video_sink/vpp.h>
#ifdef CONFIG_AMLOGIC_MEDIA_VSYNC_RDMA
#include "../common/rdma/rdma.h"
#endif
#include <linux/amlogic/media/video_sink/video.h>
#include "../common/vfm/vfm.h"

struct pi_reg_s {
	u32 pi_dic_num;
	u32 pi_out_scl_mode;
	u32 pi_out_win;
	u32 pi_in_win;
	u32 pi_out_ofst;
};

static struct pi_reg_s pi_reg[3] = {
	{32, 0, 4, 3, 1},
	{48, 1, 6, 3, 2},
	{64, 2, 8, 3, 2}
};

void dump_vd_vsr_safa_reg(void)
{
	u32 reg_addr, reg_val = 0;
	struct hw_vsr_safa_reg_s *vsr_reg;

	vsr_reg = &vsr_safa_reg;

	pr_info("vsr top regs:\n");
	reg_addr = vsr_reg->vpp_vsr_top_c42c44_mode;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_vsr_top_c42c44_mode]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_vsr_top_misc;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_vsr_top_misc]\n",
		reg_addr, reg_val);

	pr_info("vsr pi regs:\n");
	reg_addr = vsr_reg->vpp_pi_en_mode;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_en_mode]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_misc;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_misc]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_dict_num;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_dict_num]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_win_ofst;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_win_ofst]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_in_hsc_part;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_in_hsc_part]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_in_hsc_ini;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_in_hsc_ini]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_in_vsc_part;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_in_vsc_part]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_in_vsc_ini;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_in_vsc_ini]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_hf_hsc_part;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_hf_hsc_part]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_hf_hsc_ini;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_hf_hsc_ini]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_hf_vsc_part;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_hf_vsc_part]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->vpp_pi_hf_vsc_ini;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [vpp_pi_hf_vsc_ini]\n",
		reg_addr, reg_val);

	pr_info("vsr safa regs:\n");
	reg_addr = vsr_reg->safa_pps_sr_422_en;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_sr_422_en]\n",
		reg_addr, reg_val);

	reg_addr = vsr_reg->safa_pps_pre_scale;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_pre_scale]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_pre_hscale_coef_y1;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_pre_hscale_coef_y1]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_pre_hscale_coef_y0;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_pre_hscale_coef_y0]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_pre_hscale_coef_c1;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_pre_hscale_coef_c1]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_pre_hscale_coef_c0;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_pre_hscale_coef_c0]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_pre_vscale_coef;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_pre_vscale_coef]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_hw_ctrl;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_hw_ctrl]\n",
		reg_addr, reg_val);

	reg_addr = vsr_reg->safa_pps_interp_en_mode;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_interp_en_mode]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_yuv_sharpen_en;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_yuv_sharpen_en]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_dir_en_mode;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_dir_en_mode]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_sr_alp_info;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_sr_alp_info]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_pi_info;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_pi_info]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_hsc_start_phase_step;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_hsc_start_phase_step]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_vsc_start_phase_step;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_vsc_start_phase_step]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_vsc_init;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_vsc_init]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_hsc_init;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_hsc_init]\n",
		reg_addr, reg_val);
	reg_addr = vsr_reg->safa_pps_sc_misc;
	reg_val = READ_VCBUS_REG(reg_addr);
	pr_info("[0x%x] = 0x%x [safa_pps_sc_misc]\n",
		reg_addr, reg_val);
};

static void safa_pps_scale_set_coef(struct vsr_setting_s *vsr,
	u32 SAFA_PPS_CNTL_SCALE_COEF_IDX,
	u32 SAFA_PPS_CNTL_SCALE_COEF)
{
	int i;
	u8 vpp_index = vsr->vpp_index;
	rdma_wr_op rdma_wr = cur_dev->rdma_func[vpp_index].rdma_wr;
	rdma_wr_bits_op rdma_wr_bits =
		cur_dev->rdma_func[vpp_index].rdma_wr_bits;
	int pps_lut_tap4_s11_default[33][4] = {
		{0,   512,  0,   0},
		{-5,  512,  5,   0},
		{-10, 511, 11,   0},
		{-14, 510, 17,  -1},
		{-18, 508, 23,  -1},
		{-22, 506, 29,  -1},
		{-25, 503, 36,  -2},
		{-28, 500, 43,  -3},
		{-32, 496, 51,  -3},
		{-34, 491, 59,  -4},
		{-37, 487, 67,  -5},
		{-39, 482, 75,  -6},
		{-41, 476, 84,  -7},
		{-42, 470, 92,  -8},
		{-44, 463, 102,  -9},
		{-45, 456, 111, -10},
		{-45, 449, 120, -12},
		{-47, 442, 130, -13},
		{-47, 434, 140, -15},
		{-47, 425, 151, -17},
		{-47, 416, 161, -18},
		{-47, 407, 172, -20},
		{-47, 398, 182, -21},
		{-47, 389, 193, -23},
		{-46, 379, 204, -25},
		{-45, 369, 215, -27},
		{-44, 358, 226, -28},
		{-43, 348, 237, -30},
		{-43, 337, 249, -31},
		{-41, 326, 260, -33},
		{-40, 316, 271, -35},
		{-39, 305, 282, -36},
		{-37, 293, 293, -37}
	};

	int pps_lut_tap6_s11_default[33][6] = {
		{ 0,   0, 512,   0,   0,  0},
		{ 2,  -6, 512,   7,  -2, -1},
		{ 3, -13, 511,  14,  -3,  0},
		{ 5, -19, 510,  21,  -5,  0},
		{ 6, -24, 508,  29,  -7,  0},
		{ 7, -29, 507,  37,  -9, -1},
		{ 8, -34, 504,  45, -11,  0},
		{ 9, -39, 501,  53, -13,  1},
		{10, -44, 498,  62, -16,  2},
		{11, -48, 494,  71, -18,  2},
		{12, -51, 490,  80, -20,  1},
		{13, -55, 486,  89, -23,  2},
		{14, -58, 481,  99, -25,  1},
		{14, -61, 475, 108, -27,  3},
		{15, -64, 470, 118, -30,  3},
		{15, -66, 464, 128, -32,  3},
		{15, -68, 457, 139, -35,  4},
		{16, -70, 450, 149, -37,  4},
		{16, -72, 443, 160, -40,  5},
		{16, -73, 436, 170, -42,  5},
		{16, -74, 428, 181, -45,  6},
		{16, -75, 419, 192, -47,  7},
		{16, -75, 411, 203, -50,  7},
		{16, -76, 402, 214, -52,  8},
		{16, -76, 393, 225, -54,  8},
		{15, -76, 384, 236, -56,  9},
		{15, -75, 374, 247, -59, 10},
		{15, -75, 365, 258, -61, 10},
		{14, -74, 355, 269, -63, 11},
		{14, -73, 344, 281, -65, 11},
		{13, -72, 334, 291, -66, 12},
		{13, -71, 324, 302, -68, 12},
		{13, -70, 313, 313, -70, 13}
	};

	int pps_lut_tap8_s11_default[33][8] = {
		{0, 0,  0, 512,	0,  0, 0, 0},
		{-1,  3,  -7, 512,	 7,  -3,  1,  0},
		{-2,  5, -14, 511,	15,  -5,  2,  0},
		{-2,  7, -20, 510,	23,  -8,  3, -1},
		{-3, 10, -27, 509,	31, -11,  3,  0},
		{-4, 12, -32, 507,	39, -14,  4,  0},
		{-4, 14, -38, 504,	48, -17,  5,  0},
		{-5, 16, -43, 501,	57, -19,  6, -1},
		{-5, 18, -49, 498,	66, -22,  7, -1},
		{-6, 19, -53, 495,	75, -26,  8,  0},
		{-6, 21, -58, 490,	85, -29, 10, -1},
		{-6, 22, -62, 486,	94, -32, 11, -1},
		{-7, 24, -66, 481, 104, -35, 12, -1},
		{-7, 25, -69, 476, 114, -38, 13, -2},
		{-7, 26, -72, 470, 124, -41, 14, -2},
		{-8, 27, -75, 464, 134, -44, 15, -1},
		{-8, 28, -78, 458, 145, -47, 16, -2},
		{-8, 29, -80, 451, 155, -50, 17, -2},
		{-8, 30, -83, 444, 166, -53, 18, -2},
		{-8, 31, -84, 437, 177, -56, 19, -4},
		{-8, 31, -86, 429, 188, -59, 20, -3},
		{-9, 32, -87, 421, 199, -62, 22, -4},
		{-8, 32, -88, 413, 209, -64, 23, -5},
		{-8, 32, -89, 405, 220, -67, 24, -5},
		{-9, 33, -89, 396, 231, -69, 25, -6},
		{-8, 33, -90, 387, 242, -72, 25, -5},
		{-8, 33, -90, 377, 253, -74, 26, -5},
		{-8, 32, -89, 368, 264, -76, 27, -6},
		{-8, 32, -89, 358, 275, -78, 28, -6},
		{-8, 32, -88, 348, 286, -80, 29, -7},
		{-7, 32, -88, 338, 297, -82, 29, -7},
		{-7, 31, -86, 328, 307, -84, 30, -7},
		{-8, 31, -85, 318, 318, -85, 31, -8}
	};

	/*postsc coef lut*/
	/* dir 4tap */
	rdma_wr_bits(SAFA_PPS_CNTL_SCALE_COEF_IDX,
		0x0000, 0, 9);
	for (i = 0; i < 33; i++) {
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap4_s11_default[i][0] >> 8) & 0xff) << 24) |
			((pps_lut_tap4_s11_default[i][0]  & 0xff) << 16) |
			(((pps_lut_tap4_s11_default[i][1] >> 8) & 0xff) << 8) |
			((pps_lut_tap4_s11_default[i][1]  & 0xff) << 0));
		/* 4tap */
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap4_s11_default[i][2] >> 8) & 0xff) << 24) |
			((pps_lut_tap4_s11_default[i][2]  & 0xff) << 16) |
			(((pps_lut_tap4_s11_default[i][3] >> 8) & 0xff) << 8) |
			((pps_lut_tap4_s11_default[i][3]  & 0xff) << 0));
	}

	/* hor 8tap */
	rdma_wr_bits(SAFA_PPS_CNTL_SCALE_COEF_IDX,
		0x0080, 0, 9);
	for (i = 0; i < 33; i++) {
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap8_s11_default[i][0] >> 8) & 0xff) << 24) |
			((pps_lut_tap8_s11_default[i][0]  & 0xff) << 16) |
			(((pps_lut_tap8_s11_default[i][1] >> 8) & 0xff) << 8) |
			((pps_lut_tap8_s11_default[i][1] & 0xff) << 0));

		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap8_s11_default[i][2] >> 8) & 0xff) << 24) |
			((pps_lut_tap8_s11_default[i][2] & 0xff) << 16) |
			(((pps_lut_tap8_s11_default[i][3] >> 8) & 0xff) << 8) |
			((pps_lut_tap8_s11_default[i][3] & 0xff) << 0));
	}

	rdma_wr_bits(SAFA_PPS_CNTL_SCALE_COEF_IDX,
		0x0100, 0, 9);
	for (i = 0; i < 33; i++) {
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap8_s11_default[i][4] >> 8) & 0xff) << 24) |
			((pps_lut_tap8_s11_default[i][4] & 0xff) << 16) |
			(((pps_lut_tap8_s11_default[i][5] >> 8) & 0xff) << 8) |
			((pps_lut_tap8_s11_default[i][5] & 0xff) << 0));
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap8_s11_default[i][6] >> 8) & 0xff) << 24) |
			((pps_lut_tap8_s11_default[i][6] & 0xff) << 16) |
			(((pps_lut_tap8_s11_default[i][7] >> 8) & 0xff) << 8) |
			((pps_lut_tap8_s11_default[i][7] & 0xff) << 0));
	}

	/* hor 4tap */
	rdma_wr_bits(SAFA_PPS_CNTL_SCALE_COEF_IDX,
		0x0180, 0, 9);
	for (i = 0; i < 33; i++) {
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap4_s11_default[i][0] >> 8) & 0xff) << 24) |
			((pps_lut_tap4_s11_default[i][0] & 0xff) << 16) |
			(((pps_lut_tap4_s11_default[i][1] >> 8) & 0xff) << 8) |
			((pps_lut_tap4_s11_default[i][1] & 0xff) << 0));
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap4_s11_default[i][2] >> 8) & 0xff) << 24) |
			((pps_lut_tap4_s11_default[i][2] & 0xff) << 16) |
			(((pps_lut_tap4_s11_default[i][3] >> 8) & 0xff) << 8) |
			((pps_lut_tap4_s11_default[i][3] & 0xff) << 0));
	}

	/* ver 6tap */
	rdma_wr_bits(SAFA_PPS_CNTL_SCALE_COEF_IDX,
		0x0200, 0, 9);
	for (i = 0; i < 33; i++) {
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap6_s11_default[i][0] >> 8) & 0xff) << 24) |
			((pps_lut_tap6_s11_default[i][0]  & 0xff) << 16) |
			(((pps_lut_tap6_s11_default[i][1] >> 8) & 0xff) << 8) |
			((pps_lut_tap6_s11_default[i][1] & 0xff) << 0));

		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap6_s11_default[i][2] >> 8) & 0xff) << 24) |
			((pps_lut_tap6_s11_default[i][2] & 0xff) << 16) |
			(((pps_lut_tap6_s11_default[i][3] >> 8) & 0xff) << 8) |
			((pps_lut_tap6_s11_default[i][3] & 0xff) << 0));
	}

	rdma_wr_bits(SAFA_PPS_CNTL_SCALE_COEF_IDX,
		0x0280, 0, 9);
	for (i = 0; i < 33; i++) {
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap6_s11_default[i][4] >> 8) & 0xff) << 24) |
			((pps_lut_tap6_s11_default[i][4] & 0xff) << 16) |
			(((pps_lut_tap6_s11_default[i][5] >> 8) & 0xff) << 8) |
			((pps_lut_tap6_s11_default[i][5] & 0xff) << 0));
	}

	/* ver 4tap */
	rdma_wr_bits(SAFA_PPS_CNTL_SCALE_COEF_IDX,
		0x0300, 0, 9);
	for (i = 0; i < 33; i++) {
		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap4_s11_default[i][0] >> 8) & 0xff) << 24) |
			((pps_lut_tap4_s11_default[i][0] & 0xff) << 16) |
			(((pps_lut_tap4_s11_default[i][1] >> 8) & 0xff) << 8) |
			((pps_lut_tap4_s11_default[i][1] & 0xff) << 0));

		rdma_wr(SAFA_PPS_CNTL_SCALE_COEF,
			(((pps_lut_tap4_s11_default[i][2] >> 8) & 0xff) << 24) |
			((pps_lut_tap4_s11_default[i][2] & 0xff) << 16) |
			(((pps_lut_tap4_s11_default[i][3] >> 8) & 0xff) << 8)  |
			((pps_lut_tap4_s11_default[i][3] & 0xff) << 0));
	}
}

static void set_cfg_pi_safa(struct vsr_setting_s *vsr)
{
	struct vsr_pi_setting_s *vsr_pi = &vsr->vsr_pi;
	struct vsr_safa_setting_s *vsr_safa = &vsr->vsr_safa;
	struct vsr_top_setting_s *vsr_top = &vsr->vsr_top;
	u32 hsize_in = vsr->vsr_top.hsize_in;
	u32 vsize_in = vsr->vsr_top.vsize_in;
	u32 hsize_out = vsr->vsr_top.hsize_out;
	u32 vsize_out = vsr->vsr_top.vsize_out;
	u32 out_pi_xsize, out_pi_ysize;
	u32 pre_hsize, pre_vsize;
	u32 pi_scl_rate;
	u32 ratio = 2;

	if (vsr_pi->pi_en) {
		ratio = MIN((vsr_pi->hsize_out / vsr_pi->hsize_in),
			(vsr_pi->vsize_out / vsr_pi->vsize_in));
		if (ratio > 4)
			ratio = 4;
		if (ratio < 2 && vsr->vsr_top.vsr_en) {
			vsr_pi->pi_en = 0;
			pr_info("%s: the ratio is too small, disable PI\n", __func__);
		}
	}

	if (vsr_pi->pi_en) {
		vsr_pi->pi_dict_num = pi_reg[ratio - 2].pi_dic_num;
		vsr_pi->pi_out_scl_mode = pi_reg[ratio - 2].pi_out_scl_mode;
		vsr_pi->pi_out_win = pi_reg[ratio - 2].pi_out_win;
		vsr_pi->pi_in_win = pi_reg[ratio - 2].pi_in_win;
		vsr_pi->pi_out_ofst = pi_reg[ratio - 2].pi_out_ofst;
	} else {
		vsr_pi->pi_out_scl_mode = 0;
	}

	pi_scl_rate = (vsr_pi->pi_out_scl_mode == 0) ?
		2 : (vsr_pi->pi_out_scl_mode == 1) ? 3 : 4;
	if (vsr->vsr_top.input_422_en)
		hsize_in = hsize_in % 2 ? hsize_in + 1 : hsize_in;
	out_pi_xsize = hsize_in * pi_scl_rate;
	out_pi_ysize = vsize_in * pi_scl_rate;
	vsr_pi->pi_hf_hsc_integer_part = out_pi_xsize / hsize_out;
	vsr_pi->pi_hf_hsc_fraction_part = ((ulong)out_pi_xsize << 24) /
		hsize_out - (vsr_pi->pi_hf_hsc_integer_part << 24);
	vsr_pi->pi_hf_hsc_ini_phase = (((ulong)out_pi_xsize << 16) /
		hsize_out + (1 << 16)) / 2;
	vsr_pi->pi_hf_vsc_integer_part = out_pi_ysize / vsize_out;
	vsr_pi->pi_hf_vsc_fraction_part = ((ulong)out_pi_ysize << 24) /
		vsize_out - (vsr_pi->pi_hf_vsc_integer_part << 24);
	vsr_pi->pi_hf_vsc_ini_phase = (((ulong)out_pi_ysize << 16) /
		vsize_out + (1 << 16)) / 2;
	pre_hsize = vsr_safa->preh_en ? vsr_safa->preh_ratio == 0 ?
		hsize_in : (vsr_safa->preh_ratio == 1) ? ((hsize_in + 1) >> 1) :
		vsr_safa->preh_ratio == 2 ? ((hsize_in + 3) >> 2) :
		((hsize_in + 7) >> 3) : hsize_in;
	pre_vsize  = vsr_safa->prev_en ? (vsr_safa->prev_ratio == 0) ?
		vsize_in : (vsr_safa->prev_ratio == 1) ? ((vsize_in + 1) >> 1) :
		(vsr_safa->prev_ratio == 2) ? ((vsize_in + 3) >> 2) :
		((vsize_in + 7) >> 3) : vsize_in;
	vsr_safa->pre_hsize = pre_hsize;
	vsr_safa->pre_vsize = pre_vsize;
	vsr_top->pi_safa_hsc_integer_part = pre_hsize / hsize_out;
	vsr_top->pi_safa_hsc_fraction_part = ((ulong)pre_hsize << 24) /
		hsize_out - (vsr_top->pi_safa_hsc_integer_part << 24);
	vsr_top->pi_safa_hsc_ini_phase = (((ulong)pre_hsize << 16) /
		hsize_out + (1 << 16)) / 2;
	vsr_top->pi_safa_vsc_integer_part  = pre_vsize / vsize_out;
	vsr_top->pi_safa_vsc_fraction_part = ((ulong)pre_vsize << 24) /
		vsize_out - (vsr_top->pi_safa_vsc_integer_part << 24);
	vsr_top->pi_safa_vsc_ini_phase = (((ulong)pre_vsize << 16) /
		vsize_out + (1 << 16)) / 2;
	vsr_top->pi_safa_hsc_ini_integer = 0x1f;
	vsr_top->pi_safa_vsc_ini_integer = 0x1f;
	if (debug_common_flag & DEBUG_FLAG_COMMON_SAFA) {
		pr_info("%s:vsr top: h/vsize_in:%d,%d, h/vsize_out:%d, %d\n",
			__func__,
			hsize_in,
			vsize_in,
			hsize_out,
			vsize_out);
		pr_info("%s:safa pre_scaler pre_h/vsize:%d, %d\n",
			__func__,
			pre_hsize,
			pre_vsize);
		pr_info("%s:pi_en:%d, h/vsize_in:%d, %d, h/vsize_out:%d, %d, ratio:%d\n",
			__func__,
			vsr_pi->pi_en,
			vsr_pi->hsize_in,
			vsr_pi->vsize_in,
			vsr_pi->hsize_out,
			vsr_pi->vsize_out,
			ratio);
		pr_info("%s:pi_scl_rate:%d, out_pi_x/ysize:%d, %d\n",
			__func__,
			pi_scl_rate,
			out_pi_xsize,
			out_pi_ysize);
	}
}

static void set_vsr_pi(struct vsr_setting_s *vsr)
{
	struct vsr_pi_setting_s *vsr_pi = &vsr->vsr_pi;
	struct vsr_top_setting_s *vsr_top = &vsr->vsr_top;
	u8 vpp_index = vsr->vpp_index;
	rdma_wr_op rdma_wr = cur_dev->rdma_func[vpp_index].rdma_wr;
	rdma_wr_bits_op rdma_wr_bits = cur_dev->rdma_func[vpp_index].rdma_wr_bits;
	struct hw_vsr_safa_reg_s *vsr_reg;

	vsr_reg = &vsr_safa_reg;
	if (vsr_pi->pi_en) {
		u32 pi_en = 0;
		/* first disable pi_en, at last enable pi_en */
		rdma_wr(vsr_reg->vpp_pi_en_mode,
			(vsr_pi->pi_out_scl_mode << 0) |
			(pi_en << 4) |
			(0 << 8));
		rdma_wr(vsr_reg->vpp_pi_misc,
			(8 << 16) | //inp_hblan1
			(8 << 8) | //inp_hblan0
			(1 << 1) | //hf_pps_bypss_mode
			(1 << 0));//inp_hold_en
		rdma_wr(vsr_reg->vpp_pi_dict_num, vsr_pi->pi_dict_num);
		rdma_wr(vsr_reg->vpp_pi_win_ofst,
			(vsr_pi->pi_in_win << 0) |
			(vsr_pi->pi_out_ofst << 4) |
			(vsr_pi->pi_out_win << 8));

		vsr_pi->pi_hf_hsc_ini_integer = 0x1f;
		vsr_pi->pi_hf_vsc_ini_integer = 0x1f;
		if (vsr_pi->pi_hf_hsc_ini_integer == 0x1f &&
			(vsr_pi->pi_hf_hsc_ini_phase == (1 << 16))) {
			vsr_pi->pi_hf_hsc_ini_integer = 0;
			vsr_pi->pi_hf_hsc_ini_phase = 0;
		}
		if (vsr_pi->pi_hf_vsc_ini_integer == 0x1f &&
			(vsr_pi->pi_hf_vsc_ini_phase == (1 << 16))) {
			vsr_pi->pi_hf_vsc_ini_integer = 0;
			vsr_pi->pi_hf_vsc_ini_phase = 0;
		}
		rdma_wr(vsr_reg->vpp_pi_in_hsc_part,
			(vsr_top->pi_safa_hsc_fraction_part << 4) |
			(vsr_top->pi_safa_hsc_integer_part << 0));
		rdma_wr(vsr_reg->vpp_pi_in_hsc_ini,
			(vsr_top->pi_safa_hsc_ini_integer << 16) |
			(vsr_top->pi_safa_hsc_ini_phase << 0));
		rdma_wr(vsr_reg->vpp_pi_in_vsc_part,
			(vsr_top->pi_safa_vsc_fraction_part << 4) |
			(vsr_top->pi_safa_vsc_integer_part << 0));
		rdma_wr(vsr_reg->vpp_pi_in_vsc_ini,
			(vsr_top->pi_safa_vsc_ini_integer << 16) |
			(vsr_top->pi_safa_vsc_ini_phase << 0));
		rdma_wr(vsr_reg->vpp_pi_hf_hsc_part,
			(vsr_pi->pi_hf_hsc_fraction_part << 4) |
			(vsr_pi->pi_hf_hsc_integer_part << 0));
		rdma_wr(vsr_reg->vpp_pi_hf_hsc_ini,
			(vsr_pi->pi_hf_hsc_ini_integer << 24) |
			(vsr_pi->pi_hf_hsc_ini_phase << 0));
		rdma_wr(vsr_reg->vpp_pi_hf_vsc_part,
			(vsr_pi->pi_hf_vsc_fraction_part << 4) |
			(vsr_pi->pi_hf_vsc_integer_part << 0));
		rdma_wr(vsr_reg->vpp_pi_hf_vsc_ini,
			(vsr_pi->pi_hf_vsc_ini_integer << 24) |
			(vsr_pi->pi_hf_vsc_ini_phase << 0));
	}
	/* at last enable pi_en */
	rdma_wr_bits(vsr_reg->vpp_pi_en_mode,
		(vsr_pi->pi_en), 4, 1);
}

void set_safa_pps(struct vsr_setting_s *vsr)
{
	struct vsr_safa_setting_s *vsr_safa = &vsr->vsr_safa;
	struct vsr_top_setting_s *vsr_top = &vsr->vsr_top;
	u32 postsc_size_mux = 0, dir_info_ds_x_en = 0;
	u32 drt_intp_en, sharp_en = 0;
	u32 pi_vofst = 0, adp_tap_alp_mode = 0;
	u32 beta_mode = 0;
	u32 sr_delta_alp_mode = 0, sr_gamma_alp_mode = 0;
	u32 pi_gamma_mode = 0, pi_max_sad_mode = 0;
	u32 analy_en = vsr_safa->postsc_en;
	//1 bits prehscaler en
	u32 preh_en = vsr_safa->preh_en;
	//1 bits prevscaler en
	u32 prev_en = vsr_safa->prev_en;
	//2 bits prehor ds ratio 0:1/1 1:1/2 2:1/4 3:1/8
	u32 preh_ratio = vsr_safa->preh_ratio;
	//2 bits prever ds ratio 0:1/1 1:1/2 2:1/4 3:1/8
	u32 prev_ratio = vsr_safa->prev_ratio;
	//1 bits postscaler en
	u32 postsc_en = vsr_safa->postsc_en;
	//1 bits 0: yuv444 pc mode 1: yuv422
	u32 input_422_en = vsr_top->input_422_en;
	u32 safa_pps_top_en = postsc_en | preh_en | prev_en;
	u8 vpp_index = vsr->vpp_index;
	rdma_wr_op rdma_wr = cur_dev->rdma_func[vpp_index].rdma_wr;
	rdma_wr_bits_op rdma_wr_bits = cur_dev->rdma_func[vpp_index].rdma_wr_bits;
	struct hw_vsr_safa_reg_s *vsr_reg;

	vsr_reg = &vsr_safa_reg;
	adp_tap_alp_mode = 1;
	beta_mode        = 1;
	sr_delta_alp_mode = 1;
	sr_gamma_alp_mode = 1;
	pi_gamma_mode    = 1;
	pi_max_sad_mode  = 1;

	if (safa_pps_top_en) {
		//postsc hsize region
		if (vsr_safa->pre_hsize <= 1024) {
			postsc_size_mux = 0;
			dir_info_ds_x_en = 0;
			drt_intp_en = 1;
		} else if (vsr_safa->pre_hsize <= 2048) {
			postsc_size_mux = 0;
			dir_info_ds_x_en = 1;
			drt_intp_en = 1;
		} else {
			postsc_size_mux = 1;
			dir_info_ds_x_en = 1;
			drt_intp_en = 0;
			adp_tap_alp_mode = 3;
			beta_mode = 3;
			sr_delta_alp_mode = 3;
			sr_gamma_alp_mode = 3;
			pi_gamma_mode = 3;
			pi_max_sad_mode = 3;
		}

		if (input_422_en) {
			sharp_en = 1;
			pi_vofst = 2;
		} else {
			sharp_en = 0;
			pi_vofst = 1;
			adp_tap_alp_mode = 3;
			beta_mode = 3;
			sr_delta_alp_mode = 3;
			sr_gamma_alp_mode = 3;
			pi_gamma_mode = 3;
			pi_max_sad_mode = 3;
		}
		if (debug_common_flag & DEBUG_FLAG_COMMON_SAFA)
			pr_info("%s:preh/v_en:%d, %d, pre_h/vsize:%d, %d, preh/v_ratio:%d, %d, postsc_en:%d\n",
				__func__,
				vsr_safa->preh_en,
				vsr_safa->prev_en,
				vsr_safa->pre_hsize,
				vsr_safa->pre_vsize,
				vsr_safa->preh_ratio,
				vsr_safa->prev_ratio,
				vsr_safa->postsc_en);
		//postsc coef lut config
		safa_pps_scale_set_coef(vsr,
			vsr_reg->safa_pps_cntl_scale_coef_idx_luma,
			vsr_reg->safa_pps_cntl_scale_coef_luma);
		safa_pps_scale_set_coef(vsr,
			vsr_reg->safa_pps_cntl_scale_coef_idx_chro,
			vsr_reg->safa_pps_cntl_scale_coef_chro);

		//reg config
		rdma_wr_bits(vsr_reg->safa_pps_sr_422_en,
			input_422_en, 0, 1);
		rdma_wr(vsr_reg->safa_pps_pre_scale,
			(2 << 16) |
			(4 << 12) |
			(4 << 8) |
			(preh_ratio << 4) |
			(prev_ratio << 0));
		rdma_wr(vsr_reg->safa_pps_pre_hscale_coef_y1,
			(0 << 16) |
			(0 << 0));
		rdma_wr(vsr_reg->safa_pps_pre_hscale_coef_y0,
			(0 << 16) |
			(256 << 0));
		rdma_wr(vsr_reg->safa_pps_pre_hscale_coef_c1,
			(0 << 16) |
			(0 << 0));
		rdma_wr(vsr_reg->safa_pps_pre_hscale_coef_c0,
			(0 << 16) |
			(256 << 0));
		rdma_wr(vsr_reg->safa_pps_pre_vscale_coef,
			(0 << 16) |
			(256 << 0));

		rdma_wr_bits(vsr_reg->safa_pps_hw_ctrl,
			postsc_size_mux, 1, 1);
		rdma_wr_bits(vsr_reg->safa_pps_hw_ctrl,
			pi_vofst, 9, 2);
		rdma_wr_bits(vsr_reg->safa_pps_interp_en_mode,
			adp_tap_alp_mode, 8, 2);
		rdma_wr_bits(vsr_reg->safa_pps_interp_en_mode,
			beta_mode, 12, 2);
		rdma_wr_bits(vsr_reg->safa_pps_interp_en_mode,
			drt_intp_en, 24, 1);
		rdma_wr_bits(vsr_reg->safa_pps_yuv_sharpen_en,
			sharp_en, 4, 1);
		rdma_wr_bits(vsr_reg->safa_pps_dir_en_mode,
			dir_info_ds_x_en, 24, 1);
		rdma_wr_bits(vsr_reg->safa_pps_sr_alp_info,
			sr_delta_alp_mode, 24, 2);
		rdma_wr_bits(vsr_reg->safa_pps_sr_alp_info,
			sr_gamma_alp_mode, 8, 2);
		rdma_wr_bits(vsr_reg->safa_pps_pi_info,
			pi_gamma_mode, 20, 2);
		rdma_wr_bits(vsr_reg->safa_pps_pi_info,
			pi_max_sad_mode, 8, 2);
		rdma_wr(vsr_reg->safa_pps_hsc_start_phase_step,
			((vsr_top->pi_safa_hsc_integer_part & 0xf) << 24) |
			((vsr_top->pi_safa_hsc_fraction_part & 0xffffff) << 0));
		rdma_wr(vsr_reg->safa_pps_vsc_start_phase_step,
			((vsr_top->pi_safa_vsc_integer_part & 0xf) << 24) |
			((vsr_top->pi_safa_vsc_fraction_part & 0xffffff) << 0));
		rdma_wr_bits(vsr_reg->safa_pps_vsc_init,
			vsr_top->pi_safa_vsc_ini_phase, 0, 16);
		rdma_wr_bits(vsr_reg->safa_pps_hsc_init,
			vsr_top->pi_safa_hsc_ini_phase, 0, 16);
		rdma_wr_bits(vsr_reg->safa_pps_sc_misc,
			prev_en, 4, 1);
		rdma_wr_bits(vsr_reg->safa_pps_sc_misc,
			preh_en, 8, 1);
		rdma_wr_bits(vsr_reg->safa_pps_hw_ctrl,
			postsc_en, 2, 1);
		rdma_wr_bits(vsr_reg->safa_pps_hw_ctrl,
			analy_en, 4, 1);
	}
	rdma_wr_bits(vsr_reg->safa_pps_hw_ctrl,
		safa_pps_top_en, 8, 1);
}

static void set_vsr_input_format(struct vsr_setting_s *vsr)
{
	u32 vsr_en = vsr->vsr_top.vsr_en;
	u8 vpp_index = vsr->vpp_index;
	rdma_wr_op rdma_wr = cur_dev->rdma_func[vpp_index].rdma_wr;
	rdma_wr_bits_op rdma_wr_bits = cur_dev->rdma_func[vpp_index].rdma_wr_bits;
	struct hw_vsr_safa_reg_s *vsr_reg;

	vsr_reg = &vsr_safa_reg;
	/* config vsr top */
	if (vsr->vsr_top.vsr_en) {
		u32 reg_inp_422 = 0, reg_use_inp_fmt = 1, reg_sync_ctrl = 0;
		u32 reg_444to422_mode = 0, reg_444to422_en = 0;
		u32 reg_422to444_mode = 0, reg_422to444_en = 0;

		if (vsr->vsr_top.input_422_en) {
			reg_inp_422 = 1;
			reg_444to422_mode = 2;
			reg_444to422_en = 1;
			reg_422to444_mode = 2;
			reg_422to444_en = 1;
		}
		rdma_wr(vsr_reg->vpp_vsr_top_c42c44_mode,
			(reg_444to422_mode << 0) |
			(reg_444to422_en << 2) |
			(reg_422to444_mode << 4) |
			(reg_422to444_en << 6));

		rdma_wr(vsr_reg->vpp_vsr_top_misc,
			(vsr_en << 0) |
			(reg_inp_422 << 1) |
			(reg_use_inp_fmt << 2) |
			(reg_sync_ctrl << 16));
	} else {
		rdma_wr_bits(vsr_reg->vpp_vsr_top_misc,
			vsr_en, 0, 1);
		vsr->vsr_pi.pi_en = 0;
		vsr->vsr_safa.preh_en = 0;
		vsr->vsr_safa.prev_en = 0;
		vsr->vsr_safa.postsc_en = 0;
	}
	if (debug_common_flag & DEBUG_FLAG_COMMON_SAFA)
		pr_info("%s:vsr_en:%d, input_422_en:%d\n",
			__func__,
			vsr_en,
			vsr->vsr_top.input_422_en);
}

void set_vsr_scaler(struct vsr_setting_s *vsr)
{
	set_vsr_input_format(vsr);
	if (vsr->vsr_top.vsr_en) {
		set_cfg_pi_safa(vsr);
		set_vsr_pi(vsr);
		set_safa_pps(vsr);
	}
}
