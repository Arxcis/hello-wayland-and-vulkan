#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "./macros.h"
#include <playland/playland.h>

static bool done = false;
static i32 status = EXIT_SUCCESS;
#define GOTO(symbol) (status = EXIT_FAILURE; goto symbol;)

void
on_button(u32 button);

i32
main() {
    const struct playland* playland = playland_create();
    if (! playland) {
        GOTO(panic);
    }
    //
    // 0. Create resource file and corresponding bufferes
    //
    const struct playland_file* images = playland_create_file(playland, "images.bin");
    if (! images) {
        GOTO(panic_playland);
    }

    const struct wl_buffer* window_background = playland_file_create_buffer(images, 320, 200);
    if (! buffer) {
        GOTO(panic_images);
    }

    const struct wl_buffer* cursor_sprite = playland_file_create_buffer(images, 100, 59);
    if (! sprite_buffer) {
        GOTO(panic_window_background);
    }
    //
    // 1. Create window
    //
    const struct playland_window*  window = playlandcreate_window(playland);
    if (! window) {
        GOTO(panic_cursor_sprite);
    }
    playland_window_set_background(window, window_background);
    //
    // 2. Create cursor
    //
    const struct playland_cursor* cursor = playland_create_cursor(playland);
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
    playland_destroy_window(window)
panic_cursor_sprite:
    wl_buffer_destroy(cursor_sprite)
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

void
on_button(u32 button) {
    done = true;
}
