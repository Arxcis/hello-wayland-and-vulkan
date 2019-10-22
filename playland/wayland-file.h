#ifndef _WAYLAND_FILE_H_
#define _WAYLAND_FILE_H_

#include <wayland-client.h>
#include "./wayland-file.h"

struct wl_buffer*
wayland_file_create_buffer(const struct wayland_file*);

#endif
