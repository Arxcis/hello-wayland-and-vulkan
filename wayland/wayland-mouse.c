#include "./wayland-pointer.h"


void
wayland_mouse_on_button(
    const struct wayland_mouse* mouse,
    void (*callback)(u32)
) {
    struct wl_surface* surface = wl_shell_surface_get_user_data(shell_surface);
    wl_surface_set_user_data(surface, callback);
}

i32
wayland_mouse_set_sprite(
    const struct wayland_mouse* mouse
    const u32 width,
    const u32 height,
    const i32 hot_spot_x,
    const i32 hot_spot_y,
    const struct wl_shm_pool* pool,
    const struct wl_compositor* compositor,
) {
    pointer->hot_spot_x = hot_spot_x;
    pointer->hot_spot_y = hot_spot_y;
    pointer->surface = wl_compositor_create_surface(compositor);

    if (pointer->surface == NULL) {
        perror("Failed to set pointer sprite: data->surface == NULL")
        return EXIT_FAILURE;
    }

    pointer->buffer = wayland_create_buffer(pool, width, height);
    if (pointer->buffer == NULL) {
        perror("Failed to set pointer sprite: data->buffer == NULL")
        wl_surface_destroy(pointer->surface);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
