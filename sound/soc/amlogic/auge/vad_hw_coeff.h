/* SPDX-License-Identifier: GPL-2.0 */

/*
 * VAD coeff parameters
 *
 * Copyright (C) 2019 Amlogic,inc
 *
 */

#ifndef __VAD_HW_COEFF_H__
#define __VAD_HW_COEFF_H__

/* parameters for downsample and emphasis filter */
static int vad_de_coeff[] = {
	0x31007f05,
	0x000003e6,
	0x0000070c,
	0x0716071a,
	0x071207fa,
	0x07d407b1,
	0x07a907d0,
	0x0728064c,
	0x06750671,
	0x075906b1,
	0x059404b4,
	0x04be0785,
	0x044c0361,
	0x024d0263,
	0x026a0000,
};

static int vad_ram_coeff[] = {
	0x00003A3D,
	0x00003A42,
	0x00003A50,
	0x00003A67,
	0x00003A87,
	0x00003AB0,
	0x00003AE2,
	0x00003B1D,
	0x00003B61,
	0x00003BAF,
	0x00003C05,
	0x00003C64,
	0x00003CCC,
	0x00003D3C,
	0x00003DB5,
	0x00003E37,
	0x00003EC2,
	0x00003F55,
	0x00003FF0,
	0x0000284A,
	0x0000289F,
	0x000028F9,
	0x00002957,
	0x000029B9,
	0x00002A1F,
	0x00002A88,
	0x00002AF5,
	0x00002B66,
	0x00002BDA,
	0x00002C52,
	0x00002CCE,
	0x00002D4D,
	0x00002DCF,
	0x00002E54,
	0x00002EDC,
	0x00002F68,
	0x00002FF6,
	0x00001844,
	0x0000188E,
	0x000018D9,
	0x00001926,
	0x00001974,
	0x000019C3,
	0x00001A14,
	0x00001A65,
	0x00001AB8,
	0x00001B0C,
	0x00001B60,
	0x00001BB6,
	0x00001C0C,
	0x00001C63,
	0x00001CBB,
	0x00001D14,
	0x00001D6D,
	0x00001DC7,
	0x00001E22,
	0x00001E7C,
	0x00001ED8,
	0x00001F34,
	0x00001F90,
	0x00001FEC,
	0x00000824,
	0x00000853,
	0x00000881,
	0x000008AF,
	0x000008DE,
	0x0000090C,
	0x0000093B,
	0x00000969,
	0x00000997,
	0x000009C5,
	0x000009F3,
	0x00000A20,
	0x00000A4E,
	0x00000A7B,
	0x00000AA7,
	0x00000AD4,
	0x00000B00,
	0x00000B2C,
	0x00000B57,
	0x00000B82,
	0x00000BAD,
	0x00000BD7,
	0x00000C00,
	0x00000C29,
	0x00000C52,
	0x00000C7A,
	0x00000CA1,
	0x00000CC8,
	0x00000CEE,
	0x00000D13,
	0x00000D38,
	0x00000D5C,
	0x00000D7F,
	0x00000DA2,
	0x00000DC3,
	0x00000DE4,
	0x00000E05,
	0x00000E24,
	0x00000E42,
	0x00000E60,
	0x00000E7C,
	0x00000E98,
	0x00000EB3,
	0x00000ECD,
	0x00000EE6,
	0x00000EFE,
	0x00000F15,
	0x00000F2B,
	0x00000F40,
	0x00000F54,
	0x00000F66,
	0x00000F78,
	0x00000F89,
	0x00000F99,
	0x00000FA7,
	0x00000FB5,
	0x00000FC1,
	0x00000FCD,
	0x00000FD7,
	0x00000FE0,
	0x00000FE8,
	0x00000FEF,
	0x00000FF4,
	0x00000FF9,
	0x00000FFC,
	0x00000FFF,
	0x00000FFF
};

#endif
