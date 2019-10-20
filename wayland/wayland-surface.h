#ifndef _WAYLAND_SURFACE_H_
#define _WAYLAND_SURFACE_H_

#include <wayland-client.h>

struct wl_shell_surface*
wayland_create_surface(struct wl_compositor* compositor, struct wl_shell* shell);

void
wayland_free_surface(struct wl_shell_surface* shell_surface);

#endif
