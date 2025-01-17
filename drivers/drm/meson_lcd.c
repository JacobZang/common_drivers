// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <drm/drm_modeset_helper.h>
#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_mipi_dsi.h>
#include <linux/component.h>
#include <vout/vout_serve/vout_func.h>
#include "meson_crtc.h"
#include "meson_lcd.h"

struct meson_panel_framerate_map {
	int lcd_frac_hint;
	int drm_fps;
};

struct meson_panel_framerate_map framerate_map[] = {
	{28800, 288},
	{24000, 240},
	{23976, 239},
	{20000, 200},
	{19200, 192},
	{19180, 191},
	{14400, 144},
	{12000, 120},
	{11988, 119},
	{10000, 100},
	{9600, 96},
	{9590, 95},
	{6000, 60},
	{5994, 59},
	{5000, 50},
	{4800, 48},
	{4795, 47}
};

static int find_frac_hint_by_fps(int fps)
{
	int i;
	struct meson_panel_framerate_map *map;
	int size = ARRAY_SIZE(framerate_map);

	for (i = 0; i < size; i++) {
		map = &framerate_map[i];
		if (map->drm_fps == fps)
			return map->lcd_frac_hint;
	}

	return 6000;
}

int meson_panel_get_modes(struct drm_connector *connector)
{
	struct meson_panel *am_lcd = connector_to_meson_panel(connector);
	struct meson_panel_dev *panel_dev = am_lcd->panel_dev;
	struct drm_display_mode *modes, *mode;
	int modes_cnt = 0;
	int i = 0, ret = 0;

	if (!panel_dev->get_modes) {
		DRM_ERROR("No get Modes function.");
		return 0;
	}

	if (panel_dev->get_modes_vrr_range) {
		ret = panel_dev->get_modes_vrr_range(panel_dev, (void *)am_lcd->groups,
						     MAX_VRR_MODE_GROUP,
						     &am_lcd->num_vrr_group);
		if (ret || !am_lcd->num_vrr_group)
			drm_connector_set_vrr_capable_property(connector, false);
		else
			drm_connector_set_vrr_capable_property(connector, true);
	} else {
		drm_connector_set_vrr_capable_property(connector, false);
	}

	ret = panel_dev->get_modes(panel_dev, &modes, &modes_cnt);
	DRM_DEBUG("%s: get modes %d, ret %d\n", __func__, modes_cnt, ret);
	if (ret == 0 && modes_cnt > 0) {
		if (!connector->display_info.width_mm || !connector->display_info.height_mm) {
			connector->display_info.width_mm = modes[i].width_mm;
			connector->display_info.height_mm = modes[i].height_mm;
		}

		for (i = 0; i < modes_cnt; i++) {
			DRM_DEBUG("[%s]-[%d] mode_name-%s\n", __func__, __LINE__, modes[i].name);
			mode = drm_mode_duplicate(connector->dev, &modes[i]);
			if (mode) {
				drm_mode_probed_add(connector, mode);
				drm_mode_debug_printmodeline(mode);
			}
		}
		kfree(modes);
	}

	return modes_cnt;
}

enum drm_mode_status meson_panel_check_mode(struct drm_connector *connector,
	struct drm_display_mode *mode)
{
	return MODE_OK;
}

static struct drm_encoder *meson_panel_best_encoder
	(struct drm_connector *connector)
{
	struct meson_panel *lcd = connector_to_meson_panel(connector);

	return &lcd->encoder;
}

static const struct drm_connector_helper_funcs meson_panel_helper_funcs = {
	.get_modes = meson_panel_get_modes,
	.mode_valid = meson_panel_check_mode,
	.best_encoder = meson_panel_best_encoder,
};

static enum drm_connector_status
meson_panel_detect(struct drm_connector *connector, bool force)
{
	return connector_status_connected;
}

static void am_lcd_connector_destroy(struct drm_connector *connector)
{
	DRM_DEBUG("[%s]-[%d] called\n", __func__, __LINE__);

	drm_connector_unregister(connector);
	drm_connector_cleanup(connector);
}

static void am_lcd_connector_reset(struct drm_connector *connector)
{
	DRM_DEBUG("[%s]-[%d] called\n", __func__, __LINE__);

	drm_atomic_helper_connector_reset(connector);
}

static struct drm_connector_state *
am_lcd_connector_duplicate_state(struct drm_connector *connector)
{
	struct drm_connector_state *state;

	DRM_DEBUG("[%s]-[%d] called\n", __func__, __LINE__);

	state = drm_atomic_helper_connector_duplicate_state(connector);
	return state;
}

static void am_lcd_connector_destroy_state(struct drm_connector *connector,
					   struct drm_connector_state *state)
{
	DRM_DEBUG("[%s]-[%d] called\n", __func__, __LINE__);

	drm_atomic_helper_connector_destroy_state(connector, state);
}

int meson_panel_atomic_set_property(struct drm_connector *connector,
			   struct drm_connector_state *state,
			   struct drm_property *property,
			   uint64_t val)
{
	return -EINVAL;
}

int meson_panel_atomic_get_property(struct drm_connector *connector,
			   const struct drm_connector_state *state,
			   struct drm_property *property,
			   uint64_t *val)
{
	struct meson_panel *panel = connector_to_meson_panel(connector);

	if (property == panel->panel_type_prop) {
		*val = panel->panel_type;
		return 0;
	}

	return -EINVAL;
}

void meson_panel_atomic_print_state(struct drm_printer *p,
				    const struct drm_connector_state *state)
{
	int i;
	struct drm_vrr_mode_group *group;
	struct meson_panel *am_lcd;
	struct drm_connector *conn = state->connector;

	if (!conn)
		return;

	am_lcd = connector_to_meson_panel(conn);
	drm_printf(p, "\tdrm lcd state:\n");
	drm_printf(p, "\t\t  vrr range :\n");
	for (i = 0; i < am_lcd->num_vrr_group; i++) {
		group = &am_lcd->groups[i];
		drm_printf(p, "\t\t %u,%u,%u-%u,%s\n", group->width, group->height,
			   group->vrr_min, group->vrr_max, group->modename);
	}
}

static const struct drm_connector_funcs meson_panel_funcs = {
	.detect			= meson_panel_detect,
	.fill_modes		= drm_helper_probe_single_connector_modes,
	.destroy		= am_lcd_connector_destroy,
	.reset			= am_lcd_connector_reset,
	.atomic_duplicate_state	= am_lcd_connector_duplicate_state,
	.atomic_destroy_state	= am_lcd_connector_destroy_state,
	.atomic_set_property	= meson_panel_atomic_set_property,
	.atomic_get_property	= meson_panel_atomic_get_property,
	.atomic_print_state = meson_panel_atomic_print_state,
};

static void meson_panel_encoder_atomic_enable(struct drm_encoder *encoder,
	struct drm_atomic_state *state)
{
	struct drm_crtc *crtc;
	int lcd_frac_hint;
	struct am_meson_crtc_state *meson_crtc_state =
				to_am_meson_crtc_state(encoder->crtc->state);
	struct am_meson_crtc *amcrtc = to_am_meson_crtc(encoder->crtc);
	struct drm_display_mode *mode = &meson_crtc_state->base.adjusted_mode;
	struct drm_crtc_state *old_crtc_state;
	struct am_meson_crtc_state *meson_old_crtc_state;
	enum vmode_e vmode = meson_crtc_state->vmode;
	unsigned int vrefresh = drm_mode_vrefresh(mode);

	crtc = encoder->crtc;

	if ((vmode & VMODE_MODE_BIT_MASK) != VMODE_LCD) {
		DRM_DEBUG("%s:enable fail! vmode:%d\n", __func__, vmode);
		return;
	}

	old_crtc_state = drm_atomic_get_old_crtc_state(state, crtc);
	if (!old_crtc_state) {
		DRM_ERROR("%s crtc state is NULL!\n", __func__);
		return;
	}
	meson_old_crtc_state = to_am_meson_crtc_state(old_crtc_state);

	if (meson_crtc_state->uboot_mode_init == 1)
		vmode |= VMODE_INIT_BIT_MASK;

	DRM_INFO("[%s]-[%d] called, %d, %d\n", __func__, __LINE__,
		 meson_crtc_state->prev_vrefresh, vrefresh);

	if (encoder->crtc->state->vrr_enabled) {
		if (strcmp(meson_crtc_state->brr_mode, meson_old_crtc_state->brr_mode)) {
			meson_vout_notify_mode_change(amcrtc->vout_index,
				vmode, EVENT_MODE_SET_START);
			vout_func_set_vmode(amcrtc->vout_index, vmode);
			meson_vout_notify_mode_change(amcrtc->vout_index,
				vmode, EVENT_MODE_SET_FINISH);
			meson_vout_update_mode_name(amcrtc->vout_index, mode->name, "lcd");
			DRM_INFO("vrr mode, use set_vmode\n");
		}

		if (vrefresh != meson_crtc_state->brr ||
		    meson_crtc_state->prev_vrefresh != vrefresh) {
			lcd_frac_hint = find_frac_hint_by_fps(vrefresh);
			vout_func_set_vframe_rate_hint(amcrtc->vout_index, lcd_frac_hint);
			DRM_INFO("vrr mode, use set_vframe_rate_hint, %s-%s, %d-%d\n",
				 meson_crtc_state->brr_mode, meson_old_crtc_state->brr_mode,
				 meson_crtc_state->brr, lcd_frac_hint);
		}

		meson_crtc_state->prev_vrefresh = vrefresh;

		return;
	}

	if (crtc->enabled && !crtc->state->active_changed &&
	    meson_crtc_state->prev_height == mode->vdisplay &&
	    meson_crtc_state->prev_vrefresh != vrefresh) {
		lcd_frac_hint = find_frac_hint_by_fps(vrefresh);
		vout_func_set_vframe_rate_hint(amcrtc->vout_index, lcd_frac_hint);
		meson_crtc_state->prev_vrefresh = vrefresh;
		DRM_INFO("skip set_vmode, use set_vframe_rate_hint\n");
		return;
	}

	meson_crtc_state->prev_vrefresh = vrefresh;
	meson_crtc_state->prev_height = mode->vdisplay;
	meson_vout_notify_mode_change(amcrtc->vout_index,
		vmode, EVENT_MODE_SET_START);
	vout_func_set_vmode(amcrtc->vout_index, vmode);
	meson_vout_notify_mode_change(amcrtc->vout_index,
		vmode, EVENT_MODE_SET_FINISH);
	meson_vout_update_mode_name(amcrtc->vout_index, mode->name, "lcd");

	DRM_DEBUG("[%s]-[%d] exit\n", __func__, __LINE__);
}

static void meson_panel_encoder_atomic_disable(struct drm_encoder *encoder,
	struct drm_atomic_state *state)
{
	DRM_DEBUG("[%s]-[%d] called\n", __func__, __LINE__);
}

static void meson_panel_cal_brr(struct meson_panel *am_lcd,
				struct am_meson_crtc_state *crtc_state,
				struct drm_display_mode *adj_mode)
{
	int i;
	struct drm_vrr_mode_group *group;

	for (i = 0; i < am_lcd->num_vrr_group; i++) {
		group = &am_lcd->groups[i];
		DRM_DEBUG("%s %u-%u,%u-%u,%u-%u,%u\n", __func__, group->width,
			  adj_mode->hdisplay, group->height, adj_mode->vdisplay,
			  group->vrr_min, group->vrr_max,
			  drm_mode_vrefresh(adj_mode));
		if (group->width == adj_mode->hdisplay &&
		    group->height == adj_mode->vdisplay &&
		    group->vrr_min <= drm_mode_vrefresh(adj_mode) &&
		    group->vrr_max >= drm_mode_vrefresh(adj_mode)) {
			break;
		}
	}

	DRM_DEBUG("%s, %d, %d\n", __func__, i, am_lcd->num_vrr_group);
	if (i != am_lcd->num_vrr_group) {
		strncpy(crtc_state->brr_mode, group->modename, DRM_DISPLAY_MODE_LEN);
		crtc_state->brr_mode[DRM_DISPLAY_MODE_LEN - 1] = '\0';
		crtc_state->brr = group->brr;
		crtc_state->valid_brr = 1;
	} else {
		DRM_ERROR("%s, get panel brr fail\n", adj_mode->name);
	}
}

static int meson_panel_encoder_atomic_check(struct drm_encoder *encoder,
				       struct drm_crtc_state *crtc_state,
				struct drm_connector_state *conn_state)
{
	struct meson_panel *am_lcd = encoder_to_meson_panel(encoder);
	struct drm_display_mode *adj_mode = &crtc_state->adjusted_mode;
	struct am_meson_crtc_state *meson_crtc_state =
		to_am_meson_crtc_state(crtc_state);

	if (crtc_state->vrr_enabled || meson_crtc_state->uboot_mode_init)
		meson_panel_cal_brr(am_lcd, meson_crtc_state, adj_mode);
	DRM_DEBUG("[%s]-[%d] called\n", __func__, __LINE__);
	return 0;
}

static const struct drm_encoder_helper_funcs meson_panel_encoder_helper_funcs = {
	.atomic_enable = meson_panel_encoder_atomic_enable,
	.atomic_disable = meson_panel_encoder_atomic_disable,
	.atomic_check = meson_panel_encoder_atomic_check,
};

static const struct drm_encoder_funcs meson_panel_encoder_funcs = {
	.destroy        = drm_encoder_cleanup,
};

int meson_panel_dev_bind(struct drm_device *drm,
	int type, struct meson_connector_dev *intf)
{
	struct meson_drm *priv = drm->dev_private;
	struct drm_connector *connector = NULL;
	struct drm_encoder *encoder = NULL;
	struct meson_panel *panel_instance = NULL;
	struct drm_property *type_prop = NULL;
	char *connector_name = NULL;
	int connector_type = type;
	int encoder_type = DRM_MODE_ENCODER_LVDS;
	int ret = 0;

	DRM_INFO("[%s]-[%d] called\n", __func__, __LINE__);

	panel_instance = kzalloc(sizeof(*panel_instance), GFP_KERNEL);
	if (!panel_instance) {
		DRM_ERROR("[%s]: alloc panel_instance failed\n", __func__);
		return -ENOMEM;
	}

	panel_instance->panel_type = type;
	panel_instance->drm = drm;
	panel_instance->panel_dev = to_meson_panel_dev(intf);
	encoder = &panel_instance->encoder;
	if (!encoder)
		return -EINVAL;

	connector = &panel_instance->base.connector;

	if (type > DRM_MODE_MESON_CONNECTOR_PANEL_START &&
		type < DRM_MODE_MESON_CONNECTOR_PANEL_END) {
		switch (type) {
		case DRM_MODE_CONNECTOR_MESON_LVDS_A:
			connector_type = DRM_MODE_CONNECTOR_LVDS;
			encoder_type = DRM_MODE_ENCODER_LVDS;
			connector_name = "LVDS-A";
			break;
		case DRM_MODE_CONNECTOR_MESON_LVDS_B:
			connector_type = DRM_MODE_CONNECTOR_LVDS;
			encoder_type = DRM_MODE_ENCODER_LVDS;
			connector_name = "LVDS-B";
			break;
		case DRM_MODE_CONNECTOR_MESON_LVDS_C:
			connector_type = DRM_MODE_CONNECTOR_LVDS;
			encoder_type = DRM_MODE_ENCODER_LVDS;
			connector_name = "LVDS-C";
			break;
		case DRM_MODE_CONNECTOR_MESON_VBYONE_A:
			connector_type = DRM_MODE_CONNECTOR_LVDS;
			encoder_type = DRM_MODE_ENCODER_LVDS;
			connector_name = "VBYONE-A";
			break;
		case DRM_MODE_CONNECTOR_MESON_VBYONE_B:
			connector_type = DRM_MODE_CONNECTOR_LVDS;
			encoder_type = DRM_MODE_ENCODER_LVDS;
			connector_name = "VBYONE-B";
			break;
		case DRM_MODE_CONNECTOR_MESON_MIPI_A:
			connector_type = DRM_MODE_CONNECTOR_DSI;
			encoder_type = DRM_MODE_ENCODER_DSI;
			connector_name = "MIPI-A";
			break;
		case DRM_MODE_CONNECTOR_MESON_MIPI_B:
			connector_type = DRM_MODE_CONNECTOR_DSI;
			encoder_type = DRM_MODE_ENCODER_DSI;
			connector_name = "MIPI-B";
			break;
		case DRM_MODE_CONNECTOR_MESON_EDP_A:
			connector_type = DRM_MODE_CONNECTOR_eDP;
			encoder_type = DRM_MODE_ENCODER_TMDS;
			connector_name = "EDP-A";
			break;
		case DRM_MODE_CONNECTOR_MESON_EDP_B:
			connector_type = DRM_MODE_CONNECTOR_eDP;
			encoder_type = DRM_MODE_ENCODER_TMDS;
			connector_name = "EDP-B";
			break;
		default:
			connector_type = DRM_MODE_CONNECTOR_Unknown;
			encoder_type = DRM_MODE_ENCODER_NONE;
			break;
		};

		DRM_INFO("%s: bind %d -> connector-%s-%d,encoder-%d\n",
			__func__, type, connector_name, connector_type, encoder_type);
	}

	/* Encoder */
	encoder->possible_crtcs = priv->of_conf.crtc_masks[ENCODER_LCD];
	drm_encoder_helper_add(encoder, &meson_panel_encoder_helper_funcs);
	ret = drm_encoder_init(drm, encoder, &meson_panel_encoder_funcs,
			       encoder_type, "am_lcd_encoder");
	if (ret) {
		DRM_ERROR("error:%s-%d: Failed to init lcd encoder\n",
			__func__, __LINE__);
		goto free_resource;
	}

	/* Connector */
	drm_connector_helper_add(connector, &meson_panel_helper_funcs);
	ret = drm_connector_init(drm, connector, &meson_panel_funcs,
				 connector_type);
	if (ret) {
		DRM_ERROR("%s-%d: Failed to init lcd connector\n",
			__func__, __LINE__);
		goto free_resource;
	}

	/*update name to amlogic name*/
	if (connector_name) {
		kfree(connector->name);
		connector->name = kasprintf(GFP_KERNEL, "%s", connector_name);
		if (!connector->name)
			DRM_ERROR("[%s]: alloc name failed\n", __func__);
	}

	ret = drm_connector_attach_encoder(connector, encoder);
	if (ret != 0) {
		DRM_ERROR("%s-%d: attach failed.\n",
			__func__, __LINE__);
		goto free_resource;
	}

	/*prop for userspace to acquire prop*/
	type_prop = drm_property_create_range(drm, DRM_MODE_PROP_IMMUTABLE,
		MESON_CONNECTOR_TYPE_PROP_NAME, 0, INT_MAX);
	if (type_prop) {
		panel_instance->panel_type_prop = type_prop;
		drm_object_attach_property(&panel_instance->base.connector.base,
			type_prop, type);
	} else {
		DRM_ERROR("%s: Failed to create property %s\n",
			__func__, MESON_CONNECTOR_TYPE_PROP_NAME);
	}

	drm_connector_attach_vrr_capable_property(connector);

	DRM_INFO("%s: bind %d success\n", __func__, type);
	return 0;

free_resource:
	if (connector)
		drm_connector_cleanup(connector);
	if (encoder)
		drm_encoder_cleanup(encoder);
	kfree(panel_instance);

	DRM_DEBUG("%s: %d Exit\n", __func__, ret);
	return ret;
}

int meson_panel_dev_unbind(struct drm_device *drm,
	int type, int connector_id)
{
	struct drm_connector *connector =
		drm_connector_lookup(drm, 0, connector_id);
	struct meson_panel *drm_lcd = 0;

	if (!connector)
		DRM_ERROR("%s got invalid connector id %d\n",
			__func__, connector_id);

	drm_lcd = connector_to_meson_panel(connector);
	if (!drm_lcd)
		return -EINVAL;

	drm_lcd->base.connector.funcs->destroy(&drm_lcd->base.connector);
	drm_lcd->encoder.funcs->destroy(&drm_lcd->encoder);

	kfree(drm_lcd);
	DRM_DEBUG("[%s]-[%d] called\n", __func__, __LINE__);
	return 0;
}

int am_meson_lcd_get_vrr_range(struct drm_connector *connector,
			struct drm_vrr_mode_group *groups, int max_group)
{
	int ret, modes_cnt;
	struct meson_panel *am_lcd = connector_to_meson_panel(connector);
	struct meson_panel_dev *panel_dev = am_lcd->panel_dev;

	if (!panel_dev->get_modes_vrr_range)
		return 0;

	ret = panel_dev->get_modes_vrr_range(panel_dev, (void *)groups, max_group, &modes_cnt);
	if (ret) {
		DRM_ERROR("get vrr error or not support qms\n");
		return 0;
	}

	return modes_cnt;
}
