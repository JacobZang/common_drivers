/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef AMDV_REGS_HW5_HEADER_
#define AMDV_REGS_HW5_HEADER_

/***************DV TOP1 REGS START **********************/

#define DOLBY_TOP1_PIC_SIZE     0x0a00
//Bit 31:16 reg_pic_vsize       //unsigned, RW, default=1080, dos src pic vsize
//Bit 15:0  reg_pic_hsize       //unsigned, RW, default=1920, dos src pic hsize

#define DOLBY_TOP1_RDMA_CTRL    0x0a01
//Bit 31    reg_rdma_sw_rst     //unsigned, RW, default=0, sw rst for rdma
//Bit 30    reg_rdma_on         //unsigned, RW, default=1, top1 rdma on
//Bit 29    reg_rdma_shdw_rst   //unsigned, RW, default=0, shadow rst for rdma
//Bit 28:19 reserved
//Bit 18:16 reg_rdma_num        //unsigned, RW, default=1, rdma num
//Bit 15:0  reg_rdma_size       //unsigned, RW, default=149, rdma transaction size

#define DOLBY_TOP1_PYWR_CTRL    0x0a02
//Bit 31    reg_pywrmif_sw_rst  //unsigned, RW, default=0, sw rst for pyramif wrmif
//Bit 30:20 reserved
//Bit 19:4  reg_gclk_ctrl       //unsigned, RW, default=0, clk gating ctrl
//Bit 3:1   reserved
//Bit 0     reg_py_level        //unsigned, RW, default=1, pymid level, src pic>512x288 ? 1 : 0

#define DOLBY_TOP1_PYWR_BADDR1  0x0a03
//Bit 31:0 reg_py_baddr1        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP1_PYWR_BADDR2  0x0a04
//Bit 31:0 reg_py_baddr2        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP1_PYWR_BADDR3  0x0a05
//Bit 31:0 reg_py_baddr3        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP1_PYWR_BADDR4  0x0a06
//Bit 31:0 reg_py_baddr4        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP1_PYWR_BADDR5  0x0a07
//Bit 31:0 reg_py_baddr5        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP1_PYWR_BADDR6  0x0a08
//Bit 31:0 reg_py_baddr6        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP1_PYWR_BADDR7  0x0a09
//Bit 31:0 reg_py_baddr7        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP1_PYWR_STRIDE12 0x0a0a
//Bit 31:29 reserved
//Bit 28:16 reg_py_stride2      //unsigned, RW, default=512
//Bit 15:13 reserved
//Bit 12:0  reg_py_stride1      //unsigned, RW, default=1024
#define DOLBY_TOP1_PYWR_STRIDE34 0x0a0b
//Bit 31:29 reserved
//Bit 28:16 reg_py_stride4      //unsigned, RW, default=128
//Bit 15:13 reserved
//Bit 12:0  reg_py_stride3      //unsigned, RW, default=256
#define DOLBY_TOP1_PYWR_STRIDE56 0x0a0c
//Bit 31:29 reserved
//Bit 28:16 reg_py_stride6      //unsigned, RW, default=32
//Bit 15:13 reserved
//Bit 12:0  reg_py_stride5      //unsigned, RW, default=64

#define DOLBY_TOP1_PYWR_STRIDE7  0x0a0d
//Bit 31:15 reg_py_urgent       //unsigned, RW, default=0, pyramid wrmif urgent
//Bit 14:13 reserved
//Bit 12:0  reg_py_stride7      //unsigned, RW, default=16

#define DOLBY_TOP1_WDMA_CTRL    0x0a0e
//Bit 31    reg_wdma_sw_rst     //unsigned ,RW, default=0, sw rst for wdma
//Bit 30:20 reserved
//Bit 19:17 reg_wdma_slc_num    //unsigned, RW, default=1, histogram && metadata
//Bit 16:0  reg_wdma_urgent     //unsigned, RW, default=0, wdma wrmif urgent
#define DOLBY_TOP1_WDMA_BADDR0  0x0a0f
//Bit 31:0  reg_wdma_baddr0     //unsigned, RW, default=0, histogram baddr
//
#define DOLBY_TOP1_WDMA_BADDR1  0x0a10
//Bit 31:0  reg_wdma_baddr1     //unsigned, RW, default=0, metadata baddr
//
#define DOLBY_TOP1_CTRL0    0x0a11
//Bit 31    reg_sw_reset   //unsigned, RW, default=0, sw rst for the whole module, used as rst_n
//Bit 30    reg_frm_rst    //unsigned, RW, default=0, sw-triggered frm_rst
//Bit 29:27 reserved
//Bit 26    reg_rdmif_arsec//unsigned, RW, default=0, pix rdmif security
//Bit 25:24 reg_din_sel    //unsigned, RW, default=2, 1=reg_frm_rst 2=vsync as frm rst, 0/3=idle
//Bit 23    reg_hs_sel     //unsigned, RW, default=0, 0=hsync, 1=hold hend as hsync
//Bit 22:13 reserved
//Bit 12:0  reg_stdly_num  //unsigned, RW, default=2, hold line cnt after frm rst to generate frm_en

#define DOLBY_TOP1_CTRL1    0x0a12
//Bit 31:29 reserved
//Bit 28:16 reg_hold_vnum       //unsigned, RW, default=2, hold vnum
//Bit 15:13 reserved
//Bit 12:0  reg_hold_hnum       //unsigned, RW, default=2, hold hnum
#define DOLBY_TOP1_HOLD_CTRL0    0x0a13
//Bit 31:0  reg_hs_hold_num0    //unsigned, RW, default=256, delay between rdma done && pix hs

#define DOLBY_TOP1_CTRL2 0x0a14
//Bit 31:30 reg_conv_mode   //default=0, 0:10bit 1:12bit detunnel, 2:12bit depack by 8bit
//Bit 29:12 reg_tunnel_sel  //unsigned, RW, default=0, tunnel_sel
//Bit 11:10 reserved
//Bit 9:0   reg_int_sel
//default=0,output interrupt select,1:shutdown 0:open bit5:0=[dos din,rdma,core1,core1b,pyra,hist]
//
//
#define DOLBY_TOP1_RO_0   0x0a15
//Bit 31:0  ro_dbg0         //unsigned, RO, default=0
//
#define DOLBY_TOP1_RO_1   0x0a16
//Bit 31:0  ro_dbg1         //unsigned, RO, default=0
//
#define DOLBY_TOP1_RO_2   0x0a17
//Bit 31:0  ro_dbg2         //unsigned, RO, default=0
//
#define DOLBY_TOP1_RO_3   0x0a18
//Bit 31:0  ro_dbg3         //unsigned, RO, default=0
//
#define DOLBY_TOP1_RO_4   0x0a19
//Bit 31:0  ro_dbg4         //unsigned, RO, default=0
//
#define DOLBY_TOP1_RO_5   0x0a1a
//Bit 31:0  ro_dbg5         //unsigned, RO, default=0
//
#define DOLBY_TOP1_RO_6   0x0a1b
//Bit 31:0  ro_dbg6         //unsigned, RO, default=0
//

#define DOLBY_TOP1_GEN_REG 0x0a20
//Bit 31        cntl_enable_free_clk            //unsigned, RW, default=0
//Bit 30        cntl_sw_reset                   //unsigned, RW, default=0, pulse
//Bit 29        cntl_reset_on_go_field          //unsigned, RW, default=0
//Bit 28        cntl_urgent_chroma              //unsigned, RW, default=0
//Bit 27        cntl_urgent_luma                //unsigned, RW, default=0
//Bit 26        cntl_chroma_end_at_last_line    //unsigned, RW, default=0
//Bit 25        cntl_luma_end_at_last_line      //unsigned, RW, default=0
//Bit 24:19     cntl_hold_lines                 //unsigned, RW, default=0, hold lines[5:0]
//Bit 18        cntl_last_line_mode             //unsigned, RW, default=1
//Bit 17        reserved
//Bit 16        cntl_demux_mode                 //unsigned, RW, default=0
//Bit 15:14     cntl_bytes_per_pixel            //unsigned, RW, default=0
//Bit 13:12     cntl_ddr_burst_size_cr          //unsigned, RW, default=0
//Bit 11:10     cntl_ddr_burst_size_cb          //unsigned, RW, default=0
//Bit 9:8       cntl_ddr_burst_size_y           //unsigned, RW, default=0
//Bit 7         cntl_start_frame_man            //unsigned, RW, default=0, pulse
//Bit 6         cntl_chro_rpt_lastl             //unsigned, RW, default=0
//Bit 5         reserved
//Bit 4         cntl_little_endian              //unsigned, RW, default=0
//Bit 3         cntl_chroma_hz_avg              //unsigned, RW, default=0
//Bit 2         cntl_luma_hz_avg                //unsigned, RW, default=0
//Bit 1         cntl_st_separate_en             //unsigned, RW, default=0
//Bit 0         cntl_enable                     //unsigned, RW, default=0
#define DOLBY_TOP1_GEN_REG2 0x0a21
//Bit 31:30     reserved
//Bit 29        cntl_chroma_line_read_sel       //unsigned, RW, default=0
//Bit 28        cntl_luma_line_read_sel         //unsigned, RW, default=0
//Bit 27:26     reserved
//Bit 25:24     cntl_shift_pat_cr               //unsigned, RW, default=0
//Bit 23:18     reserved
//Bit 17:16     cntl_shift_pat_cb               //unsigned, RW, default=0
//Bit 15:10     reserved
//Bit 9:8       cntl_shift_pat_y                //unsigned, RW, default=0
//Bit 7         reserved
//Bit 6         cntl_hold_lines                 //unsigned, RW, default=0, hold_lines[6]
//Bit 5:4       reserved
//Bit 3         cntl_y_rev                      //unsigned, RW, default=0
//Bit 2         cntl_x_rev                      //unsigned, RW, default=0
//Bit 1:0       cntl_color_map                  //unsigned, RW, default=0

#define DOLBY_TOP1_CANVAS 0x0a22
//Bit 31        cntl_canvas_addr_syncen         //unsigned, RW, default=0
//Bit 30:24     reserved
//Bit 23:16     cntl_canvas_addr2               //unsigned, RW, default=0
//Bit 15:8      cntl_canvas_addr1               //unsigned, RW, default=0
//Bit 7:0       cntl_canvas_addr0               //unsigned, RW, default=0

#define DOLBY_TOP1_LUMA_X 0x0a23
//Bit 31        reserved
//Bit 30:16     cntl_luma_x_end                //unsigned, RW, default=0
//Bit 15        reserved
//Bit 14:0      cntl_luma_x_start              //unsigned, RW, default=0

#define DOLBY_TOP1_LUMA_Y 0x0a24
//Bit 31:29     reserved
//Bit 28:16     cntl_luma_y_end                //unsigned, RW, default=0
//Bit 15:13     reserved
//Bit 12:0      cntl_luma_y_start              //unsigned, RW, default=0

#define DOLBY_TOP1_CHROMA_X 0x0a25
//Bit 31        reserved
//Bit 30:16     cntl_chroma_x_end                //unsigned, RW, default=0
//Bit 15        reserved
//Bit 14:0      cntl_chroma_x_start              //unsigned, RW, default=0

#define DOLBY_TOP1_CHROMA_Y 0x0a26
//Bit 31:29     reserved
//Bit 28:16     cntl_chroma_y_end                //unsigned, RW, default=0
//Bit 15:13     reserved
//Bit 12:0      cntl_chroma_y_start              //unsigned, RW, default=0

#define DOLBY_TOP1_RPT_LOOP 0x0a27
//Bit 31:16     reserved
//Bit 15:8      cntl_chroma_rpt_loop           //unsigned, RW, default=0
//Bit 7:0       cntl_luma_rpt_loop             //unsigned, RW, default=0

#define DOLBY_TOP1_LUMA_RPT_PAT 0x0a28
//Bit 31:0      cntl_luma_rpt_pat              //unsigned, RW, default=0
#define DOLBY_TOP1_CHROMA_RPT_PAT 0x0a29
//Bit 31:0      cntl_chroma_rpt_pat            //unsigned, RW, default=0

#define DOLBY_TOP1_DUMMY_PIXEL 0x0a2a
//Bit 31:0      cntl_dummy_pixel_val            //unsigned, RW, default=32'h00808000
//
#define DOLBY_TOP1_LUMA_FIFO_SIZE 0x0a2b
//Bit 31:13     reserved
//Bit 12:0      cntl_luma_fifo_size             //unsigned, RW, default=128

#define DOLBY_TOP1_RANGE_MAP_Y 0x0a2c
//Bit 31:23     cntl_din_offset_y               //unsigned, RW, default=0
//Bit 22:15     cntl_range_map_coef_y           //unsigned, RW, default=0
//Bit 14        reserved
//Bit 13:10     cntl_range_map_sr_y             //unsigned, RW, default=0
//Bit 9:1       cntl_dout_offset_y              //unsigned, RW, default=0
//Bit 0         cntl_range_map_en_y             //unsigned, RW, default=0

#define DOLBY_TOP1_RANGE_MAP_CB 0x0a2d
//Bit 31:23     cntl_din_offset_cb               //unsigned, RW, default=0
//Bit 22:15     cntl_range_map_coef_cb           //unsigned, RW, default=0
//Bit 14        reserved
//Bit 13:10     cntl_range_map_sr_cb             //unsigned, RW, default=0
//Bit 9:1       cntl_dout_offset_cb              //unsigned, RW, default=0
//Bit 0         cntl_range_map_en_cb             //unsigned, RW, default=0

#define DOLBY_TOP1_RANGE_MAP_CR 0x0a2e
//Bit 31:23     cntl_din_offset_cr               //unsigned, RW, default=0
//Bit 22:15     cntl_range_map_coef_cr           //unsigned, RW, default=0
//Bit 14        reserved
//Bit 13:10     cntl_range_map_sr_cr             //unsigned, RW, default=0
//Bit 9:1       cntl_dout_offset_cr              //unsigned, RW, default=0
//Bit 0         cntl_range_map_en_cr             //unsigned, RW, default=0
#define DOLBY_TOP1_URGENT_CTRL 0x0a2f
//Bit 31:16     cntl_urgent_ctrl_luma           //unsigned, RW, default=0
//Bit 15:0      cntl_urgent_ctrl_chroma         //unsigned, RW, default=0
#define DOLBY_TOP1_GEN_REG3 0x0a30
//Bit 31:27     reserved
//Bit 26        cntl_f0_stride32aligned2        //unsigned, RW, default=0
//Bit 25        cntl_f0_stride32aligned1        //unsigned, RW, default=0
//Bit 24        cntl_f0_stride32aligned0        //unsigned, RW, default=0
//Bit 23:22     cntl_f0_cav_blk_mode2           //unsigned, RW, default=0
//Bit 21:20     cntl_f0_cav_blk_mode1           //unsigned, RW, default=0
//Bit 19:18     cntl_f0_cav_blk_mode0           //unsigned, RW, default=0
//Bit 17:16     cntl_abort_mode                 //unsigned, RW, default=0
//Bit 15:14     cntl_burst_len2                 //unsigned, RW, default=2
//Bit 13:12     cntl_burst_len1                 //unsigned, RW, default=2
//Bit 11:10     cntl_dbg_mode                   //unsigned, RW, default=0
//Bit 9:8       cntl_bits_mode                  //unsigned, RW, default=0
//Bit 7         reserved
//Bit 6:4       cntl_blk_len                    //unsigned, RW, default=3
//Bit 3         reserved
//Bit 2:1       cntl_burst_len0                 //unsigned, RW, default=2
//Bit 0         cntl_64bit_rev                  //unsigned, RW, default=1

#define DOLBY_TOP1_AXI_CMD_CNT 0x0a31
//Bit 31:0      dbg_axi_cmd_cnt_sel             //unsigned, RO, default=0

#define DOLBY_TOP1_AXI_RDAT_CNT 0x0a32
//Bit 31:0      dbg_axi_rdat_cnt_sel            //unsigned, RO, default=0

#define DOLBY_TOP1_FMT_CTRL            0x0a33
//Bit 31        cntl_cfmt_gclk_bit_dis              //unsigned, RW, default = 0;
//Bit 30        cntl_cfmt_soft_rst_bit              //unsigned, RW, default = 0;
//Bit 29        reserved
//Bit 28        cntl_chfmt_rpt_pix                  //unsigned, RW, default = 0;
//Bit 27:24     cntl_chfmt_ini_phase                //unsigned, RW, default = 0;
//Bit 23        cntl_chfmt_rpt_p0_en                //unsigned, RW, default = 0;
//Bit 22:21     cntl_chfmt_yc_ratio                 //unsigned, RW, default = 0;
//Bit 20        cntl_chfmt_en                       //unsigned, RW, default = 0;
//Bit 19        cntl_cvfmt_phase0_always_en         //unsigned, RW, default = 0;
//Bit 18        cntl_cvfmt_rpt_last_dis             //unsigned, RW, default = 0;
//Bit 17        cntl_cvfmt_phase0_nrpt_en           //unsigned, RW, default = 0;
//Bit 16        cntl_cvfmt_rpt_line0_en             //unsigned, RW, default = 0;
//Bit 15:12     cntl_cvfmt_skip_line_num            //unsigned, RW, default = 0;
//Bit 11:8      cntl_cvfmt_ini_phase                //unsigned, RW, default = 0;
//Bit 7:1       cntl_cvfmt_phase_step               //unsigned, RW, default = 0;
//Bit 0         cntl_cvfmt_en                       //unsigned, RW, default = 0;
#define DOLBY_TOP1_FMT_W               0x0a34
//Bit 31:29     reserved
//Bit 28:16     cntl_chfmt_w                        //unsigned, RW, default = 0;
//Bit 15:13     reserved
//Bit 12:0      cntl_cvfmt_w                        //unsigned, RW, default = 0;

#define DOLBY_TOP1_BADDR_Y 0x0a35
//Bit 31:0      cntl_f0_baddr_y                 //unsigned, RW, default=0
#define DOLBY_TOP1_BADDR_CB 0x0a36
//Bit 31:0      cntl_f0_baddr_cb                 //unsigned, RW, default=0
#define DOLBY_TOP1_BADDR_CR 0x0a37
//Bit 31:0      cntl_f0_baddr_cr                 //unsigned, RW, default=0
#define DOLBY_TOP1_STRIDE_0 0x0a38
//Bit 31:29     reserved
//Bit 28:16     cntl_f0_stride_cb               //unsigned, RW, default=256
//Bit 15:13     reserved
//Bit 12:0      cntl_f0_stride_y                //unsigned, RW, default=256

#define DOLBY_TOP1_STRIDE_1 0x0a39
//Bit 31:17     reserved
//Bit 16        cntl_f0_acc_mode                //unsigned, RW, default=0
//Bit 15:13     reserved
//Bit 12:0      cntl_f0_stride_cr               //unsigned, RW, default=256

#define DOLBY_TOP1_BADDR_Y_F1 0x0a3a
//Bit 31:0      cntl_f1_baddr_y                 //unsigned, RW, default=0

#define DOLBY_TOP1_BADDR_CB_F1 0x0a3b
//Bit 31:0      cntl_f1_baddr_cb                 //unsigned, RW, default=0

#define DOLBY_TOP1_BADDR_CR_F1 0x0a3c
//Bit 31:0      cntl_f1_baddr_cr                 //unsigned, RW, default=0
#define DOLBY_TOP1_STRIDE_0_F1 0x0a3d
//Bit 31:29     reserved
//Bit 28:16     cntl_f1_stride_cb               //unsigned, RW, default=256
//Bit 15:13     reserved
//Bit 12:0      cntl_f1_stride_y                //unsigned, RW, default=256

#define DOLBY_TOP1_STRIDE_1_F1 0x0a3e
//Bit 31:27     reserved
//Bit 26        cntl_f1_stride32aligned2        //unsigned, RW, default=0
//Bit 25        cntl_f1_stride32aligned1        //unsigned, RW, default=0
//Bit 24        cntl_f1_stride32aligned0        //unsigned, RW, default=0
//Bit 23:22     cntl_f1_cav_blk_mode2           //unsigned, RW, default=0
//Bit 21:20     cntl_f1_cav_blk_mode1           //unsigned, RW, default=0
//Bit 19:18     cntl_f1_cav_blk_mode0           //unsigned, RW, default=0
//Bit 17        reserved
//Bit 16        cntl_f1_acc_mode                //unsigned, RW, default=0
//Bit 15:13     reserved
//Bit 12:0      cntl_f1_stride_cr               //unsigned, RW, default=256

/***************DV TOP1 REGS END **********************/

/***************DV TOP2 REGS START **********************/

#define DOLBY_TOP2_PIC_SIZE     0x0c00
//Bit 31:16 reg_pic_vsize       //unsigned, RW, default=1080, dos src pic vsize
//Bit 15:0  reg_pic_hsize       //unsigned, RW, default=1920, dos src pic hsize

#define DOLBY_TOP2_RDMA_CTRL    0x0c01
//Bit 31    reg_rdma_sw_rst     //unsigned, RW, default=0, sw rst for rdma
//Bit 30    reg_rdma_on         //unsigned, RW, default=1, top2 rdma on
//Bit 29    reg_rdma_shdw_rst   //unsigned, RW, default=0, used in rdma shadow, to config 2nd time
//Bit 28    reg_rdma_shdw_en    //unsigned, RW, default=0, shadow_en, used for change rdma_num/size
//Bit 27:11 reserved
//Bit 10:8  reg_rdma1_num       //unsigned, RW, default=2, rdma1 lut num
//Bit 7:5   reserved
//Bit 4:0   reg_rdma_num        //unsigned, RW, default=12,rdma1+rdma2 lut num
#define DOLBY_TOP2_RDMA_SIZE0 0x0c02
//Bit 31:16 reg_rdma_size0      //unsigned, RW, default=0, rdma lut0 size
//Bit 15:0  reg_rdma_size1      //unsigned, RW, default=0, rdma lut1 size

#define DOLBY_TOP2_RDMA_SIZE1 0x0c03
//Bit 31:16 reg_rdma_size2      //unsigned, RW, default=0, rdma lut2 size
//Bit 15:0  reg_rdma_size3      //unsigned, RW, default=0, rdma lut3 size

#define DOLBY_TOP2_RDMA_SIZE2 0x0c04
//Bit 31:16 reg_rdma_size4      //unsigned, RW, default=0, rdma lut4 size
//Bit 15:0  reg_rdma_size5      //unsigned, RW, default=0, rdma lut5 size

#define DOLBY_TOP2_RDMA_SIZE3 0x0c05
//Bit 31:16 reg_rdma_size6      //unsigned, RW, default=0, rdma lut6 size
//Bit 15:0  reg_rdma_size7      //unsigned, RW, default=0, rdma lut7 size

#define DOLBY_TOP2_RDMA_SIZE4 0x0c06
//Bit 31:16 reg_rdma_size8      //unsigned, RW, default=0, rdma lut8 size
//Bit 15:0  reg_rdma_size9      //unsigned, RW, default=0, rdma lut9 size
#define DOLBY_TOP2_RDMA_SIZE5 0x0c07
//Bit 31:16 reg_rdma_size10      //unsigned, RW, default=0, rdma lut10 size
//Bit 15:0  reg_rdma_size11      //unsigned, RW, default=0, rdma lut11 size

#define DOLBY_TOP2_PYRD_CTRL    0x0c08
//Bit 31    reg_pyrdmif_sw_rst  //unsigned, RW, default=0, sw rst for pyramif rdmif
//Bit 30    reg_mmu_sw_rst  //unsigned, RW, default=0, sw rst for pyramif rdmif
//Bit 29:20 reserved
//Bit 19:4  reg_gclk_ctrl       //unsigned, RW, default=0, clk gating ctrl
//Bit 3     reserved
//Bit 2     reg_py_int_tri
//unsigned, RW, default=0, last frame dolby done int to trigger next frame dolby pyramid read
//Bit 1:0   reg_py_level        //default=1, pymid level, src pic>512x288 ? 1 : 0, bit1=1 no pyramid

#define DOLBY_TOP2_PYRD_BADDR1  0x0c09
//Bit 31:0 reg_py_baddr1        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP2_PYRD_BADDR2  0x0c0a
//Bit 31:0 reg_py_baddr2        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP2_PYRD_BADDR3  0x0c0b
//Bit 31:0 reg_py_baddr3        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP2_PYRD_BADDR4  0x0c0c
//Bit 31:0 reg_py_baddr4        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP2_PYRD_BADDR5  0x0c0d
//Bit 31:0 reg_py_baddr5        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP2_PYRD_BADDR6  0x0c0e
//Bit 31:0 reg_py_baddr6        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP2_PYRD_BADDR7  0x0c0f
//Bit 31:0 reg_py_baddr7        //unsigned, RW, default=0, py1 baddr

#define DOLBY_TOP2_PYRD_STRIDE12 0x0c10
//Bit 31:29 reserved
//Bit 28:16 reg_py_stride2      //unsigned, RW, default=512
//Bit 15:13 reserved
//Bit 12:0  reg_py_stride1      //unsigned, RW, default=1024
#define DOLBY_TOP2_PYRD_STRIDE34 0x0c11
//Bit 31:29 reserved
//Bit 28:16 reg_py_stride4      //unsigned, RW, default=128
//Bit 15:13 reserved
//Bit 12:0  reg_py_stride3      //unsigned, RW, default=256
#define DOLBY_TOP2_PYRD_STRIDE56 0x0c12
//Bit 31:29 reserved
//Bit 28:16 reg_py_stride6      //unsigned, RW, default=32
//Bit 15:13 reserved
//Bit 12:0  reg_py_stride5      //unsigned, RW, default=64

#define DOLBY_TOP2_PYRD_STRIDE7  0x0c13
//Bit 31:15 reg_py_urgent       //unsigned, RW, default=0, pyramid rdmif urgent
//Bit 14:13 reserved
//Bit 12:0  reg_py_stride7      //unsigned, RW, default=16

#define DOLBY_TOP2_CTRL0    0x0c14
//Bit 31    reg_sw_reset     //unsigned, RW, default=0, sw rst for whole module, used as rst_n
//Bit 30:24 reserved
//Bit 23:20 reg_int_sel
//default=0, 0=close,1=open, bit0: err_int(din_hend), bit1: err_int(rdma end),
//bit2:int(rdma_done), bit3:int(core2_done)
//Bit 19:8  reg_int_line_num //default=0,reg_int_sel=0,err int trigger when din_hcnt=line_num
//Bit 7:5   reserved
//Bit 4     reg_rdmif_arsec  //unsigned, RW, default=0, pyrdmif arsec
//Bit 3:2   reserved
//Bit 1:0   reg_src_sel     // unsigned, RW, default=0, 0=vdin,1=vpp,2=di

#define DOLBY_TOP2_RO_0 0x0c15
//Bit 31:0  ro_dbg0         //unsigned, RO, default=0
#define DOLBY_TOP2_RO_1 0x0c16
//Bit 31:0  ro_dbg1         //unsigned, RO, default=0
#define DOLBY_TOP2_RO_2 0x0c17
//Bit 31:0  ro_dbg2         //unsigned, RO, default=0
#define DOLBY_TOP2_RO_3 0x0c18
//Bit 31:0  ro_dbg3         //unsigned, RO, default=0
#define DOLBY_TOP2_RO_4 0x0c19
//Bit 31:0  ro_dbg4         //unsigned, RO, default=0
#define DOLBY_TOP2_RO_5 0x0c1a
//Bit 31:0  ro_dbg5         //unsigned, RO, default=0

#define DOLBY_TOP2_PYRMIF_MMU_ADDR 0x0c1e
//Bit 31:0 reg_pyrmif_mmu_addr  //unsigned, RW, default=0
#define DOLBY_TOP2_PYRMIF_MMU_DATA 0x0c1f
//Bit 31:0 reg_pyrmif_mmu_data  //unsigned, RW, default=0

/***************DV TOP2 REGS END **********************/

/***************DV WRAP REGS START **********************/

#define VPU_DOLBY_WRAP_GCLK                           0X0900
//Bit 31:24         reserved                          // unsigned
//Bit 23:16         reg_sw_rst                        // unsigned ,    RW, default = 0
//Bit 15:0          reg_gclk_ctrl                     // unsigned ,    RW, default = 0

#define VPU_DOLBY_WRAP_CTRL                           0X0901
//Bit 31            reg_dv_core2_byp                  // unsigned ,    RW, default = 0
//Bit 30            reg_dv_sec_ctrl                   // unsigned ,    RW, default = 0
//Bit 29:20         reserved                          // unsigned
//Bit 19:18         reg_ovlp_in_1to2_en               // unsigned ,    RW, default = 3
//Bit 17:16         reg_ovlp_in_2to1_en               // unsigned ,    RW, default = 3
//Bit 15:14         reg_dolby_out_sel                 // unsigned ,    RW, default = 0
//Bit 13:12         reg_dolby_in_sel                  // unsigned ,    RW, default = 0
//Bit 11:4          reg_vdin_p2s_ovlp_size            // unsigned ,    RW, default = 20
//Bit 3:2           reg_dv_in_sel                     // unsigned ,    RW, default = 0
//Bit 1:0           reg_dv_out_sel                    // unsigned ,    RW, default = 0

#define VPU_DOLBY_WRAP_P2S                            0X0902
//Bit 31            reserved                          // unsigned
//Bit 30:29         reg_vdin_p2s_mode                 // unsigned ,  RW,  default = 0;
//Bit 28:16         reg_vdin_p2s_vsize                // unsigned ,  RW,  default = 480;
//Bit 15:13         reserved                          // unsigned
//Bit 12:0          reg_vdin_p2s_hsize                // unsigned ,  RW,  default = 720;

#define VPU_DOLBY_WRAP_S2P                            0X0903
//Bit 31            reserved                          // unsigned
//Bit 30:29         reg_vdin_s2p_mode                 // unsigned ,  RW,  default = 0;
//Bit 28:16         reg_vdin_s2p_vsize                // unsigned ,  RW,  default = 480;
//Bit 15:13         reserved                          // unsigned
//Bit 12:0          reg_vdin_s2p_hsize                // unsigned ,  RW,  default = 720;

#define VPU_DOLBY_WRAP_IRQ                            0X0904
//Bit 31:21         reserved                          // unsigned
//Bit 20:19         reg_detunnel_en                   // unsigned ,  RW,  default = 0;
//Bit 18:17         reg_detunnel_u_start              // unsigned ,  RW,  default = 0;
//Bit 16            reg_irq_flg_sel                   // unsigned ,  RW,  default = 0;
//Bit 15:8          reg_irq_clr                       // unsigned ,  RW,  default = 0;
//Bit 7:0           reg_irq_en                        // unsigned ,  RW,  default = 0;

#define VPU_DOLBY_WRAP_DTNL                           0X0905
//Bit 31            reserved                          // unsigned
//Bit 30:18         reg_detunnel_hsize                // unsigned ,  RW,  default = 1920;
//Bit 17:0          reg_detunnel_sel                  // unsigned ,  RW,  default = 34658;

#define VPU_DOLBY_WRAP_OVLP                           0X0910
//Bit 31            reg_ovlp_en                       // unsigned ,    RW, default = 0;
//Bit 30:29         reserved                          // unsigned
//Bit 28:16         reg_ovlp_size                     // unsigned ,  RW,  default = 64;
//Bit 15:11         reserved                          // unsigned
//Bit 10            reg_ovlp_win3_en                  // unsigned ,  RW,  default = 0;
//Bit 9             reg_ovlp_win2_en                  // unsigned ,  RW,  default = 0;
//Bit 8             reg_ovlp_win1_en                  // unsigned ,  RW,  default = 0;
//Bit 7             reg_ovlp_win0_en                  // unsigned ,  RW,  default = 0;
//Bit 6:0           reg_hold_line_num                 // unsigned ,  RW,  default = 4;

#define VPU_DOLBY_WRAP_OVLP_SIZE                      0X0911
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_hsize_in               // unsigned ,  RW,  default = 1280;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_vsize_in               // unsigned ,  RW,  default = 720;

#define VPU_DOLBY_WRAP_OVLP_WIN0_SIZE                 0X0912
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win0_hsize             // unsigned ,  RW,  default = 1280;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win0_vsize             // unsigned ,  RW,  default = 720;

#define VPU_DOLBY_WRAP_OVLP_WIN0_H                    0X0913
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win0_bgn_h             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win0_end_h             // unsigned ,  RW,  default = 1279;

#define VPU_DOLBY_WRAP_OVLP_WIN0_V                    0X0914
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win0_bgn_v             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win0_end_v             // unsigned ,  RW,  default = 719;

#define VPU_DOLBY_WRAP_OVLP_WIN1_SIZE                 0X0915
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win1_hsize             // unsigned ,  RW,  default = 1280;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win1_vsize             // unsigned ,  RW,  default = 720;

#define VPU_DOLBY_WRAP_OVLP_WIN1_H                    0X0916
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win1_bgn_h             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win1_end_h             // unsigned ,  RW,  default = 1279;

#define VPU_DOLBY_WRAP_OVLP_WIN1_V                    0X0917
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win1_bgn_v             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win1_end_v             // unsigned ,  RW,  default = 719;

#define VPU_DOLBY_WRAP_OVLP_WIN2_SIZE                 0X0918
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win2_hsize             // unsigned ,  RW,  default = 1280;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win2_vsize             // unsigned ,  RW,  default = 720;

#define VPU_DOLBY_WRAP_OVLP_WIN2_H                   0X0919
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win2_bgn_h             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win2_end_h             // unsigned ,  RW,  default = 1279;

#define VPU_DOLBY_WRAP_OVLP_WIN2_V                   0X091a
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win2_bgn_v             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win2_end_v             // unsigned ,  RW,  default = 719;

#define VPU_DOLBY_WRAP_OVLP_WIN3_SIZE                0X091b
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win3_hsize             // unsigned ,  RW,  default = 1280;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win3_vsize             // unsigned ,  RW,  default = 720;

#define VPU_DOLBY_WRAP_OVLP_WIN3_H                   0X091c
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win3_bgn_h             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win3_end_h             // unsigned ,  RW,  default = 1279;

#define VPU_DOLBY_WRAP_OVLP_WIN3_V                   0X091d
//Bit 31:29          reserved                        // unsigned
//Bit 28:16          reg_ovlp_win3_bgn_v             // unsigned ,  RW,  default = 0;
//Bit 15:13          reserved                        // unsigned
//Bit 12:0           reg_ovlp_win3_end_v             // unsigned ,  RW,  default = 719;

#define VPU_DOLBY_WRAP_PROB_CTRL                    0X091e
//Bit 31:8          reserved                        // unsigned
//Bit 7:2           reg_prob_sel                    // unsigned ,  RW,  default = 0;
//Bit 1:1           reg_prob_clr                    // unsigned ,  RW,  default = 0;
//Bit 0:0           reg_prob_en                     // unsigned ,  RW,  default = 0;

#define VPU_DOLBY_WRAP_PROB_SIZE                    0X091f
//Bit 31:16         reg_prob_vsize                  // unsigned ,  RW,  default = 720;
//Bit 15:0          reg_prob_hsize                  // unsigned ,  RW,  default = 1280;

#define VPU_DOLBY_WRAP_PROB_WIN                    0X0920
//Bit 31:16         reg_prob_ypos                   // unsigned ,  RW,  default = 0;
//Bit 15:0          reg_prob_xpos                   // unsigned ,  RW,  default = 0;

#define VPU_DOLBY_WRAP_IDAT_SWAP                    0X0921
//Bit 31:18         reserved                        // unsigned
//Bit 17:0          reg_dv_idat_swap                // unsigned ,  RW,  default = 181896;

#define VPU_DOLBY_WRAP_ODAT_SWAP                    0X0922
//Bit 31:18         reserved                        // unsigned
//Bit 17:0          reg_dv_odat_swap                // unsigned ,  RW,  default = 181896;

#define VPU_DOLBY_WRAP_VDIN_CTRL                     0X0928
//Bit 31:10         reserved                         // unsigned
//Bit 9:8           reg_vdin_hs_ppc_out              // unsigned ,  RW,  default = 1;
//Bit 7:6           reg_vdin_hs_slc_byp              // unsigned ,  RW,  default = 3;
//Bit 5:4           reg_vdin_hs_slc_out              // unsigned ,  RW,  default = 1;
//Bit 3:2           reg_vdin_hs_byp_in               // unsigned ,  RW,  default = 3;
//Bit 1:0           reg_vdin_hs_ppc_in               // unsigned ,  RW,  default = 1;
//
#define VPU_DOLBY_WRAP_RO_IRQ                        0X0930
//Bit 31:8         reserved                          // unsigned
//Bit 7:0          ro_irq_status                     // unsigned ,  RO,  default = 0;

#define VPU_DOLBY_WRAP_RO_DV0                        0X0931
//Bit 31:0         ro_dolby_debug0                  // unsigned ,  RO,  default = 0;

#define VPU_DOLBY_WRAP_RO_DV1                        0X0932
//Bit 31:0         ro_dolby_debug1                  // unsigned ,  RO,  default = 0;

#define VPU_DOLBY_WRAP_RO_DV2                        0X0933
//Bit 31:0         ro_dolby_debug2                  // unsigned ,  RO,  default = 0;

#define VPU_DOLBY_WRAP_RO_DAT0                        0X0934
//Bit 31:0         ro_prob_dat0                     // unsigned ,  RO,  default = 0;

#define VPU_DOLBY_WRAP_RO_DAT1                         0X0935
//Bit 31:0         ro_prob_dat1                     // unsigned ,  RO,  default = 0;

#define VPU_DOLBY_WRAP_RO_DAT2                         0X0936
//Bit 31:0         ro_prob_dat2                     // unsigned ,  RO,  default = 0;

/***************DV TOP2 REGS END **********************/

#define VDIN0_CORE_CTRL             0x0200
#define VDIN1_CORE_CTRL             0x0400

#define DOLBY5_CORE1_REG_BASE       0x0b00
#define DOLBY5_CORE1b_REG_BASE      0x0bd1
#define DOLBY5_CORE2_REG_BASE0      0x0d00
#define DOLBY5_CORE2_REG_BASE1      0x0e00
#define DOLBY5_CORE2_REG_BASE2      0x0f00

#define T3X_VD_PROC_BYPASS_CTRL     0x2811
#define T3X_VPP_DOLBY_CTRL          0x2501

#define T3X_VD1_S0_DV_BYPASS_CTRL   0x2824
#define T3X_VD1_S1_DV_BYPASS_CTRL   0x2826

#define VIUB_MISC_CTRL0             0x2006
#define DI_TOP_CTRL1                0x17d3
#define DI_TOP_POST_CTRL            0x17c6

#define VDIN_TOP_CTRL               0x0101
//Bit 31:26        reserved
//Bit 25           reg_vdin0to1_en                      // unsigned ,    RW, default = 0
//Bit 24:23        reg_hsk_mode                         // unsigned ,    RW, default = 0
//Bit 22:21        reg_vdin_out_sel                     // unsigned ,    RW, default = 0
//Bit 20           reg_dvpath_sel                       // unsigned ,    RW, default = 0
//Bit 19           reg_outpath_sel                      // unsigned ,    RW, default = 0
//Bit 18:13        reg_meas0_pol_ctrl                   // unsigned ,    RW, default = 0
//Bit 12: 7        reg_meas1_pol_ctrl                   // unsigned ,    RW, default = 0
//Bit  6: 2        reg_reset                            // unsigned ,    RW, default = 0
//Bit  1: 0        reg_line_int_sel                     // unsigned ,    RW, default = 0

#endif