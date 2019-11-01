#include "./playland-pool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>

struct playland_pool*
playland_pool_create(struct playland_client* client, const char* filepath) {
    struct stat stat;

    int fd = open(filepath, O_RDWR);
    if (fd < 0) {
        return NULL;
    }

    if (fstat(fd, &stat) != 0) {
        return NULL;
    }

    struct playland_pool* file = malloc(sizeof(struct playland_pool));
    if (file == NULL) {
        return NULL;
    }

    file->capacity = stat.st_size;
    file->fd = fd;

    file->memory = mmap(NULL, file->capacity, PROT_READ, MAP_SHARED, file->fd, 0);
    if (file->memory == MAP_FAILED) {
        free(file);
        return NULL;
    }

    file->pool = wl_shm_create_pool(client->shm, file->fd, file->capacity);
    if (file->pool == NULL) {
        munmap(file->memory, file->capacity);
        free(file);
        return NULL;
    }
    close(fd);

    return file;
}

void
playland_pool_destroy(struct playland_pool* file) {
    wl_shm_pool_destroy(file->pool);
    munmap(file->memory, file->capacity);
    free(file);
}


struct wl_buffer*
playland_pool_create_buffer(
    struct playland_pool* file,
    const int32_t width,
    const int32_t height
) {
    static const unsigned PIXEL_FORMAT_ID = WL_SHM_FORMAT_ARGB8888;

    struct wl_buffer* buffer = wl_shm_pool_create_buffer(
        file->pool,
        0,
        width,
        height,
        width*sizeof(pixel_t),
        PIXEL_FORMAT_ID
    );
    if (buffer == NULL) {
        return NULL;
    }

    return buffer;
}
