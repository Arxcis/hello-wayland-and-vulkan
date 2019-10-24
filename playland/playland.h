#ifndef PLAYLAND
#define PLAYLAND

#include <wayland-client.h>
#include <stdbool.h>
#include "./playland-file.h"
#include "./playland-window.h"
#include "./playland-cursor.h"
#include "./playland-keyboard.h"

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

struct playland_file*
playland_create_file(const struct playland* playland, const char* filepath);

struct playland_window*
playland_create_window(const struct playland* playland, const char* title);

struct playland_pointer*
playland_create_cursor(const struct playland* playland);

struct playland_keyboard*
playland_create_keyboard(const struct playland* playland);

bool
playland_listen(const struct playland* playland);

void
playland_destroy(struct playland* playland);

void
playland_destroy_file(struct playland_file* file);

void
playland_destroy_window(struct playland_window* window);

void
playland_destroy_cursor(struct playland_pointer* pointer);

#endif
