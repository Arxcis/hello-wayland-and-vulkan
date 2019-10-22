#include "./wayland-buffer.h"

#include "./wayland-pool-data.h"

struct wl_buffer*
wayland_create_buffer(
	struct wl_shm_pool* pool,
    u32 width,
    u32 height
) {

}


void
wayland_free_buffer(
	struct wl_buffer* buffer
) {
    wl_buffer_destroy(buffer);
}


void
wayland_bind_buffer(
	struct wl_buffer* buffer,
    struct wl_shell_surface* shell_surface
) {
    struct wl_surface* surface = wl_shell_surface_get_user_data(shell_surface);
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);
}

