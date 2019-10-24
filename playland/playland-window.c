#include "./playland-window.h"

void
playland_window_set_background(
    const struct playland_window* window,
    struct wl_buffer* buffer
) {
    wl_surface_attach(window->surface, buffer, 0, 0);
    wl_surface_commit(window->surface);
}

static void
shell_surface_ping(
    void* data,
    struct wl_shell_surface* shell_surface,
    uint32_t serial
) {
    wl_shell_surface_pong(shell_surface, serial);
}

static void
shell_surface_configure(
    void* data,
    struct wl_shell_surface* shell_surface,
    uint32_t edges,
    int32_t width,
    int32_t height
) { }

const struct wl_shell_surface_listener
shell_surface_listener = {
    .ping = shell_surface_ping,
    .configure = shell_surface_configure,
};
