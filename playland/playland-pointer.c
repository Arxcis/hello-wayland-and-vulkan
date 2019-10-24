#include "./playland-pointer.h"

void
playland_pointer_set_cursor(
    struct playland_pointer* const pointer,
    struct wl_buffer* const cursor,
    const int32_t hotspot_x,
    const int32_t hotspot_y
) {
    pointer->hotspot_x = hotspot_x;
    pointer->hotspot_y = hotspot_y;
    pointer->cursor = cursor;
    wl_surface_attach(pointer->surface, cursor, 0, 0);
    wl_surface_commit(pointer->surface);
}


static void
pointer_enter(
    void* data,
    struct wl_pointer* _pointer,
    uint32_t serial,
    struct wl_surface* surface,
    wl_fixed_t surface_x,
    wl_fixed_t surface_y
) {
    struct playland_pointer* pointer = wl_pointer_get_user_data(_pointer);

    pointer->target_surface = surface;

    wl_surface_attach(pointer->surface, pointer->cursor, 0, 0);
    wl_surface_commit(pointer->surface);

    wl_pointer_set_cursor(
        _pointer,
        serial,
        pointer->surface,
        pointer->hotspot_x,
        pointer->hotspot_y
    );
}

static void
pointer_leave(
    void* data,
    struct wl_pointer* pointer,
    uint32_t serial,
    struct wl_surface* wl_surface
) { }

static void
pointer_motion(
    void* data,
    struct wl_pointer* pointer,
    uint32_t time,
    wl_fixed_t surface_x,
    wl_fixed_t surface_y
) { }

static void
pointer_button(
    void* data,
    struct wl_pointer* _pointer,
    uint32_t serial,
    uint32_t time,
    uint32_t button,
    uint32_t state
) {
    struct playland_pointer* pointer = wl_pointer_get_user_data(_pointer);
    if (pointer->on_button == NULL) {
        return;
    }

    pointer->on_button(button);
}

static void
pointer_axis(
    void* data,
    struct wl_pointer* wl_pointer,
    uint32_t time,
    uint32_t axis,
    wl_fixed_t value
) { }


const struct wl_pointer_listener
pointer_listener = {
    .enter = pointer_enter,
    .leave = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis = pointer_axis
};
