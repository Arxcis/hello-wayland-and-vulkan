#ifndef PLAYLAND_FILE
#define PLAYLAND_FILE

#include <wayland-client.h>

struct playland_file {
    i32          fd;
    u32          capacity;
    u32          size;
    pixel*       memory;
    wl_shm_pool* pool;
};

struct wl_buffer*
wayland_file_create_buffer(const struct playland_file*);

#endif
