#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "./types.h"
#include "./wayland/wayland.h"

static bool done = false;

void
on_button(u32 button) {
	done = true;
}

i32
main() {
	struct wayland* wayland = wayland_create();

    i32 image = open("images.bin", O_RDWR);
    if (image < 0) {
        perror("Error opening surface image");
        return EXIT_FAILURE;
    }
    struct wl_shm_pool* pool = wayland_create_memory_pool(image, wayland->shared_memory);
    if (! pool) {
        perror("Failed to create memory pool");
        return EXIT_FAILURE;
    }
    close(image);
    struct wl_shell_surface* surface = wayland_create_surface(wayland->compositor, wayland->shell);
    struct wl_buffer* buffer = wayland_create_buffer(pool, 800, 600);
    wayland_bind_buffer(buffer, surface);

    wayland_set_pointer_sprite(
    	pool,
    	100,
        59,
        10,
        35,
        wayland->compositor,
        wayland->pointer
    );
    wayland_set_pointer_callback(surface, on_button);

    while (! done) {
        done = wayland_listen(wayland->display);
    }

    fprintf(stderr, "Exiting sample wayland client...\n");

    wayland_free_buffer(buffer);
    wayland_free_surface(surface);
    wayland_free_memory_pool(pool);

	return wayland_free(wayland);
}
