#include "./wayland-surface.h"

#include <wayland-client.h>
#include "../types.h"

static const struct wl_shell_surface_listener shell_surface_listener;

struct wl_shell_surface* 
wayland_create_surface(
	struct wl_compositor* compositor,
	struct wl_shell* shell
) {
    struct wl_surface* surface = wl_compositor_create_surface(compositor);
    if (surface == NULL)
        return NULL;

    struct wl_shell_surface* shell_surface = wl_shell_get_shell_surface(shell, surface);
    if (shell_surface == NULL) {
        wl_surface_destroy(surface);
        return NULL;
    }

    wl_shell_surface_add_listener(
        shell_surface,
        &shell_surface_listener, 
        0
    );
    wl_shell_surface_set_toplevel(shell_surface);
    wl_shell_surface_set_user_data(shell_surface, surface);
    wl_surface_set_user_data(surface, NULL);

    return shell_surface;
}

void 
wayland_free_surface(
	struct wl_shell_surface* shell_surface
) {
    struct wl_surface *surface = wl_shell_surface_get_user_data(shell_surface);

    wl_shell_surface_destroy(shell_surface);
    wl_surface_destroy(surface);
}


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