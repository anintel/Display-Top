#include <xf86drm.h>
#include <xf86drmMode.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <ncurses.h>

#include "summary.h"

#define DRM_DEVICE "/dev/dri/card0"

const char *get_connector_type_name(uint32_t connector_type);

void PrintSummary(WINDOW *win)
{
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "DRM SUMMARY");

    int line = 3;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(win, line++, 2, "Failed to open DRM Device!\n");
        goto error;
    }

    drmModeRes *resources;
    drmModeCrtc *crtc;
    int i;

    resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(win, line++, 2, "Failed to get DRM resources\n");
        goto error;
    }

    mvwprintw(win, line++, 1, "Active CRTCs:");
    for (i = 0; i < resources->count_crtcs; i++)
    {
        crtc = drmModeGetCrtc(drm_fd, resources->crtcs[i]);
        if (crtc)
        {
            mvwprintw(win, line++, 2, "CRTC %d is %s\n", resources->crtcs[i],
                      (crtc->mode_valid) ? "Active" : "Inactive");
            drmModeFreeCrtc(crtc);
        }
        else
        {
            mvwprintw(win, line++, 2, "Failed to get CRTC %d\n", resources->crtcs[i]);
        }
    }

    mvwprintw(win, line++, 1, "Connectors - %d", resources->count_connectors);
    for (int i = 0; i < resources->count_connectors; i++)
    {
        drmModeConnector *connector = drmModeGetConnector(drm_fd, resources->connectors[i]);
        if (connector)
        {
            const char *type_name = get_connector_type_name(connector->connector_type);
            mvwprintw(win, line++, 2, "Connector %d (%s): %s", resources->connectors[i], type_name,
                      (connector->connection == DRM_MODE_CONNECTED) ? "Connected" : "Disconnected");
            drmModeFreeConnector(connector);
        }
        else
        {
            mvwprintw(win, line++, 2, "Failed to get connector %d", resources->connectors[i]);
        }
    }

    // mvwprintw(win, 3, 1, "Display Configuration:");
    // mvwprintw(win, 4, 1, "Active CRTCs: crtc-0, crtc-1");
    // mvwprintw(win, 5, 1, "Connected Outputs: DP-1: Connected, HDMI-A-1: Disconnected");
    mvwprintw(win, line++, 1, "Encoders: Encoder-0: crtc-0, Encoder-1: DP-1");
    mvwprintw(win, line++, 1, "GPU Information:");
    mvwprintw(win, line++, 1, "GPU Model: Intel Iris Xe Graphics");
    mvwprintw(win, line++, 1, "Frequency: Base: 300 MHz, Boost: 900 MHz");
    mvwprintw(win, line++, 1, "Power Management:");
    mvwprintw(win, line++, 1, "Runtime PM: Active");
    mvwprintw(win, line++, 1, "PSR: Enabled");
    mvwprintw(win, line++, 1, "FBC: Enabled");
    mvwprintw(win, line++, 1, "Display Capabilities:");
    mvwprintw(win, line++, 1, "supports_tv: no, has_hdcp: yes, has_dmc: yes, has_dsc: no");
    mvwprintw(win, line++, 1, "Error and Debug:");
    mvwprintw(win, line++, 1, "GPU Errors: No active errors");
    mvwprintw(win, line++, 1, "Hotplug Storm: No storm detected");
    mvwprintw(win, line++, 1, "System State: Stable, No active warnings");

    wrefresh(win);
    drmModeFreeResources(resources);

    return;

error:
    wrefresh(win);
    return;
}

const char *get_connector_type_name(uint32_t connector_type)
{
    switch (connector_type)
    {
    case DRM_MODE_CONNECTOR_VGA:
        return "VGA";
    case DRM_MODE_CONNECTOR_DVII:
        return "DVI-I";
    case DRM_MODE_CONNECTOR_DVID:
        return "DVI-D";
    case DRM_MODE_CONNECTOR_DVIA:
        return "DVI-A";
    case DRM_MODE_CONNECTOR_Composite:
        return "Composite";
    case DRM_MODE_CONNECTOR_SVIDEO:
        return "S-Video";
    case DRM_MODE_CONNECTOR_LVDS:
        return "LVDS";
    case DRM_MODE_CONNECTOR_Component:
        return "Component";
    case DRM_MODE_CONNECTOR_9PinDIN:
        return "9-pin DIN";
    case DRM_MODE_CONNECTOR_DisplayPort:
        return "DisplayPort";
    case DRM_MODE_CONNECTOR_HDMIA:
        return "HDMI-A";
    case DRM_MODE_CONNECTOR_HDMIB:
        return "HDMI-B";
    case DRM_MODE_CONNECTOR_TV:
        return "TV";
    case DRM_MODE_CONNECTOR_eDP:
        return "eDP";
    case DRM_MODE_CONNECTOR_VIRTUAL:
        return "Virtual";
    case DRM_MODE_CONNECTOR_DSI:
        return "DSI";
    case DRM_MODE_CONNECTOR_DPI:
        return "DPI";
    default:
        return "Unknown";
    }
}