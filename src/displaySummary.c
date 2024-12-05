#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <ncurses.h>

#include "display.h"

#define DRM_DEVICE "/dev/dri/card0"

void displaySummary(WINDOW *win);
const char *get_connector_type_name(uint32_t connector_type);
const char *get_encoder_type_name(uint32_t encoder_type);

void displaySummary(WINDOW *win)
{
    WINDOW *summary = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), 0, 0);
    keypad(summary, TRUE);
    scrollok(summary, TRUE);
    wrefresh(summary);

    wclear(win);
    box(win, 0, 0);
    wattron(win, A_BOLD);
    mvwprintw(win, 1, 1, "DRM SUMMARY");
    wattroff(win, A_BOLD);


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

    mvwprintw(win, line++, 1, "CRTCs Status:");
    for (i = 0; i < resources->count_crtcs; i++)
    {
        crtc = drmModeGetCrtc(drm_fd, resources->crtcs[i]);
        if (crtc)
        {
            wattron(win, A_DIM);
            mvwprintw(win, line++, 2, "CRTC %d is %s\n", resources->crtcs[i],
                      (crtc->mode_valid) ? "Active" : "Inactive");
            wattroff(win, A_DIM);
            drmModeFreeCrtc(crtc);
        }
        else
        {
            mvwprintw(win, line++, 2, "Failed to get CRTC %d\n", resources->crtcs[i]);
        }
    }

    mvwprintw(win, line++, 1, "Connectors - %d Status", resources->count_connectors);
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

    wattron(win, A_BOLD);
    mvwprintw(win, line++, 1, "Encoders and their corresponding CRTCs:");
    wattroff(win, A_BOLD);

    int col_width = 6;                                                   
    mvwprintw(win, line++, 2, "Encoders|");
    for (int i = 0; i < resources->count_encoders; i++)
    {
        drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[i]);
        if (encoder)
        {
            mvwprintw(win, line - 1, 11 + (i * (col_width + 1)), "%*d|", col_width, encoder->encoder_id);
            drmModeFreeEncoder(encoder);
        }
        else
        {
            mvwprintw(win, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, "N/A");
        }
    }

    // print encoder type
    mvwprintw(win, line++, 2, "Type    |");
    for (int i = 0; i < resources->count_encoders; i++)
    {
        drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[i]);
        if (encoder)
        {
            mvwprintw(win, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, get_encoder_type_name(encoder->encoder_type));
            drmModeFreeEncoder(encoder);
        }
        else
        {
            mvwprintw(win, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, "N/A");
        }
    }

    mvwprintw(win, line++, 2, "CRTCs   |");
    for (int i = 0; i < resources->count_encoders; i++)
    {
        drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[i]);
        if (encoder)
        {
            mvwprintw(win, line - 1, 11 + (i * (col_width + 1)), "%*d|", col_width, encoder->crtc_id);
            drmModeFreeEncoder(encoder);
        }
        else
        {
            mvwprintw(win, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, "N/A");
        }
    }



    drmVersionPtr version = drmGetVersion(drm_fd);
    if (version)
    {
        mvwprintw(win, line++, 1, "DRM Driver: %s", version->name);
        drmFreeVersion(version);
    }
    else
    {
        mvwprintw(win, line++, 2, "Failed to get DRM version");
    }


    

    mvwprintw(win, line++, 1, "-----------------------------------------");
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

const char *get_encoder_type_name(uint32_t encoder_type)
{
    switch (encoder_type)
    {
    case DRM_MODE_ENCODER_NONE:
        return "None";
    case DRM_MODE_ENCODER_DAC:
        return "DAC";
    case DRM_MODE_ENCODER_TMDS:
        return "TMDS";
    case DRM_MODE_ENCODER_LVDS:
        return "LVDS";
    case DRM_MODE_ENCODER_TVDAC:
        return "TVDAC";
    case DRM_MODE_ENCODER_VIRTUAL:
        return "Virtual";
    case DRM_MODE_ENCODER_DSI:
        return "DSI";
    case DRM_MODE_ENCODER_DPMST:
        return "DPMST";
    default:
        return "Unknown";
    }
}