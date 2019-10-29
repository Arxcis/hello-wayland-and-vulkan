#ifndef PLAYLAND_POINTER
#define PLAYLAND_POINTER

#include "./playland.h"
#include "./playland-window.h"

enum playland_pointer_state {
    PLAYLAND_POINTER_UP = 0,
    PLAYLAND_POINTER_DOWN = 1,
};

typedef void (*playland_pointer_on_button_t)(struct playland_window*, uint32_t, uint32_t, enum playland_pointer_state);

struct playland_pointer {
    struct wl_surface* surface;
    struct wl_buffer* cursor;
    struct wl_surface* target_surface;
    int32_t hotspot_x;
    int32_t hotspot_y;
    playland_pointer_on_button_t on_button;
};

struct playland_pointer*
playland_pointer_create(struct playland* playland);

void
playland_pointer_destroy(struct playland_pointer* pointer);

void
playland_pointer_set_cursor(
    struct playland_pointer* const pointer,
    struct wl_buffer* const cursor,
    const int32_t hotspot_x,
    const int32_t hotspot_y
);

extern const struct wl_pointer_listener
pointer_listener;

#endif
