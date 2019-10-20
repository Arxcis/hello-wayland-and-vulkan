#ifndef _WAYLAND_H_
#define _WAYLAND_H_

#include <wayland-client.h>
#include "../types.h"
#include "./wayland-buffer.h"
#include "./wayland-memory-pool.h"
#include "./wayland-pointer.h"
#include "./wayland-surface.h"

struct wayland {
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

struct wayland*
wayland_create();

bool
wayland_listen(struct wl_display* display);

i32
wayland_free(struct wayland* wayland);


#endif