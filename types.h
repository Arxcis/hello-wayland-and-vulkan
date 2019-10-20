#ifndef _TYPES_H_
#define _TYPES_H_

#include <sys/types.h>
#include <wayland-client.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define false 0
#define true 1

typedef uint8_t bool;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint32_t pixel;

#endif
