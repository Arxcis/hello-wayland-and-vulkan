#ifndef PLAYLAND_WINDOW
#define PLAYLAND_WINDOW

struct wayland_window {
    struct wl_shell_surface* shell;
    struct wl_surface* surface;
}

void
playland_window_set_background(
    const struct playland_window* window,
    const struct wl_buffer* buffer,
);

#endif
