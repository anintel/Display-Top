#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <ncurses.h>

#include "display.h"

#define DRM_DEVICE "/dev/dri/card0"

void displayCrtc(WINDOW *win, const char *page_name)
{
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "%s - information", page_name);
    wrefresh(win);

    int line = 3;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(win, line++, 1, "Failed to open DRM Device!\n");
        wrefresh(win);
        wgetch(win);
        close(drm_fd);
        return;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(win, line++, 1, "Failed to get DRM resources\n");
        wrefresh(win);
        wgetch(win);
        close(drm_fd);
        return;
    }

    int crtc_id = -1;
    char *endptr;

    crtc_id = strtol(page_name + 4, &endptr, 10) - 1;
    if (*endptr != '\0' || crtc_id < 0 || crtc_id >= resources->count_crtcs)
    {
        mvwprintw(win, line++, 1, "Invalid CRTC name: %s\n", page_name);
        drmModeFreeResources(resources);
        wrefresh(win);
        wgetch(win);
        close(drm_fd);
        return;
    }

    drmModeCrtc *crtc = drmModeGetCrtc(drm_fd, resources->crtcs[crtc_id]);
    if (!crtc)
    {
        mvwprintw(win, line++, 1, "Failed to get CRTC %d\n", crtc_id);
        drmModeFreeResources(resources);
        close(drm_fd);
        wrefresh(win);
        wgetch(win);
        return;
    }

    mvwprintw(win, line++, 1, "CRTC ID: %d", crtc->crtc_id);
    mvwprintw(win, line++, 1, "Buffer ID: %d", crtc->buffer_id);
    mvwprintw(win, line++, 1, "X: %d", crtc->x);
    mvwprintw(win, line++, 1, "Y: %d", crtc->y);
    mvwprintw(win, line++, 1, "Width: %d", crtc->width);
    mvwprintw(win, line++, 1, "Height: %d", crtc->height);
    mvwprintw(win, line++, 1, "Mode Valid: %s", crtc->mode_valid ? "Yes" : "No");

    mvwprintw(win, ++line, 1, "Press 'e' to go back.");

    drmModeFreeCrtc(crtc);
    drmModeFreeResources(resources);
    close(drm_fd);
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e')
    {
    }
}

void displayConnector(WINDOW *win, const char *page_name)
{
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "%s - information", page_name);
    wrefresh(win);

    int line = 3;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(win, line++, 1, "Failed to open DRM Device!\n");
        wrefresh(win);
        wgetch(win);
        close(drm_fd);
        return;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(win, line++, 1, "Failed to get DRM resources\n");
        wrefresh(win);
        wgetch(win);
        close(drm_fd);
        return;
    }

    int connector_id = -1;
    char *endptr;

    connector_id = strtol(page_name + 9, &endptr, 10) - 1;
    if (*endptr != '\0' || connector_id < 0 || connector_id >= resources->count_connectors)
    {
        mvwprintw(win, line++, 1, "Invalid Connector name: %s\n", page_name);
        drmModeFreeResources(resources);
        wrefresh(win);
        wgetch(win);
        close(drm_fd);
        return;
    }

    drmModeConnector *connector = drmModeGetConnector(drm_fd, resources->connectors[connector_id]);
    if (!connector)
    {
        mvwprintw(win, line++, 1, "Failed to get Connector %d\n", connector_id);
        drmModeFreeResources(resources);
        wrefresh(win);
        wgetch(win);
        close(drm_fd);
        return;
    }

    mvwprintw(win, line++, 1, "Connector ID: %d", connector->connector_id);
    mvwprintw(win, line++, 1, "Encoder ID: %d", connector->encoder_id);
    mvwprintw(win, line++, 1, "Connection: %s", connector->connection == DRM_MODE_CONNECTED ? "Connected" : "Disconnected");
    mvwprintw(win, line++, 1, "Connector Type: %s", get_connector_type_name(connector->connector_type));
    mvwprintw(win, line++, 1, "Modes: %d", connector->count_modes);
    mvwprintw(win, line++, 1, "Subpixel: %d", connector->subpixel);

    mvwprintw(win, ++line, 1, "Press 'e' to go back.");
    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);
    close(drm_fd);
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e')
    {
    }
}

void displayEncoder(WINDOW *win, const char *page_name)
{
    wclear(win);
    // make window full screen

    box(win, 0, 0);
    mvwprintw(win, 1, 1, "%s - information", page_name);
    wrefresh(win);

    int line = 3;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(win, line++, 1, "Failed to open DRM Device!\n");
        wrefresh(win);
        wgetch(win);
        return;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(win, line++, 1, "Failed to get DRM resources\n");
        close(drm_fd);
        wrefresh(win);
        wgetch(win);
        return;
    }

    int encoder_id = -1;
    char *endptr;

    encoder_id = strtol(page_name + 7, &endptr, 10) - 1;
    if (*endptr != '\0' || encoder_id < 0 || encoder_id >= resources->count_encoders)
    {
        mvwprintw(win, line++, 1, "Invalid Encoder name: %s\n", page_name);
        drmModeFreeResources(resources);
        close(drm_fd);
        wrefresh(win);
        wgetch(win);
        return;
    }

    drmModeEncoder *encoder = drmModeGetEncoder(drm_fd, resources->encoders[encoder_id]);
    if (!encoder)
    {
        mvwprintw(win, line++, 1, "Failed to get Encoder %d\n", encoder_id);
        drmModeFreeResources(resources);
        close(drm_fd);
        wrefresh(win);
        wgetch(win);
        return;
    }

    mvwprintw(win, line++, 1, "Encoder ID: %d", encoder->encoder_id);
    mvwprintw(win, line++, 1, "CRTC ID: %d", encoder->crtc_id);
    mvwprintw(win, line++, 1, "Possible CRTCs: %d", encoder->possible_crtcs);
    mvwprintw(win, line++, 1, "Possible Clones: %d", encoder->possible_clones);

    mvwprintw(win, ++line, 1, "Press 'e' to go back.");

    drmModeFreeEncoder(encoder);
    drmModeFreeResources(resources);
    close(drm_fd);
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e')
    {
    }
}

void displayFramebuffer(WINDOW *win, const char *page_name)
{
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "%s - information", page_name);
    wrefresh(win);

    int line = 3;

    const int drm_fd = open(DRM_DEVICE, O_RDWR);
    if (drm_fd < 0)
    {
        mvwprintw(win, line++, 1, "Failed to open DRM Device!\n");
        wrefresh(win);
        wgetch(win);
        return;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources)
    {
        mvwprintw(win, line++, 1, "Failed to get DRM resources\n");
        close(drm_fd);
        wrefresh(win);
        wgetch(win);
        return;
    }

    int framebuffer_id = -1;
    char *endptr;

    framebuffer_id = strtol(page_name + 11, &endptr, 10) - 1;
    if (*endptr != '\0' || framebuffer_id < 0 || framebuffer_id >= resources->count_fbs)
    {
        mvwprintw(win, line++, 1, "Invalid Framebuffer name: %s\n", page_name);
        drmModeFreeResources(resources);
        close(drm_fd);
        wrefresh(win);
        wgetch(win);
        return;
    }

    drmModeFB *fb = drmModeGetFB(drm_fd, resources->fbs[framebuffer_id]);
    if (!fb)
    {
        mvwprintw(win, line++, 1, "Failed to get Framebuffer %d\n", framebuffer_id);
        drmModeFreeResources(resources);
        close(drm_fd);
        wrefresh(win);
        wgetch(win);
        return;
    }

    mvwprintw(win, line++, 1, "Framebuffer ID: %d", fb->fb_id);
    mvwprintw(win, line++, 1, "Width: %d", fb->width);
    mvwprintw(win, line++, 1, "Height: %d", fb->height);
    mvwprintw(win, line++, 1, "Pitch: %d", fb->pitch);
    mvwprintw(win, line++, 1, "BPP: %d", fb->bpp);
    mvwprintw(win, line++, 1, "Depth: %d", fb->depth);
    mvwprintw(win, line++, 1, "Handle: %d", fb->handle);

    mvwprintw(win, ++line, 1, "Press 'e' to go back.");

    drmModeFreeFB(fb);
    drmModeFreeResources(resources);
    close(drm_fd);
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e')
    {
    }
}