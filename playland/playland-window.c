#include "./playland-window.h"
#include <stdlib.h>

struct playland_window*
playland_window_create(struct playland* playland, const char* title) {

    struct wl_surface* surface = wl_compositor_create_surface(playland->compositor);
    if (! surface) {
        return NULL;
    }

    struct wl_shell_surface* shell_surface = wl_shell_get_shell_surface(playland->shell, surface);
    if (! shell_surface) {
        return NULL;
    }
    wl_shell_surface_set_title(shell_surface, title);

    wl_shell_surface_add_listener(
        shell_surface,
        &shell_surface_listener,
        0
    );
    wl_shell_surface_set_toplevel(shell_surface);

    struct playland_window* window = malloc(sizeof(struct playland_window));
    window->surface = surface;
    window->shell_surface = shell_surface;
    window->playland = playland;

    wl_shell_surface_set_user_data(shell_surface, window);
    wl_surface_set_user_data(surface, window);

    return window;
}

void
playland_window_destroy(struct playland_window* window) {
    // shell surface must be destroyed before the surface
    wl_shell_surface_destroy(window->shell_surface);
    wl_surface_destroy(window->surface);
    free(window);
}

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
