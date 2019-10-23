#ifndef PLAYLAND_CURSOR
#define PLAYLAND_CURSOR

#include <wayland-client.h>

typedef void (*playland_cursor_on_button_t)(unsigned);

struct playland_cursor {
    struct wl_surface* surface;
    struct wl_buffer* sprite;
    struct wl_surface* target_surface;
    int hotspot_x;
    int hotspot_y;
    playland_cursor_on_button_t on_button;
};

void
playland_cursor_set_sprite(
    struct playland_cursor* const cursor, 
    struct wl_buffer* const sprite,
    const int hotspot_x,
    const int hotspot_y
);

extern const struct wl_pointer_listener pointer_listener;

#endif
