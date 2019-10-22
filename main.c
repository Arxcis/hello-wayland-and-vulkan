#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#include "./playland/playland.h"

static bool done = false;
static int status = EXIT_SUCCESS;
#define GOTO(symbol) {\
    status = EXIT_FAILURE;\
    goto symbol;\
}

void
on_button(unsigned button) {
    done = true;
}

int
main() {
    struct playland* const playland = playland_create();
    if (! playland) {
        GOTO(panic);
    }
    //
    // 0. Create resource file and corresponding bufferes
    //
    struct playland_file* const images = playland_create_file(playland, "images.bin");
    if (! images) {
        GOTO(panic_playland);
    }

    struct wl_buffer* const window_background = playland_file_create_buffer(images, 320, 200);
    if (! window_background) {
        GOTO(panic_images);
    }

    struct wl_buffer* const cursor_sprite = playland_file_create_buffer(images, 100, 59);
    if (! cursor_sprite) {
        GOTO(panic_window_background);
    }
    //
    // 1. Create window
    //
    struct playland_window* const  window = playland_create_window(playland);
    if (! window) {
        GOTO(panic_cursor_sprite);
    }
    playland_window_set_background(window, window_background);
    //
    // 2. Create cursor
    //
    struct playland_cursor* const cursor = playland_create_cursor(playland);
    if (! cursor) {
        GOTO(panic_window);
    }
    cursor->on_button = on_button;
    playland_cursor_set_sprite(cursor, cursor_sprite);
    //
    // 3. Listen for events
    //
    while (! done) {
        done = playland_listen(playland);
    }
    //
    // 4. Cleanup
    //
    playland_destroy_cursor(cursor);
panic_window:
    playland_destroy_window(window);
panic_cursor_sprite:
    wl_buffer_destroy(cursor_sprite);
panic_window_background:
    wl_buffer_destroy(window_background);
panic_images:
    playland_destroy_file(images);
panic_playland:
    playland_destroy(playland);
panic:
    //
    // 5. Exit
    //
    return status;
}
