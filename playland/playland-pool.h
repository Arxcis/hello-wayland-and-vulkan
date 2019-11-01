#ifndef PLAYLAND_POOL
#define PLAYLAND_POOL

#include "./playland-client.h"

#define pixel_t uint32_t

struct playland_pool {
    int32_t  fd;
    uint32_t capacity;
    pixel_t* memory;
    struct wl_shm_pool* pool;
};

struct playland_pool*
playland_pool_create(struct playland_client* client, const char* filepath);

void
playland_pool_destroy(struct playland_pool* file);

struct wl_buffer*
playland_pool_create_buffer(
    struct playland_pool* file,
    const int32_t width,
    const int32_t height
);

#endif
