#include "./playland-window.h"
#include "./playland-pool.h"
#include <stdlib.h>
#include "../xdg/xdg-shell-client.h"

static const struct xdg_toplevel_listener
playland_toplevel_listener;

static const struct wl_shell_surface_listener
playland_window_listener;

struct playland_window*
playland_window_create(struct playland* playland, const char* title) {

    //
    // 1. Allocate
    //
    struct playland_window* window = malloc(sizeof(struct playland_window));
    if (! window) {
        return NULL;
    }

    struct playland_pool* pool = playland_pool_create(playland, "bin/pool.bin");
    if (! pool) {
        goto panic_pool;
    }

    struct wl_buffer* background = playland_pool_create_buffer(pool, 320, 200);
    if (! background) {
        goto panic_background;
    }

    struct wl_surface* surface = wl_compositor_create_surface(playland->compositor);
    if (! surface) {
        goto panic_surface;
    }

    struct xdg_surface* shell = xdg_wm_base_get_xdg_surface(playland->xdg, surface);
    if (! shell) {
        goto panic_shell;
    }

    struct xdg_toplevel* toplevel = xdg_surface_get_toplevel(shell);
    if (! toplevel) {
        goto panic_toplevel;
    }
    //
    // 2. Intialize
    //
    window->surface = surface;
    window->shell = shell;
    window->toplevel = toplevel;
    window->pool = pool;
    window->background = background;
    window->playland = playland;

    wl_surface_attach(surface, background, 0, 0);
    wl_surface_commit(surface);

    xdg_toplevel_set_title(toplevel, title);
    xdg_toplevel_add_listener(toplevel, &playland_toplevel_listener, 0);

    wl_surface_set_user_data(surface, window);
    xdg_toplevel_set_user_data(toplevel, window);

    return window;

panic_toplevel:
    xdg_surface_destroy(shell);
panic_shell:
    wl_surface_destroy(surface);
panic_surface:
    wl_buffer_destroy(background);
panic_background:
    playland_pool_destroy(pool);
panic_pool:
    free(window);
    return NULL;
}

void
playland_window_destroy(struct playland_window* window) {
    // shell surface must be destroyed before the surface
    xdg_toplevel_destroy(window->toplevel);
    xdg_surface_destroy(window->shell);
    wl_surface_destroy(window->surface);
    wl_buffer_destroy(window->background);
    playland_pool_destroy(window->pool);
    free(window);
}

void playland_toplevel_configure(
    void* data,
	struct xdg_toplevel* toplevel,
	int32_t width,
	int32_t height,
	struct wl_array* states
) {
    const DEFAULT_WIDTH = 320;
    const DEFAULT_HEIGHT = 200;

    if (width == 0) {
        width = DEFAULT_WIDTH;
    }
    if (height == 0) {
        height = DEFAULT_HEIGHT;
    }

    struct playland_window* window = xdg_toplevel_get_user_data(toplevel);

    struct wl_buffer* const background = playland_pool_create_buffer(window->pool, width, height);
    if (! background) {
        return;
    }

    wl_surface_attach(window->surface, background, 0, 0);
    wl_surface_commit(window->surface);

    wl_buffer_destroy(window->background);
    window->background = background;

    uint32_t* it;
    wl_array_for_each(it, states) {
        uint32_t state = *it;
        window->is_fullscreen = state == XDG_TOPLEVEL_STATE_FULLSCREEN;
        window->is_maximized = state == XDG_TOPLEVEL_STATE_MAXIMIZED;
    }
}

static const struct xdg_toplevel_listener
playland_toplevel_listener = {
    .configure = playland_toplevel_configure,
};
