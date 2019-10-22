#ifndef PLAYLAND_CURSOR
#define PLAYLAND_CURSOR

#include <wayland-client.h>

struct playland_cursor {
    struct wl_surface* surface;
    struct wl_buffer* sprite;
    struct wl_surface* target_surface;
    int hotspot_x;
    int hotspot_y;
    void (*on_button)(unsigned);
};

#endif
