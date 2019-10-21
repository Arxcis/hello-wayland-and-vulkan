#ifndef _WAYLAND_H_
#define _WAYLAND_H_

#include <wayland-client.h>

struct wayland {
   /**
    * The root global object is the display object.
    * It represents the connection to the display server.
    * It is used for sending requests and receiving events.
    * It is used for running the main loop.
    */
    struct wl_display* display;
    struct wl_compositor* compositor;
    struct wl_seat* seat;
    struct wl_shell* shell;
    struct wl_shm* shared_memory;
    struct wl_pointer* pointer;
};

struct wayland*
wayland_create();

void
wayland_free(const struct wayland*);



#endif
