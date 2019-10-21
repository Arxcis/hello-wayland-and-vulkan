#ifndef _WAYLAND_WINDOW_H_
#define _WAYLAND_WINDOW_H_

#include <wayland-client.h>

struct wayland_window {
    struct wl_shell_surface* shell;
    struct wl_surface* surface;
}

struct wayland_window*
wayland_shell_create_window(const struct wl_shell*, const struct wl_surface*);

void
wayland_shell_destroy_window(const struct wayland_window*);

#endif
