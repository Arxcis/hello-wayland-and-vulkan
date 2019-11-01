#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include "playland/playland-client.h"
#include "playland/playland-pool.h"
#include "playland/playland-window.h"
#include "playland/playland-pointer.h"
#include "playland/playland-keyboard.h"

volatile sig_atomic_t quit = 0;

int
main(const int argc, const char** argv);

void
signal_handler(int dummy);

void
on_button(struct playland_window* target, uint32_t serial, uint32_t button, enum playland_pointer_state state);

void
on_key(struct playland_window* target, enum playland_keyboard_key key, enum playland_keyboard_state state);

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
    struct playland_client* const client = playland_client_create();
    if (! client) {
        GOTO(free_nothing);
    }
    struct playland_window* const  window = playland_window_create(client, "Playland");
    if (! window) {
        GOTO(free_playland);
    }

    struct playland_window* const  window2 = playland_window_create(client, "Playland Other");
    if (! window2) {
        GOTO(free_window);
    }

    struct playland_pointer* const pointer = playland_pointer_create(client);
    if (! pointer) {
        GOTO(free_window2);
    }

    struct playland_keyboard* const keyboard = playland_keyboard_create(client);
    if (! keyboard) {
        GOTO(free_cursor);
    }

    pointer->on_button = on_button;
    keyboard->on_key = on_key;
    //
    // 2. Loop
    //
    bool done = false;
    while (! done && ! quit) {
        done = playland_client_listen(client);
    }
    //
    // 3. Cleanup
    //
    playland_keyboard_destroy(keyboard);
free_cursor:
    playland_pointer_destroy(pointer);
free_window2:
    playland_window_destroy(window2);
free_window:
    playland_window_destroy(window);
free_playland:
    playland_client_destroy(client);
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

    }
    else if (state == PLAYLAND_POINTER_DOWN) {
        xdg_toplevel_move(target->xtoplevel, target->client->seat, serial);
    }
}

void
on_key(
    struct playland_window* target,
    enum playland_keyboard_key key,
    enum playland_keyboard_state state
) {
    if (state == PLAYLAND_KEYBOARD_STATE_UP) {
        return;
    }

    if (key == PLAYLAND_KEYBOARD_Q){
        quit = 1;
        return;
    }

    if (key == PLAYLAND_KEYBOARD_ESCAPE) {
        xdg_toplevel_unset_fullscreen(target->xtoplevel);
        return;
    }

    if (key == PLAYLAND_KEYBOARD_F) {
        if (target->is_fullscreen) {
            xdg_toplevel_unset_fullscreen(target->xtoplevel);
        }
        else {
            xdg_toplevel_set_fullscreen(target->xtoplevel, target->client->output);
        }
        return;
    }

    if (key == PLAYLAND_KEYBOARD_M) {
        if (target->is_maximized) {
            xdg_toplevel_unset_maximized(target->xtoplevel);
        }
        else {
            xdg_toplevel_set_maximized(target->xtoplevel);
        }
        return;
    }
}
