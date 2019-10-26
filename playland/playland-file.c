#include "./playland-file.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>

struct playland_file*
playland_file_create(struct playland* playland, const char* filepath) {
    struct stat stat;

    int fd = open(filepath, O_RDWR);
    if (fd < 0) {
        return NULL;
    }

    if (fstat(fd, &stat) != 0) {
        return NULL;
    }

    struct playland_file* file = malloc(sizeof(struct playland_file));
    if (file == NULL) {
        return NULL;
    }

    file->capacity = stat.st_size;
    file->size = 0;
    file->fd = fd;

    file->memory = mmap(0, file->capacity, PROT_READ, MAP_SHARED, file->fd, 0);
    if (file->memory == MAP_FAILED) {
        free(file);
        return NULL;
    }

    file->pool = wl_shm_create_pool(playland->shm, file->fd, file->capacity);
    if (file->pool == NULL) {
        munmap(file->memory, file->capacity);
        free(file);
        return NULL;
    }
    close(fd);

    return file;
}

void
playland_file_destroy(struct playland_file* file) {
    wl_shm_pool_destroy(file->pool);
    munmap(file->memory, file->capacity);
    free(file);
}


struct wl_buffer*
playland_file_create_buffer(
    struct playland_file* file,
    const int32_t width,
    const int32_t height
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
