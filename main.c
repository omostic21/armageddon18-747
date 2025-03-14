#include <libinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static int open_restricted(const char *path, int flags, void *user_data) {
    (void)user_data;  // Unused parameter
    return open(path, flags);
}

// Correct function signature for close_restricted
static void close_restricted(int fd, void *user_data) {
    (void)user_data;  // Unused parameter
    close(fd);
}

int main() {
    struct libinput *li;
    struct libinput_event *event;
    struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted,
    };

    int fd = open("/dev/input/event1", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open input device");
        return 1;
    }

    li = libinput_path_create_context(&interface, NULL);
    libinput_path_add_device(li, "/dev/input/event1");

    while (1) {
        libinput_dispatch(li);
        while ((event = libinput_get_event(li))) {
            if (libinput_event_get_type(event) == LIBINPUT_EVENT_KEYBOARD_KEY) {
                printf("Key event detected!\n");
            }
            libinput_event_destroy(event);
        }
        usleep(10000);
    }

    libinput_unref(li);
    close(fd);
    return 0;
}
