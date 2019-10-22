#ifndef PLAYLAND_CURSOR
#define PLAYLAND_CURSOR

#include <wayland-client.h>

struct playland_cursor {
    struct wl_pointer* pointer;
    struct wl_surface* surface;
    struct wl_buffer* buffer;
    struct wl_surface* target_surface;
    int hot_spot_x;
    int hot_spot_y;
    void (*on_button)(unsigned);
};

int
playland_cursor_set_sprite(const struct playland_cursor*, const struct wl_buffer*);

#endif
