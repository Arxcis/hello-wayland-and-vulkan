#include <stdlib.h>
#include "../xdg/xdg-shell-client.h"

#include "playland-window.h"
#include "playland-pool.h"

static const struct xdg_toplevel_listener
toplevel_listener;

struct playland_window*
playland_window_create(struct playland_client* client, const char* title) {
    //
    // 1. Allocate
    //
    struct playland_window* window = malloc(sizeof(struct playland_window));
    if (! window) {
        return NULL;
    }

    struct playland_pool* pool = playland_pool_create(client, "bin/pool.bin");
    if (! pool) {
        return NULL;
    }

    struct wl_buffer* background = playland_pool_create_buffer(pool, 320, 200);
    if (! background) {
        return NULL;
    }

    struct wl_surface* surface = wl_compositor_create_surface(client->compositor);
    if (! surface) {
        return NULL;
    }

    struct xdg_surface* xsurface = xdg_wm_base_get_xdg_surface(client->xdg, surface);
    if (! xsurface) {
        return NULL;
    }

    struct xdg_toplevel* xtoplevel = xdg_surface_get_toplevel(xsurface);
    if (! xtoplevel) {
        return NULL;
    }
    //
    // 2. Init
    //
    window->client = client;
    window->pool = pool;
    window->background = background;
    window->surface = surface;
    window->xsurface = xsurface;
    window->xtoplevel = xtoplevel;

    wl_surface_attach(surface, background, 0, 0);
    wl_surface_commit(surface);

    xdg_toplevel_set_title(xtoplevel, title);
    xdg_toplevel_add_listener(xtoplevel, &toplevel_listener, 0);

    wl_surface_set_user_data(surface, window);
    xdg_toplevel_set_user_data(xtoplevel, window);

    return window;
}

void
playland_window_destroy(struct playland_window* window) {
    xdg_toplevel_destroy(window->xtoplevel);
    xdg_surface_destroy(window->xsurface);
    wl_surface_destroy(window->surface);
    wl_buffer_destroy(window->background);
    playland_pool_destroy(window->pool);
    free(window);
}

static void toplevel_configure(
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
    xdg_surface_ack_configure(window->xsurface);
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
toplevel_listener = {
    .configure = toplevel_configure,
};
