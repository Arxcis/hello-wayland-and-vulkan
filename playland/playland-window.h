#ifndef PLAYLAND_WINDOW
#define PLAYLAND_WINDOW

#include "./playland.h"
#include "./playland-pool.h"

struct playland_window {
    struct wl_shell_surface* shell_surface;
    struct wl_surface* surface;
    struct playland* playland;
    struct playland_pool* pool;
    struct wl_buffer* background;
};

struct playland_window*
playland_window_create(struct playland* playland, const char* title);

void
playland_window_destroy(struct playland_window* window);

#endif
