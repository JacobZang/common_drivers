/*
*
* SPDX-License-Identifier: GPL-2.0
*
* Copyright (C) 2011-2018 ARM or its affiliates
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; version 2.
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

#ifndef _ISP_V4L2_COMMON_H_
#define _ISP_V4L2_COMMON_H_

#include "acamera_firmware_config.h"

/* Sensor data types */
#define MAX_SENSOR_PRESET_SIZE 10
#define MAX_SENSOR_FPS_SIZE 6
typedef struct _isp_v4l2_sensor_preset {
    uint32_t width;
    uint32_t height;
    uint32_t fps[MAX_SENSOR_FPS_SIZE];
    uint32_t idx[MAX_SENSOR_FPS_SIZE];
    uint8_t fps_num;
    uint8_t fps_cur;
    uint8_t exposures[MAX_SENSOR_FPS_SIZE];
    uint8_t wdr_mode[MAX_SENSOR_FPS_SIZE];
} isp_v4l2_sensor_preset;

typedef struct _isp_v4l2_sensor_info {
    /* resolution preset */
    isp_v4l2_sensor_preset preset[MAX_SENSOR_PRESET_SIZE];
    uint8_t preset_num;
    uint8_t preset_cur;
} isp_v4l2_sensor_info;

#define V4L2_CAN_UPDATE_SENSOR 0
#define V4L2_RESTORE_FR_BASE0 1

/* Base address of DDR */
#if V4L2_RUNNING_ON_JUNO /* On Juno environment */
#define ISP_DDR_START 0x64400000
#define ISP_DDR_SIZE 0xB000000
#define FPGA_BASE_ADDR ISP_DDR_SIZE
#if JUNO_DIRECT_DDR_ACCESS
#define JUNO_DDR_START 0x9c1000000
#define JUNO_DDR_SIZE 0x10000000
#define JUNO_DDR_ADDR_BASE ( JUNO_DDR_START - 0x980000000 ) // (JUNO_DDR_START + 0xF80000000 - 0x1900000000) = (JUNO_DDR_START - 0x980000000) = 0x41000000
#endif
#else /* Probably on PCIE environment */
#define ISP_DDR_START 0x80000000
#define ISP_DDR_SIZE 0x10000000
#endif

/* custom v4l2 formats */
#define ISP_V4L2_PIX_FMT_META v4l2_fourcc( 'M', 'E', 'T', 'A' )        /* META */
#define ISP_V4L2_PIX_FMT_ARGB2101010 v4l2_fourcc( 'B', 'A', '3', '0' ) /* ARGB2101010 */
#define ISP_V4L2_PIX_FMT_NULL v4l2_fourcc( 'N', 'U', 'L', 'L' )        /* format NULL to disable */

/* custom v4l2 events */
#define V4L2_EVENT_ACAMERA_CLASS ( V4L2_EVENT_PRIVATE_START + 0xA * 1000 )
#define V4L2_EVENT_ACAMERA_FRAME_READY ( V4L2_EVENT_ACAMERA_CLASS + 0x1 )
#define V4L2_EVENT_ACAMERA_STREAM_OFF ( V4L2_EVENT_ACAMERA_CLASS + 0x2 )

/* custom v4l2 controls */
#define ISP_V4L2_CID_ISP_V4L2_CLASS ( 0x00f00000 | 1 )
#define ISP_V4L2_CID_BASE ( 0x00f00000 | 0xf000 )
#define ISP_V4L2_CID_TEST_PATTERN ( ISP_V4L2_CID_BASE + 0 )
#define ISP_V4L2_CID_TEST_PATTERN_TYPE ( ISP_V4L2_CID_BASE + 1 )
#define ISP_V4L2_CID_AF_REFOCUS ( ISP_V4L2_CID_BASE + 2 )
#define ISP_V4L2_CID_SENSOR_PRESET ( ISP_V4L2_CID_BASE + 3 )
#define ISP_V4L2_CID_AF_ROI ( ISP_V4L2_CID_BASE + 4 )
#define ISP_V4L2_CID_OUTPUT_FR_ON_OFF ( ISP_V4L2_CID_BASE + 5 )
#define ISP_V4L2_CID_OUTPUT_DS1_ON_OFF ( ISP_V4L2_CID_BASE + 6 )
#define ISP_V4L2_CID_CUSTOM_SENSOR_WDR_MODE ( ISP_V4L2_CID_BASE + 7 )
#define ISP_V4L2_CID_CUSTOM_SENSOR_EXPOSURE ( ISP_V4L2_CID_BASE + 8 )
#define ISP_V4L2_CID_CUSTOM_FR_FPS ( ISP_V4L2_CID_BASE + 9 )
#define ISP_V4L2_CID_CUSTOM_SET_SENSOR_TESTPATTERN (ISP_V4L2_CID_BASE + 10)
#define ISP_V4L2_CID_CUSTOM_SENSOR_IR_CUT ( ISP_V4L2_CID_BASE + 11 )
#define ISP_V4L2_CID_CUSTOM_AE_ZONE_WEIGHT ( ISP_V4L2_CID_BASE + 12 )
#define ISP_V4L2_CID_CUSTOM_AWB_ZONE_WEIGHT ( ISP_V4L2_CID_BASE + 13 )
#define ISP_V4L2_CID_CUSTOM_SET_MANUAL_EXPOSURE ( ISP_V4L2_CID_BASE + 14 )
#define ISP_V4L2_CID_CUSTOM_SET_SENSOR_INTEGRATION_TIME ( ISP_V4L2_CID_BASE + 15 )
#define ISP_V4L2_CID_CUSTOM_SET_SENSOR_ANALOG_GAIN ( ISP_V4L2_CID_BASE + 16 )
#define ISP_V4L2_CID_CUSTOM_SET_ISP_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 17 )
#define ISP_V4L2_CID_CUSTOM_SET_STOP_SENSOR_UPDATE ( ISP_V4L2_CID_BASE + 18 )
#define ISP_V4L2_CID_CUSTOM_SET_DS1_FPS ( ISP_V4L2_CID_BASE + 19 )
#define ISP_V4L2_CID_AE_COMPENSATION ( ISP_V4L2_CID_BASE + 20 )
#define ISP_V4L2_CID_CUSTOM_SET_SENSOR_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 21 )
#define ISP_V4L2_CID_CUSTOM_SET_AWB_RED_GAIN ( ISP_V4L2_CID_BASE + 22 )
#define ISP_V4L2_CID_CUSTOM_SET_AWB_BLUE_GAIN ( ISP_V4L2_CID_BASE + 23 )
#define ISP_V4L2_CID_CUSTOM_SET_MAX_INTEGRATION_TIME (ISP_V4L2_CID_BASE + 24)
#define ISP_V4L2_CID_CUSTOM_SENSOR_FPS (ISP_V4L2_CID_BASE + 26)
#define ISP_V4L2_CID_CUSTOM_SNR_MANUAL (ISP_V4L2_CID_BASE + 27)
#define ISP_V4L2_CID_CUSTOM_SNR_STRENGTH (ISP_V4L2_CID_BASE + 28)
#define ISP_V4L2_CID_CUSTOM_TNR_MANUAL (ISP_V4L2_CID_BASE + 29)
#define ISP_V4L2_CID_CUSTOM_TNR_OFFSET (ISP_V4L2_CID_BASE + 30)
#define ISP_V4L2_CID_CUSTOM_TEMPER_MODE (ISP_V4L2_CID_BASE + 31)
#define ISP_V4L2_CID_CUSTOM_WDR_SWITCH (ISP_V4L2_CID_BASE + 32)
#define ISP_V4L2_CID_CUSTOM_DEFOG_SWITCH (ISP_V4L2_CID_BASE + 33)
#define ISP_V4L2_CID_CUSTOM_DEFOG_STRENGTH (ISP_V4L2_CID_BASE + 34)
#define ISP_V4L2_CID_CUSTOM_ANTIFLICKER (ISP_V4L2_CID_BASE + 35)
#define ISP_V4L2_CID_CUSTOM_CALIBRATION (ISP_V4L2_CID_BASE + 36)
#define ISP_V4L2_CID_AE_ROI ( ISP_V4L2_CID_BASE + 37 )

//ISP SYSTEM
#define ISP_V4L2_CID_SYSTEM_FREEZE_FIRMWARE ( ISP_V4L2_CID_BASE + 48 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_EXPOSURE ( ISP_V4L2_CID_BASE + 49 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_INTEGRATION_TIME ( ISP_V4L2_CID_BASE + 50 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_MAX_INTEGRATION_TIME ( ISP_V4L2_CID_BASE + 51 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_SENSOR_ANALOG_GAIN ( ISP_V4L2_CID_BASE + 52 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_SENSOR_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 53 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_ISP_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 54 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_EXPOSURE_RATIO ( ISP_V4L2_CID_BASE + 55 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_AWB ( ISP_V4L2_CID_BASE + 56 )
#define ISP_V4L2_CID_SYSTEM_MANUAL_SATURATION ( ISP_V4L2_CID_BASE + 57 )
#define ISP_V4L2_CID_SYSTEM_MAX_EXPOSURE_RATIO ( ISP_V4L2_CID_BASE + 58 )
#define ISP_V4L2_CID_SYSTEM_EXPOSURE ( ISP_V4L2_CID_BASE + 59 )
#define ISP_V4L2_CID_SYSTEM_INTEGRATION_TIME ( ISP_V4L2_CID_BASE + 60 )
#define ISP_V4L2_CID_SYSTEM_EXPOSURE_RATIO ( ISP_V4L2_CID_BASE + 61 )
#define ISP_V4L2_CID_SYSTEM_MAX_INTEGRATION_TIME ( ISP_V4L2_CID_BASE + 62 )
#define ISP_V4L2_CID_SYSTEM_SENSOR_ANALOG_GAIN ( ISP_V4L2_CID_BASE + 63 )
#define ISP_V4L2_CID_SYSTEM_MAX_SENSOR_ANALOG_GAIN ( ISP_V4L2_CID_BASE + 64 )
#define ISP_V4L2_CID_SYSTEM_SENSOR_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 65 )
#define ISP_V4L2_CID_SYSTEM_MAX_SENSOR_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 66 )
#define ISP_V4L2_CID_SYSTEM_ISP_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 67 )
#define ISP_V4L2_CID_SYSTEM_MAX_ISP_DIGITAL_GAIN ( ISP_V4L2_CID_BASE + 68 )
#define ISP_V4L2_CID_SYSTEM_AWB_RED_GAIN ( ISP_V4L2_CID_BASE + 69 )
#define ISP_V4L2_CID_SYSTEM_AWB_BLUE_GAIN ( ISP_V4L2_CID_BASE + 70 )

// ISP_MODULES
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_IRIDIX ( ISP_V4L2_CID_BASE + 71 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_SINTER ( ISP_V4L2_CID_BASE + 72 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_TEMPER ( ISP_V4L2_CID_BASE + 73 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_AUTO_LEVEL ( ISP_V4L2_CID_BASE + 74 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_FRAME_STITCH ( ISP_V4L2_CID_BASE + 75 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_RAW_FRONTEND ( ISP_V4L2_CID_BASE + 76 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_BLACK_LEVEL ( ISP_V4L2_CID_BASE + 77 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_SHADING ( ISP_V4L2_CID_BASE + 78 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_DEMOSAIC ( ISP_V4L2_CID_BASE + 79 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_CNR ( ISP_V4L2_CID_BASE + 80 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_SHARPEN ( ISP_V4L2_CID_BASE + 81 )

// TIMAGE
#define ISP_V4L2_CID_IMAGE_RESIZE_ENABLE_ID ( ISP_V4L2_CID_BASE + 82 )
#define ISP_V4L2_CID_IMAGE_RESIZE_TYPE_ID ( ISP_V4L2_CID_BASE + 83 )
#define ISP_V4L2_CID_IMAGE_CROP_XOFFSET_ID ( ISP_V4L2_CID_BASE + 84 )
#define ISP_V4L2_CID_IMAGE_CROP_YOFFSET_ID ( ISP_V4L2_CID_BASE + 85 )
#define ISP_V4L2_CID_IMAGE_RESIZE_WIDTH_ID ( ISP_V4L2_CID_BASE + 86 )
#define ISP_V4L2_CID_IMAGE_RESIZE_HEIGHT_ID ( ISP_V4L2_CID_BASE + 87 )

#define ISP_V4L2_CID_STATUS_INFO_EXPOSURE_LOG2 ( ISP_V4L2_CID_BASE + 88 )
#define ISP_V4L2_CID_STATUS_INFO_GAIN_LOG2 ( ISP_V4L2_CID_BASE + 89 )
#define ISP_V4L2_CID_STATUS_INFO_GAIN_ONES ( ISP_V4L2_CID_BASE + 90 )
#define ISP_V4L2_CID_STATUS_INFO_AWB_MIX_LIGHT_CONTRAST ( ISP_V4L2_CID_BASE + 91 )
#define ISP_V4L2_CID_STATUS_INFO_AF_LENS_POS ( ISP_V4L2_CID_BASE + 92 )
#define ISP_V4L2_CID_STATUS_INFO_AF_FOCUS_VALUE ( ISP_V4L2_CID_BASE + 93 )
#define ISP_V4L2_CID_INFO_FW_REVISION ( ISP_V4L2_CID_BASE + 94 )

#define ISP_V4L2_CID_SENSOR_LINES_PER_SECOND ( ISP_V4L2_CID_BASE + 95 )

#define ISP_V4L2_CID_AF_MODE_ID ( ISP_V4L2_CID_BASE + 97 )
enum isp_v4l2_af_mode_id {
    ISP_V4L2_AF_MODE_AUTO_SINGLE,
    ISP_V4L2_AF_MODE_AUTO_CONTINUOUS,
    ISP_V4L2_AF_MODE_MANUAL,
};

#define ISP_V4L2_CID_AF_MANUAL_CONTROL_ID ( ISP_V4L2_CID_BASE + 98 )

#define ISP_V4L2_CID_AF_STATE_ID ( ISP_V4L2_CID_BASE + 99 )
enum isp_v4l2_af_state_id {
    ISP_V4L2_AF_STATE_INACTIVE,
    ISP_V4L2_AF_STATE_SCAN,
    ISP_V4L2_AF_STATE_FOCUSED,
    ISP_V4L2_AF_STATE_UNFOCUSED,
};

#define ISP_V4L2_CID_AWB_MODE_ID ( ISP_V4L2_CID_BASE + 100 )
enum isp_v4l2_awb_mode_id {
    ISP_V4L2_AWB_MODE_AUTO,
    ISP_V4L2_AWB_MODE_MANUAL,
    ISP_V4L2_AWB_MODE_DAY_LIGHT,
    ISP_V4L2_AWB_MODE_CLOUDY,
    ISP_V4L2_AWB_MODE_INCANDESCENT,
    ISP_V4L2_AWB_MODE_FLUORESCENT,
    ISP_V4L2_AWB_MODE_TWILIGHT,
    ISP_V4L2_AWB_MODE_SHADE,
    ISP_V4L2_AWB_MODE_WARM_FLUORESCENT,
};

#define ISP_V4L2_CID_SENSOR_INFO_PHYSICAL_WIDTH ( ISP_V4L2_CID_BASE + 101 )
#define ISP_V4L2_CID_SENSOR_INFO_PHYSICAL_HEIGHT ( ISP_V4L2_CID_BASE + 102 )

#define ISP_V4L2_CID_LENS_INFO_MINFOCUS_DISTANCE ( ISP_V4L2_CID_BASE + 103 )
#define ISP_V4L2_CID_LENS_INFO_HYPERFOCAL_DISTANCE ( ISP_V4L2_CID_BASE + 104 )
#define ISP_V4L2_CID_LENS_INFO_FOCAL_LENGTH ( ISP_V4L2_CID_BASE + 105 )
#define ISP_V4L2_CID_LENS_INFO_APERTURE ( ISP_V4L2_CID_BASE + 106 )

#define ISP_V4L2_CID_AE_STATE_ID ( ISP_V4L2_CID_BASE + 107 )
enum isp_v4l2_ae_state_id {
    ISP_V4L2_AE_STATE_INACTIVE,
    ISP_V4L2_AE_STATE_SEARCHING,
    ISP_V4L2_AE_STATE_CONVERGED,
};

#define ISP_V4L2_CID_AWB_STATE_ID ( ISP_V4L2_CID_BASE + 108 )
enum isp_v4l2_awb_state_id {
    ISP_V4L2_AWB_STATE_INACTIVE,
    ISP_V4L2_AWB_STATE_SEARCHING,
    ISP_V4L2_AWB_STATE_CONVERGED,
};

#define ISP_V4L2_CID_SYSTEM_AWB_GREEN_EVEN_GAIN ( ISP_V4L2_CID_BASE + 109 )
#define ISP_V4L2_CID_SYSTEM_AWB_GREEN_ODD_GAIN ( ISP_V4L2_CID_BASE + 110 )

#define ISP_V4L2_CID_NOISE_REDUCTION_MODE_ID ( ISP_V4L2_CID_BASE + 111 )
enum isp_v4l2_noise_reduction_mode_id {
    ISP_V4L2_NOISE_REDUCTION_MODE_OFF,
    ISP_V4L2_NOISE_REDUCTION_MODE_ON,
};

#define ISP_V4L2_CID_SYSTEM_CCM_MATRIX ( ISP_V4L2_CID_BASE + 112 )
#define ISP_V4L2_CCM_MATRIX_SZ          9

#define ISP_V4L2_CID_SYSTEM_MANUAL_CCM ( ISP_V4L2_CID_BASE + 113 )

#define ISP_V4L2_CID_SHADING_STRENGTH ( ISP_V4L2_CID_BASE + 114 )

#define ISP_V4L2_CID_SYSTEM_DYNAMIC_GAMMA_ENABLE ( ISP_V4L2_CID_BASE + 115 )

#define ISP_V4L2_CID_SYSTEM_RGB_GAMMA_LUT ( ISP_V4L2_CID_BASE + 116 )
#define ISP_V4L2_RGB_GAMMA_LUT_SZ          129

#define ISP_V4L2_CID_AWB_TEMPERATURE ( ISP_V4L2_CID_BASE + 117 )

#define ISP_V4L2_CID_SYSTEM_STATUATION_TARGET ( ISP_V4L2_CID_BASE + 118 )

#define ISP_V4L2_CID_AE_STATS ( ISP_V4L2_CID_BASE + 119 )
#define ISP_V4L2_AE_STATS_SIZE             (1 + 1024)

#define ISP_V4L2_CID_SENSOR_VMAX_FPS ( ISP_V4L2_CID_BASE + 120 )

#define ISP_V4L2_CID_SYSTEM_IQ_CALIBRATION_DYNAMIC ( ISP_V4L2_CID_BASE + 121 )
#define ISP_V4L2_IQ_CALIBRATION_MAX          (64*3+100)*2
#define CALIBRATION_SET_INFO_FOR_GET_CALIBRATION ISP_V4L2_IQ_CALIBRATION_MAX

#define ISP_V4L2_CID_AWB_STATS ( ISP_V4L2_CID_BASE + 122 )
#define ISP_V4L2_AWB_STATS_SIZE             (2 + 2178)
#define ISP_V4L2_AF_STATS_SIZE ISP_V4L2_AWB_STATS_SIZE


#define ISP_V4L2_CID_DPC_THRES_SLOPE ( ISP_V4L2_CID_BASE + 123 )
#define ISP_V4L2_CID_BLACK_LEVEL_R ( ISP_V4L2_CID_BASE + 124 )
#define ISP_V4L2_CID_BLACK_LEVEL_GR ( ISP_V4L2_CID_BASE + 125 )
#define ISP_V4L2_CID_BLACK_LEVEL_GB ( ISP_V4L2_CID_BASE + 126 )
#define ISP_V4L2_CID_BLACK_LEVEL_B ( ISP_V4L2_CID_BASE + 127 )
#define ISP_V4L2_CID_SHARP_LU_DU_LD ( ISP_V4L2_CID_BASE + 128 )
#define ISP_V4L2_CID_CNR_STRENGTH ( ISP_V4L2_CID_BASE + 129 )
#define ISP_V4L2_CID_SYSTEM_ANTIFLICKER_ENABLE ( ISP_V4L2_CID_BASE + 130 )
#define ISP_V4L2_CID_SYSTEM_ANTIFLICKER_FREQUENCY ( ISP_V4L2_CID_BASE + 131 )
#define ISP_V4L2_CID_ISP_MODULES_IRIDIX_ENABLE ( ISP_V4L2_CID_BASE + 132 )
#define ISP_V4L2_CID_ISP_MODULES_IRIDIX_STRENGTH ( ISP_V4L2_CID_BASE + 133 )
#define ISP_V4L2_CID_ISP_MODULES_SWHW_REGISTERS ( ISP_V4L2_CID_BASE + 134 )
#define ISP_V4L2_CID_AE_GET_GAIN ( ISP_V4L2_CID_BASE + 135 )
#define ISP_V4L2_CID_AE_GET_EXPOSURE ( ISP_V4L2_CID_BASE + 136 )
#define ISP_V4L2_CID_ISP_MODULES_FR_SHARPEN_STRENGTH ( ISP_V4L2_CID_BASE + 137 )
#define ISP_V4L2_CID_ISP_MODULES_DS1_SHARPEN_STRENGTH ( ISP_V4L2_CID_BASE + 138 )
#define ISP_V4L2_CID_ISP_MODULES_BYPASS ( ISP_V4L2_CID_BASE + 139 )
#define ISP_V4L2_CID_ISP_DAYNIGHT_DETECT ( ISP_V4L2_CID_BASE + 140 )
#define ISP_V4L2_CID_SYSTEM_LONG_INTEGRATION_TIME ( ISP_V4L2_CID_BASE + 141 )
#define ISP_V4L2_CID_SYSTEM_SHORT_INTEGRATION_TIME ( ISP_V4L2_CID_BASE + 142 )
#define ISP_V4L2_CID_CMOS_HIST_MEAN ( ISP_V4L2_CID_BASE + 143 )
#define ISP_V4L2_CID_AE_TARGET ( ISP_V4L2_CID_BASE + 144 )
#define ISP_V4L2_CID_ISP_MODULES_MANUAL_PF ( ISP_V4L2_CID_BASE + 145 )
#define ISP_V4L2_CID_STATUS_ISO ( ISP_V4L2_CID_BASE + 146 )
#define ISP_V4L2_CID_AF_STATS ( ISP_V4L2_CID_BASE + 147 )

#define ISP_V4L2_CID_SCALE_CROP_STARTX ( ISP_V4L2_CID_BASE + 148 )
#define ISP_V4L2_CID_SCALE_CROP_STARTY ( ISP_V4L2_CID_BASE + 149 )
#define ISP_V4L2_CID_SCALE_CROP_WIDTH ( ISP_V4L2_CID_BASE + 150 )
#define ISP_V4L2_CID_SCALE_CROP_HEIGHT ( ISP_V4L2_CID_BASE + 151 )
#define ISP_V4L2_CID_SCALE_CROP_ENABLE ( ISP_V4L2_CID_BASE + 152 )
#define ISP_V4L2_CID_GET_STREAM_STATUS ( ISP_V4L2_CID_BASE + 153 )
#define ISP_V4L2_CID_SET_SENSOR_POWER ( ISP_V4L2_CID_BASE + 154 )
#define ISP_V4L2_CID_SET_SENSOR_MD_EN ( ISP_V4L2_CID_BASE + 155 )
#define ISP_V4L2_CID_SET_SENSOR_DECMPR_EN ( ISP_V4L2_CID_BASE + 156 )
#define ISP_V4L2_CID_SET_SENSOR_DECMPR_LOSSLESS_EN ( ISP_V4L2_CID_BASE + 157 )
#define ISP_V4L2_CID_SET_SENSOR_FLICKER_EN ( ISP_V4L2_CID_BASE + 158 )
#define ISP_V4L2_CID_MD_STATS ( ISP_V4L2_CID_BASE + 159 )
#define ISP_V4L2_MD_STATS_SIZE             (0x40000)
#define ISP_V4L2_CID_SCALER_FPS ( ISP_V4L2_CID_BASE + 160 )
#define ISP_V4L2_CID_FLICKER_STATS ( ISP_V4L2_CID_BASE + 161 )
#define ISP_V4L2_FLICKER_STATS_SIZE             (0x1000)
#define ISP_V4L2_CID_AE_ROI_EXACT_COORDINATES ( ISP_V4L2_CID_BASE + 162 )
#define ISP_V4L2_CID_AF_ROI_EXACT_COORDINATES ( ISP_V4L2_CID_BASE + 163 )
#define ISP_V4L2_RIO_SIZE             (6*10 + 1)
#define ISP_V4L2_CID_SET_IS_CAPTURING ( ISP_V4L2_CID_BASE + 164 )
#define ISP_V4L2_CID_SET_FPS_RANGE ( ISP_V4L2_CID_BASE + 165 )

#define ISP_V4L2_CID_ISP_DCAM_MODE (ISP_V4L2_CID_BASE + 166)

#define IQ_ID_OFFSET 0
#define IQ_ELEMENT_SIZE_OFFSET 1
#define IQ_ARRAY_SIZE_OFFSET 2
#define IQ_ARRAY_SET_GET_CMD 3
#define IQ_ARRAY_DATA_OFFSET 4

typedef struct {
    uint32_t calib_id;
    uint32_t element_size;
    uint32_t array_s;
}get_cmd_para_t;

typedef struct {
    uint32_t isget;
    uint32_t addr;
    uint32_t value;
}isp_ctl_swhw_registers_cmd_t;

/* type of stream */
typedef enum {
    V4L2_RESIZE_TYPE_CROP_FR = 0,
    V4L2_RESIZE_TYPE_CROP_DS1 = 1,
    V4L2_RESIZE_TYPE_SCALER_DS1 = 2,
} isp_v4l2_resize_type_t;

/* type of stream */
typedef enum {
#if ISP_HAS_SC0
    V4L2_STREAM_TYPE_SC0 = 0,
#endif

#if ISP_HAS_SC3
    V4L2_STREAM_TYPE_CROP,
#endif

#if ISP_HAS_SC1
    V4L2_STREAM_TYPE_SC1,
#endif
#if ISP_HAS_SC2
    V4L2_STREAM_TYPE_SC2,
#endif

    V4L2_STREAM_TYPE_MAX,

    V4L2_STREAM_TYPE_FR,

#if ISP_HAS_META_CB
    V4L2_STREAM_TYPE_META,
#endif
#if ISP_HAS_DS1
    V4L2_STREAM_TYPE_DS1,
#endif

#if ISP_HAS_RAW_CB
    V4L2_STREAM_TYPE_RAW
#endif
} isp_v4l2_stream_type_t;

/*bypass mode bit defination*/
#define ACAMERA_ISP_TOP_ISP_GET_BY_PASS_CMD (1LL<<0)
#define ACAMERA_ISP_TOP_BYPASS_VIDEO_TEST_GEN (1LL<<1)
#define ACAMERA_ISP_TOP_BYPASS_INPUT_FORMATTER (1LL<<2)
#define ACAMERA_ISP_TOP_BYPASS_DECOMPANDER (1LL<<3)
#define ACAMERA_ISP_TOP_BYPASS_SENSOR_OFFSET_WDR (1LL<<4)
#define ACAMERA_ISP_TOP_BYPASS_GAIN_WDR (1LL<<5)
#define ACAMERA_ISP_TOP_BYPASS_FRAME_STITCH (1LL<<6)
#define ACAMERA_ISP_TOP_BYPASS_DIGITAL_GAIN (1LL<<7)
#define ACAMERA_ISP_TOP_BYPASS_FRONTEND_SENSOR_OFFSET (1LL<<8)
#define ACAMERA_ISP_TOP_BYPASS_FE_SQRT (1LL<<9)
#define ACAMERA_ISP_TOP_BYPASS_RAW_FRONTEND (1LL<<10)
#define ACAMERA_ISP_TOP_BYPASS_DEFECT_PIXEL (1LL<<11)
#define ACAMERA_ISP_TOP_BYPASS_SINTER (1LL<<12)
#define ACAMERA_ISP_TOP_BYPASS_TEMPER (1LL<<13)
#define ACAMERA_ISP_TOP_BYPASS_CA_CORRECTION (1LL<<14)
#define ACAMERA_ISP_TOP_BYPASS_SQUARE_BE (1LL<<15)
#define ACAMERA_ISP_TOP_BYPASS_SENSOR_OFFSET_PRE_SHADING (1LL<<16)
#define ACAMERA_ISP_TOP_BYPASS_RADIAL_SHADING (1LL<<17)
#define ACAMERA_ISP_TOP_BYPASS_MESH_SHADING (1LL<<18)
#define ACAMERA_ISP_TOP_BYPASS_WHITE_BALANCE (1LL<<19)
#define ACAMERA_ISP_TOP_BYPASS_IRIDIX_GAIN (1LL<<20)
#define ACAMERA_ISP_TOP_BYPASS_IRIDIX (1LL<<21)
#define ACAMERA_ISP_TOP_BYPASS_MIRROR (1LL<<22)
#define ACAMERA_ISP_TOP_BYPASS_DEMOSAIC_RGB (1LL<<23)
#define ACAMERA_ISP_TOP_BYPASS_DEMOSAIC_RGBIR (1LL<<24)
#define ACAMERA_ISP_TOP_BYPASS_PF_CORRECTION (1LL<<25)
#define ACAMERA_ISP_TOP_BYPASS_CCM (1LL<<26)
#define ACAMERA_ISP_TOP_BYPASS_CNR (1LL<<27)
#define ACAMERA_ISP_TOP_BYPASS_3D_LUT (1LL<<28)
#define ACAMERA_ISP_TOP_BYPASS_NONEQU_GAMMA (1LL<<29)
#define ACAMERA_ISP_TOP_BYPASS_FR_CROP (1LL<<30)
#define ACAMERA_ISP_TOP_BYPASS_FR_GAMMA_RGB (1LL<<31)
#define ACAMERA_ISP_TOP_BYPASS_FR_SHARPEN (1LL<<32)
#define ACAMERA_ISP_TOP_BYPASS_FR_CS_CONV (1LL<<33)
#define ACAMERA_ISP_TOP_BYPASS_DS1_CROP (1LL<<34)
#define ACAMERA_ISP_TOP_BYPASS_DS1_SCALER (1LL<<35)
#define ACAMERA_ISP_TOP_BYPASS_DS1_GAMMA_RGB (1LL<<36)
#define ACAMERA_ISP_TOP_BYPASS_DS1_SHARPEN (1LL<<37)
#define ACAMERA_ISP_TOP_BYPASS_DS1_CS_CONV (1LL<<38)
#define ACAMERA_ISP_TOP_ISP_RAW_BYPASS (1LL<<39)
#define ACAMERA_ISP_TOP_ISP_PROCESSING_FR_BYPASS_MODE (1LL<<40)

#endif
