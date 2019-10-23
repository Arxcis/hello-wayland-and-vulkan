#include "playland.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

static struct playland* playland = NULL;
static const struct wl_registry_listener registry_listener;

struct playland*
playland_create() {
    playland = malloc(sizeof(struct playland));

    playland->display = wl_display_connect(NULL);
    if (playland->display == NULL) {
        return NULL;
    }

    struct wl_registry* registry = wl_display_get_registry(playland->display);

    wl_registry_add_listener(
        registry,
        &registry_listener,
        NULL
    );
    wl_display_roundtrip(playland->display);
    wl_registry_destroy(registry);

    return playland;
}

bool
playland_listen(const struct playland* playland) {
    if (wl_display_dispatch(playland->display) < 0) {
        return true;
    }
    return false;
}

void
playland_destroy(struct playland* playland) {

    wl_pointer_destroy(playland->pointer);
    wl_seat_destroy(playland->seat);
    wl_shell_destroy(playland->shell);
    wl_shm_destroy(playland->shm);
    wl_compositor_destroy(playland->compositor);
    wl_display_disconnect(playland->display);

    free(playland);
}


struct playland_file*
playland_create_file(const struct playland* playland, const char* filepath) {
    struct stat stat;

    int fd = open(filepath, O_RDWR);
    if (fd < 0) {
        return NULL;
    }

    if (fstat(fd, &stat) != 0) {
        return NULL;
    }

    struct playland_file* file = malloc(sizeof(struct playland_file));
    if (file == NULL) {
        return NULL;
    }

    file->capacity = stat.st_size;
    file->size = 0;
    file->fd = fd;

    file->memory = mmap(0, file->capacity, PROT_READ, MAP_SHARED, file->fd, 0);
    if (file->memory == MAP_FAILED) {
        free(file);
        return NULL;
    }

    file->pool = wl_shm_create_pool(playland->shm, file->fd, file->capacity);
    if (file->pool == NULL) {
        munmap(file->memory, file->capacity);
        free(file);
        return NULL;
    }
    close(fd);

    return file;
}

void
playland_destroy_file(struct playland_file* file) {
    wl_shm_pool_destroy(file->pool);
    munmap(file->memory, file->capacity);
    free(file);
}


struct playland_window*
playland_create_window(const struct playland* playland) {

    struct wl_surface* surface = wl_compositor_create_surface(playland->compositor);
    if (! surface) {
        return NULL;
    }

    struct wl_shell_surface* shell_surface = wl_shell_get_shell_surface(playland->shell, surface);
    if (! shell_surface) {
        return NULL;
    }

    wl_shell_surface_add_listener(
        shell_surface,
        &shell_surface_listener,
        0
    );
    wl_shell_surface_set_toplevel(shell_surface);

    struct playland_window* window = malloc(sizeof(struct playland_window));
    window->surface = surface;
    window->shell_surface = shell_surface;

    wl_shell_surface_set_user_data(shell_surface, window);
    wl_surface_set_user_data(surface, window);

    return window;
}

void
playland_destroy_window(struct playland_window* window) {
    // shell surface must be destroyed before the surface
    wl_shell_surface_destroy(window->shell_surface);
    wl_surface_destroy(window->surface);
    free(window);
}


struct playland_cursor*
playland_create_cursor(
    const struct playland* playland, 
    const playland_cursor_on_button_t on_button
) {
    struct wl_surface* surface = wl_compositor_create_surface(playland->compositor);
    if (! surface) {
        return NULL;
    }

    struct playland_cursor* cursor = malloc(sizeof(struct playland_cursor));
    cursor->surface = surface;
    cursor->on_button = on_button;

    wl_pointer_set_user_data(playland->pointer, cursor);

    return cursor;
}

void
playland_destroy_cursor(struct playland_cursor* cursor) {
    wl_surface_destroy(cursor->surface);
    free(cursor);
}

//
// Setup registry listeners
//
static void
registry_global(
    void* data,
    struct wl_registry* registry,
    uint32_t name,
    const char* interface,
    uint32_t version
) {
    if (strcmp(interface, wl_compositor_interface.name) == 0)
        playland->compositor = wl_registry_bind(
        	registry,
        	name,
            &wl_compositor_interface,
            min(version, 4)
        );
    else if (strcmp(interface, wl_shm_interface.name) == 0)
        playland->shm = wl_registry_bind(
        	registry,
        	name,
            &wl_shm_interface,
            min(version, 1)
        );
    else if (strcmp(interface, wl_shell_interface.name) == 0)
        playland->shell = wl_registry_bind(
        	registry,
        	name,
            &wl_shell_interface,
            min(version, 1)
        );
    else if (strcmp(interface, wl_seat_interface.name) == 0) {
        playland->seat = wl_registry_bind(
        	registry,
        	name,
            &wl_seat_interface,
            min(version, 2)
        );
        playland->pointer = wl_seat_get_pointer(playland->seat);
        wl_pointer_add_listener(
        	playland->pointer,
        	&pointer_listener,
            NULL
        );
    }
}

static void
registry_global_remove(
	void* a,
    struct wl_registry* b,
    unsigned c
) { }


static const struct wl_registry_listener
registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove
};

