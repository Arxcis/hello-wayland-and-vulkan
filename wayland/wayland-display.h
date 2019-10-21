#ifndef _WAYLAND_DISPLAY_H_
#define _WAYLAND_DISPLAY_H_

#include <wayland-display.h>

bool
wayland_display_listen(const struct wl_display* display);

#endif
