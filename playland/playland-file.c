#include "./playland-file.h"


struct wl_buffer*
playland_file_create_buffer(
    struct playland_file* file,
    const int width,
    const int height
) {
    static const unsigned PIXEL_FORMAT_ID = WL_SHM_FORMAT_ARGB8888;

    struct wl_buffer* buffer = wl_shm_pool_create_buffer(
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
