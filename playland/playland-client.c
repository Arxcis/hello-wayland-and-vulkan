#include <string.h>
#include <stdlib.h>
#include "../xdg/xdg-shell-client.h"

#include "playland-client.h"
#include "playland-pointer.h"
#include "playland-keyboard.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

static const struct xdg_wm_base_listener xdg_listener;
static const struct wl_registry_listener registry_listener;

struct playland_client*
playland_client_create() {
    struct playland_client* playland = malloc(sizeof(struct playland_client));

    playland->display = wl_display_connect(NULL);
    if (playland->display == NULL) {
        return NULL;
    }

    struct wl_registry* registry = wl_display_get_registry(playland->display);

    wl_registry_add_listener(
        registry,
        &registry_listener,
        playland
    );

    wl_display_roundtrip(playland->display);
    wl_registry_destroy(registry);

    return playland;
}

bool
playland_client_listen(const struct playland_client* playland) {
    if (wl_display_dispatch(playland->display) < 0) {
        return true;
    }
    return false;
}

void
playland_client_destroy(struct playland_client* playland) {

    wl_pointer_destroy(playland->pointer);
    wl_seat_destroy(playland->seat);
    wl_shm_destroy(playland->shm);
    wl_compositor_destroy(playland->compositor);
    wl_display_disconnect(playland->display);

    free(playland);
}

//
// Listeners
//
static void
registry_global(
    void* data,
    struct wl_registry* registry,
    uint32_t name,
    const char* interface,
    uint32_t version
) {
    struct playland_client* const playland = data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        playland->compositor = wl_registry_bind(
        	registry,
        	name,
            &wl_compositor_interface,
            min(version, 4)
        );
    }
    else if (strcmp(interface, wl_shm_interface.name) == 0) {
        playland->shm = wl_registry_bind(
        	registry,
        	name,
            &wl_shm_interface,
            min(version, 1)
        );
    }
    else if (strcmp(interface, wl_output_interface.name) == 0) {
        playland->output = wl_registry_bind(
        	registry,
        	name,
            &wl_output_interface,
            min(version, 2)
        );
    }
    else if (strcmp(interface, wl_seat_interface.name) == 0) {
        playland->seat = wl_registry_bind(
        	registry,
        	name,
            &wl_seat_interface,
            min(version, 2)
        );
        playland->pointer = wl_seat_get_pointer(playland->seat);
        playland->keyboard = wl_seat_get_keyboard(playland->seat);

        wl_keyboard_add_listener(
            playland->keyboard,
            &playland_keyboard_listener,
            playland
        );
        wl_pointer_add_listener(
        	playland->pointer,
        	&pointer_listener,
            playland
        );
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        playland->xdg = wl_registry_bind(
        	registry,
        	name,
            &xdg_wm_base_interface,
            min(version, 2)
        );

        xdg_wm_base_add_listener(playland->xdg,  xdg_listener, playland);
    }
}

static void
registry_global_remove(
	void* a,
    struct wl_registry* b,
    uint32_t c
) { }

static const struct wl_registry_listener
registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove
};

static void xdg_ping(
    void *data,
	struct xdg_wm_base *base,
	uint32_t serial
) {
    xdg_wm_base_pong(base, serial);
}

static const struct xdg_wm_base_listener
xdg_listener = {
    .ping = xdg_ping
};
