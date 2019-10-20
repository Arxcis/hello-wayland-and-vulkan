#ifndef _WAYLAND_MEMORY_POOL_H_
#define _WAYLAND_MEMORY_POOL_H_

#include <wayland-client.h>
#include "../types.h"

struct wl_shm_pool*
wayland_create_memory_pool(i32 file, struct wl_shm* shm);

void 
wayland_free_memory_pool(struct wl_shm_pool* pool);

#endif