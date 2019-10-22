#include "./wayland-pointer.h"

i32
playland_cursor_set_sprite(
    const struct playland_cursor* mouse,
    const struct wl_buffer* sprite
) {
    // pointer->hot_spot_x = hot_spot_x;
    // pointer->hot_spot_y = hot_spot_y;
    // pointer->surface = wl_compositor_create_surface(compositor);

    // if (pointer->surface == NULL) {
    //     perror("Failed to set pointer sprite: data->surface == NULL")
    //     return EXIT_FAILURE;
    // }

    // pointer->buffer = wayland_create_buffer(pool, width, height);
    // if (pointer->buffer == NULL) {
    //     perror("Failed to set pointer sprite: data->buffer == NULL")
    //     wl_surface_destroy(pointer->surface);
    //     return EXIT_FAILURE;
    // }

    return EXIT_SUCCESS;
}
