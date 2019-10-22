#include "./playland-window.h"

void
playland_window_set_background(
    const struct playland_window* window,
    struct wl_buffer* buffer
) {
    wl_surface_attach(window->surface, buffer, 0, 0);
    wl_surface_commit(window->surface);
}

