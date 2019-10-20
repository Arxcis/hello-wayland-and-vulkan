#include "./wayland-memory-pool.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "./wayland-pool-data.h"

struct wl_shm_pool*
wayland_create_memory_pool(
	i32 file,
    struct wl_shm* shm
){
    struct stat stat;

    if (fstat(file, &stat) != 0) {
        return NULL;
    }

    struct wayland_pool_data* data = malloc(sizeof(struct wayland_pool_data));
    if (data == NULL) {
        return NULL;
    }

    data->capacity = stat.st_size;
    data->size = 0;
    data->fd = file;

    data->memory = mmap(0, data->capacity,
        PROT_READ, MAP_SHARED, data->fd, 0);

    if (data->memory == MAP_FAILED) {
        free(data);
        return NULL;
    }

    struct wl_shm_pool* pool = wl_shm_create_pool(shm, data->fd, data->capacity);

    if (pool == NULL) {
        munmap(data->memory, data->capacity);
        free(data);
        return NULL;
    }

    wl_shm_pool_set_user_data(pool, data);

    return pool;
}

void
wayland_free_memory_pool(
	struct wl_shm_pool* pool
) {
    struct wayland_pool_data* data = wl_shm_pool_get_user_data(pool);
    wl_shm_pool_destroy(pool);
    munmap(data->memory, data->capacity);
    free(data);
}
