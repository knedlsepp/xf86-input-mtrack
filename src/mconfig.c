/***************************************************************************
 *
 * Multitouch X driver
 * Copyright (C) 2011 Ryan Bourgeois <bluedragonx@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 **************************************************************************/

#include "mconfig.h"

void mconfig_defaults(struct MConfig* cfg)
{
	// Configure MTState
	cfg->touch_down = DEFAULT_TOUCH_DOWN;
	cfg->touch_up = DEFAULT_TOUCH_UP;
	cfg->ignore_thumb = DEFAULT_IGNORE_THUMB;
	cfg->ignore_palm = DEFAULT_IGNORE_PALM;
	cfg->disable_on_palm = DEFAULT_DISABLE_ON_PALM;
	cfg->disable_on_thumb = DEFAULT_DISABLE_ON_THUMB;
	cfg->thumb_ratio = DEFAULT_THUMB_RATIO;
	cfg->thumb_size = DEFAULT_THUMB_SIZE;
	cfg->palm_size = DEFAULT_PALM_SIZE;

	// Configure Gestures
	cfg->button_enable = DEFAULT_BUTTON_ENABLE;
	cfg->button_integrated = DEFAULT_BUTTON_INTEGRATED;
	cfg->button_1touch = DEFAULT_BUTTON_1TOUCH - 1;
	cfg->button_2touch = DEFAULT_BUTTON_2TOUCH - 1;
	cfg->tap_1touch = DEFAULT_TAP_1TOUCH - 1;
	cfg->tap_2touch = DEFAULT_TAP_2TOUCH - 1;
	cfg->tap_3touch = DEFAULT_TAP_3TOUCH - 1;
	cfg->tap_timeout = DEFAULT_TAP_TIMEOUT;
	cfg->tap_hold = DEFAULT_TAP_HOLD;
	cfg->tap_dist = SQRVAL(DEFAULT_TAP_DIST);
	cfg->gesture_hold = DEFAULT_GESTURE_HOLD;
	cfg->gesture_wait = DEFAULT_GESTURE_WAIT;
	cfg->scroll_dist = DEFAULT_SCROLL_DIST;
	cfg->scroll_up_btn = DEFAULT_SCROLL_UP_BTN - 1;
	cfg->scroll_dn_btn = DEFAULT_SCROLL_DN_BTN - 1;
	cfg->scroll_lt_btn = DEFAULT_SCROLL_LT_BTN - 1;
	cfg->scroll_rt_btn = DEFAULT_SCROLL_RT_BTN - 1;
	cfg->swipe_dist = DEFAULT_SWIPE_DIST;
	cfg->swipe_up_btn = DEFAULT_SWIPE_UP_BTN - 1;
	cfg->swipe_dn_btn = DEFAULT_SWIPE_DN_BTN - 1;
	cfg->swipe_lt_btn = DEFAULT_SWIPE_LT_BTN - 1;
	cfg->swipe_rt_btn = DEFAULT_SWIPE_RT_BTN - 1;
	cfg->scale_dist = SQRVAL(DEFAULT_SCALE_DIST);
	cfg->scale_up_btn = DEFAULT_SCALE_UP_BTN - 1;
	cfg->scale_dn_btn = DEFAULT_SCALE_DN_BTN - 1;
	cfg->rotate_dist = SQRVAL(DEFAULT_ROTATE_DIST);
	cfg->rotate_lt_btn = DEFAULT_ROTATE_LT_BTN - 1;
	cfg->rotate_rt_btn = DEFAULT_ROTATE_RT_BTN - 1;
	cfg->drag_enable = DEFAULT_DRAG_ENABLE;
	cfg->drag_timeout = DEFAULT_DRAG_TIMEOUT;
}

void mconfig_init(struct MConfig* cfg,
			const struct Capabilities* caps)
{
	cfg->touch_minor = caps->has_abs[MTDEV_TOUCH_MINOR];
	cfg->pad_width = get_cap_xsize(caps);
	cfg->pad_height = get_cap_ysize(caps);

	if (caps->has_abs[MTDEV_TOUCH_MAJOR] && caps->has_abs[MTDEV_WIDTH_MAJOR]) {
		cfg->touch_type = MCFG_SCALE;
		cfg->touch_min = caps->abs[MTDEV_TOUCH_MAJOR].minimum;
		cfg->touch_max = caps->abs[MTDEV_TOUCH_MAJOR].maximum;
		xf86Msg(X_INFO, "Touchpad supports regular and approaching touches.\n");
		xf86Msg(X_INFO, "  touch_min = %d, touch_max = %d\n", cfg->touch_min, cfg->touch_max);
	}
	else if (caps->has_abs[MTDEV_TOUCH_MAJOR]) {
		cfg->touch_type = MCFG_SIZE;
		cfg->touch_min = caps->abs[MTDEV_TOUCH_MAJOR].minimum;
		cfg->touch_max = caps->abs[MTDEV_TOUCH_MAJOR].maximum;
		xf86Msg(X_INFO, "Touchpad supports regular touches.\n");
		xf86Msg(X_INFO, "  touch_min = %d, touch_max = %d\n", cfg->touch_min, cfg->touch_max);
	}
	else if (caps->has_abs[MTDEV_PRESSURE]) {
		cfg->touch_type = MCFG_PRESSURE;
		cfg->touch_min = caps->abs[MTDEV_PRESSURE].minimum;
		cfg->touch_max = caps->abs[MTDEV_PRESSURE].maximum;
		xf86Msg(X_INFO, "Touchpad is pressure based.\n");
		xf86Msg(X_INFO, "  touch_min = %d, touch_max = %d\n", cfg->touch_min, cfg->touch_max);
	}
	else {
		cfg->touch_type = MCFG_NONE;
		xf86Msg(X_WARNING, "Touchpad has minimal capabilities. Some features will be unavailable.\n");
	}

	if (cfg->touch_minor)
		xf86Msg(X_INFO, "Touchpad supports minor touch widths.\n");
}

void mconfig_configure(struct MConfig* cfg,
			pointer opts)
{
	// Configure MTState
	cfg->touch_down = CLAMPVAL(xf86SetIntOption(opts, "FingerHigh", DEFAULT_TOUCH_DOWN), 0, 100);
	cfg->touch_up = CLAMPVAL(xf86SetIntOption(opts, "FingerLow", DEFAULT_TOUCH_UP), 0, 100);
	cfg->ignore_thumb = xf86SetBoolOption(opts, "IgnoreThumb", DEFAULT_IGNORE_THUMB);
	cfg->ignore_palm = xf86SetBoolOption(opts, "IgnorePalm", DEFAULT_IGNORE_PALM);
	cfg->disable_on_thumb = xf86SetBoolOption(opts, "DisableOnThumb", DEFAULT_DISABLE_ON_THUMB);
	cfg->disable_on_palm = xf86SetBoolOption(opts, "DisableOnPalm", DEFAULT_DISABLE_ON_PALM);
	cfg->thumb_ratio = CLAMPVAL(xf86SetIntOption(opts, "ThumbRatio", DEFAULT_THUMB_RATIO), 0, 100);
	cfg->thumb_size = CLAMPVAL(xf86SetIntOption(opts, "ThumbSize", DEFAULT_THUMB_SIZE), 0, 100);
	cfg->palm_size = CLAMPVAL(xf86SetIntOption(opts, "PalmSize", DEFAULT_PALM_SIZE), 0, 100);

	// Configure Gestures
	cfg->button_enable = xf86SetBoolOption(opts, "ButtonEnable", DEFAULT_BUTTON_ENABLE);
	cfg->button_integrated = xf86SetBoolOption(opts, "ButtonIntegrated", DEFAULT_BUTTON_INTEGRATED);
	cfg->button_1touch = CLAMPVAL(xf86SetIntOption(opts, "ClickFinger1", DEFAULT_BUTTON_1TOUCH), 0, 32) - 1;
	cfg->button_2touch = CLAMPVAL(xf86SetIntOption(opts, "ClickFinger2", DEFAULT_BUTTON_2TOUCH), 0, 32) - 1;
	cfg->tap_1touch = CLAMPVAL(xf86SetIntOption(opts, "TapButton1", DEFAULT_TAP_1TOUCH), 0, 32) - 1;
	cfg->tap_2touch = CLAMPVAL(xf86SetIntOption(opts, "TapButton2", DEFAULT_TAP_2TOUCH), 0, 32) - 1;
	cfg->tap_3touch = CLAMPVAL(xf86SetIntOption(opts, "TapButton3", DEFAULT_TAP_3TOUCH), 0, 32) - 1;
	cfg->tap_hold = MAXVAL(xf86SetIntOption(opts, "ClickTime", DEFAULT_TAP_HOLD), 1);
	cfg->tap_timeout = MAXVAL(xf86SetIntOption(opts, "MaxTapTime", DEFAULT_TAP_TIMEOUT), 1);
	cfg->tap_dist = SQRVAL(MAXVAL(xf86SetIntOption(opts, "MaxTapMove", DEFAULT_TAP_DIST), 1));
	cfg->gesture_hold = MAXVAL(xf86SetIntOption(opts, "GestureClickTime", DEFAULT_GESTURE_HOLD), 1);
	cfg->gesture_wait = MAXVAL(xf86SetIntOption(opts, "GestureWaitTime", DEFAULT_GESTURE_WAIT), 0);
	cfg->scroll_dist = MAXVAL(xf86SetIntOption(opts, "ScrollDistance", DEFAULT_SCROLL_DIST), 1);
	cfg->scroll_up_btn = CLAMPVAL(xf86SetIntOption(opts, "ScrollUpButton", DEFAULT_SCROLL_UP_BTN), 0, 32) - 1;
	cfg->scroll_dn_btn = CLAMPVAL(xf86SetIntOption(opts, "ScrollDownButton", DEFAULT_SCROLL_DN_BTN), 0, 32) - 1;
	cfg->scroll_lt_btn = CLAMPVAL(xf86SetIntOption(opts, "ScrollLeftButton", DEFAULT_SCROLL_LT_BTN), 0, 32) - 1;
	cfg->scroll_rt_btn = CLAMPVAL(xf86SetIntOption(opts, "ScrollRightButton", DEFAULT_SCROLL_RT_BTN), 0, 32) - 1;
	cfg->swipe_dist = MAXVAL(xf86SetIntOption(opts, "SwipeDistance", DEFAULT_SWIPE_DIST), 1);
	cfg->swipe_up_btn = CLAMPVAL(xf86SetIntOption(opts, "SwipeUpButton", DEFAULT_SWIPE_UP_BTN), 0, 32) - 1;
	cfg->swipe_dn_btn = CLAMPVAL(xf86SetIntOption(opts, "SwipeDownButton", DEFAULT_SWIPE_DN_BTN), 0, 32) - 1;
	cfg->swipe_lt_btn = CLAMPVAL(xf86SetIntOption(opts, "SwipeLeftButton", DEFAULT_SWIPE_LT_BTN), 0, 32) - 1;
	cfg->swipe_rt_btn = CLAMPVAL(xf86SetIntOption(opts, "SwipeRightButton", DEFAULT_SWIPE_RT_BTN), 0, 32) - 1;
	cfg->scale_dist = SQRVAL(MAXVAL(xf86SetIntOption(opts, "ScaleDistance", DEFAULT_SCALE_DIST), 1));
	cfg->scale_up_btn = CLAMPVAL(xf86SetIntOption(opts, "ScaleUpButton", DEFAULT_SCALE_UP_BTN), 0, 32) - 1;
	cfg->scale_dn_btn = CLAMPVAL(xf86SetIntOption(opts, "ScaleDownButton", DEFAULT_SCALE_DN_BTN), 0, 32) - 1;
	cfg->rotate_dist = SQRVAL(MAXVAL(xf86SetIntOption(opts, "RotateDistance", DEFAULT_ROTATE_DIST), 1));
	cfg->rotate_lt_btn = CLAMPVAL(xf86SetIntOption(opts, "RotateLeftButton", DEFAULT_ROTATE_LT_BTN), 0, 32) - 1;
	cfg->rotate_rt_btn = CLAMPVAL(xf86SetIntOption(opts, "RotateRightButton", DEFAULT_ROTATE_RT_BTN), 0, 23) - 1;
	cfg->drag_enable = xf86SetBoolOption(opts, "TapDragEnable", DEFAULT_DRAG_ENABLE);
	cfg->drag_timeout = MAXVAL(xf86SetIntOption(opts, "TapDragTime", DEFAULT_DRAG_TIMEOUT), 1);

	xf86Msg(X_INFO, "ClickTime = %d (default %d)\n", cfg->tap_hold, DEFAULT_TAP_HOLD);
	xf86Msg(X_INFO, "TapButton1 = %d (default %d)\n", cfg->tap_1touch, DEFAULT_TAP_1TOUCH);
	xf86Msg(X_INFO, "TapButton2 = %d (default %d)\n", cfg->tap_2touch, DEFAULT_TAP_2TOUCH);
	xf86Msg(X_INFO, "TapButton3 = %d (default %d)\n", cfg->tap_3touch, DEFAULT_TAP_3TOUCH);
	xf86Msg(X_INFO, "ButtonEnable = %d (default %d)\n", cfg->button_enable, DEFAULT_BUTTON_ENABLE);
	xf86Msg(X_INFO, "ButtonIntegrated = %d (default %d)\n", cfg->button_integrated, DEFAULT_BUTTON_INTEGRATED);
	xf86Msg(X_INFO, "ClickFinger1 = %d (default %d)\n", cfg->button_1touch, DEFAULT_BUTTON_1TOUCH);
	xf86Msg(X_INFO, "ClickFinger2 = %d (default %d)\n", cfg->button_2touch, DEFAULT_BUTTON_2TOUCH);
}

