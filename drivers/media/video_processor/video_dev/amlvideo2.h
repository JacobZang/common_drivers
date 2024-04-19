/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef AMLVIDEO2_H_
#define AMLVIDEO2_H_

#ifndef CONFIG_AMLOGIC_MEDIA_TVIN
enum port_vpp_e {
	PORT_VPP0_OSD_VIDEO  = 0,/* vpp0 osd+video */
	PORT_VPP0_VIDEO_ONLY = 1,/* vpp0 video only */
	PORT_VPP1_OSD_VIDEO  = 2,/* vpp1 osd+video */
	PORT_VPP1_VIDEO_ONLY = 3,/* vpp2 video only */
	PORT_VPP2_OSD_VIDEO  = 4,/* vpp2 osd+video */
	PORT_VPP2_VIDEO_ONLY = 5,/* vpp2 video only */
	PORT_VPP0_VIDEO_ONLY_NO_PQ = 6,/* vpp0 video only no pq*/

	PORT_VPP0_OSD1_ONLY,/* vpp0 osd1 only */
	PORT_VPP0_OSD2_ONLY,/* vpp0 osd2 only */
	PORT_VPP0_OSD3_ONLY,/* vpp0 osd3 only */

	PORT_VPP1_OSD1_ONLY,/* vpp1 osd1 only */
	PORT_VPP1_OSD2_ONLY,/* vpp1 osd2 only */
	PORT_VPP1_OSD3_ONLY,/* vpp1 osd3 only */

	PORT_VPP2_OSD1_ONLY,/* vpp2 osd1 only */
	PORT_VPP2_OSD2_ONLY,/* vpp2 osd2 only */
	PORT_VPP2_OSD3_ONLY,/* vpp2 osd3 only */

	PORT_VPP_MAX
};
#endif

/* void vf_inqueue(struct vframe_s *vf, const char *receiver); */
void get_vdx_axis(u32 index, int *buf);
void get_vdx_real_axis(u32 index, int *buf);

#endif /* AMLVIDEO2_H_ */
