#ifndef PLAYLAND_WINDOW
#define PLAYLAND_WINDOW

#include <stdbool.h>

#include "playland-client.h
#include "playland-pool.h"

struct playland_window {
    struct wl_surface* surface;
    struct wl_buffer* background;
    struct xdg_toplevel* xtoplevel;
    struct xdg_surface* xsurface;
    struct playland_pool* pool;
    struct playland_client* client;

    bool is_fullscreen;
    bool is_maximized;
};

struct playland_window*
playland_window_create(struct playland_client* client, const char* title);

void
playland_window_destroy(struct playland_window* window);

#endif
