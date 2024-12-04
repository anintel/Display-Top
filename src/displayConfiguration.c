#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <ncurses.h>
#include <drm/drm_mode.h>
#include <drm/drm_fourcc.h>

#include "display.h"
#include "utils.h"

#define DRM_DEVICE "/dev/dri/card0"

void displayCrtc(WINDOW *pad, const char *page_name, int *content_line)
{
    int line = 0;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(pad, line++, 1, "Failed to open DRM Device!\n");
        close(drm_fd);
        goto err;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(pad, line++, 1, "Failed to get DRM resources\n");
        close(drm_fd);
        goto err;
    }

    int crtc_id = -1;
    char *endptr;

    crtc_id = strtol(page_name + 4, &endptr, 10) - 1;
    if (*endptr != '\0' || crtc_id < 0 || crtc_id >= resources->count_crtcs)
    {
        mvwprintw(pad, line++, 1, "Invalid CRTC name: %s\n", page_name);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    drmModeCrtc *crtc = drmModeGetCrtc(drm_fd, resources->crtcs[crtc_id]);
    if (!crtc)
    {
        mvwprintw(pad, line++, 1, "Failed to get CRTC %d\n", crtc_id);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    mvwprintw(pad, line++, 1, "CRTC ID: %d", crtc->crtc_id);
    mvwprintw(pad, line++, 1, "Framebuffer ID: %d", crtc->buffer_id);
    mvwprintw(pad, line++, 1, "Position(X, Y): (%d, %d)", crtc->x, crtc->y);
    mvwprintw(pad, line++, 1, "Mode: ");
    if (crtc->mode_valid)
        print_green_text(pad, -1, -1, "VALID");
    else
        print_red_text(pad, -1, -1, "INVALID");

    if (crtc->mode_valid)
    {
        drmModeModeInfo *mode = &crtc->mode;
        mvwprintw(pad, line++, 3, "Name: %s", mode->name);
        mvwprintw(pad, line++, 3, "Resolution: %dx%d", mode->hdisplay, mode->vdisplay);
        mvwprintw(pad, line++, 3, "Refresh Rate: %.2f Hz", mode->clock / (float)(mode->htotal * mode->vtotal));
        mvwprintw(pad, line++, 3, "Type: 0x%x", mode->type);
        mvwprintw(pad, line++, 3, "Flags: 0x%x", mode->flags);
    }

    mvwprintw(pad, line++, 1, "Gamma size: %u", crtc->gamma_size);
    drmModeObjectProperties *props = drmModeObjectGetProperties(drm_fd, crtc->crtc_id, DRM_MODE_OBJECT_CRTC);
    if (!props)
    {
        mvwprintw(pad, line++, 1, "Failed to get properties for CRTC %u\n", crtc->crtc_id);
        drmModeFreeCrtc(crtc);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    line++;
    mvwprintw(pad, line++, 1, "Properties for CRTC %u:", crtc->crtc_id);
    line++;

    for (uint32_t i = 0; i < props->count_props; i++)
    {
        drmModePropertyRes *prop = drmModeGetProperty(drm_fd, props->props[i]);
        if (prop)
        {
            mvwprintw(pad, line++, 1, "Property %u: %s", prop->prop_id, prop->name);
            wattron(pad, A_DIM);
            if (prop->flags & DRM_MODE_PROP_IMMUTABLE)
                wprintw(pad, " (Immutable) ");

            mvwprintw(pad, line++, 3, "blob Counts: %llu, Enum Counts: %d, Values Count: %llu", prop->count_blobs, prop->count_enums, prop->count_values);
            if (prop->flags & DRM_MODE_PROP_RANGE)
            {
                mvwprintw(pad, line++, 3, "Range: [%llu - %llu]", prop->values[0], prop->values[1]);
            }
            else if (prop->flags & DRM_MODE_PROP_ENUM)
            {
                for (int j = 0; j < prop->count_enums; j++)
                {
                    mvwprintw(pad, line++, 3, "Enum %d: %s = %llu", j, prop->enums[j].name, prop->enums[j].value);
                }
            }
            else if (prop->flags & DRM_MODE_PROP_BLOB)
            {
                mvwprintw(pad, line++, 3, "Blob: %llu", props->prop_values[i]);
            }
            else if (prop->flags & DRM_MODE_PROP_BITMASK)
            {
                mvwprintw(pad, line++, 3, "Bitmask: %llu", props->prop_values[i]);
            }

            wattroff(pad, A_DIM);
            drmModeFreeProperty(prop);
        }
        else
        {
            mvwprintw(pad, line++, 1, "  Property %u: (failed to get property details)", props->props[i]);
        }
    }

    drmModeFreeObjectProperties(props);

    mvwprintw(pad, ++line, 1, "Press 'e' to go back.");

    *content_line = line;
    drmModeFreeCrtc(crtc);
    drmModeFreeResources(resources);
    close(drm_fd);

err:
    wrefresh(pad);
    wgetch(pad);
    return;
}

void displayConnector(WINDOW *pad, const char *page_name, int *content_line)
{
    int line = 0;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(pad, line++, 1, "Failed to open DRM Device!\n");
        close(drm_fd);
        goto err;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(pad, line++, 1, "Failed to get DRM resources\n");
        close(drm_fd);
        goto err;
    }

    int connector_index = -1;
    char *endptr;

    connector_index = strtol(page_name + 9, &endptr, 10) - 1;

    if (*endptr != '\0' || connector_index < 0 || connector_index >= resources->count_connectors)
    {
        mvwprintw(pad, line++, 1, "Invalid Connector name: %s\n", page_name);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    drmModeConnector *connector = drmModeGetConnector(drm_fd, resources->connectors[connector_index]);
    if (!connector)
    {
        mvwprintw(pad, line++, 1, "Failed to get Connector %d\n", connector_index);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    mvwprintw(pad, line++, 1, "Connector ID: %d", connector->connector_id);
    mvwprintw(pad, line++, 1, "No. of Encoders: %d", connector->count_encoders);
    mvwprintw(pad, line++, 1, "Encoder ID: %d", connector->encoders[connector->encoder_id]);
    mvwprintw(pad, line++, 1, "Connection: ");

    if (connector->connection == DRM_MODE_CONNECTED)
        print_green_text(pad, -1, -1, "Connected");
    else
        print_red_text(pad, -1, -1, "Disconnected");

    mvwprintw(pad, line++, 1, "Connector Type: %s", get_connector_type_name(connector->connector_type));
    mvwprintw(pad, line++, 1, "Connector Type ID: %d", connector->connector_type_id);

    mvwprintw(pad, line++, 1, "Subpixel Order: %d", connector->subpixel);
    mvwprintw(pad, line++, 1, "MM Height: %d", connector->mmHeight);
    mvwprintw(pad, line++, 1, "MM Width: %d", connector->mmWidth);

    mvwprintw(pad, line++, 1, "Modes: %d", connector->count_modes);
    wattron(pad, A_DIM);
    for (int i = 0; i < connector->count_modes; i++)
    {
        if (i % 2 == 0)
            mvwprintw(pad, line++, 2, "Mode %d: %dx%d@%dHz", i, connector->modes[i].hdisplay, connector->modes[i].vdisplay, connector->modes[i].vrefresh);
        else
            mvwprintw(pad, line - 1, 30, "Mode %d: %dx%d@%dHz", i, connector->modes[i].hdisplay, connector->modes[i].vdisplay, connector->modes[i].vrefresh);
    }
    wattroff(pad, A_DIM);

    mvwprintw(pad, line++, 1, "Count Props: %d", connector->count_props);

    wattron(pad, A_DIM);
    for (int i = 0; i < connector->count_props; i++)
    {
        drmModePropertyPtr prop = drmModeGetProperty(drm_fd, connector->props[i]);
        if (prop)
        {
            uint64_t value = connector->prop_values[i];
            mvwprintw(pad, line++, 2, "%d. %s: %lu", i, prop->name, value);
            drmModeFreeProperty(prop);
        }
        else
        {
            mvwprintw(pad, line++, 2, "Failed to get property for ID %d", connector->props[i]);
        }
    }
    wattroff(pad, A_DIM);

    mvwprintw(pad, ++line, 1, "Press 'e' to go back.");

    *content_line = line;
    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);
    close(drm_fd);

err:
    wrefresh(pad);
    wgetch(pad);
    return;
}

void displayEncoder(WINDOW *pad, const char *page_name, int *content_line)
{
    int line = 0;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(pad, line++, 1, "Failed to open DRM Device!\n");
        close(drm_fd);
        goto err;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(pad, line++, 1, "Failed to get DRM resources\n");
        close(drm_fd);
        goto err;
    }

    int encoder_id = -1;
    char *endptr;

    encoder_id = strtol(page_name + 7, &endptr, 10) - 1;
    if (*endptr != '\0' || encoder_id < 0 || encoder_id >= resources->count_encoders)
    {
        mvwprintw(pad, line++, 1, "Invalid Encoder name: %s\n", page_name);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[encoder_id]);
    if (!encoder)
    {
        mvwprintw(pad, line++, 1, "Failed to get Encoder %d\n", encoder_id);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    line++;
    mvwprintw(pad, line++, 1, "Encoder Type: ");
    if (encoder->encoder_type == DRM_MODE_ENCODER_NONE)
        print_red_text(pad, -1, -1, "NONE");
    else
        print_bold_text(pad, -1, -1, get_encoder_type_name(encoder->encoder_type));
    line++;

    mvwprintw(pad, line++, 1, "Encoder ID: %d", encoder->encoder_id);
    mvwprintw(pad, line++, 1, "CRTC ID: %d", encoder->crtc_id);
    mvwprintw(pad, line++, 1, "Possible CRTCs: %d", encoder->possible_crtcs);
    mvwprintw(pad, line++, 1, "Possible Clones: %d", encoder->possible_clones);

    mvwprintw(pad, ++line, 1, "Press 'e' to go back.");

    *content_line = line;
    drmModeFreeEncoder(encoder);
    drmModeFreeResources(resources);
    close(drm_fd);

err:
    wrefresh(pad);
    wgetch(pad);
    return;
}

void displayFramebuffer(WINDOW *pad, const char *page_name, int *content_line)
{
    int line = 0;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(pad, line++, 1, "Failed to open DRM Device!\n");
        close(drm_fd);
        goto err;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(pad, line++, 1, "Failed to get DRM resources\n");
        close(drm_fd);
        goto err;
    }

    int framebuffer_id = -1;
    char *endptr;

    framebuffer_id = strtol(page_name + 11, &endptr, 10) - 1;
    if (*endptr != '\0' || framebuffer_id < 0 || framebuffer_id >= resources->count_fbs)
    {
        mvwprintw(pad, line++, 1, "Invalid Framebuffer name: %s\n", page_name);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    drmModeFB *fb = drmModeGetFB(drm_fd, resources->fbs[framebuffer_id]);
    if (!fb)
    {
        mvwprintw(pad, line++, 1, "Failed to get Framebuffer %d\n", framebuffer_id);
        drmModeFreeResources(resources);
        close(drm_fd);
        goto err;
    }

    mvwprintw(pad, line++, 1, "Framebuffer ID: %d", fb->fb_id);
    mvwprintw(pad, line++, 1, "Width: %d", fb->width);
    mvwprintw(pad, line++, 1, "Height: %d", fb->height);
    mvwprintw(pad, line++, 1, "Pitch: %d", fb->pitch);
    mvwprintw(pad, line++, 1, "BPP: %d", fb->bpp);
    mvwprintw(pad, line++, 1, "Depth: %d", fb->depth);
    mvwprintw(pad, line++, 1, "Handle: %d", fb->handle);

    mvwprintw(pad, ++line, 1, "Press 'e' to go back.");

    *content_line = line;
    drmModeFreeFB(fb);
    drmModeFreeResources(resources);
    close(drm_fd);

err:
    wrefresh(pad);
    wgetch(pad);
    return;
}

void displayPlane(WINDOW *pad, const char *page_name, int *content_line)
{
    int line = 0;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(pad, line++, 1, "Failed to open DRM Device!\n");
        close(drm_fd);
        goto err;
    }

    drmSetClientCap(drm_fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);

    drmModePlaneRes *plane_resources = drmModeGetPlaneResources(drm_fd);
    if (!plane_resources)
    {
        mvwprintw(pad, line++, 1, "Failed to get DRM plane resources\n");
        close(drm_fd);
        goto err;
    }

    int plane_id = -1;
    char *endptr;

    plane_id = strtol(page_name + 5, &endptr, 10) - 1;
    if (*endptr != '\0' || plane_id < 0 || plane_id >= plane_resources->count_planes)
    {
        mvwprintw(pad, line++, 1, "Invalid Plane name: %s\n", page_name);
        drmModeFreePlaneResources(plane_resources);
        close(drm_fd);
        goto err;
    }

    drmModePlane *plane = drmModeGetPlane(drm_fd, plane_resources->planes[plane_id]);
    if (!plane)
    {
        mvwprintw(pad, line++, 1, "Failed to get Plane %d\n", plane_id);
        drmModeFreePlaneResources(plane_resources);
        close(drm_fd);
        goto err;
    }

    mvwprintw(pad, line++, 1, "Plane ID: %d", plane->plane_id);
    mvwprintw(pad, line++, 1, "CRTC ID: %d", plane->crtc_id);
    mvwprintw(pad, line++, 1, "Framebuffer ID: %d", plane->fb_id);
    mvwprintw(pad, line++, 1, "Possible CRTCs: %d", plane->possible_crtcs);
    mvwprintw(pad, line++, 1, "Gamma size: %u", plane->gamma_size);
    mvwprintw(pad, line++, 1, "CRTC Position: (%d, %d)", plane->crtc_x, plane->crtc_y);

    mvwprintw(pad, line++, 1, "Possible CRTC IDs: ");
    for (int i = 0; i < 32; i++)
    {
        if (plane->possible_crtcs & (1 << i))
        {
            wprintw(pad, "%u ", i);
        }
    }
    line++;

    mvwprintw(pad, line++, 1, "Number of formats: %u", plane->count_formats);
    if (plane->count_formats > 0)
    {
        mvwprintw(pad, line++, 1, "Formats: ");
        wattron(pad, A_DIM);
        for (uint32_t i = 0; i < plane->count_formats; i++)
        {
            if (i % 2 == 0)
                mvwprintw(pad, line++, 3, "0x%08X", plane->formats[i]);
            else
                mvwprintw(pad, line - 1, 30, "0x%08X", plane->formats[i]);
        }
        line++;
        wattroff(pad, A_DIM);
    }

    drmModeObjectProperties *props = drmModeObjectGetProperties(drm_fd, plane->plane_id, DRM_MODE_OBJECT_PLANE);
    if (!props)
    {
        mvwprintw(pad, line++, 1, "Failed to get properties for Plane %u\n", plane->plane_id);
        drmModeFreePlane(plane);
        drmModeFreePlaneResources(plane_resources);
        close(drm_fd);
        goto err;
    }

    line++;
    mvwprintw(pad, line++, 1, "Properties for Plane %u:", plane->plane_id);
    line++;

    for (uint32_t i = 0; i < props->count_props; i++)
    {
        drmModePropertyRes *prop = drmModeGetProperty(drm_fd, props->props[i]);
        if (prop)
        {
            mvwprintw(pad, line++, 1, "Property %u: %s", prop->prop_id, prop->name);
            wattron(pad, A_DIM);
            if (prop->flags & DRM_MODE_PROP_IMMUTABLE)
                wprintw(pad, " (Immutable) ");

            mvwprintw(pad, line++, 3, "blob Counts: %llu, Enum Counts: %d, Values Count: %llu", prop->count_blobs, prop->count_enums, prop->count_values);
            if (prop->flags & DRM_MODE_PROP_RANGE)
            {
                mvwprintw(pad, line++, 3, "Range: [%llu - %llu]", prop->values[0], prop->values[1]);
            }
            else if (prop->flags & DRM_MODE_PROP_ENUM)
            {
                for (int j = 0; j < prop->count_enums; j++)
                {
                    mvwprintw(pad, line++, 3, "Enum %d: %s = %llu", j, prop->enums[j].name, prop->enums[j].value);
                }
            }
            else if (prop->flags & DRM_MODE_PROP_BLOB)
            {
                mvwprintw(pad, line++, 3, "Blob: %llu", props->prop_values[i]);
            }
            else if (prop->flags & DRM_MODE_PROP_BITMASK)
            {
                mvwprintw(pad, line++, 3, "Bitmask: %llu", props->prop_values[i]);
            }

            wattroff(pad, A_DIM);
            drmModeFreeProperty(prop);
        }
        else
        {
            mvwprintw(pad, line++, 1, "  Property %u: (failed to get property details)", props->props[i]);
        }
    }

    drmModeFreeObjectProperties(props);

    mvwprintw(pad, ++line, 1, "Press 'e' to go back.");

    *content_line = line;
    drmModeFreePlane(plane);
    drmModeFreePlaneResources(plane_resources);
    close(drm_fd);

err:
    wrefresh(pad);
    wgetch(pad);
    return;
}
