#ifndef PLAYLAND_KEYBOARD
#define PLAYLAND_KEYBOARD

#include "playland-client.h
#include "playland-window.h"

enum playland_keyboard_state {
    PLAYLAND_KEYBOARD_STATE_UP = 0,
    PLAYLAND_KEYBOARD_STATE_DOWN = 1,
};

enum playland_keyboard_key {
    PLAYLAND_KEYBOARD_ESCAPE = 1,
    PLAYLAND_KEYBOARD_Q = 16,
    PLAYLAND_KEYBOARD_F = 33,
    PLAYLAND_KEYBOARD_G = 34,
    PLAYLAND_KEYBOARD_M = 50,
    PLAYLAND_KEYBOARD_N = 49,
};

struct playland_keyboard {
    struct wl_surface* target;
    struct playland_client* client;

    void (*on_key)(
        struct playland_window*,
        enum playland_keyboard_key,
        enum playland_keyboard_state
    );
};

struct playland_keyboard*
playland_keyboard_create(struct playland_client* client);

void
playland_keyboard_destroy(struct playland_keyboard* keyboard);

extern const struct wl_keyboard_listener
playland_keyboard_listener;

#endif
