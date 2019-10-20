#ifndef _WAYLAND_BUFFER_H_
#define _WAYLAND_BUFFER_H_

#include <wayland-client.h>
#include "../types.h"

struct wl_buffer*
wayland_create_buffer(struct wl_shm_pool* pool, u32 width, u32 height);

void
wayland_bind_buffer(struct wl_buffer* buffer, struct wl_shell_surface* shell_surface);

void
wayland_free_buffer(struct wl_buffer* buffer);

#endif
