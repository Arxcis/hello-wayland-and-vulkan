#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "./macros.h"
#include "./wayland/wayland.h"
#include "./wayland/wayland-window.h"
#include "./wayland/wayland-shm.h"

static bool done = false;

void
on_button(u32 button) {
	done = true;
}

i32
main() {
	const struct wayland* wayland = wayland_create();
    if (! wayland) {
        return EXIT_FAILURE;
    }

    const struct wayland_file* file = wayland_shm_create_file(wayland->shm, "images.bin");
    if (! file) {
        return EXIT_FAILURE;
    }
    //
    // 1. Create window
    //
    struct wayland_window* window = NULL;
    {
        const struct wl_surface* surface = wl_compositor_create_surface(wayland->compositor);
        if (! window_surface) {
            return EXIT_FAILURE;
        }

        const struct wl_buffer* buffer = wayland_file_create_buffer(file, 320, 200);
        if (! buffer) {
            wl_surface_destroy(surface);
            return EXIT_FAILURE;
        }
        wayland_bind_buffer(buffer, surface);

        window = wayland_shell_create_window(wayland->shell, surface, buffer);
        if (! window) {
            return EXIT_FAILURE;
        }
    }
    //
    // 2. Create mouse
    //
    struct wayland_mouse* mouse = NULL;
    {
        const struct wl_surface* surface = wl_compositor_create_surface(wayland->compositor);
        if (! sprite_surface) {
            return EXIT_FAILURE;
        }

        const struct wl_buffer* buffer = wayland_file_create_buffer(file, 100, 59);
        if (! sprite_buffer) {
            wl_surface_destroy(surface);
            return EXIT_FAILURE;
        }
        wayland_bind_buffer(buffer, surface);

        mouse = wayland_pointer_create_mouse(wayland->pointer, surface, buffer, 10, 35);
        if (! mouse) {
            return EXIT_FAILURE;
        }
        wayland_mouse_on_button(mouse, on_button);
    }
    //
    // 3. Listen for events
    //
    while (! done) {
        done = wayland_display_listen(wayland->display);
    }

    //
    // Cleanup
    //
    wayland_pointer_destroy_mouse(mouse);
    wayland_shell_destroy_window(window);
    wayland_shm_destroy_file(file);
	wayland_free(wayland);

    return EXIT_SUCCESS;
}
