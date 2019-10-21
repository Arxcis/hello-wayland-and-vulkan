#ifndef _WAYLAND_SHM_H_
#define _WAYLAND_SHM_H_

#include <wayland-client.h>

struct wayland_file {
    i32          fd;
    u32          capacity;
    u32          size;
    pixel*       memory;
    wl_shm_pool* pool;
};

struct wayland_file*
wayland_shm_create_file(const struct wl_shm*, const char* filepath);

void
wayland_shm_destroy_file(const struct wayland_file*);

#endif
