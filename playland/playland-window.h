#ifndef PLAYLAND_WINDOW
#define PLAYLAND_WINDOW

#include <wayland-client.h>

struct playland_window {
    struct wl_shell_surface* shell_surface;
    struct wl_surface* surface;
};

void
playland_window_set_background(
    const struct playland_window* window,
    struct wl_buffer* buffer
);

extern const struct wl_shell_surface_listener shell_surface_listener;

#endif
