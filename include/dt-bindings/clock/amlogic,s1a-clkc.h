/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __S1A_CLKC_H
#define __S1A_CLKC_H

/*
 * CLKID index values
 */

#define CLKID_PLL_BASE			0
#define CLKID_FIXED_PLL_DCO		(CLKID_PLL_BASE + 0)
#define CLKID_FIXED_PLL			(CLKID_PLL_BASE + 1)
#define CLKID_FCLK50M_DIV40		(CLKID_PLL_BASE + 2)
#define CLKID_FCLK50M			(CLKID_PLL_BASE + 3)
#define CLKID_FCLK_DIV2_DIV		(CLKID_PLL_BASE + 4)
#define CLKID_FCLK_DIV2			(CLKID_PLL_BASE + 5)
#define CLKID_FCLK_DIV2P5_DIV		(CLKID_PLL_BASE + 6)
#define CLKID_FCLK_DIV2P5		(CLKID_PLL_BASE + 7)
#define CLKID_FCLK_DIV3_DIV		(CLKID_PLL_BASE + 8)
#define CLKID_FCLK_DIV3			(CLKID_PLL_BASE + 9)
#define CLKID_FCLK_DIV4_DIV		(CLKID_PLL_BASE + 10)
#define CLKID_FCLK_DIV4			(CLKID_PLL_BASE + 11)
#define CLKID_FCLK_DIV5_DIV		(CLKID_PLL_BASE + 12)
#define CLKID_FCLK_DIV5			(CLKID_PLL_BASE + 13)
#define CLKID_FCLK_DIV7_DIV		(CLKID_PLL_BASE + 14)
#define CLKID_FCLK_DIV7			(CLKID_PLL_BASE + 15)
#define CLKID_SYS_PLL_DCO		(CLKID_PLL_BASE + 16)
#define CLKID_SYS_PLL			(CLKID_PLL_BASE + 17)
#define CLKID_GP0_PLL_DCO		(CLKID_PLL_BASE + 18)
#define CLKID_GP0_PLL			(CLKID_PLL_BASE + 19)
#define CLKID_HIFI_PLL_DCO		(CLKID_PLL_BASE + 20)
#define CLKID_HIFI_PLL			(CLKID_PLL_BASE + 21)
#define CLKID_HIFI1_PLL_DCO		(CLKID_PLL_BASE + 22)
#define CLKID_HIFI1_PLL			(CLKID_PLL_BASE + 23)

#define CLKID_CPU_BASE			(CLKID_PLL_BASE + 24)
#define CLKID_CPU_DYN_CLK		(CLKID_CPU_BASE + 0)
#define CLKID_CPU_CLK			(CLKID_CPU_BASE + 1)

#define CLKID_CLK_BASE			(CLKID_CPU_BASE + 2)
#define CLKID_RTC_XTAL_CLKIN		(CLKID_CLK_BASE + 0)
#define CLKID_RTC_32K_DIV		(CLKID_CLK_BASE + 1)
#define CLKID_RTC_32K_DIV_SEL		(CLKID_CLK_BASE + 2)
#define CLKID_RTC_32K_SEL		(CLKID_CLK_BASE + 3)
#define CLKID_RTC_CLK			(CLKID_CLK_BASE + 4)
#define CLKID_SYS_CLK_A_MUX		(CLKID_CLK_BASE + 5)
#define CLKID_SYS_CLK_A_DIV		(CLKID_CLK_BASE + 6)
#define CLKID_SYS_CLK_A_GATE		(CLKID_CLK_BASE + 7)
#define CLKID_SYS_CLK_B_MUX		(CLKID_CLK_BASE + 8)
#define CLKID_SYS_CLK_B_DIV		(CLKID_CLK_BASE + 9)
#define CLKID_SYS_CLK_B_GATE		(CLKID_CLK_BASE + 10)
#define CLKID_SYS_CLK			(CLKID_CLK_BASE + 11)
#define CLKID_AXI_CLK_A_MUX		(CLKID_CLK_BASE + 12)
#define CLKID_AXI_CLK_A_DIV		(CLKID_CLK_BASE + 13)
#define CLKID_AXI_CLK_A_GATE		(CLKID_CLK_BASE + 14)
#define CLKID_AXI_CLK_B_MUX		(CLKID_CLK_BASE + 15)
#define CLKID_AXI_CLK_B_DIV		(CLKID_CLK_BASE + 16)
#define CLKID_AXI_CLK_B_GATE		(CLKID_CLK_BASE + 17)
#define CLKID_AXI_CLK			(CLKID_CLK_BASE + 18)
#define CLKID_SYS_DEV_ARB		(CLKID_CLK_BASE + 19)
#define CLKID_SYS_DOS			(CLKID_CLK_BASE + 20)
#define CLKID_SYS_ETH_PHY		(CLKID_CLK_BASE + 21)
#define CLKID_SYS_AOCPU			(CLKID_CLK_BASE + 22)
#define CLKID_SYS_CEC			(CLKID_CLK_BASE + 23)
#define CLKID_SYS_NIC			(CLKID_CLK_BASE + 24)
#define CLKID_SYS_SD_EMMC_C		(CLKID_CLK_BASE + 25)
#define CLKID_SYS_SC			(CLKID_CLK_BASE + 26)
#define CLKID_SYS_ACODEC		(CLKID_CLK_BASE + 27)
#define CLKID_SYS_MSR_CLK		(CLKID_CLK_BASE + 28)
#define CLKID_SYS_IR_CTRL		(CLKID_CLK_BASE + 29)
#define CLKID_SYS_AUDIO			(CLKID_CLK_BASE + 30)
#define CLKID_SYS_ETH_MAC		(CLKID_CLK_BASE + 31)
#define CLKID_SYS_UART_A		(CLKID_CLK_BASE + 32)
#define CLKID_SYS_UART_B		(CLKID_CLK_BASE + 33)
#define CLKID_SYS_TS_PLL		(CLKID_CLK_BASE + 34)
#define CLKID_SYS_G2ED			(CLKID_CLK_BASE + 35)
#define CLKID_SYS_USB			(CLKID_CLK_BASE + 36)
#define CLKID_SYS_I2C_M_A		(CLKID_CLK_BASE + 37)
#define CLKID_SYS_I2C_M_B		(CLKID_CLK_BASE + 38)
#define CLKID_SYS_I2C_M_C		(CLKID_CLK_BASE + 39)
#define CLKID_SYS_HDMITX		(CLKID_CLK_BASE + 40)
#define CLKID_SYS_H2MI20_AES		(CLKID_CLK_BASE + 41)
#define CLKID_SYS_HDCP22		(CLKID_CLK_BASE + 42)
#define CLKID_SYS_MMC			(CLKID_CLK_BASE + 43)
#define CLKID_SYS_CPU_DBG		(CLKID_CLK_BASE + 44)
#define CLKID_SYS_VPU_INTR		(CLKID_CLK_BASE + 45)
#define CLKID_SYS_DEMOD			(CLKID_CLK_BASE + 46)
#define CLKID_SYS_SAR_ADC		(CLKID_CLK_BASE + 47)
#define CLKID_SYS_GIC			(CLKID_CLK_BASE + 48)
#define CLKID_SYS_PWM_AB		(CLKID_CLK_BASE + 49)
#define CLKID_SYS_PWM_CD		(CLKID_CLK_BASE + 50)
#define CLKID_SYS_PAD_CTRL		(CLKID_CLK_BASE + 51)
#define CLKID_SYS_STARTUP		(CLKID_CLK_BASE + 52)
#define CLKID_SYS_SECURETOP		(CLKID_CLK_BASE + 53)
#define CLKID_SYS_ROM			(CLKID_CLK_BASE + 54)
#define CLKID_SYS_CLKTREE		(CLKID_CLK_BASE + 55)
#define CLKID_SYS_PWR_CTRL		(CLKID_CLK_BASE + 56)
#define CLKID_SYS_SYS_CTRL		(CLKID_CLK_BASE + 57)
#define CLKID_SYS_CPU_CTRL		(CLKID_CLK_BASE + 58)
#define CLKID_SYS_SRAM			(CLKID_CLK_BASE + 59)
#define CLKID_SYS_MAILBOX		(CLKID_CLK_BASE + 60)
#define CLKID_SYS_ANA_CTRL		(CLKID_CLK_BASE + 61)
#define CLKID_SYS_JTAG_CTRL		(CLKID_CLK_BASE + 62)
#define CLKID_SYS_IRQ_CTRL		(CLKID_CLK_BASE + 63)
#define CLKID_SYS_RESET_CTRL		(CLKID_CLK_BASE + 64)
#define CLKID_SYS_CAPU			(CLKID_CLK_BASE + 65)
#define CLKID_AXI_SYS_NIC		(CLKID_CLK_BASE + 66)
#define CLKID_AXI_SRAM			(CLKID_CLK_BASE + 67)
#define CLKID_AXI_DEV0_DMC		(CLKID_CLK_BASE + 68)
#define CLKID_PWM_A_MUX			(CLKID_CLK_BASE + 69)
#define CLKID_PWM_A_DIV			(CLKID_CLK_BASE + 70)
#define CLKID_PWM_A			(CLKID_CLK_BASE + 71)
#define CLKID_PWM_B_MUX			(CLKID_CLK_BASE + 72)
#define CLKID_PWM_B_DIV			(CLKID_CLK_BASE + 73)
#define CLKID_PWM_B			(CLKID_CLK_BASE + 74)
#define CLKID_PWM_C_MUX			(CLKID_CLK_BASE + 75)
#define CLKID_PWM_C_DIV			(CLKID_CLK_BASE + 76)
#define CLKID_PWM_C			(CLKID_CLK_BASE + 77)
#define CLKID_PWM_D_MUX			(CLKID_CLK_BASE + 78)
#define CLKID_PWM_D_DIV			(CLKID_CLK_BASE + 79)
#define CLKID_PWM_D			(CLKID_CLK_BASE + 80)
#define CLKID_SD_EMMC_C_MUX		(CLKID_CLK_BASE + 81)
#define CLKID_SD_EMMC_C_DIV		(CLKID_CLK_BASE + 82)
#define CLKID_SD_EMMC_C			(CLKID_CLK_BASE + 83)
#define CLKID_ETH_125M_DIV		(CLKID_CLK_BASE + 84)
#define CLKID_ETH_125M			(CLKID_CLK_BASE + 85)
#define CLKID_ETH_RMII_DIV		(CLKID_CLK_BASE + 86)
#define CLKID_ETH_RMII			(CLKID_CLK_BASE + 87)
#define CLKID_TS_DIV			(CLKID_CLK_BASE + 88)
#define CLKID_TS			(CLKID_CLK_BASE + 89)
#define CLKID_SC_MUX			(CLKID_CLK_BASE + 90)
#define CLKID_SC_DIV			(CLKID_CLK_BASE + 91)
#define CLKID_SC			(CLKID_CLK_BASE + 92)
#define CLKID_CECB_32K_CLKIN		(CLKID_CLK_BASE + 93)
#define CLKID_CECB_32K_DIV		(CLKID_CLK_BASE + 94)
#define CLKID_CECB_32K_DIV_MUX		(CLKID_CLK_BASE + 95)
#define CLKID_CECB_32K_MUX		(CLKID_CLK_BASE + 96)
#define CLKID_CECB_CLK			(CLKID_CLK_BASE + 97)
#define CLKID_HDMITX_SYS_MUX		(CLKID_CLK_BASE + 98)
#define CLKID_HDMITX_SYS_DIV		(CLKID_CLK_BASE + 99)
#define CLKID_HDMITX_SYS		(CLKID_CLK_BASE + 100)
#define CLKID_HDMITX_PRIF_MUX		(CLKID_CLK_BASE + 101)
#define CLKID_HDMITX_PRIF_DIV		(CLKID_CLK_BASE + 102)
#define CLKID_HDMITX_PRIF		(CLKID_CLK_BASE + 103)
#define CLKID_HDMITX_200M_MUX		(CLKID_CLK_BASE + 104)
#define CLKID_HDMITX_200M_DIV		(CLKID_CLK_BASE + 105)
#define CLKID_HDMITX_200M		(CLKID_CLK_BASE + 106)
#define CLKID_HDMITX_AUD_MUX		(CLKID_CLK_BASE + 107)
#define CLKID_HDMITX_AUD_DIV		(CLKID_CLK_BASE + 108)
#define CLKID_HDMITX_AUD		(CLKID_CLK_BASE + 109)
#define CLKID_VCLK_MUX			(CLKID_CLK_BASE + 110)
#define CLKID_VCLK_INPUT		(CLKID_CLK_BASE + 111)
#define CLKID_VCLK			(CLKID_CLK_BASE + 112)
#define CLKID_VCLK_DIV			(CLKID_CLK_BASE + 113)
#define CLKID_VCLK_DIV1			(CLKID_CLK_BASE + 114)
#define CLKID_VCLK_DIV2_EN		(CLKID_CLK_BASE + 115)
#define CLKID_VCLK_DIV2			(CLKID_CLK_BASE + 116)
#define CLKID_VCLK_DIV4_EN		(CLKID_CLK_BASE + 117)
#define CLKID_VCLK_DIV4			(CLKID_CLK_BASE + 118)
#define CLKID_VCLK_DIV6_EN		(CLKID_CLK_BASE + 119)
#define CLKID_VCLK_DIV6			(CLKID_CLK_BASE + 120)
#define CLKID_VCLK_DIV12_EN		(CLKID_CLK_BASE + 121)
#define CLKID_VCLK_DIV12		(CLKID_CLK_BASE + 122)
#define CLKID_VCLK2_MUX			(CLKID_CLK_BASE + 123)
#define CLKID_VCLK2_INPUT		(CLKID_CLK_BASE + 124)
#define CLKID_VCLK2			(CLKID_CLK_BASE + 125)
#define CLKID_VCLK2_DIV			(CLKID_CLK_BASE + 126)
#define CLKID_VCLK2_DIV1		(CLKID_CLK_BASE + 127)
#define CLKID_VCLK2_DIV2_EN		(CLKID_CLK_BASE + 128)
#define CLKID_VCLK2_DIV2		(CLKID_CLK_BASE + 129)
#define CLKID_VCLK2_DIV4_EN		(CLKID_CLK_BASE + 130)
#define CLKID_VCLK2_DIV4		(CLKID_CLK_BASE + 131)
#define CLKID_VCLK2_DIV6_EN		(CLKID_CLK_BASE + 132)
#define CLKID_VCLK2_DIV6		(CLKID_CLK_BASE + 133)
#define CLKID_VCLK2_DIV12_EN		(CLKID_CLK_BASE + 134)
#define CLKID_VCLK2_DIV12		(CLKID_CLK_BASE + 135)
#define CLKID_HDMITX_PIXEL_MUX		(CLKID_CLK_BASE + 136)
#define CLKID_HDMITX_PIXEL		(CLKID_CLK_BASE + 137)
#define CLKID_HDMITX_FE_MUX		(CLKID_CLK_BASE + 138)
#define CLKID_HDMITX_FE			(CLKID_CLK_BASE + 139)
#define CLKID_ENCI_MUX			(CLKID_CLK_BASE + 140)
#define CLKID_ENCI			(CLKID_CLK_BASE + 141)
#define CLKID_ENCP_MUX			(CLKID_CLK_BASE + 142)
#define CLKID_ENCP			(CLKID_CLK_BASE + 143)
#define CLKID_ENCL_MUX			(CLKID_CLK_BASE + 144)
#define CLKID_ENCL			(CLKID_CLK_BASE + 145)
#define CLKID_VDAC_MUX			(CLKID_CLK_BASE + 146)
#define CLKID_VDAC			(CLKID_CLK_BASE + 147)
#define CLKID_VPU_0_MUX			(CLKID_CLK_BASE + 148)
#define CLKID_VPU_0_DIV			(CLKID_CLK_BASE + 149)
#define CLKID_VPU_0			(CLKID_CLK_BASE + 150)
#define CLKID_VPU_1_MUX			(CLKID_CLK_BASE + 151)
#define CLKID_VPU_1_DIV			(CLKID_CLK_BASE + 152)
#define CLKID_VPU_1			(CLKID_CLK_BASE + 153)
#define CLKID_VPU			(CLKID_CLK_BASE + 154)
#define CLKID_VPU_CLKB_TMP_MUX		(CLKID_CLK_BASE + 155)
#define CLKID_VPU_CLKB_TMP_DIV		(CLKID_CLK_BASE + 156)
#define CLKID_VPU_CLKB_TMP		(CLKID_CLK_BASE + 157)
#define CLKID_VPU_CLKB_DIV		(CLKID_CLK_BASE + 158)
#define CLKID_VPU_CLKB			(CLKID_CLK_BASE + 159)
#define CLKID_VPU_CLKC_0_MUX		(CLKID_CLK_BASE + 160)
#define CLKID_VPU_CLKC_0_DIV		(CLKID_CLK_BASE + 161)
#define CLKID_VPU_CLKC_0		(CLKID_CLK_BASE + 162)
#define CLKID_VPU_CLKC_1_MUX		(CLKID_CLK_BASE + 163)
#define CLKID_VPU_CLKC_1_DIV		(CLKID_CLK_BASE + 164)
#define CLKID_VPU_CLKC_1		(CLKID_CLK_BASE + 165)
#define CLKID_VPU_CLKC			(CLKID_CLK_BASE + 166)
#define CLKID_VAPB_0_MUX		(CLKID_CLK_BASE + 167)
#define CLKID_VAPB_0_DIV		(CLKID_CLK_BASE + 168)
#define CLKID_VAPB_0			(CLKID_CLK_BASE + 169)
#define CLKID_VAPB_1_MUX		(CLKID_CLK_BASE + 170)
#define CLKID_VAPB_1_DIV		(CLKID_CLK_BASE + 171)
#define CLKID_VAPB_1			(CLKID_CLK_BASE + 172)
#define CLKID_VAPB			(CLKID_CLK_BASE + 173)
#define CLKID_GE2D			(CLKID_CLK_BASE + 174)
#define CLKID_VDIN_MEAS_MUX		(CLKID_CLK_BASE + 175)
#define CLKID_VDIN_MEAS_DIV		(CLKID_CLK_BASE + 176)
#define CLKID_VDIN_MEAS			(CLKID_CLK_BASE + 177)
#define CLKID_VID_LOCK_MUX		(CLKID_CLK_BASE + 178)
#define CLKID_VID_LOCK_DIV		(CLKID_CLK_BASE + 179)
#define CLKID_VID_LOCK			(CLKID_CLK_BASE + 180)
#define CLKID_VDEC_0_MUX		(CLKID_CLK_BASE + 181)
#define CLKID_VDEC_0_DIV		(CLKID_CLK_BASE + 182)
#define CLKID_VDEC_0			(CLKID_CLK_BASE + 183)
#define CLKID_VDEC_1_MUX		(CLKID_CLK_BASE + 184)
#define CLKID_VDEC_1_DIV		(CLKID_CLK_BASE + 185)
#define CLKID_VDEC_1			(CLKID_CLK_BASE + 186)
#define CLKID_VDEC			(CLKID_CLK_BASE + 187)
#define CLKID_HEVCF_0_MUX		(CLKID_CLK_BASE + 188)
#define CLKID_HEVCF_0_DIV		(CLKID_CLK_BASE + 189)
#define CLKID_HEVCF_0			(CLKID_CLK_BASE + 190)
#define CLKID_HEVCF_1_MUX		(CLKID_CLK_BASE + 191)
#define CLKID_HEVCF_1_DIV		(CLKID_CLK_BASE + 192)
#define CLKID_HEVCF_1			(CLKID_CLK_BASE + 193)
#define CLKID_HEVCF			(CLKID_CLK_BASE + 194)
#define CLKID_DEMOD_CORE_MUX		(CLKID_CLK_BASE + 195)
#define CLKID_DEMOD_CORE_DIV		(CLKID_CLK_BASE + 196)
#define CLKID_DEMOD_CORE		(CLKID_CLK_BASE + 197)
#define CLKID_DEMOD_CORE_T2_MUX		(CLKID_CLK_BASE + 198)
#define CLKID_DEMOD_CORE_T2_DIV		(CLKID_CLK_BASE + 199)
#define CLKID_DEMOD_CORE_T2		(CLKID_CLK_BASE + 200)
#define CLKID_ADC_EXTCLK_IN_MUX		(CLKID_CLK_BASE + 201)
#define CLKID_ADC_EXTCLK_IN_DIV		(CLKID_CLK_BASE + 202)
#define CLKID_ADC_EXTCLK_IN		(CLKID_CLK_BASE + 203)
#define CLKID_DEMOD_32K_CLKIN		(CLKID_CLK_BASE + 204)
#define CLKID_DEMOD_32K_DIV		(CLKID_CLK_BASE + 205)
#define CLKID_DEMOD_32K_DIV_MUX		(CLKID_CLK_BASE + 206)
#define CLKID_DEMOD_CLK			(CLKID_CLK_BASE + 207)
#define CLKID_12_24M_IN			(CLKID_CLK_BASE + 208)
#define CLKID_12_24M			(CLKID_CLK_BASE + 209)

#define NR_CLKS				(CLKID_CLK_BASE + 210)

#endif /* __S1A_CLKC_H */
