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
	struct wayland_window* window = wayland_create();
    if (! window) {
        return EXIT_FAILURE;
    }

    i32 image = open("images.bin", O_RDWR);
    if (image < 0) {
        perror("Error opening surface image");
        return EXIT_FAILURE;
    }
    struct wl_shm_pool* pool = wayland_create_memory_pool(image, window->shared_memory);
    close(image);
    if (! pool) {
        return EXIT_FAILURE;
    }

    struct wl_shell_surface* surface = wayland_create_surface(window->compositor, window->shell);
    if (! surface) {
        return EXIT_FAILURE;
    }

    struct wl_buffer* buffer = wayland_create_buffer(pool, 320, 200);
    if (! buffer) {
        return EXIT_FAILURE;
    }

    wayland_bind_buffer(buffer, surface);

    i32 err = wayland_set_pointer_sprite(pool, 100, 59, 10, 35, window->compositor, window->pointer);
    if (err) {
        return err;
    }
    wayland_set_pointer_callback(surface, on_button);

    while (! done) {
        done = wayland_listen(window->display);
    }

    wayland_free_buffer(buffer);
    wayland_free_surface(surface);
    wayland_free_memory_pool(pool);

	return wayland_free(wayland);
}
