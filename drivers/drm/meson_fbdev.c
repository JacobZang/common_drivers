// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <drm/drm.h>
#include <drm/drmP.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_uapi.h>
#include <drm/drm_atomic_helper.h>
#include <linux/dma-buf.h>
#include <linux/amlogic/ion.h>
#include <linux/sysrq.h>

#include "meson_drv.h"
#include "meson_gem.h"
#include "meson_fb.h"
#include "meson_fbdev.h"
#include "meson_plane.h"
#include "meson_vpu_pipeline.h"

#define PREFERRED_BPP		32
#define PREFERRED_DEPTH	32
#define MESON_DRM_MAX_CONNECTOR	2
#define MAX_RETRY_CNT 20

//static bool drm_leak_fbdev_smem = false;
//used for double buffer, one buffer is 100
static int drm_fbdev_overalloc = 200;
static u32 rdma_chk_addr[] = {
	VIU_OSD1_TCOLOR_AG3,
	VIU_OSD2_TCOLOR_AG3,
	VIU_OSD3_TCOLOR_AG3,
};

static LIST_HEAD(kernel_fb_helper_list);
static DEFINE_MUTEX(kernel_fb_helper_lock);

static int am_meson_fbdev_alloc_fb_gem(struct fb_info *info)
{
	struct am_meson_fb *meson_fb;
	struct drm_fb_helper *helper = info->par;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct drm_framebuffer *fb = helper->fb;
	struct drm_device *dev = helper->dev;
	size_t size = info->screen_size;
	struct am_meson_gem_object *meson_gem;
	void *vaddr;
	struct page **pages;
	struct page **tmp;
	struct sg_page_iter piter;
	pgprot_t pgprot;
	int npages;

	if (!fbdev->fb_gem) {
		meson_gem = am_meson_gem_object_create(dev, 0, size);
		if (IS_ERR(meson_gem)) {
			DRM_ERROR("alloc memory %d fail\n", (u32)size);
			return -ENOMEM;
		}
		fbdev->fb_gem = &meson_gem->base;
		fb = helper->fb;
		meson_fb = container_of(fb, struct am_meson_fb, base);
		if (!meson_fb) {
			DRM_INFO("meson_fb is NULL!\n");
			return -EINVAL;
		}
		meson_fb->bufp[0] = meson_gem;
		if (meson_gem->is_dma) {
			npages = PAGE_ALIGN(meson_gem->base.size) / PAGE_SIZE;
			pages = vmalloc(array_size(npages, sizeof(struct page *)));
			tmp = pages;

			if (!pages)
				return -ENOMEM;

			pgprot = pgprot_writecombine(PAGE_KERNEL);

			for_each_sgtable_page(meson_gem->sg, &piter, 0) {
				WARN_ON(tmp - pages >= npages);
				*tmp++ = sg_page_iter_page(&piter);
			}

			vaddr = vmap(pages, npages, VM_MAP, pgprot);
			vfree(pages);
		} else {
			vaddr = ion_heap_map_kernel(meson_gem->ionbuffer->heap,
						meson_gem->ionbuffer);
		}
		info->screen_base = (char __iomem *)vaddr;
		info->fix.smem_start = meson_gem->addr;

		MESON_DRM_FBDEV("alloc memory %d done\n", (u32)size);
	} else {
		MESON_DRM_FBDEV("no need repeate alloc memory %d\n", (u32)size);
	}
	return 0;
}

static void am_meson_fbdev_free_fb_gem(struct fb_info *info)
{
	struct drm_fb_helper *helper = info->par;
	struct meson_drm_fbdev *fbdev;
	struct drm_framebuffer *fb;
	struct am_meson_fb *meson_fb;

	if (!helper) {
		DRM_ERROR("fb helper is NULL!\n");
		return;
	}

	fbdev = container_of(helper, struct meson_drm_fbdev, base);
	if (fbdev && fbdev->fb_gem) {
		struct drm_gem_object *gem_obj = fbdev->fb_gem;
		struct am_meson_gem_object *meson_gem = container_of(gem_obj,
					struct am_meson_gem_object, base);

		if (!meson_gem->is_dma)
			ion_heap_unmap_kernel(meson_gem->ionbuffer->heap,
					meson_gem->ionbuffer);
		info->screen_base = NULL;

		meson_gem_object_free(fbdev->fb_gem);
		fbdev->fb_gem = NULL;

		fb = helper->fb;
		if (fb) {
			meson_fb = container_of(fb, struct am_meson_fb, base);
			if (meson_fb)
				meson_fb->bufp[0] = NULL;
			else
				DRM_ERROR("meson_fb is NULL!\n");
		} else {
			DRM_ERROR("drm framebuffer is NULL!\n");
		}

		MESON_DRM_FBDEV("free memory done\n");
	} else {
		MESON_DRM_FBDEV("memory already free before\n");
	}
}

static int am_meson_fbdev_open(struct fb_info *info, int arg)
{
	int ret = 0;
	struct drm_fb_helper *helper = info->par;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct am_osd_plane *osdplane = container_of(fbdev->plane, struct am_osd_plane, base);

	MESON_DRM_FBDEV("%s - %d\n", __func__, osdplane->plane_index);
	ret = am_meson_fbdev_alloc_fb_gem(info);
	return ret;
}

static int am_meson_fbdev_release(struct fb_info *info, int arg)
{
	MESON_DRM_FBDEV("may no need to release memory\n");
	return 0;
}

static int am_meson_fbdev_mmap(struct fb_info *info,
			       struct vm_area_struct *vma)
{
	struct drm_fb_helper *helper = info->par;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct am_meson_gem_object *meson_gem;

	meson_gem = container_of(fbdev->fb_gem,
				 struct am_meson_gem_object, base);

	return meson_gem_prime_mmap(fbdev->fb_gem, vma);
}

static int am_meson_drm_fbdev_sync(struct fb_info *info)
{
	return 0;
}

static int am_meson_drm_fbdev_setcmap(struct fb_cmap *cmap, struct fb_info *info)
{
	int count, index, r;
	u16 *red, *green, *blue, *transp;
	u16 trans = 0xffff;

	red     = cmap->red;
	green   = cmap->green;
	blue    = cmap->blue;
	transp  = cmap->transp;
	index   = cmap->start;
	DRM_DEBUG("%s\n", __func__);

	if (info->fix.visual == FB_VISUAL_PSEUDOCOLOR) {
		for (count = 0; count < cmap->len; count++) {
			if (transp)
				trans = *transp++;
			if (info->fbops->fb_setcolreg)
				r = info->fbops->fb_setcolreg(index++, *red++, *green++, *blue++,
					trans, info);
			if (r != 0)
				return r;
		}
	} else {
		r = drm_fb_helper_setcmap(cmap, info);
		return r;
	}

	return 0;
}

static int
am_meson_drm_fbdev_setcolreg(unsigned int regno, unsigned int red, unsigned int green,
			     unsigned int blue, unsigned int transp, struct fb_info *info)
{
	struct drm_fb_helper *helper = info->par;
	struct fb_var_screeninfo *var = &info->var;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct drm_device *dev = helper->dev;
	struct am_osd_plane *osd_plane = to_am_osd_plane(fbdev->plane);

	MESON_DRM_FBDEV("%s, pixel is %d bits\n", __func__, var->bits_per_pixel);
	if (var->bits_per_pixel == 8 && regno < 256) {
		drm_modeset_lock_all(dev);
		fbdev->fbdev_rec_palette[regno] = ((red & 0xff) << 24) |
						  ((green & 0xff) << 16) |
						  ((blue  & 0xff) <<  8) |
						  (transp & 0xff);
		osd_plane->receive_palette = &fbdev->fbdev_rec_palette[0];
		MESON_DRM_FBDEV("palette index-%d value-0x%x.\n", regno,
			osd_plane->receive_palette[regno]);
		drm_modeset_unlock_all(dev);
	}

	return 0;
}

static int am_meson_drm_fbdev_ioctl(struct fb_info *info,
				    unsigned int cmd, unsigned long arg)
{
	int ret = 0, crtc_index = 0, i = 0;
	u32 val;
	void __user *argp = (void __user *)arg;
	struct fb_dmabuf_export fbdma;
	struct drm_fb_helper *helper = info->par;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct drm_plane *plane = fbdev->plane;
	struct am_meson_fb *meson_fb;

	memset(&fbdma, 0, sizeof(fbdma));
	MESON_DRM_FBDEV("%s CMD   [%x] - [%d] IN\n", __func__, cmd, plane->index);

	/*amlogic fbdev ioctl, used by gpu fbdev backend.*/
	if (cmd == FBIOGET_OSD_DMABUF) {
		meson_fb = container_of(helper->fb, struct am_meson_fb, base);
		fbdma.fd = dma_buf_fd(meson_fb->bufp[0]->dmabuf, O_CLOEXEC);
		fbdma.flags = O_CLOEXEC;
		dma_buf_get(fbdma.fd);
		ret = copy_to_user(argp, &fbdma, sizeof(fbdma)) ? -EFAULT : 0;
	} else if (cmd == FBIO_WAITFORVSYNC) {
		if (plane->crtc)
			crtc_index = plane->crtc->index;
		else if (fbdev->modeset.crtc)
			crtc_index = fbdev->modeset.crtc->index;
		else
			crtc_index = 0;

		drm_wait_one_vblank(helper->dev, crtc_index);
		val = meson_drm_read_reg(rdma_chk_addr[plane->index]);

		while (i < MAX_RETRY_CNT && val != frame_seq[plane->index]) {
			usleep_range(2000, 2500);
			i++;
			val = meson_drm_read_reg(rdma_chk_addr[plane->index]);
		}

		if (i == MAX_RETRY_CNT)
			DRM_ERROR("%s timeout, frame seq %u-%u\n", __func__,
				  frame_seq[plane->index], val);

		MESON_DRM_FBDEV("wait for vsync last for %d ms, %u-%u\n", i * 2,
			  frame_seq[plane->index], val);
	}

	MESON_DRM_FBDEV("%s CMD   [%x] - [%d] OUT\n", __func__, cmd, plane->index);
	return ret;
}

/**
 * am_meson_drm_fb_helper_check_var - implementation for ->fb_check_var
 * @var: screeninfo to check
 * @info: fbdev registered by the helper
 */
static int am_meson_drm_fb_helper_check_var(struct fb_var_screeninfo *var,
					    struct fb_info *info)
{
	struct drm_fb_helper *fb_helper = info->par;
	struct drm_framebuffer *fb = fb_helper->fb;
	int depth;

	if (var->pixclock != 0 || in_dbg_master()) {
		DRM_ERROR("%s FAILED.\n", __func__);
		return -EINVAL;
	}

	/*
	 * Changes struct fb_var_screeninfo are currently not pushed back
	 * to KMS, hence fail if different settings are requested.
	 */
	MESON_DRM_FBDEV("fb requested w/h/bpp  %dx%d-%d (virtual %dx%d)\n",
		  var->xres, var->yres, var->bits_per_pixel,
		  var->xres_virtual, var->yres_virtual);
	MESON_DRM_FBDEV("current fb w/h/bpp %dx%d-%d\n",
		  fb->width, fb->height, fb->format->depth);
	if (var->bits_per_pixel != fb->format->depth ||
	    var->xres_virtual != fb->width ||
	    var->yres_virtual != fb->height)
		MESON_DRM_FBDEV("%s need realloc buffer\n", __func__);

	switch (var->bits_per_pixel) {
	case 16:
		depth = (var->green.length == 6) ? 16 : 15;
		break;
	case 32:
		depth = (var->transp.length > 0) ? 32 : 24;
		break;
	default:
		depth = var->bits_per_pixel;
		break;
	}

	switch (depth) {
	case 8:
		var->red.offset = 0;
		var->green.offset = 0;
		var->blue.offset = 0;
		var->red.length = 8;
		var->green.length = 8;
		var->blue.length = 8;
		var->transp.length = 0;
		var->transp.offset = 0;
		break;
	case 15:
		var->red.offset = 10;
		var->green.offset = 5;
		var->blue.offset = 0;
		var->red.length = 5;
		var->green.length = 5;
		var->blue.length = 5;
		var->transp.length = 1;
		var->transp.offset = 15;
		break;
	case 16:
		var->red.offset = 11;
		var->green.offset = 5;
		var->blue.offset = 0;
		var->red.length = 5;
		var->green.length = 6;
		var->blue.length = 5;
		var->transp.length = 0;
		var->transp.offset = 0;
		break;
	case 24:
		var->red.offset = 16;
		var->green.offset = 8;
		var->blue.offset = 0;
		var->red.length = 8;
		var->green.length = 8;
		var->blue.length = 8;
		var->transp.length = 0;
		var->transp.offset = 0;
		break;
	case 32:
		var->red.offset = 16;
		var->green.offset = 8;
		var->blue.offset = 0;
		var->red.length = 8;
		var->green.length = 8;
		var->blue.length = 8;
		var->transp.length = 8;
		var->transp.offset = 24;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

/**
 * drm_fb_helper_set_par - implementation for ->fb_set_par
 * @info: fbdev registered by the helper
 *
 * This will let fbcon do the mode init and is called at initialization time by
 * the fbdev core when registering the driver, and later on through the hotplug
 * callback.
 */
int am_meson_drm_fb_helper_set_par(struct fb_info *info)
{
	struct drm_fb_helper *fb_helper = info->par;
	struct fb_var_screeninfo *var = &info->var;
	struct drm_framebuffer *fb = fb_helper->fb;
	struct meson_drm_fbdev *fbdev = container_of(fb_helper, struct meson_drm_fbdev, base);
	struct drm_gem_object *fb_gem = fbdev->fb_gem;
	struct drm_fb_helper_surface_size sizes;
	struct drm_device *dev = fb_helper->dev;
	struct drm_mode_fb_cmd2 mode_cmd = { 0 };
	unsigned int bytes_per_pixel;
	u32 xres_set = var->xres;
	u32 yres_set = var->yres;
	int depth;

	if (oops_in_progress)
		return -EBUSY;

	if (var->pixclock != 0) {
		DRM_ERROR("PIXEL CLOCK SET\n");
		return -EINVAL;
	}

	DRM_INFO("%s IN\n", __func__);
	if (var->bits_per_pixel != fb->format->depth ||
		var->xres_virtual != fb->width ||
		var->yres_virtual != fb->height) {
		fbdev->vscreen_info_changed = true;
		/*realloc framebuffer, free old then alloc new gem*/
		sizes.fb_height = var->yres_virtual;
		sizes.fb_width = var->xres_virtual;
		sizes.surface_width = sizes.fb_width;
		sizes.surface_height = sizes.fb_height;
		sizes.surface_bpp = var->bits_per_pixel;
		sizes.surface_depth = PREFERRED_DEPTH;

		switch (var->bits_per_pixel) {
		case 16:
			depth = (var->green.length == 6) ? 16 : 15;
			break;
		case 32:
			depth = (var->transp.length > 0) ? 32 : 24;
			break;
		default:
			depth = var->bits_per_pixel;
			break;
		}

		bytes_per_pixel = DIV_ROUND_UP(sizes.surface_bpp, 8);
		mode_cmd.width = sizes.surface_width;
		mode_cmd.height = sizes.surface_height;
		mode_cmd.pixel_format = drm_mode_legacy_fb_format(sizes.surface_bpp, depth);
		mode_cmd.pitches[0] = ALIGN(mode_cmd.width * bytes_per_pixel, 64);
		MESON_DRM_FBDEV("DRM Pixel Format: %c%c%c%c\n",
				(mode_cmd.pixel_format & 0xFF),
				((mode_cmd.pixel_format >> 8) & 0xFF),
				((mode_cmd.pixel_format >> 16) & 0xFF),
				((mode_cmd.pixel_format >> 24) & 0xFF));

		fb->width = sizes.fb_width;
		fb->height = sizes.fb_height;
		fb->pitches[0] =  ALIGN(fb->width * bytes_per_pixel, 64);
		fb->format = drm_get_format_info(dev, &mode_cmd);

		info->screen_size = fb->pitches[0] * fb->height;
		//info->fix.smem_len = info->screen_size;

		drm_fb_helper_fill_info(info, fb_helper, &sizes);
		/* fix some bug in drm_fb_helper_full_info */
		var->xres = xres_set;
		var->yres = yres_set;

		if (fb_gem && fb_gem->size < info->screen_size) {
			MESON_DRM_FBDEV("GEM SIZE is not enough, no re-allocate.\n");
			am_meson_fbdev_free_fb_gem(info);
			fb_gem = NULL;
		}

		if (!fb_gem) {
			if (am_meson_fbdev_alloc_fb_gem(info)) {
				DRM_ERROR("%s realloc fb fail\n", __func__);
				return -ENOMEM;
			}
			MESON_DRM_FBDEV("%s reallocate success.\n", __func__);
		}
	}
	drm_wait_one_vblank(fb_helper->dev, 0);
	DRM_INFO("fb_set_par: %s OUT\n", __func__);

	return 0;
}

/* sync of drm_client_modeset_commit_atomic(),
 * 1. add fbdev for non-primary plane.
 * 2. remove crtc set.
 */
static int am_meson_drm_fb_pan_display(struct fb_var_screeninfo *var,
				       struct fb_info *info)
{
	struct drm_fb_helper *fb_helper = info->par;
	struct meson_drm_fbdev *fbdev = container_of(fb_helper, struct meson_drm_fbdev, base);
	struct drm_device *dev = fb_helper->dev;
	struct drm_atomic_state *state;
	struct drm_plane_state *plane_state;
	struct am_meson_plane_state *am_plane_state;
	struct drm_plane *plane = fbdev->plane;
	struct drm_mode_set *mode_set;
	int hdisplay, vdisplay;
	int ret;

	if (fbdev->blank) {
		MESON_DRM_FBDEV("%s skip blank.\n", __func__);
		return 0;
	}

	if (fbdev->vscreen_info_changed) {
		fbdev->vscreen_info_changed = false;
		DRM_INFO("%s, skip set_par's pan display\n", __func__);
		return 0;
	}

	drm_modeset_lock_all(dev);
	MESON_DRM_FBDEV("%s IN [%d]\n", __func__, plane->index);

	state = drm_atomic_state_alloc(dev);
	if (!state) {
		ret = -ENOMEM;
		goto unlock_exit;
	}

	state->acquire_ctx = dev->mode_config.acquire_ctx;
retry:
	MESON_DRM_FBDEV("%s for plane [%d-%p]\n", __func__, plane->type, fb_helper->fb);

	mode_set = &fbdev->modeset;

	/*update plane state, refer to drm_atomic_plane_set_property()*/
	plane_state = drm_atomic_get_plane_state(state, plane);
	if (IS_ERR(plane_state)) {
		ret = PTR_ERR(plane_state);
		goto fail;
	}

	ret = drm_atomic_set_crtc_for_plane(plane_state, mode_set->crtc);
	if (ret != 0) {
		DRM_ERROR("set crtc for plane failed.\n");
		goto fail;
	}

	drm_mode_get_hv_timing(&mode_set->crtc->mode, &hdisplay, &vdisplay);
	plane_state->crtc_x = 0;
	plane_state->crtc_y = 0;
	plane_state->crtc_w = hdisplay;
	plane_state->crtc_h = vdisplay;

	drm_atomic_set_fb_for_plane(plane_state, fb_helper->fb);
	if (fb_helper->fb) {
		plane_state->src_x = var->xoffset << 16;
		plane_state->src_y = var->yoffset << 16;
		plane_state->src_w = var->xres << 16;
		plane_state->src_h = var->yres << 16;
		plane_state->zpos = fbdev->zorder;
	} else {
		plane_state->src_x = 0;
		plane_state->src_y = 0;
		plane_state->src_w = 0;
		plane_state->src_h = 0;
		plane_state->zpos = fbdev->zorder;
	}
	/* fix alpha */
	plane_state->pixel_blend_mode = DRM_MODE_BLEND_PREMULTI;

	MESON_DRM_FBDEV("update fb [%x-%x, %x-%x]-%d->[%d-%d]",
		plane_state->src_x, plane_state->src_y,
		plane_state->src_w, plane_state->src_h,
		plane_state->zpos, plane_state->crtc_w,
		plane_state->crtc_h);

	state->legacy_cursor_update = true;
	am_plane_state = to_am_meson_plane_state(plane_state);
	am_plane_state->fbdev_commit = true;
	ret = drm_atomic_commit(state);
	if (ret != 0)
		goto fail;
	am_plane_state->fbdev_commit = false;
	info->var.xoffset = var->xoffset;
	info->var.yoffset = var->yoffset;

fail:
	if (ret == -EDEADLK)
		goto backoff;

	drm_atomic_state_put(state);

unlock_exit:
	drm_modeset_unlock_all(dev);

	if (ret)
		DRM_ERROR("%s failed .\n", __func__);
	else
		DRM_DEBUG("%s OUT [%d]\n", __func__, plane->index);

	return ret;

backoff:
	drm_atomic_state_clear(state);
	drm_modeset_backoff(state->acquire_ctx);

	goto retry;
}

/**
 * the implement if different from drm_fb_helper.
 * for plane based fbdev, we only disable corresponding plane
 * but not the whole crtc.
 */
int am_meson_drm_fb_blank(int blank, struct fb_info *info)
{
	struct drm_fb_helper *helper = info->par;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct drm_device *dev = helper->dev;
	struct meson_drm *priv = dev->dev_private;
	int ret = 0;

	if (blank == 0) {
		MESON_DRM_FBDEV("meson_fbdev[%s] goto UNBLANK.\n", fbdev->plane->name);
		fbdev->blank = false;
		ret = am_meson_drm_fb_pan_display(&info->var, info);
		drm_wait_one_vblank(dev, 0);
	} else {
		MESON_DRM_FBDEV("meson_fbdev[%s-%p] goto blank.\n",
			fbdev->plane->name, fbdev->plane->fb);
		drm_modeset_lock_all(dev);
		if (priv->pan_async_commit_ran) {
			DRM_INFO("Force to wait one vblank!\n");
			drm_wait_one_vblank(dev, 0);
		}
		drm_atomic_helper_disable_plane(fbdev->plane, dev->mode_config.acquire_ctx);
		drm_modeset_unlock_all(dev);

		fbdev->blank = true;
	}

	return ret;
}

static struct fb_ops meson_drm_fbdev_ops = {
	.owner		= THIS_MODULE,
	.fb_open        = am_meson_fbdev_open,
	.fb_release     = am_meson_fbdev_release,
	.fb_mmap	= am_meson_fbdev_mmap,
	.fb_fillrect	= drm_fb_helper_cfb_fillrect,
	.fb_copyarea	= drm_fb_helper_cfb_copyarea,
	.fb_imageblit	= drm_fb_helper_cfb_imageblit,
	.fb_check_var	= am_meson_drm_fb_helper_check_var,
	.fb_set_par	= am_meson_drm_fb_helper_set_par,
	.fb_blank	= am_meson_drm_fb_blank,
	.fb_pan_display	= am_meson_drm_fb_pan_display,
	.fb_setcmap	= am_meson_drm_fbdev_setcmap,
	.fb_setcolreg   = am_meson_drm_fbdev_setcolreg,
	.fb_sync	= am_meson_drm_fbdev_sync,
	.fb_ioctl       = am_meson_drm_fbdev_ioctl,
#ifdef CONFIG_COMPAT
	.fb_compat_ioctl = am_meson_drm_fbdev_ioctl,
#endif
};

static int am_meson_drm_fbdev_modeset_create(struct drm_fb_helper *helper)
{
	struct drm_device *dev = helper->dev;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct am_osd_plane *amp = to_am_osd_plane(fbdev->plane);
	struct meson_drm *private = dev->dev_private;
	int crtc_id = private->of_conf.crtcmask_osd[amp->plane_index];

	fbdev->modeset.crtc = drm_crtc_from_index(dev, crtc_id);
	DRM_INFO("%s in, plane_index = %d, crtc_id = %d\n",
		__func__, amp->plane_index, crtc_id);

	return 0;
}

static int am_meson_drm_fbdev_probe(struct drm_fb_helper *helper,
				     struct drm_fb_helper_surface_size *sizesxx)
{
	struct drm_device *dev = helper->dev;
	struct meson_drm *private = dev->dev_private;
	struct meson_drm_fbdev *fbdev = container_of(helper, struct meson_drm_fbdev, base);
	struct drm_mode_fb_cmd2 mode_cmd = { 0 };
	struct drm_fb_helper_surface_size sizes;
	struct drm_framebuffer *fb;
	struct fb_info *fbi;
	unsigned int bytes_per_pixel;
	int ret;

	if (private->ui_config.overlay_flag == 1) {
		sizes.fb_width = private->ui_config.overlay_ui_w;
		sizes.fb_height = private->ui_config.overlay_ui_h;
		sizes.surface_width = private->ui_config.overlay_fb_w;
		sizes.surface_height = private->ui_config.overlay_fb_h;
		sizes.surface_bpp = private->ui_config.overlay_fb_bpp;
	} else {
		sizes.fb_width = private->ui_config.ui_w;
		sizes.fb_height = private->ui_config.ui_h;
		sizes.surface_width = private->ui_config.fb_w;
		sizes.surface_height = private->ui_config.fb_h;
		sizes.surface_bpp = private->ui_config.fb_bpp;
	}

	sizes.surface_depth = PREFERRED_DEPTH;

	bytes_per_pixel = DIV_ROUND_UP(sizes.surface_bpp, 8);
	mode_cmd.width = sizes.surface_width;
	mode_cmd.height = sizes.surface_height;
	mode_cmd.pixel_format = drm_mode_legacy_fb_format(sizes.surface_bpp, PREFERRED_DEPTH);
	mode_cmd.pitches[0] = ALIGN(mode_cmd.width * bytes_per_pixel, 64);

	DRM_INFO("mode_cmd.width = %d\n", mode_cmd.width);
	DRM_INFO("mode_cmd.height = %d\n", mode_cmd.height);
	DRM_INFO("mode_cmd.pixel_format = %d-%d\n", mode_cmd.pixel_format, DRM_FORMAT_ARGB8888);

	fbi = drm_fb_helper_alloc_fbi(helper);
	if (IS_ERR(fbi)) {
		DRM_ERROR("Failed to create framebuffer info.\n");
		ret = PTR_ERR(fbi);
		return ret;
	}

	helper->fb = am_meson_drm_framebuffer_init(dev, &mode_cmd,
						   fbdev->fb_gem);
	if (IS_ERR(helper->fb)) {
		dev_err(dev->dev, "Failed to allocate DRM framebuffer.\n");
		ret = PTR_ERR(helper->fb);
		goto err_release_fbi;
	}
	fb = helper->fb;

	fbi->par = helper;
	fbi->flags = FBINFO_FLAG_DEFAULT;
	fbi->fbops = &meson_drm_fbdev_ops;
	fbi->skip_vt_switch = true;
	fbi->screen_size = fb->pitches[0] * fb->height;
	fbi->fix.smem_len = fbi->screen_size;

	drm_fb_helper_fill_info(fbi, helper, &sizes);
	am_meson_drm_fbdev_modeset_create(helper);

	return 0;

err_release_fbi:
	drm_fb_helper_fini(helper);
	return ret;
}

static const struct drm_fb_helper_funcs meson_drm_fb_helper_funcs = {
	.fb_probe = am_meson_drm_fbdev_probe,
};

static int am_meson_fbdev_parse_config(struct drm_device *dev)
{
	struct meson_drm *private = dev->dev_private;
	struct drm_display_mode mode;
	struct meson_vpu_pipeline *pipeline = private->pipeline;
	u32 sizes[5], overlay_sizes[5];
	int ret = 0, tmp, i;
	const void *prop = NULL;

	if (private->primary_plane && private->primary_plane->state && private->primary_plane->state->crtc && private->primary_plane->state->crtc->state) {
		mode = private->primary_plane->state->crtc->state->mode;

		sizes[0] = mode.hdisplay;
		sizes[1] = mode.vdisplay;
		sizes[2] = mode.hdisplay;
		sizes[3] = mode.vdisplay * 2;
		sizes[4] = 32;

		// Default enable HDMI 4k fb
		prop = of_get_property(dev->dev->of_node, "4k2k_fb", NULL);
		if (prop) {
			if ((0 == of_read_ulong(prop, 1)) && strstr(mode.name, "hz")) {
				// Limit HDMI fb to 1080P
				if (mode.hdisplay >= 1920 && mode.vdisplay >= 1080) {
					sizes[0] = 1920;
					sizes[1] = 1080;
					sizes[2] = 1920;
					sizes[3] = 2160;
					sizes[4] = 32;
				}
			}
		}
	} else {
		ret = of_property_read_u32_array(dev->dev->of_node,
					   "fbdev_sizes", sizes, 5);
	}

	tmp = of_property_read_u32_array(dev->dev->of_node,
				   "fbdev_overlay_sizes", overlay_sizes, 5);
	if (!ret) {
		private->ui_config.ui_w = sizes[0];
		private->ui_config.ui_h = sizes[1];
		private->ui_config.fb_w = sizes[2];
		private->ui_config.fb_h = sizes[3];
		private->ui_config.fb_bpp = sizes[4];
		private->ui_config.overlay_ui_w = sizes[0];
		private->ui_config.overlay_ui_h = sizes[1];
		private->ui_config.overlay_fb_w = sizes[2];
		private->ui_config.overlay_fb_h = sizes[3];
		private->ui_config.overlay_fb_bpp = sizes[4];
	}
	if (!tmp) {
		private->ui_config.overlay_ui_w = overlay_sizes[0];
		private->ui_config.overlay_ui_h = overlay_sizes[1];
		private->ui_config.overlay_fb_w = overlay_sizes[2];
		private->ui_config.overlay_fb_h = overlay_sizes[3];
		private->ui_config.overlay_fb_bpp = overlay_sizes[4];
	}
	/*initial fbdev zorder*/
	for (i = 0; i < MESON_MAX_OSD; i++)
		private->fbdev_zorder[i] = i;

	tmp = of_property_read_u32_array(dev->dev->of_node, "fbdev_zorder",
		private->fbdev_zorder, pipeline->num_osds);
	if (tmp)
		MESON_DRM_FBDEV("undefined fbdev_zorder!\n");

	return ret;
}

static ssize_t show_force_free_mem(struct device *device,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "Usage: echo 1 > force_free mem\n");
}

static ssize_t store_force_free_mem(struct device *device,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);

	if (strncmp("1", buf, 1) == 0) {
		am_meson_fbdev_free_fb_gem(fb_info);
		DRM_INFO("fb mem is freed !\n");
	}
	return count;
}

static struct device_attribute fbdev_device_attrs[] = {
	__ATTR(force_free_mem, 0644, show_force_free_mem, store_force_free_mem),
};

static void meson_setup_crtcs_fb(struct drm_fb_helper *fb_helper)
{
	struct drm_client_dev *client = &fb_helper->client;
	struct drm_connector_list_iter conn_iter;
	struct fb_info *info = fb_helper->fbdev;
	unsigned int rotation, sw_rotations = 0;
	struct drm_connector *connector;
	struct drm_mode_set *modeset;

	mutex_lock(&client->modeset_mutex);
	drm_client_for_each_modeset(modeset, client) {
		if (!modeset->num_connectors)
			continue;

		modeset->fb = fb_helper->fb;

		if (drm_client_rotation(modeset, &rotation))
			/* Rotating in hardware, fbcon should not rotate */
			sw_rotations |= DRM_MODE_ROTATE_0;
		else
			sw_rotations |= rotation;
	}
	mutex_unlock(&client->modeset_mutex);

	drm_connector_list_iter_begin(fb_helper->dev, &conn_iter);
	drm_client_for_each_connector_iter(connector, &conn_iter) {
		/* use first connected connector for the physical dimensions */
		if (connector->status == connector_status_connected) {
			info->var.width = connector->display_info.width_mm;
			info->var.height = connector->display_info.height_mm;
			break;
		}
	}
	drm_connector_list_iter_end(&conn_iter);

	switch (sw_rotations) {
	case DRM_MODE_ROTATE_0:
		info->fbcon_rotate_hint = FB_ROTATE_UR;
		break;
	case DRM_MODE_ROTATE_90:
		info->fbcon_rotate_hint = FB_ROTATE_CCW;
		break;
	case DRM_MODE_ROTATE_180:
		info->fbcon_rotate_hint = FB_ROTATE_UD;
		break;
	case DRM_MODE_ROTATE_270:
		info->fbcon_rotate_hint = FB_ROTATE_CW;
		break;
	default:
		/*
		 * Multiple bits are set / multiple rotations requested
		 * fbcon cannot handle separate rotation settings per
		 * output, so fallback to unrotated.
		 */
		info->fbcon_rotate_hint = FB_ROTATE_UR;
	}
}

#ifdef CONFIG_MAGIC_SYSRQ
/* emergency restore, don't bother with error reporting */
static void meson_fb_helper_restore_work_fn(struct work_struct *ignored)
{
	struct drm_fb_helper *helper;

	mutex_lock(&kernel_fb_helper_lock);
	list_for_each_entry(helper, &kernel_fb_helper_list, kernel_fb_list) {
		struct drm_device *dev = helper->dev;

		if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
			continue;

		mutex_lock(&helper->lock);
		drm_client_modeset_commit_locked(&helper->client);
		mutex_unlock(&helper->lock);
	}
	mutex_unlock(&kernel_fb_helper_lock);
}

static DECLARE_WORK(drm_fb_helper_restore_work, meson_fb_helper_restore_work_fn);

static void meson_fb_helper_sysrq(int dummy1)
{
	schedule_work(&drm_fb_helper_restore_work);
}

static const struct sysrq_key_op sysrq_drm_fb_helper_restore_op = {
	.handler = meson_fb_helper_sysrq,
	.help_msg = "force-fb(v)",
	.action_msg = "Restore framebuffer console",
};
#else
static const struct sysrq_key_op sysrq_drm_fb_helper_restore_op = { };
#endif

static int meson_fb_helper_single_fb_probe(struct drm_fb_helper *fb_helper,
					 int preferred_bpp)
{
	struct drm_client_dev *client = &fb_helper->client;
	struct drm_device *dev = fb_helper->dev;
	struct drm_mode_config *config = &dev->mode_config;
	int ret = 0;
	int crtc_count = 0;
	struct drm_connector_list_iter conn_iter;
	struct drm_fb_helper_surface_size sizes;
	struct drm_connector *connector;
	struct drm_mode_set *mode_set;
	int best_depth = 0;

	memset(&sizes, 0, sizeof(struct drm_fb_helper_surface_size));
	sizes.surface_depth = 24;
	sizes.surface_bpp = 32;
	sizes.fb_width = (u32)-1;
	sizes.fb_height = (u32)-1;

	/*
	 * If driver picks 8 or 16 by default use that for both depth/bpp
	 * to begin with
	 */
	if (preferred_bpp != sizes.surface_bpp) {
		sizes.surface_depth = preferred_bpp;
		sizes.surface_bpp = preferred_bpp;
	}

	drm_connector_list_iter_begin(fb_helper->dev, &conn_iter);
	drm_client_for_each_connector_iter(connector, &conn_iter) {
		struct drm_cmdline_mode *cmdline_mode;

		cmdline_mode = &connector->cmdline_mode;

		if (cmdline_mode->bpp_specified) {
			switch (cmdline_mode->bpp) {
			case 8:
				sizes.surface_depth = 8;
				sizes.surface_bpp = 8;
				break;
			case 15:
				sizes.surface_depth = 15;
				sizes.surface_bpp = 16;
				break;
			case 16:
				sizes.surface_depth = 16;
				sizes.surface_bpp = 16;
				break;
			case 24:
				sizes.surface_depth = 24;
				sizes.surface_bpp = 24;
				break;
			case 32:
				sizes.surface_depth = 24;
				sizes.surface_bpp = 32;
				break;
			}
			break;
		}
	}
	drm_connector_list_iter_end(&conn_iter);

	/*
	 * If we run into a situation where, for example, the primary plane
	 * supports RGBA5551 (16 bpp, depth 15) but not RGB565 (16 bpp, depth
	 * 16) we need to scale down the depth of the sizes we request.
	 */
	mutex_lock(&client->modeset_mutex);
	drm_client_for_each_modeset(mode_set, client) {
		struct drm_crtc *crtc = mode_set->crtc;
		struct drm_plane *plane = crtc->primary;
		int j;

		drm_dbg_kms(dev, "test CRTC %u primary plane\n", drm_crtc_index(crtc));

		for (j = 0; j < plane->format_count; j++) {
			const struct drm_format_info *fmt;
			struct drm_mode_fb_cmd2 cmd;

			cmd.pixel_format = plane->format_types[j];
			/* drm_format_info do not support parser private format,
			 * but drm_get_format_info can do it, so replaced it with
			 * drm_get_format_info.
			 */
			fmt = drm_get_format_info(fb_helper->dev, &cmd);

			/*
			 * Do not consider YUV or other complicated formats
			 * for framebuffers. This means only legacy formats
			 * are supported (fmt->depth is a legacy field) but
			 * the framebuffer emulation can only deal with such
			 * formats, specifically RGB/BGA formats.
			 */
			if (fmt->depth == 0)
				continue;

			/* We found a perfect fit, great */
			if (fmt->depth == sizes.surface_depth) {
				best_depth = fmt->depth;
				break;
			}

			/* Skip depths above what we're looking for */
			if (fmt->depth > sizes.surface_depth)
				continue;

			/* Best depth found so far */
			if (fmt->depth > best_depth)
				best_depth = fmt->depth;
		}
	}
	if (sizes.surface_depth != best_depth && best_depth) {
		drm_info(dev, "requested bpp %d, scaled depth down to %d",
			 sizes.surface_bpp, best_depth);
		sizes.surface_depth = best_depth;
	}

	/* first up get a count of crtcs now in use and new min/maxes width/heights */
	crtc_count = 0;
	drm_client_for_each_modeset(mode_set, client) {
		struct drm_display_mode *desired_mode;
		int x, y, j;
		/* in case of tile group, are we the last tile vert or horiz?
		 * If no tile group you are always the last one both vertically
		 * and horizontally
		 */
		bool lastv = true, lasth = true;

		desired_mode = mode_set->mode;

		if (!desired_mode)
			continue;

		crtc_count++;

		x = mode_set->x;
		y = mode_set->y;

		sizes.surface_width  = max_t(u32, desired_mode->hdisplay + x, sizes.surface_width);
		sizes.surface_height = max_t(u32, desired_mode->vdisplay + y, sizes.surface_height);

		for (j = 0; j < mode_set->num_connectors; j++) {
			struct drm_connector *connector = mode_set->connectors[j];

			if (connector->has_tile &&
			    desired_mode->hdisplay == connector->tile_h_size &&
			    desired_mode->vdisplay == connector->tile_v_size) {
				lasth = (connector->tile_h_loc == (connector->num_h_tile - 1));
				lastv = (connector->tile_v_loc == (connector->num_v_tile - 1));
				/* cloning to multiple tiles is just crazy-talk, so: */
				break;
			}
		}

		if (lasth)
			sizes.fb_width  = min_t(u32, desired_mode->hdisplay + x, sizes.fb_width);
		if (lastv)
			sizes.fb_height = min_t(u32, desired_mode->vdisplay + y, sizes.fb_height);
	}
	mutex_unlock(&client->modeset_mutex);

	if (crtc_count == 0 || sizes.fb_width == -1 || sizes.fb_height == -1) {
		drm_info(dev, "Cannot find any crtc or sizes\n");

		/* First time: disable all crtc's.. */
		if (!fb_helper->deferred_setup)
			drm_client_modeset_commit(client);
		return -EAGAIN;
	}

	/* Handle our overallocation */
	sizes.surface_height *= drm_fbdev_overalloc;
	sizes.surface_height /= 100;
	if (sizes.surface_height > config->max_height) {
		drm_dbg_kms(dev, "Fbdev over-allocation too large; clamping height to %d\n",
			    config->max_height);
		sizes.surface_height = config->max_height;
	}

	/* push down into drivers */
	ret = (*fb_helper->funcs->fb_probe)(fb_helper, &sizes);
	if (ret < 0)
		return ret;

	strcpy(fb_helper->fb->comm, "[fbcon]");
	return 0;
}

static int
__meson_fb_helper_initial_config_and_unlock(struct drm_fb_helper *fb_helper,
					  int bpp_sel)
{
	struct drm_device *dev = fb_helper->dev;
	struct fb_info *info;
	unsigned int width, height;
	int ret;

	width = dev->mode_config.max_width;
	height = dev->mode_config.max_height;

	drm_client_modeset_probe(&fb_helper->client, width, height);
	ret = meson_fb_helper_single_fb_probe(fb_helper, bpp_sel);
	if (ret < 0) {
		if (ret == -EAGAIN) {
			fb_helper->preferred_bpp = bpp_sel;
			fb_helper->deferred_setup = true;
			ret = 0;
		}
		mutex_unlock(&fb_helper->lock);

		return ret;
	}
	meson_setup_crtcs_fb(fb_helper);

	fb_helper->deferred_setup = false;

	info = fb_helper->fbdev;
	info->var.pixclock = 0;
	/* Shamelessly allow physical address leaking to userspace */
#if IS_ENABLED(CONFIG_DRM_FBDEV_LEAK_PHYS_SMEM)
	if (!drm_leak_fbdev_smem)
#endif
		/* don't leak any physical addresses to userspace */
		info->flags |= FBINFO_HIDE_SMEM_START;

	/* Need to drop locks to avoid recursive deadlock in
	 * register_framebuffer. This is ok because the only thing left to do is
	 * register the fbdev emulation instance in kernel_fb_helper_list.
	 */
	mutex_unlock(&fb_helper->lock);

	ret = register_framebuffer(info);
	if (ret < 0)
		return ret;

	drm_info(dev, "fb%d: %s frame buffer device\n",
		 info->node, info->fix.id);

	mutex_lock(&kernel_fb_helper_lock);
	if (list_empty(&kernel_fb_helper_list))
		register_sysrq_key('v', &sysrq_drm_fb_helper_restore_op);

	list_add(&fb_helper->kernel_fb_list, &kernel_fb_helper_list);
	mutex_unlock(&kernel_fb_helper_lock);

	return 0;
}

int meson_fb_helper_initial_config(struct drm_fb_helper *fb_helper, int bpp_sel)
{
	int ret;

	mutex_lock(&fb_helper->lock);
	ret = __meson_fb_helper_initial_config_and_unlock(fb_helper, bpp_sel);

	return ret;
}

struct meson_drm_fbdev *am_meson_create_drm_fbdev(struct drm_device *dev,
					    struct drm_plane *plane)
{
	struct meson_drm *drmdev = dev->dev_private;
	struct meson_drm_fbdev *fbdev;
	struct drm_fb_helper *helper;
	int ret, bpp;
	struct fb_info *fbinfo;
	int i = 0;

	bpp = drmdev->ui_config.fb_bpp;
	fbdev = devm_kzalloc(dev->dev, sizeof(struct meson_drm_fbdev), GFP_KERNEL);
	if (!fbdev)
		return NULL;

	helper = &fbdev->base;

	if (plane)
		fbdev->plane = plane;
	else
		return NULL;

	drm_fb_helper_prepare(dev, helper, &meson_drm_fb_helper_funcs);

	ret = drm_fb_helper_init(dev, helper);
	if (ret < 0) {
		dev_err(dev->dev, "Failed to initialize drm fb helper - %d.\n",
			ret);
		goto err_free;
	}

	ret = meson_fb_helper_initial_config(helper, bpp);
	if (ret < 0) {
		dev_err(dev->dev, "Failed to set initial hw config - %d.\n",
			ret);
		goto err_drm_fb_helper_fini;
	}

	fbinfo = helper->fbdev;
	if (fbinfo && fbinfo->dev) {
		for (i = 0; i < ARRAY_SIZE(fbdev_device_attrs); i++) {
			ret = device_create_file(fbinfo->dev,
						&fbdev_device_attrs[i]);
			if (ret) {
				DRM_ERROR("Failed to create file - %d.\n", ret);
				continue;
			}
		}
		fbinfo->flags &= ~FBINFO_HIDE_SMEM_START;
	}

	fbdev->blank = false;
	fbdev->vscreen_info_changed = false;

	DRM_INFO("create fbdev success.\n");
	return fbdev;

err_drm_fb_helper_fini:
	drm_fb_helper_fini(helper);
err_free:
	kfree(fbdev);
	fbdev = NULL;
	DRM_INFO("create drm fbdev failed[%d]\n", ret);
	return NULL;
}

int am_meson_drm_fbdev_init(struct drm_device *dev)
{
	struct meson_drm *drmdev = dev->dev_private;
	struct meson_drm_fbdev *fbdev;
	struct am_osd_plane *osd_plane;
	int i, fbdev_cnt = 0;
	int ret = 0;

	DRM_INFO("%s in\n", __func__);

	ret = am_meson_fbdev_parse_config(dev);
	if (ret) {
		DRM_ERROR("don't find fbdev_sizes, please config it\n");
		return ret;
	}

	if (drmdev->primary_plane) {
		drmdev->ui_config.overlay_flag = 0;
		fbdev = am_meson_create_drm_fbdev(dev, drmdev->primary_plane);
		fbdev->zorder = OSD_PLANE_BEGIN_ZORDER + drmdev->fbdev_zorder[0];
		DRM_INFO("create fbdev for primary plane [%p]\n", fbdev);
	}

	/*only create fbdev for viu1*/
	for (i = 0; i < MESON_MAX_OSD; i++) {
		osd_plane = drmdev->osd_planes[i];
		if (!osd_plane)
			break;

		if (osd_plane->base.type == DRM_PLANE_TYPE_PRIMARY)
			continue;

		drmdev->ui_config.overlay_flag = 1;
		fbdev = am_meson_create_drm_fbdev(dev, &osd_plane->base);
		if (fbdev) {
			fbdev->zorder = OSD_PLANE_BEGIN_ZORDER + drmdev->fbdev_zorder[i];
			fbdev_cnt++;
			DRM_INFO("create fbdev for plane (%d %d) zorder=%d\n",
				i, osd_plane->plane_index, drmdev->fbdev_zorder[i]);
		} else {
			DRM_ERROR("create fbdev for plane %d failed\n", i);
			break;
		}
	}

	DRM_INFO("%s create %d out\n", __func__, fbdev_cnt);
	return 0;
}

void am_meson_drm_fbdev_fini(struct drm_device *dev)
{
	struct meson_drm *private = dev->dev_private;
	struct meson_drm_fbdev *fbdev;
	struct drm_fb_helper *helper;
	struct fb_info *fbinfo;
	int i;

	for (i = 0; i < MESON_MAX_OSD; i++) {
		fbdev = private->osd_fbdevs[i];
		if (!fbdev) {
			dev_err(dev->dev, "fbdev is NULL.\n");
			continue;
		}

		helper = &fbdev->base;
		if (!helper || !helper->fbdev) {
			kfree(fbdev);
			dev_err(dev->dev, "helper or fbinfo is NULL.\n");
			continue;
		}

		fbinfo = helper->fbdev;
		if (fbinfo && fbinfo->dev) {
			for (i = 0; i < ARRAY_SIZE(fbdev_device_attrs); i++) {
				device_remove_file(fbinfo->dev,
						&fbdev_device_attrs[i]);
			}
		}

		drm_fb_helper_unregister_fbi(helper);
		drm_fb_helper_fini(helper);
		if (helper->fb)
			drm_framebuffer_put(helper->fb);
		fbdev->fb_gem = NULL;
		drm_fb_helper_fini(helper);
		kfree(fbdev);
	}
}
