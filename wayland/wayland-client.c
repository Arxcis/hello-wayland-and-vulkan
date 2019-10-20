#include "./wayland-client.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./wayland-buffer.h"

struct wayland_pointer_data {
    struct wl_surface* surface;
    struct wl_buffer* buffer;
    i32 hot_spot_x;
    i32 hot_spot_y;
    struct wl_surface* target_surface;
};

static const struct wl_pointer_listener pointer_listener;
static const struct wl_registry_listener registry_listener;
static struct wayland_client* client;

struct wayland_client*
wayland_create() {
    client = malloc(sizeof(struct wayland_client));

    client->display = wl_display_connect(NULL);
    if (client->display == NULL) {
        perror("Failed to connect to display: client->display == NULL");
        return NULL;
    }

    struct wl_registry* registry = wl_display_get_registry(client->display);

    wl_registry_add_listener(
        registry,
        &registry_listener,
        NULL
    );
    wl_display_roundtrip(client->display);
    wl_registry_destroy(registry);

    return client;
}

bool
wayland_listen(struct wl_display* display) {
    if (wl_display_dispatch(display) < 0) {
        perror("Main loop error: wl_display_dispatch(display) < 0");
        return true;
    }
    return false;
}

i32
wayland_free(struct wayland_client* client) {

    struct pointer_data* data = wl_pointer_get_user_data(client->pointer);
    wl_buffer_destroy(data->buffer);
    wl_surface_destroy(data->surface);
    free(data);
    wl_pointer_set_user_data(client->pointer, NULL);
    wl_pointer_destroy(client->pointer);

    wl_seat_destroy(client->seat);
    wl_shell_destroy(client->shell);
    wl_shm_destroy(client->shared_memory);
    wl_compositor_destroy(client->compositor);
    wl_display_disconnect(client->display);

    return EXIT_SUCCESS;
}

void
wayland_set_pointer_callback(
    struct wl_shell_surface* shell_surface,
    void (*callback)(u32)
) {
    struct wl_surface* surface = wl_shell_surface_get_user_data(shell_surface);
    wl_surface_set_user_data(surface, callback);
}

i32
wayland_set_pointer_sprite(
    struct wl_shm_pool* pool,
    u32 width,
    u32 height,
    i32 hot_spot_x,
    i32 hot_spot_y,
    struct wl_compositor* compositor,
    struct wl_pointer* pointer
) {
    struct wayland_pointer_data* data = malloc(sizeof(struct wayland_pointer_data));
    if (data == NULL) {
        perror("Failed to set pointer sprite: data == NULL")
        return EXIT_FAILURE;
    }

    data->hot_spot_x = hot_spot_x;
    data->hot_spot_y = hot_spot_y;

    data->surface = wl_compositor_create_surface(compositor);
    if (data->surface == NULL) {
        perror("Failed to set pointer sprite: data->surface == NULL")
        free(data);
        return EXIT_FAILURE;
    }

    data->buffer = wayland_create_buffer(pool, width, height);

    if (data->buffer == NULL) {
        perror("Failed to set pointer sprite: data->buffer == NULL")
        wl_surface_destroy(data->surface);
        free(data);
        return EXIT_FAILURE;
    }

    wl_pointer_set_user_data(pointer, data);

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