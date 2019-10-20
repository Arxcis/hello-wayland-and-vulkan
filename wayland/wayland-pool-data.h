#ifndef _WAYLAND_POOL_DATA_H_
#define _WAYLAND_POOL_DATA_H_

#include "../types.h"

struct wayland_pool_data {
    i32 fd;
    u32 capacity;
    u32 size;
    pixel* memory;
};

#endif
