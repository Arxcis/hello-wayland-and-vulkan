#include "./wayland-display.h"

bool
wayland_display_listen(const struct wl_display* display) {
    if (wl_display_dispatch(display) < 0) {
        perror("Main loop error: wl_display_dispatch(display) < 0");
        return true;
    }
    return false;
}
