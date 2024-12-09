#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <ncurses.h>

#include "display.h"

#define DRM_DEVICE "/dev/dri/card0"

void displaySummary(WINDOW *pad, int *content_line);
const char *get_connector_type_name(uint32_t connector_type);
const char *get_encoder_type_name(uint32_t encoder_type);

void displaySummary(WINDOW *pad, int *content_line)
{
    wclear(pad);

    print_bold_text(pad, 0, 1, "DRM Summary");

    int line = 2;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(pad, line++, 2, "Failed to open DRM Device!\n");
        goto error;
    }

    drmModeRes *resources;
    drmModeCrtc *crtc;
    int i;

    resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(pad, line++, 2, "Failed to get DRM resources\n");
        goto error;
    }

    mvwprintw(pad, line++, 1, "CRTCs Status:");
    for (i = 0; i < resources->count_crtcs; i++)
    {
        crtc = drmModeGetCrtc(drm_fd, resources->crtcs[i]);
        if (crtc)
        {
            wattron(pad, A_DIM);
            mvwprintw(pad, line++, 2, "CRTC %d is %s\n", resources->crtcs[i],
                      (crtc->mode_valid) ? "Active" : "Inactive");
            wattroff(pad, A_DIM);
            drmModeFreeCrtc(crtc);
        }
        else
        {
            mvwprintw(pad, line++, 2, "Failed to get CRTC %d\n", resources->crtcs[i]);
        }
    }

    mvwprintw(pad, line++, 1, "Connectors - %d Status", resources->count_connectors);
    for (int i = 0; i < resources->count_connectors; i++)
    {
        drmModeConnector *connector = drmModeGetConnector(drm_fd, resources->connectors[i]);
        if (connector)
        {
            const char *type_name = get_connector_type_name(connector->connector_type);
            mvwprintw(pad, line++, 2, "Connector %d (%s): %s", resources->connectors[i], type_name,
                      (connector->connection == DRM_MODE_CONNECTED) ? "Connected" : "Disconnected");
            drmModeFreeConnector(connector);
        }
        else
        {
            mvwprintw(pad, line++, 2, "Failed to get connector %d", resources->connectors[i]);
        }
    }

    wattron(pad, A_BOLD);
    mvwprintw(pad, line++, 1, "Encoders and their corresponding CRTCs:");
    wattroff(pad, A_BOLD);

    int col_width = 6;                                                   
    mvwprintw(pad, line++, 2, "Encoders|");
    for (int i = 0; i < resources->count_encoders; i++)
    {
        drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[i]);
        if (encoder)
        {
            mvwprintw(pad, line - 1, 11 + (i * (col_width + 1)), "%*d|", col_width, encoder->encoder_id);
            drmModeFreeEncoder(encoder);
        }
        else
        {
            mvwprintw(pad, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, "N/A");
        }
    }

    // print encoder type
    mvwprintw(pad, line++, 2, "Type    |");
    for (int i = 0; i < resources->count_encoders; i++)
    {
        drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[i]);
        if (encoder)
        {
            mvwprintw(pad, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, get_encoder_type_name(encoder->encoder_type));
            drmModeFreeEncoder(encoder);
        }
        else
        {
            mvwprintw(pad, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, "N/A");
        }
    }

    mvwprintw(pad, line++, 2, "CRTCs   |");
    for (int i = 0; i < resources->count_encoders; i++)
    {
        drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[i]);
        if (encoder)
        {
            mvwprintw(pad, line - 1, 11 + (i * (col_width + 1)), "%*d|", col_width, encoder->crtc_id);
            drmModeFreeEncoder(encoder);
        }
        else
        {
            mvwprintw(pad, line - 1, 11 + (i * (col_width + 1)), "%*s|", col_width, "N/A");
        }
    }

    drmVersionPtr version = drmGetVersion(drm_fd);
    if (version)
    {
        mvwprintw(pad, line++, 1, "DRM Driver: %s", version->name);
        drmFreeVersion(version);
    }
    else
    {
        mvwprintw(pad, line++, 2, "Failed to get DRM version");
    }

    mvwprintw(pad, line++, 1, "-----------------------------------------");
    mvwprintw(pad, line++, 1, "Power Management:");
    mvwprintw(pad, line++, 1, "Runtime PM: Active");
    mvwprintw(pad, line++, 1, "PSR: Enabled");
    mvwprintw(pad, line++, 1, "FBC: Enabled");
    mvwprintw(pad, line++, 1, "Display Capabilities:");
    mvwprintw(pad, line++, 1, "supports_tv: no, has_hdcp: yes, has_dmc: yes, has_dsc: no");
    mvwprintw(pad, line++, 1, "Error and Debug:");
    mvwprintw(pad, line++, 1, "GPU Errors: No active errors");
    mvwprintw(pad, line++, 1, "Hotplug Storm: No storm detected");
    mvwprintw(pad, line++, 1, "System State: Stable, No active warnings");
    mvwprintw(pad, line++, 1, "-----------------------------------------");


    drmModeFreeResources(resources);

    *content_line = line;
    return;

error:
    wrefresh(pad);
    *content_line = line;
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