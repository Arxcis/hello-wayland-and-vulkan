#ifndef _WAYLAND_DISPLAY_H_
#define _WAYLAND_DISPLAY_H_

#include <wayland-client.h>
#include "../types.h"

struct wayland_window {
  /**
   * The root global object is the display object.
   * It represents the connection to the display server.
   * It is used for sending requests and receiving events.
   * It is used for running the main loop.
   */
    struct wl_display* display;
    struct wl_compositor* compositor;
    struct wl_pointer* pointer;
    struct wl_seat* seat;
    struct wl_shell* shell;
    struct wl_shm* shared_memory;
};

struct wayland_window*
wayland_create();

bool
wayland_listen(struct wl_display* display);

i32
wayland_free(struct wayland_window* wayland);

void 
wayland_set_pointer_sprite(struct wl_shm_pool* pool, u32 width, u32 height, i32 hot_spot_x, i32 hot_spot_y, struct wl_compositor* compositor, struct wl_pointer* pointer);

void 
wayland_set_pointer_callback(struct wl_shell_surface* surface, void (*callback)(u32));

#endif