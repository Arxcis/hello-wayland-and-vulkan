#ifndef PLAYLAND_CURSOR
#define PLAYLAND_CURSOR

#include <wayland-client.h>

struct playland_cursor {
    struct wl_pointer* pointer;
    struct wl_surface* surface;
    struct wl_surface* target_surface;
    i32 hot_spot_x;
    i32 hot_spot_y;
    void (*callback)(u32) on_button;
};

void
playland_cursor_set_sprite(const struct playland_cursor*, const struct wl_buffer*);

#endif
