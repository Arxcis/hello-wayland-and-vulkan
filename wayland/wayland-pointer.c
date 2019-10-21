#include "./wayland-pointer.h"

struct wayland_mouse*
wayland_pointer_create_mouse(const struct wl_pointer* pointer) {

}

void
wayland_pointer_destroy_mouse(const struct wayland_mouse* mouse) {
    wl_buffer_destroy(mouse->buffer);
    wl_surface_destroy(mouse->surface);
    free(mouse);
}
