#ifndef PLAYLAND_KEYBOARD
#define PLAYLAND_KEYBOARD

#include <wayland-client.h>

typedef void (*playland_keybaord_on_key_t)(void*, uint32_t);

struct playland_keyboard {
    playland_keybaord_on_key_t on_key;
    void* on_key_payload;
};

extern const struct wl_keyboard_listener playland_keyboard_listener;

#endif