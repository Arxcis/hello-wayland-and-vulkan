#include <wayland-client.h>
#include <stdlib.h>
#include <stdio.h>

#include "../types.h"
#include "./wayland-pointer.h"
#include "./wayland-buffer.h"

void 
wayland_set_pointer_callback(
    struct wl_shell_surface* shell_surface,
    void (*callback)(u32)
) {
    struct wl_surface* surface = wl_shell_surface_get_user_data(shell_surface);
    wl_surface_set_user_data(surface, callback);
}

void 
wayland_set_pointer_sprite(
	struct wl_shm_pool* pool,
    u32 width, 
    u32 height,
    i32 hot_spot_x, 
    i32 hot_spot_y,
    struct wl_compositor* compositor,
    struct wl_pointer* pointer
) {
    struct pointer_data* data = malloc(sizeof(struct pointer_data));
    if (data == NULL)
        goto error;

    data->hot_spot_x = hot_spot_x;
    data->hot_spot_y = hot_spot_y;
    data->surface = wl_compositor_create_surface(compositor);

    if (data->surface == NULL)
        goto cleanup_alloc;

    data->buffer = wayland_create_buffer(pool, width, height);

    if (data->buffer == NULL)
        goto cleanup_surface;

    wl_pointer_set_user_data(pointer, data);

    return;

cleanup_surface:
    wl_surface_destroy(data->surface);
cleanup_alloc:
    free(data);
error:
    perror("Unable to allocate cursor");
}
