#ifndef _TYPES_H_
#define _TYPES_H_

#include <sys/types.h>
#include <wayland-client.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define false 0
#define true 1

#define bool  uint8_t;
#define u32   uint32_t;
#define i32   int32_t;

#endif
