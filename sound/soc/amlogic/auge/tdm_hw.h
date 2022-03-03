/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __AML_TDM_HW_H__
#define __AML_TDM_HW_H__

#include "audio_io.h"
#include "regs.h"

#define TDM_A	0
#define TDM_B	1
#define TDM_C	2
#define TDM_LB	3

#define LANE_MAX_2CH  2
#define LANE_MAX_4CH  4
#define LANE_MAX_6CH  6
#define LANE_MAX_8CH  8
#define LANE_MAX_10CH 10

//	TODO: fix me, now based by tl1
enum tdmin_src {
	PAD_TDMINA_DIN = 0,
	PAD_TDMINB_DIN = 1,
	PAD_TDMINC_DIN = 2,
	PAD_TDMINA_D = 3,
	PAD_TDMINB_D = 4,
	PAD_TDMINC_D = 5,
	HDMIRX_I2S = 6,
	ACODEC_ADC = 7,
	TDMOUTA = 13,
	TDMOUTB = 14,
	TDMOUTC = 15,
	TDMIN_SRC_CNT = 16
};

struct pcm_setting {
	unsigned int pcm_mode;
	unsigned int sysclk;
	unsigned int sysclk_bclk_ratio;
	unsigned int bclk;
	unsigned int bclk_lrclk_ratio;
	unsigned int lrclk;
	unsigned int tx_mask;
	unsigned int rx_mask;
	unsigned int slots;
	unsigned int slot_width;
	unsigned int pcm_width;
	unsigned int lane_mask_out;
	unsigned int lane_mask_in;
	/* lane oe (out pad) mask */
	unsigned int lane_oe_mask_out;
	unsigned int lane_oe_mask_in;
	/* lane in selected from out, for intrenal loopback */
	unsigned int lane_lb_mask_in;

	/* eco or sclk_ws_inv */
	bool sclk_ws_inv;
};

void aml_tdm_enable(struct aml_audio_controller *actrl,
		    int stream, int index,
		    bool is_enable);

void aml_tdm_arb_config(struct aml_audio_controller *actrl);

void aml_tdm_fifo_reset(struct aml_audio_controller *actrl,
			int stream, int index);
void aml_tdmout_enable_gain(int tdmout_id, int en);

int tdmout_get_frddr_type(int bitwidth);

void aml_tdm_fifo_ctrl(struct aml_audio_controller *actrl,
		       int bitwidth, int stream,
		       int index, unsigned int fifo_id);

void aml_tdm_set_format(struct aml_audio_controller *actrl,
			struct pcm_setting *p_config,
			unsigned int clk_sel,
			unsigned int index,
			unsigned int fmt,
			unsigned int capture_active,
			unsigned int playback_active);

void aml_update_tdmin_skew(struct aml_audio_controller *actrl,
			   int idx, int skew);

void aml_update_tdmin_rev_ws(struct aml_audio_controller *actrl,
			     int idx, int is_rev);

void aml_tdm_set_slot_out(struct aml_audio_controller *actrl,
			  int index, int slots, int slot_width,
			  int force_oe, int oe_val);

void aml_tdm_set_slot_in(struct aml_audio_controller *actrl,
			 int index, int in_src, int slot_width);

void aml_update_tdmin_src(struct aml_audio_controller *actrl,
			  int index, enum tdmin_src in_src);

void tdmin_set_chnum_en(struct aml_audio_controller *actrl,
			int index, bool enable);

void aml_tdm_set_channel_mask(struct aml_audio_controller *actrl,
			      int stream, int index, int lanes, int mask);

void aml_tdm_set_lane_channel_swap(struct aml_audio_controller *actrl,
				   int stream, int index, int swap0, int swap1);

void aml_tdm_set_bclk_ratio(struct aml_audio_controller *actrl,
			    int clk_sel, int lrclk_hi, int bclk_ratio);

void aml_tdm_set_lrclkdiv(struct aml_audio_controller *actrl,
			  int clk_sel, int ratio);

void tdm_enable(int tdm_index, int is_enable);

void tdm_fifo_enable(int tdm_index, int is_enable);

void aml_tdmout_select_aed(bool enable, int tdmout_id);

void aml_tdmout_get_aed_info(int tdmout_id,
			     int *bitwidth, int *frddrtype);

void aml_tdm_mclk_pad_select(struct aml_audio_controller *actrl,
			     int mpad, int mpad_offset, int mclk_sel);
void aml_tdm_sclk_pad_select(struct aml_audio_controller *actrl,
			     int mpad_offset, int tdm_index, int clk_sel);

void i2s_to_hdmitx_ctrl(int tdm_index);
void aml_tdm_mute_playback(struct aml_audio_controller *actrl,
			   int index,
			   bool mute,
			   int lane_cnt);
void aml_tdm_mute_capture(struct aml_audio_controller *actrl,
			  int tdm_index, bool mute,
			  int lane_cnt);
void aml_tdm_out_reset(unsigned int tdm_id, int offset);
void aml_tdmout_auto_gain_enable(unsigned int tdm_id);
void aml_tdmout_set_gain(int tdmout_id, int value);
int aml_tdmout_get_gain(int tdmout_id);
void aml_tdmout_set_mute(int tdmout_id, int mute);
int aml_tdmout_get_mute(int tdmout_id);
#endif
