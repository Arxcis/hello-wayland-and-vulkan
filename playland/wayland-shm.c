#include "./wayland-shm.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>

struct wayland_file*
wayland_shm_create_file(
    const struct wl_shm* shm,
    const char* filepath
) {
    struct stat stat;

    i32 fd = open(filepath, O_RDWR);
    if (fd < 0) {
        return NULL;
    }

    if (fstat(fd, &stat) != 0) {
        return NULL;
    }

    const struct wayland_file* file = malloc(sizeof(struct wayland_file));
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

    struct wl_shm_pool* pool = wl_shm_create_pool(shm, file->fd, file->capacity);
    if (pool == NULL) {
        munmap(file->memory, file->capacity);
        free(file);
        return NULL;
    }

    file->pool = pool;
    close(fd);

    return pool;
}

void
wayland_shm_destroy_file(const struct wayland_file* file) {
    wl_shm_pool_destroy(file->pool);
    munmap(file->memory, file->capacity);
    free(file);
}
