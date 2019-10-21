#ifndef _WAYLAND_POINTER_H_
#define _WAYLAND_POINTER_H_

#include <wayland-client.h>
#include "../types.h"

struct wayland_pointer {
    struct wl_pointer* pointer;
    struct wl_surface* surface;
    struct wl_buffer* buffer;
    struct wl_surface* target_surface;
    i32 hot_spot_x;
    i32 hot_spot_y;
};


#endif
