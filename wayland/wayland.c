#include "./wayland.h"
#include "./wayland-pointer.h"
#include "../types.h"
#include <stdlib.h>
#include <wayland-client.h>
#include <stdio.h>
#include <string.h>

static const struct wl_pointer_listener pointer_listener;
static const struct wl_registry_listener registry_listener;
static struct wayland* wayland;

struct wayland* 
wayland_create() {
    wayland = malloc(sizeof(struct wayland));

    wayland->display = wl_display_connect(NULL);
    if (wayland->display == NULL) {
        perror("Error opening display");
        exit(EXIT_FAILURE);
    }

    struct wl_registry* registry = wl_display_get_registry(wayland->display);

    wl_registry_add_listener(
        registry, 
        &registry_listener,
        NULL
    );
    wl_display_roundtrip(wayland->display);
    wl_registry_destroy(registry);

    return wayland;
}

bool
wayland_listen(struct wl_display* display) {
    if (wl_display_dispatch(wayland->display) < 0) {
        perror("Main loop error");
        return true;
    }
    return false;
}

i32 
wayland_free(struct wayland* wayland) {

    struct pointer_data* data = wl_pointer_get_user_data(wayland->pointer);
    wl_buffer_destroy(data->buffer);
    wl_surface_destroy(data->surface);
    free(data);
    wl_pointer_set_user_data(wayland->pointer, NULL);
    wl_pointer_destroy(wayland->pointer);

    wl_seat_destroy(wayland->seat);
    wl_shell_destroy(wayland->shell);
    wl_shm_destroy(wayland->shared_memory);
    wl_compositor_destroy(wayland->compositor);
    wl_display_disconnect(wayland->display);

    return EXIT_SUCCESS;
}


static void 
registry_global(
	void* data,
    struct wl_registry* registry, 
    u32 name,
    const char* interface, 
	u32 version
) {
    if (strcmp(interface, wl_compositor_interface.name) == 0)
        wayland->compositor = wl_registry_bind(
        	registry, 
        	name,
            &wl_compositor_interface, 
            min(version, 4)
        );
    else if (strcmp(interface, wl_shm_interface.name) == 0)
        wayland->shared_memory = wl_registry_bind(
        	registry, 
        	name,
            &wl_shm_interface, 
            min(version, 1)
        );
    else if (strcmp(interface, wl_shell_interface.name) == 0)
        wayland->shell = wl_registry_bind(
        	registry, 
        	name,
            &wl_shell_interface, 
            min(version, 1)
        );
    else if (strcmp(interface, wl_seat_interface.name) == 0) {
        wayland->seat = wl_registry_bind(
        	registry, 
        	name,
            &wl_seat_interface, 
            min(version, 2)
        );
        wayland->pointer = wl_seat_get_pointer(wayland->seat);
        wl_pointer_add_listener(
        	wayland->pointer, 
        	&pointer_listener,
            NULL
        );
    }
}

static void 
registry_global_remove(
	void* a,
    struct wl_registry* b, 
    u32 c
) { }


static const struct wl_registry_listener 
registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove
};


static void 
pointer_enter(
    void* data,
    struct wl_pointer* pointer,
    u32 serial, 
    struct wl_surface* surface,
    wl_fixed_t surface_x, 
    wl_fixed_t surface_y
) {
    struct pointer_data* pointer_data = wl_pointer_get_user_data(pointer);

    pointer_data->target_surface = surface;
    
    wl_surface_attach(
        pointer_data->surface,
        pointer_data->buffer, 
        0, 
        0
    );
    wl_surface_commit(pointer_data->surface);
    wl_pointer_set_cursor(
        pointer, 
        serial,
        pointer_data->surface, 
        pointer_data->hot_spot_x,
        pointer_data->hot_spot_y
    );
}

static void 
pointer_leave(
    void* data,
    struct wl_pointer* pointer, 
    u32 serial,
    struct wl_surface* wl_surface
) { }

static void 
pointer_motion(
    void* data,
    struct wl_pointer* pointer, 
    u32 time,
    wl_fixed_t surface_x, 
    wl_fixed_t surface_y
) { }

static void 
pointer_button(
    void* data,
    struct wl_pointer* pointer, 
    u32 serial,
    u32 time, 
    u32 button, 
    u32 state
) {
    struct pointer_data* pointer_data = wl_pointer_get_user_data(pointer);
    
    void (*callback)(u32) = wl_surface_get_user_data(pointer_data->target_surface);    
    if (callback == NULL) {
        return;
    }
    
    callback(button);
}

static void 
pointer_axis(
    void* data,
    struct wl_pointer* wl_pointer, 
    u32 time,
    u32 axis, 
    wl_fixed_t value
) { }


static const struct wl_pointer_listener 
pointer_listener = {
    .enter = pointer_enter,
    .leave = pointer_leave,
    .motion = pointer_motion,
    .button = pointer_button,
    .axis = pointer_axis
};
