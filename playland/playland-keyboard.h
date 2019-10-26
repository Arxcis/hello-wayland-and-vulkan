#ifndef PLAYLAND_KEYBOARD
#define PLAYLAND_KEYBOARD

#include "./playland.h"
#include "./playland-window.h"

typedef void (*playland_keybaord_on_key_t)(struct playland_window*, uint32_t);

struct playland_keyboard {
    playland_keybaord_on_key_t on_key;
    void* on_key_payload;
    struct playland* playland;
    struct wl_surface* target;
};

struct playland_keyboard*
playland_keyboard_create(struct playland* playland);

extern const struct wl_keyboard_listener playland_keyboard_listener;

#endif