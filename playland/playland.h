#ifndef PLAYLAND
#define PLAYLAND

#include <wayland-client.h>
#include <playland/playland-file.h>
#include <playland/playland-window.h>

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
    struct wl_shm* shared_memory;
    struct wl_pointer* pointer;
};

struct playland*
playland_create();

bool
playland_display_listen(const struct playland* playland);

void
playland_free(const struct playland*);

struct playland_file*
playland_create_file(const struct playland*, const char* filepath);

void
playland_destroy_file(const struct playland_file*);

struct playland_window*
playland_create_window(const struct playland*);

void
playland_destroy_window(const struct playland_window*);

struct playland_cursor*
playland_create_cursor(const struct playland*);

void
playland_destroy_cursor(const struct playland_cursor*);


#endif
