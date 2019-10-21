#ifndef _WAYLAND_MOUSE_H_
#define _WAYLAND_MOUSE_H_

#include <wayland-client.h>
#include "../types.h"

struct wayland_mouse {
    struct wl_pointer* pointer;
    struct wl_surface* surface;
    struct wl_buffer* buffer;
    struct wl_surface* target_surface;
    i32 hot_spot_x;
    i32 hot_spot_y;
};

void
wayland_mouse_set_sprite(const struct wayland_mouse*, u32 width, u32 height, i32 hot_spot_x, i32 hot_spot_y, struct wl_shm_pool* pool, struct wl_compositor* compositor);

void
wayland_mouse_on_button(const struct wayland_mouse*, void (*callback)(u32));

#endif
