/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) WITH Linux-syscall-note */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef HDMI_RX_H_
#define HDMI_RX_H_

struct _hdcp_ksv {
	int bksv0;
	int bksv1;
};

struct pd_infoframe_s {
	unsigned int HB;
	unsigned int PB0;
	unsigned int PB1;
	unsigned int PB2;
	unsigned int PB3;
	unsigned int PB4;
	unsigned int PB5;
	unsigned int PB6;
};

enum hdcp14_key_mode_e {
	NORMAL_MODE,
	SECURE_MODE,
};

/* source product descriptor infoFrame  - 0x83 */
//0x00 "unknown",
//0x01 "Digital STB",
//0x02 "DVD player",
//0x03 "D-VHS",
//0x04 "HDD Videorecorder",
//0x05 "DVC",
//0x06 "DSC",
//0x07 "Video CD",
//0x08 "Game",
//0x09 "PC general",
//0x0A "Blu-Ray Disc (BD)",
//0x0B "Super Audio CD",
//0x0C "HD DVD",
//0x0D "PMP",
//0x1A "FREESYNC"
struct spd_infoframe_st {
	unsigned char pkttype;
	unsigned char version;
	unsigned char length;
	//u8 rsd;  //note: T3 has not this byte. T5 has it.
	unsigned char checksum;
	//u8 ieee_oui[3]; //data[1:3]
	union meta_u {
		struct freesync_st {
			/*PB1-3*/
			unsigned int ieee:24;
			unsigned int rsvd:8;
			unsigned char rsvd1;
			/*PB6*/
			unsigned char supported:1;
			unsigned char enabled:1;
			unsigned char active:1;
			//u8 cs_active:1;
			unsigned char rsvd2:5;
			//u8 ld_disable:1;
			//u8 rsvd3:3;
			unsigned char min_frame_rate;
			unsigned char max_frame_rate;
			/*pb9-pb27*/
			unsigned char data[19];
		} freesync;
		unsigned char data[27];
		struct spd_data_st {
			/*Vendor Name Character*/
			unsigned char vendor_name[8];
			/*Product Description Character*/
			unsigned char product_des[16];
			/*byte 25*/
			unsigned char source_info;
			unsigned char rsvd[3];
		} spddata;
	} des_u;
};

struct hdmirx_hpd_info {
	int signal;
	int port;
};

/* AVI infoFrame packet - 0x82 */
struct avi_infoframe_st {
	unsigned char pkttype;
	unsigned char version;
	unsigned char length;
	/*PB0*/
	unsigned char checksum;
	union cont_u {
		struct v1_st {
			/*byte 1*/
			unsigned char scaninfo:2;		/* S1,S0 */
			unsigned char barinfo:2;		/* B1,B0 */
			unsigned char activeinfo:1;		/* A0 */
			unsigned char colorindicator:2;		/* Y1,Y0 */
			unsigned char rev0:1;
			/*byte 2*/
			unsigned char fmt_ration:4;		/* R3-R0 */
			unsigned char pic_ration:2;		/* M1-M0 */
			unsigned char colorimetry:2;		/* C1-C0 */
			/*byte 3*/
			unsigned char pic_scaling:2;		/* SC1-SC0 */
			unsigned char rev1:6;
			/*byte 4*/
			unsigned char rev2:8;
			/*byte 5*/
			unsigned char rev3:8;
		} v1;
		struct v4_st { /* v2=v3=v4 */
			/*byte 1*/
			unsigned char scaninfo:2;		/* S1,S0 */
			unsigned char barinfo:2;		/* B1,B0 */
			unsigned char activeinfo:1;		/* A0 1 */
			unsigned char colorindicator:3;		/* Y2-Y0 */
			/*byte 2*/
			unsigned char fmt_ration:4;		/* R3-R0 */
			unsigned char pic_ration:2;		/* M1-M0 */
			unsigned char colorimetry:2;		/* C1-C0 */
			/*byte 3*/
			unsigned char pic_scaling:2;		/* SC1-SC0 */
			unsigned char qt_range:2;		/* Q1-Q0 */
			unsigned char ext_color:3;		/* EC2-EC0 */
			unsigned char it_content:1;		/* ITC */
			/*byte 4*/
			unsigned char vic:8;			/* VIC7-VIC0 */
			/*byte 5*/
			unsigned char pix_repeat:4;		/* PR3-PR0 */
			unsigned char content_type:2;		/* CN1-CN0 */
			unsigned char ycc_range:2;		/* YQ1-YQ0 */
		} v4;
	} cont;
	/*byte 6,7*/
	unsigned int line_num_end_topbar:16;	/*littel endian can use*/
	/*byte 8,9*/
	unsigned int line_num_start_btmbar:16;
	/*byte 10,11*/
	unsigned int pix_num_left_bar:16;
	/*byte 12,13*/
	unsigned int pix_num_right_bar:16;
	/* byte 14 */
	unsigned char additional_colorimetry;
};

#define HDMI_IOC_MAGIC 'H'
#define HDMI_IOC_HDCP_ON	_IO(HDMI_IOC_MAGIC, 0x01)
#define HDMI_IOC_HDCP_OFF	_IO(HDMI_IOC_MAGIC, 0x02)
#define HDMI_IOC_EDID_UPDATE	_IO(HDMI_IOC_MAGIC, 0x03)
#define HDMI_IOC_PC_MODE_ON	_IO(HDMI_IOC_MAGIC, 0x04)
#define HDMI_IOC_PC_MODE_OFF	_IO(HDMI_IOC_MAGIC, 0x05)
#define HDMI_IOC_HDCP22_AUTO	_IO(HDMI_IOC_MAGIC, 0x06)
#define HDMI_IOC_HDCP22_FORCE14	_IO(HDMI_IOC_MAGIC, 0x07)
#define HDMI_IOC_HDCP_GET_KSV	_IOR(HDMI_IOC_MAGIC, 0x09, struct _hdcp_ksv)
#define HDMI_IOC_HDMI_20_SET	_IO(HDMI_IOC_MAGIC, 0x08)
#define HDMI_IOC_PD_FIFO_PKTTYPE_EN	_IOW(HDMI_IOC_MAGIC, 0x0a, unsigned int)
#define HDMI_IOC_PD_FIFO_PKTTYPE_DIS	_IOW(HDMI_IOC_MAGIC, 0x0b, unsigned int)
#define HDMI_IOC_GET_PD_FIFO_PARAM	_IOWR(HDMI_IOC_MAGIC, 0x0c, struct pd_infoframe_s)
#define HDMI_IOC_HDCP14_KEY_MODE	_IOR(HDMI_IOC_MAGIC, 0x0d, enum hdcp14_key_mode_e)
#define HDMI_IOC_HDCP22_NOT_SUPPORT	_IO(HDMI_IOC_MAGIC, 0x0e)
#define HDMI_IOC_SET_AUD_SAD		_IOW(HDMI_IOC_MAGIC, 0x0f, int)
#define HDMI_IOC_GET_AUD_SAD		_IOR(HDMI_IOC_MAGIC, 0x10, int)
#define HDMI_IOC_GET_SPD_SRC_INFO	_IOR(HDMI_IOC_MAGIC, 0x11, struct spd_infoframe_st)
#define HDMI_5V_PIN_STATUS		_IOR(HDMI_IOC_MAGIC, 0x12, unsigned int)
#define HDMI_IOC_EDID_UPDATE_WITH_PORT  _IOW(HDMI_IOC_MAGIC, 0x13, unsigned char)
#define HDMI_IOC_5V_WAKE_UP_ON _IO(HDMI_IOC_MAGIC, 0x13)
#define HDMI_IOC_5V_WAKE_UP_OFF _IO(HDMI_IOC_MAGIC, 0x14)
#define HDMI_IOC_SET_HPD	_IOW(HDMI_IOC_MAGIC, 0x15, struct hdmirx_hpd_info)
#define HDMI_IOC_GET_HPD	_IOR(HDMI_IOC_MAGIC, 0x16, struct hdmirx_hpd_info)
#define HDMI_IOC_GET_AVI_INFO	_IOR(HDMI_IOC_MAGIC, 0x17, struct avi_infoframe_st)
#endif

