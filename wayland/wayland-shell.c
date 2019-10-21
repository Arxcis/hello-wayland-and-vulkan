#include "./wayland-surface.h"

static void
shell_surface_ping(
    void* data,
    struct wl_shell_surface* shell_surface,
    u32 serial
) {
    wl_shell_surface_pong(shell_surface, serial);
}

static void
shell_surface_configure(
    void* data,
    struct wl_shell_surface* shell_surface,
    u32 edges,
    i32 width,
    i32 height
) { }

static const struct wl_shell_surface_listener
shell_surface_listener = {
    .ping = shell_surface_ping,
    .configure = shell_surface_configure,
};

struct wayland_window*
wayland_shell_create_window(
	const struct wl_shell* shell,
    const struct wl_surface* surface,
    const struct wl_buffer* buffer
) {
    struct wl_shell_surface* shell_surface = wl_shell_get_shell_surface(shell, surface);
    if (shell_surface == NULL) {
        return NULL;
    }

    wl_shell_surface_add_listener(
        shell_surface,
        &shell_surface_listener,
        0
    );
    wl_shell_surface_set_toplevel(shell_surface);

    struct wayland_window* window = malloc(sizeof(struct wayland_window));
    data->surface = surface;
    data->buffer = buffer;

    wl_shell_surface_set_user_data(shell_surface, data);
    wl_surface_set_user_data(surface, NULL);

    return window;
}

void
wayland_shell_destroy_window(
    const struct wayland_window* window
) {
    // shell surface must be destroyed before the surface
    wl_shell_surface_destroy(window->shell_surface);
    wl_surface_destroy(window->surface);
    free(data);
}
