#include "playland.h"
#include "./playland-pointer.h"
#include "./playland-keyboard.h"
#include "../xdg/xdg.h"

#include <string.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

static const struct wl_registry_listener registry_listener;

struct playland*
playland_create() {
    struct playland* playland = malloc(sizeof(struct playland));

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
playland_listen(const struct playland* playland) {
    if (wl_display_dispatch(playland->display) < 0) {
        return true;
    }
    return false;
}

void
playland_destroy(struct playland* playland) {

    wl_pointer_destroy(playland->pointer);
    wl_seat_destroy(playland->seat);
    wl_shell_destroy(playland->shell);
    wl_shm_destroy(playland->shm);
    wl_compositor_destroy(playland->compositor);
    wl_display_disconnect(playland->display);

    free(playland);
}

//
// Setup registry listeners
//
static void
registry_global(
    void* data,
    struct wl_registry* registry,
    uint32_t name,
    const char* interface,
    uint32_t version
) {
    struct playland* const playland = data;


    if (strcmp(interface, wl_compositor_interface.name) == 0)
        playland->compositor = wl_registry_bind(
        	registry,
        	name,
            &wl_compositor_interface,
            min(version, 4)
        );
    else if (strcmp(interface, wl_shm_interface.name) == 0)
        playland->shm = wl_registry_bind(
        	registry,
        	name,
            &wl_shm_interface,
            min(version, 1)
        );
    else if (strcmp(interface, wl_shell_interface.name) == 0)
        playland->shell = wl_registry_bind(
        	registry,
        	name,
            &wl_shell_interface,
            min(version, 1)
        );
    else if (strcmp(interface, wl_output_interface.name) == 0)
        playland->output = wl_registry_bind(
        	registry,
        	name,
            &wl_output_interface,
            min(version, 2)
        );
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
            &wl_output_interface,
            min(version, 2)
        );
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


