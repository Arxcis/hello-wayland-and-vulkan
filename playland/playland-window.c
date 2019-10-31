#include "./playland-window.h"
#include "./playland-pool.h"
#include <stdlib.h>
#include "../xdg/xdg-shell.h"

static const struct wl_shell_surface_listener
playland_window_listener;

struct playland_window*
playland_window_create(struct playland* playland, const char* title) {


    struct wl_surface* surface = wl_compositor_create_surface(playland->compositor);
    if (! surface) {
        return NULL;
    }

    struct wl_shell_surface* shell_surface = wl_shell_get_shell_surface(playland->shell, surface);
    if (! shell_surface) {
        free(surface);
        return NULL;
    }
    wl_shell_surface_set_title(shell_surface, title);


    wl_shell_surface_add_listener(
        shell_surface,
        &playland_window_listener,
        0
    );
    wl_shell_surface_set_toplevel(shell_surface);


    struct playland_pool* const pool = playland_pool_create(playland, "./bin/pool.bin");
    if (! pool) {
        free(surface);
        free(shell_surface);
        return NULL;
    }

    struct wl_buffer* const background = playland_pool_create_buffer(pool, 320, 200);
    if (! background) {
        free(surface);
        free(shell_surface);
        free(pool);
        return NULL;
    }

    wl_surface_attach(surface, background, 0, 0);
    wl_surface_commit(surface);

    struct playland_window* window = malloc(sizeof(struct playland_window));
    window->surface = surface;
    window->shell_surface = shell_surface;
    window->playland = playland;
    window->background = background;
    window->pool = pool;

    wl_shell_surface_set_user_data(shell_surface, window);
    wl_surface_set_user_data(surface, window);

    return window;
}

void
playland_window_destroy(struct playland_window* window) {
    // shell surface must be destroyed before the surface
    wl_shell_surface_destroy(window->shell_surface);
    wl_surface_destroy(window->surface);
    wl_buffer_destroy(window->background);
    playland_pool_destroy(window->pool);
    free(window);
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
) {
    struct playland_window* window = wl_shell_surface_get_user_data(shell_surface);

    struct wl_buffer* const background = playland_pool_create_buffer(window->pool, width, height);
    if (! background) {
        return;
    }
    wl_surface_attach(window->surface, background, 0, 0);
    wl_surface_commit(window->surface);

    wl_buffer_destroy(window->background);
    window->background = background;
}

static const struct wl_shell_surface_listener
playland_window_listener = {
    .ping = shell_surface_ping,
    .configure = shell_surface_configure,
};
