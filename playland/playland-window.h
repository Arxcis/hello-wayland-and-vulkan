#ifndef PLAYLAND_WINDOW
#define PLAYLAND_WINDOW

#include <stdbool.h>
#include "./playland.h"
#include "./playland-pool.h"

struct playland_window {
    struct xdg_toplevel* toplevel;
    struct xdg_surface* shell;
    struct wl_surface* surface;
    struct playland* playland;
    struct playland_pool* pool;
    struct wl_buffer* background;

    bool is_fullscreen;
    bool is_maximized;
};

struct playland_window*
playland_window_create(struct playland* playland, const char* title);

void
playland_window_destroy(struct playland_window* window);

#endif
