#ifndef _WAYLAND_POINTER_H_
#define _WAYLAND_POINTER_H_

#include <wayland-client.h>
#include "./wayland-mouse.h"

struct wayland_mouse*
wayland_pointer_create_mouse(const struct wl_wayland*);

void
wayland_pointer_destroy_mouse(const struct wayland_mouse*);

#endif
