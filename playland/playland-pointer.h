#ifndef PLAYLAND_POINTER
#define PLAYLAND_POINTER

#include <wayland-client.h>

typedef void (*playland_pointer_on_button_t)(uint32_t);

struct playland_pointer {
    struct wl_surface* surface;
    struct wl_buffer* cursor;
    struct wl_surface* target_surface;
    int32_t hotspot_x;
    int32_t hotspot_y;
    playland_pointer_on_button_t on_button;
};

void
playland_pointer_set_cursor(
    struct playland_pointer* const pointer,
    struct wl_buffer* const cursor,
    const int32_t hotspot_x,
    const int32_t hotspot_y
);

extern const struct wl_pointer_listener pointer_listener;

#endif
