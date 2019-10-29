#include "./playland-keyboard.h"
#include <stdlib.h>

struct playland_keyboard*
playland_keyboard_create(struct playland* playland)
{
    struct playland_keyboard* keyboard = malloc(sizeof(struct playland_keyboard));

    wl_keyboard_set_user_data(playland->keyboard, keyboard);
	keyboard->playland = playland;

    return keyboard;
}

static void
keyboard_keymap(
    void *data,
	struct wl_keyboard* keyboard,
	uint32_t format,
	int32_t fd,
	uint32_t size
) {
}

static void
keyboard_enter(
    void *data,
	struct wl_keyboard* _keyboard,
	uint32_t serial,
	struct wl_surface *surface,
	struct wl_array *keys
) {
	struct playland_keyboard* keyboard = wl_keyboard_get_user_data(_keyboard);
	keyboard->target = surface;
}

static void
keyboard_leave(
    void *data,
	struct wl_keyboard* _keyboard,
	uint32_t serial,
	struct wl_surface *surface
) {
	struct playland_keyboard* keyboard = wl_keyboard_get_user_data(_keyboard);
	keyboard->target = NULL;
}

static void
keyboard_key(
    void* data,
	struct wl_keyboard* _keyboard,
	uint32_t serial,
	uint32_t time,
	uint32_t key,
	uint32_t state
) {
    struct playland_keyboard* keyboard = wl_keyboard_get_user_data(_keyboard);

    if (keyboard->on_key == NULL) {
        return;
    }
	struct playland_window* target = wl_surface_get_user_data(keyboard->target);
    keyboard->on_key(target, key, state);
}

static void
keyboard_modifiers(
    void *data,
	struct wl_keyboard* keyboard,
	uint32_t serial,
	uint32_t mods_depressed,
	uint32_t mods_latched,
	uint32_t mods_locked,
	uint32_t group
) {}

static void
keyboard_repeat_info(
    void *data,
	struct wl_keyboard* keyboard,
	int32_t rate,
	int32_t delay
) {}

const struct wl_keyboard_listener playland_keyboard_listener = {
    .key = keyboard_key,
    .keymap = keyboard_keymap,
    .enter = keyboard_enter,
    .leave = keyboard_leave,
    .modifiers = keyboard_modifiers,
    .repeat_info = keyboard_repeat_info,
};
