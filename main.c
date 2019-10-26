#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include "./playland/playland.h"
#include "./playland/playland-file.h"
#include "./playland/playland-window.h"
#include "./playland/playland-pointer.h"
#include "./playland/playland-keyboard.h"

static struct playland* playland = NULL;
volatile sig_atomic_t quit = 0;

int
main(const int argc, const char** argv);

void
signal_handler(int dummy);

void
on_button(struct playland_window* target, uint32_t serial, uint32_t button, enum playland_pointer_state state);

void
on_key(struct playland_window* target, enum playland_keyboard_key key);

#define GOTO(symbol) {\
    status = EXIT_FAILURE;\
    goto symbol;\
}

int
main(const int argc, const char** argv) {

    int status = EXIT_SUCCESS;
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);
    //
    // 1. Initialize
    //
    playland = playland_create();
    if (! playland) {
        GOTO(free_nothing);
    }
    struct playland_file* const images = playland_file_create(playland, "images.bin");
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
    struct playland_window* const  window = playland_window_create(playland, "Playland");
    if (! window) {
        GOTO(free_pointer_cursor);
    }
    playland_window_set_background(window, window_background);

    struct playland_pointer* const pointer = playland_pointer_create(playland);
    if (! pointer) {
        GOTO(free_window);
    }
    pointer->on_button = on_button;
    playland_pointer_set_cursor(pointer, pointer_cursor, 35, 10);

    struct playland_keyboard* const keyboard = playland_keyboard_create(playland);
    if (! keyboard) {
        GOTO(free_cursor);
    }
    keyboard->on_key = on_key;
    keyboard->on_key_payload = window;
    //
    // 2. Loop
    //
    bool done = false;
    while (! done && ! quit) {
        done = playland_listen(playland);
    }
    //
    // 3. Cleanup
    //
    free(keyboard);
free_cursor:
    playland_pointer_destroy(pointer);
free_window:
    playland_window_destroy(window);
free_pointer_cursor:
    wl_buffer_destroy(pointer_cursor);
free_window_background:
    wl_buffer_destroy(window_background);
free_images:
    playland_file_destroy(images);
free_playland:
    playland_destroy(playland);
free_nothing:
    return status;
}

void
signal_handler(int dummy) {

    perror("Quitting gracefully...\n");
    quit = 1;
}

void
on_button(
    struct playland_window* target,
    uint32_t serial,
    uint32_t button,
    enum playland_pointer_state state
) {
    if (state == PLAYLAND_POINTER_UP) {

    } else
    if (state == PLAYLAND_POINTER_DOWN) {
        wl_shell_surface_move(target->shell_surface, target->playland->seat, serial);
    }
}

void
on_key(
    struct playland_window* target,
    enum playland_keyboard_key key
) {
    if (key == PLAYLAND_KEYBOARD_ESCAPE) {
        quit = 1;
    }
    else if (key == PLAYLAND_KEYBOARD_F) {
        wl_shell_surface_set_fullscreen(
            target->shell_surface,
            WL_SHELL_SURFACE_FULLSCREEN_METHOD_SCALE,
            120000,
            target->playland->output
        );
    }
    else if (key == PLAYLAND_KEYBOARD_G) {
        wl_shell_surface_set_toplevel(target->shell_surface);
    }
    else if (key == PLAYLAND_KEYBOARD_M) {
        wl_shell_surface_set_maximized(target->shell_surface, target->playland->output);
    }
    else if (key == PLAYLAND_KEYBOARD_N) {
        wl_shell_surface_set_toplevel(target->shell_surface);
    }
}
