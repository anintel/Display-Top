#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <ncurses.h>

#include "display.h"

#define DRM_DEVICE "/dev/dri/card0"




void displayDebugfsFile(WINDOW *pad, const char *file_name, int *content_line) {

    char full_file_path[256];
    snprintf(full_file_path, sizeof(full_file_path), "/sys/kernel/debug/dri/0000:00:02.0/%s", file_name);

    FILE *file = fopen(full_file_path, "r");
    if (!file) {
        mvwprintw(pad, (*content_line)++, 2, "Error: Unable to open file %s", full_file_path);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove the newline character
        mvwprintw(pad, (*content_line)++, 2, "%s", buffer);
    }

    mvwprintw(pad, (*content_line)++, 2, "-----------------------------------");
    fclose(file);
}
