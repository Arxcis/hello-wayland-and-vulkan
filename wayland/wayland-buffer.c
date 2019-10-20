#include "./wayland-buffer.h"

#include "./wayland-pool-data.h"

struct wl_buffer*
wayland_create_buffer(
	struct wl_shm_pool* pool,
    u32 width,
    u32 height
) {
	static const u32 PIXEL_FORMAT_ID = WL_SHM_FORMAT_ARGB8888;

    struct wayland_pool_data* pool_data = wl_shm_pool_get_user_data(pool);
    struct wl_buffer* buffer = wl_shm_pool_create_buffer(
    	pool,
        pool_data->size,
        width,
        height,
        width*sizeof(pixel),
        PIXEL_FORMAT_ID
    );

    if (buffer == NULL)
        return NULL;

    pool_data->size += width*height*sizeof(pixel);

    return buffer;
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

