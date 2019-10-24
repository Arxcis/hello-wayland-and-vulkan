#include "./playland-keyboard.h"

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
    keyboard->on_key(keyboard->on_key_payload, key);
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
	struct wl_keyboard* keyboard,
	uint32_t serial,
	struct wl_surface *surface,
	struct wl_array *keys
) {}

static void
keyboard_leave(
    void *data,
	struct wl_keyboard* keyboard,
	uint32_t serial,
	struct wl_surface *surface
) {}

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