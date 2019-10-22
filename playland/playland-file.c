#include <playland/playland-file.h>

#define pixel_t uint32_t;

struct wl_buffer*
playland_file_create_buffer(
    const struct playland_file* file,
    const i32 width,
    const i32 height
) {
    static const u32 PIXEL_FORMAT_ID = WL_SHM_FORMAT_ARGB8888;

    const struct wl_buffer* buffer = wl_shm_pool_create_buffer(
        file->pool,
        file->size,
        width,
        height,
        width*sizeof(pixel_t),
        PIXEL_FORMAT_ID
    );
    if (buffer == NULL) {
        return NULL;
    }

    file->size += width*height*sizeof(pixel_t);

    return buffer;
}
