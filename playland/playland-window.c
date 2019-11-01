#include <stdlib.h>
#include "../xdg/xdg-shell-client.h"

#include "./playland-window.h"
#include "./playland-pool.h"

static const struct xdg_toplevel_listener
xtoplevel_listener;

static const struct xdg_surface_listener
xsurface_listener;

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
    xdg_surface_add_listener(xsurface, &xsurface_listener, 0);
    xdg_toplevel_add_listener(xtoplevel, &xtoplevel_listener, 0);

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





static void
xsurface_configure(
    void *data,
    struct xdg_surface* xsurface,
    uint32_t serial
) {
    struct playland_window* window = xdg_toplevel_get_user_data(xsurface);
    xdg_surface_ack_configure(xsurface, serial);


}


static const struct xdg_surface_listener
xsurface_listener = {
    .configure = xsurface_configure
};

static void xtoplevel_configure(
    void* data,
	struct xdg_toplevel* xsurface,
	int32_t width,
	int32_t height,
	struct wl_array* states
) {
    const uint32_t DEFAULT_WIDTH = 320;
    const uint32_t DEFAULT_HEIGHT = 200;

    if (width == 0) {
        width = DEFAULT_WIDTH;
    }
    if (height == 0) {
        height = DEFAULT_HEIGHT;
    }

    struct playland_window* window = xdg_toplevel_get_user_data(xsurface);

    struct wl_buffer* const background = playland_pool_create_buffer(window->pool, width, height);
    if (! background) {
        return;
    }
    window->background = background;
    wl_surface_attach(window->surface, window->background, 0, 0);
    wl_surface_commit(window->surface);

    // Reset state before each configuring event
    window->is_fullscreen = false;
    window->is_maximized = false;
    window->is_resizing = false;

    uint32_t* it;
    wl_array_for_each(it, states) {
        if (*it == XDG_TOPLEVEL_STATE_RESIZING) {
            window->is_resizing = true;
        }
        if (*it == XDG_TOPLEVEL_STATE_FULLSCREEN) {
            window->is_fullscreen = true;
        }
        if (*it == XDG_TOPLEVEL_STATE_MAXIMIZED) {
            window->is_maximized = true;
        }
    }
}

static const struct xdg_toplevel_listener
xtoplevel_listener = {
    .configure = xtoplevel_configure,
};
