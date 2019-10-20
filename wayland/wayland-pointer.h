#ifndef _WAYLAND_POINTER_H_
#define _WAYLAND_POINTER_H_

#include <wayland-client.h>
#include "../types.h"

struct pointer_data {
    struct wl_surface* surface;
    struct wl_buffer* buffer;
    i32 hot_spot_x;
    i32 hot_spot_y;
    struct wl_surface* target_surface;
};

void 
wayland_set_pointer_sprite(struct wl_shm_pool* pool, u32 width, u32 height, i32 hot_spot_x, i32 hot_spot_y, struct wl_compositor* compositor, struct wl_pointer* pointer);

void 
wayland_set_pointer_callback(struct wl_shell_surface* surface, void (*callback)(u32));

#endif