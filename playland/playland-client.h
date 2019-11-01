#ifndef PLAYLAND
#define PLAYLAND

#include <wayland-client.h>
#include <stdbool.h>
#include "../xdg/xdg-shell-client.h"

/**
 * A type of wayland client
 */
struct playland_client {
   /**
    * Represents the connection to the display server.
    * Used for sending requests and receiving events.
    * Used for running the main loop.
    */
    struct wl_display* display;

    /**
     * Used to create surfaces
     */
    struct wl_compositor* compositor;

    /**
     * Group of input devices
     */
    struct wl_seat* seat;

    /**
     * Creates shared memory pools
     */
    struct wl_shm* shm;

    /**
     * Pointer device
     */
    struct wl_pointer* pointer;

    /**
     * Keyboard device
     */
    struct wl_keyboard* keyboard;

    /**
     * Output device
     */
    struct wl_output* output;

    /**
     * Creates xdg shell surfaces
     */
    struct xdg_wm_base* xdg;
};

struct playland_client*
playland_client_create();

bool
playland_client_listen(const struct playland_client* playland);

void
playland_client_destroy(struct playland_client* playland);


#endif
