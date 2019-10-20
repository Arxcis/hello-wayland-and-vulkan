#ifndef _WAYLAND_POINTER_DATA_H_
#define _WAYLAND_POINTER_DATA_H_

#include <wayland-client.h>
#include "../types.h"

struct wayland_pointer_data {
    struct wl_surface* surface;
    struct wl_buffer* buffer;
    i32 hot_spot_x;
    i32 hot_spot_y;
    struct wl_surface* target_surface;
};

#endif