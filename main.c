#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include "./playland/playland.h"

static bool click = false;
static bool escape = false;

volatile sig_atomic_t quit = 0;


static struct playland* playland = NULL;

/**
 * Handle SIGINT
 */
void 
sigHandler(int dummy) {
    perror("Quitting gracefully...\n");
    quit = 1;
}

static int status = EXIT_SUCCESS;
#define GOTO(symbol) {\
    status = EXIT_FAILURE;\
    goto symbol;\
}

void
on_button(uint32_t button) {
    // click = true;
}



void
on_key(void* _playland, void* _window, uint32_t key) {

    struct playland_window* window = _window;

    const int Escape = 1;
    const int KeyF = 33;
    const int KeyG = 34;
    const int KeyM = 50;
    const int KeyN = 49;

    if (key == Escape) {
        quit = 1;
    }
    if (key == KeyF) {
        wl_shell_surface_set_fullscreen(
            window->shell_surface, 
            WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT, 
            120000,
            playland->output
        );
    }
    if (key == KeyG) {
        wl_shell_surface_set_toplevel(window->shell_surface);
    }
    if (key == KeyM) {
        wl_shell_surface_set_maximized(window->shell_surface, playland->output);
    }
    if (key == KeyN) {
        wl_shell_surface_set_toplevel(window->shell_surface);
    }
}

int
main() {
    signal(SIGINT, sigHandler);
    signal(SIGQUIT, sigHandler);
    signal(SIGTERM, sigHandler);

    playland = playland_create();
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
    struct playland_window* const  window = playland_create_window(playland, "Main window");
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
    playland_cursor_set_sprite(cursor, cursor_sprite, 35, 10);

    struct playland_keyboard* const keyboard = playland_create_keyboard(playland);
    if (! keyboard) {
        GOTO(panic_cursor);
    }
    keyboard->on_key = on_key;
    keyboard->on_key_payload = window;

    //
    // 3. Listen for events
    //
    bool done = false;
    while (! done && ! quit) {
        done = playland_listen(playland);
    }
    //
    // 4. Cleanup
    //
    free(keyboard);
panic_cursor:
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
