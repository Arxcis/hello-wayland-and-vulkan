#include "playland-pointer.h"
#include <stdlib.h>

struct playland_pointer*
playland_pointer_create(struct playland_client* client) {
    struct playland_pointer* pointer = malloc(sizeof(struct playland_pointer));

    wl_pointer_set_user_data(client->pointer, pointer);
    pointer->client = client;

    return pointer;
}

void
playland_pointer_destroy(struct playland_pointer* pointer) {
    free(pointer);
}


void
playland_pointer_set_cursor(
    struct playland_pointer* const pointer,
    struct wl_buffer* const cursor,
    const int32_t hotspot_x,
    const int32_t hotspot_y
) {
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
}

static void
pointer_leave(
    void* data,
    struct wl_pointer* _pointer,
    uint32_t serial,
    struct wl_surface* surface
) {
    struct playland_pointer* pointer = wl_pointer_get_user_data(_pointer);
    pointer->target_surface = NULL;
}

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
    struct playland_window* window = wl_surface_get_user_data(pointer->target_surface);
    pointer->on_button(window, serial, button, state);
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
playland_pointer_listener = {
    .enter = pointer_enter,
    .leave = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis = pointer_axis
};
