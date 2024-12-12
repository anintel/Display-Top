#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm/drm_mode.h>
#include <ncurses.h>

#include "dpcd.h"

#define DPCD_SIZE 16

void read_dpcd(int fd, uint32_t connector_id, WINDOW *win)
{
    drmModeConnector *connector = drmModeGetConnector(fd, connector_id);
    if (!connector)
    {
        wprintw(win, "Failed to get connector: %s\n", strerror(errno));
        wrefresh(win);
        return;
    }

    drmModePropertyPtr prop;
    drmModePropertyBlobPtr blob;
    int i, j;

    for (i = 0; i < connector->count_props; i++)
    {
        prop = drmModeGetProperty(fd, connector->props[i]);
        if (!prop)
        {
            wprintw(win, "Failed to get property: %s\n", strerror(errno));
            wrefresh(win);
            continue;
        }

        if (strcmp(prop->name, "DPMS") == 0)
        {
            drmModeFreeProperty(prop);
            continue;
        }

        if (prop->flags & DRM_MODE_PROP_BLOB)
        {
            wprintw(win, "Property name: %s\n", prop->name);
            blob = drmModeGetPropertyBlob(fd, connector->prop_values[i]);
            if (!blob)
            {
                wprintw(win, "Failed to get property blob: %s\n", strerror(errno));
                drmModeFreeProperty(prop);
                wrefresh(win);
                continue;
            }

            if (blob->length >= DPCD_SIZE)
            {
                uint8_t *dpcd = (uint8_t *)blob->data;
                wprintw(win, "DPCD Registers:\n");
                for (j = 0; j < DPCD_SIZE; j++)
                {
                    wprintw(win, "%02x ", dpcd[j]);
                }
                wprintw(win, "\n");
                wrefresh(win);
            }

            drmModeFreePropertyBlob(blob);
        }

        drmModeFreeProperty(prop);
    }

    drmModeFreeConnector(connector);
}

int AC_DPCD()
{
    const char *card = "/dev/dri/card0";

    int fd = open(card, O_RDWR);
    if (fd < 0)
    {
        perror("Failed to open DRM device");
        return EXIT_FAILURE;
    }

    drmModeRes *resources = drmModeGetResources(fd);
    if (!resources)
    {
        fprintf(stderr, "Failed to get DRM resources: %s\n", strerror(errno));
        close(fd);
        return EXIT_FAILURE;
    }

    initscr();
    cbreak();
    noecho();
    WINDOW *win = newwin(20, 50, 0, 0);
    scrollok(win, TRUE);

    for (int i = 0; i < resources->count_connectors; i++)
    {
        drmModeConnector *connector = drmModeGetConnector(fd, resources->connectors[i]);
        if (!connector)
        {
            wprintw(win, "Failed to get connector: %s\n", strerror(errno));
            wrefresh(win);
            continue;
        }

        if (connector->connection == DRM_MODE_CONNECTED)
        {
            read_dpcd(fd, connector->connector_id, win);
            drmModeFreeConnector(connector);
            break;
        }

        drmModeFreeConnector(connector);
    }

    drmModeFreeResources(resources);
    close(fd);

    wprintw(win, "Press any key to exit...\n");
    wrefresh(win);
    wgetch(win);
    endwin();

    return EXIT_SUCCESS;
}
