#include "./playland-cursor.h"

void
playland_cursor_set_sprite(
    struct playland_cursor* const cursor, 
    struct wl_buffer* const sprite,
    const int hotspot_x,
    const int hotspot_y
) {
    cursor->hotspot_x = hotspot_x;
    cursor->hotspot_y = hotspot_y;
    cursor->sprite = sprite;
    wl_surface_attach(cursor->surface, sprite, 0, 0);
    wl_surface_commit(cursor->surface);
}


static void
pointer_enter(
    void* data,
    struct wl_pointer* pointer,
    unsigned serial,
    struct wl_surface* surface,
    wl_fixed_t surface_x,
    wl_fixed_t surface_y
) {
    struct playland_cursor* cursor = wl_pointer_get_user_data(pointer);

    cursor->target_surface = surface;

    wl_surface_attach(cursor->surface, cursor->sprite, 0, 0);
    wl_surface_commit(cursor->surface);

    wl_pointer_set_cursor(
        pointer,
        serial,
        cursor->surface,
        cursor->hotspot_x,
        cursor->hotspot_y
    );
}

static void
pointer_leave(
    void* data,
    struct wl_pointer* pointer,
    unsigned serial,
    struct wl_surface* wl_surface
) { }

static void
pointer_motion(
    void* data,
    struct wl_pointer* pointer,
    unsigned time,
    wl_fixed_t surface_x,
    wl_fixed_t surface_y
) { }

static void
pointer_button(
    void* data,
    struct wl_pointer* pointer,
    unsigned serial,
    unsigned time,
    unsigned button,
    unsigned state
) {
    struct playland_cursor* cursor = wl_pointer_get_user_data(pointer);
    if (cursor->on_button == NULL) {
        return;
    }

    cursor->on_button(button);
}

static void
pointer_axis(
    void* data,
    struct wl_pointer* wl_pointer,
    unsigned time,
    unsigned axis,
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