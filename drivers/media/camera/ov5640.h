/* SPDX-License-Identifier: (GPL-2.0+ OR MIT)
 *
 *
 * drivers/amlogic/media/camera/ov5640.c
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

#ifndef __AML_OV5640_H__
#define __AML_OV5640_H__

#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/clk-provider.h>

#include <media/v4l2-async.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-device.h>
#include <media/media-entity.h>

#include <linux/amlogic/media/camera/aml_cam_info.h>

#include "common/aml_video.h"

#include "common/vm.h"

struct ov5640_dev_t;

struct resolution_param {
	struct v4l2_frmsize_discrete frmsize;
	struct v4l2_frmsize_discrete active_frmsize;
	int active_fps;
	int lanes;
	int bps;
	enum resolution_size size_type;
	struct aml_camera_i2c_fig_s *reg_script;
	int     reg_script_nums;
};

// device abstract object.
struct ov5640_dev_t {
	u32 index;

	struct v4l2_device v4l2_dev;
	struct i2c_client *client;

	// mutex for ov5640_dev_t instance.
	struct mutex mutex;

	struct aml_video video;

	struct aml_cam_info_s   cam_info;
	struct vm_device_s      *vm_dev;

	struct resolution_param *cur_resolution;

	struct task_struct   *kthread;
	struct aml_buffer    *cur_hw_buffer;

	int ov5640_have_opened;
	int is_streaming;
};

struct aml_cam_info_s *get_aml_cam_info_s(void);

#endif

