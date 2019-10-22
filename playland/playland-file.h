#ifndef PLAYLAND_FILE
#define PLAYLAND_FILE

#include <wayland-client.h>
#define pixel_t uint32_t

struct playland_file {
    int      fd;
    unsigned capacity;
    unsigned size;
    pixel_t* memory;
    struct wl_shm_pool* pool;
};

struct wl_buffer*
playland_file_create_buffer(
    struct playland_file* file,
    const int width,
    const int height
);

#endif
