#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include "./playland/playland.h"

static struct playland* playland = NULL;

volatile sig_atomic_t quit = 0;
void
sigHandler(int dummy) {
    perror("Quitting gracefully...\n");
    quit = 1;
}

void
on_button(uint32_t button) {
    // quit = 1;
}

void
on_key(void* _window, uint32_t key) {

    struct playland_window* window = _window;

    const uint32_t Escape = 1;
    const uint32_t KeyF = 33;
    const uint32_t KeyG = 34;
    const uint32_t KeyM = 50;
    const uint32_t KeyN = 49;

    if (key == Escape) {
        quit = 1;
    }
    if (key == KeyF) {
        wl_shell_surface_set_fullscreen(
            window->shell_surface,
            WL_SHELL_SURFACE_FULLSCREEN_METHOD_SCALE,
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

#define GOTO(symbol) {\
    status = EXIT_FAILURE;\
    goto symbol;\
}

int
main() {
    int status = EXIT_SUCCESS;
    //
    // Init
    //
    signal(SIGINT, sigHandler);
    signal(SIGQUIT, sigHandler);
    signal(SIGTERM, sigHandler);

    playland = playland_create();
    if (! playland) {
        GOTO(free_nothing);
    }
    struct playland_file* const images = playland_create_file(playland, "images.bin");
    if (! images) {
        GOTO(free_playland);
    }
    struct wl_buffer* const window_background = playland_file_create_buffer(images, 320, 200);
    if (! window_background) {
        GOTO(free_images);
    }
    struct wl_buffer* const pointer_cursor = playland_file_create_buffer(images, 100, 59);
    if (! pointer_cursor) {
        GOTO(free_window_background);
    }
    struct playland_window* const  window = playland_create_window(playland, "Main window");
    if (! window) {
        GOTO(free_pointer_cursor);
    }
    playland_window_set_background(window, window_background);

    struct playland_pointer* const pointer = playland_create_pointer(playland);
    if (! pointer) {
        GOTO(free_window);
    }
    pointer->on_button = on_button;
    playland_pointer_set_cursor(pointer, pointer_cursor, 35, 10);

    struct playland_keyboard* const keyboard = playland_create_keyboard(playland);
    if (! keyboard) {
        GOTO(free_cursor);
    }
    keyboard->on_key = on_key;
    keyboard->on_key_payload = window;
    //
    // Loop
    //
    bool done = false;
    while (! done && ! quit) {
        done = playland_listen(playland);
    }
    //
    // Cleanup
    //
    free(keyboard);
free_cursor:
    playland_destroy_pointer(pointer);
free_window:
    playland_destroy_window(window);
free_pointer_cursor:
    wl_buffer_destroy(pointer_cursor);
free_window_background:
    wl_buffer_destroy(window_background);
free_images:
    playland_destroy_file(images);
free_playland:
    playland_destroy(playland);
free_nothing:
    return status;
}
