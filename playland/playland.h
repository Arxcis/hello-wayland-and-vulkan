#ifndef PLAYLAND
#define PLAYLAND

#include <wayland-client.h>
#include <stdbool.h>

struct playland {
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
    struct wl_shm* shm;
    struct wl_pointer* pointer;
    struct wl_keyboard* keyboard;
    struct wl_output* output;
};

struct playland*
playland_create();

bool
playland_listen(const struct playland* playland);

void
playland_destroy(struct playland* playland);


#endif
