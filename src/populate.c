#include "populate.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm/drm.h>

#define DRM_DEVICE "/dev/dri/card0"

void populateData()
{
    addRootChildren();
}

int initializeCrtcPages()
{
    int fd = open(DRM_DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("drmOpen");
        return 0;
    }

    drmModeRes *resources = drmModeGetResources(fd);
    if (!resources)
    {
        perror("drmModeGetResources");
        close(fd);
        return 0;
    }

    int crtcCount = -1;
    crtcCount = resources->count_crtcs;
    crtcPages = createNode("CRTCs", NULL, displayConfig);
    if (!crtcPages)
    {
        perror("malloc");
        drmModeFreeResources(resources);
        close(fd);
        return 0;
    }

    for (int i = 0; i < crtcCount; ++i)
    {
        char crtcName[10];
        snprintf(crtcName, sizeof(crtcName), "CRTC%d", i);
        Node *crtcNode = createNode(crtcName, displayCrtc, crtcPages);
        addChild(crtcPages, crtcNode);
    }

    drmModeFreeResources(resources);
    close(fd);

    return crtcCount;
}

int initializeConnectorPages()
{
    int fd = open(DRM_DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("drmOpen");
        return 0;
    }

    drmModeRes *resources = drmModeGetResources(fd);
    if (!resources)
    {
        perror("drmModeGetResources");
        close(fd);
        return 0;
    }

    int connectorCount = -1;
    connectorCount = resources->count_connectors;
    connectorPages = createNode("Connectors", NULL, displayConfig);
    if (!connectorPages)
    {
        perror("malloc");
        drmModeFreeResources(resources);
        close(fd);
        return 0;
    }

    for (int i = 0; i < connectorCount; ++i)
    {
        char connectorName[15];
        snprintf(connectorName, sizeof(connectorName), "Connector%d", i);
        Node *connectorNode = createNode(connectorName, displayConnector, connectorPages);
        addChild(connectorPages, connectorNode);
    }

    drmModeFreeResources(resources);
    close(fd);

    return connectorCount;
}

int initializeEncoderPages()
{
    int fd = open(DRM_DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("drmOpen");
        return 0;
    }

    drmModeRes *resources = drmModeGetResources(fd);
    if (!resources)
    {
        perror("drmModeGetResources");
        close(fd);
        return 0;
    }

    int encoderCount = -1;
    encoderCount = resources->count_encoders;
    encoderPages = createNode("Encoders", NULL, displayConfig);
    if (!encoderPages)
    {
        perror("malloc");
        drmModeFreeResources(resources);
        close(fd);
        return 0;
    }

    for (int i = 0; i < encoderCount; ++i)
    {
        char encoderName[15];
        snprintf(encoderName, sizeof(encoderName), "Encoder%d", i);
        Node *encoderNode = createNode(encoderName, displayEncoder, encoderPages);
        addChild(encoderPages, encoderNode);
    }

    drmModeFreeResources(resources);
    close(fd);

    return encoderCount;
}

int initializeFrameBuffers()
{
    int fd = open(DRM_DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("drmOpen");
        return 0;
    }

    drmModeRes *resources = drmModeGetResources(fd);
    if (!resources)
    {
        perror("drmModeGetResources");
        close(fd);
        return 0;
    }

    int framebufferCount = -1;
    framebufferCount = resources->count_fbs;
    framebufferPages = createNode("Framebuffers", NULL, displayConfig);
    if (!framebufferPages)
    {
        perror("malloc");
        drmModeFreeResources(resources);
        close(fd);
        return 0;
    }

    for (int i = 0; i < framebufferCount; ++i)
    {
        char framebufferName[15];
        snprintf(framebufferName, sizeof(framebufferName), "Framebuffer%d", i);
        Node *framebufferNode = createNode(framebufferName, displayFramebuffer, framebufferPages);
        addChild(framebufferPages, framebufferNode);
    }

    drmModeFreeResources(resources);
    close(fd);

    return framebufferCount;
}

int initializePlanes()
{
    int fd = open(DRM_DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("drmOpen");
        return 0;
    }

    drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);

    drmModePlaneRes *resources = drmModeGetPlaneResources(fd);
    if (!resources)
    {
        perror("drmModeGetPlaneResources");
        close(fd);
        return 0;
    }

    int planeCount = -1;
    planeCount = resources->count_planes;
    planePages = createNode("Planes", NULL, displayConfig);
    if (!planePages)
    {
        perror("malloc");
        drmModeFreePlaneResources(resources);
        close(fd);
        return 0;
    }

    for (int i = 0; i < planeCount; ++i)
    {
        char planeName[10];
        snprintf(planeName, sizeof(planeName), "Plane%d", i);
        Node *planeNode = createNode(planeName, displayPlane, planePages);

        char informatsName[15];
        snprintf(informatsName, sizeof(informatsName), "IN_FORMATS%d", i);
        Node *informatsNode = createNode(informatsName, displayInformats, planeNode);
        addChild(planeNode, informatsNode);

        char outformatsName[15];
        snprintf(outformatsName, sizeof(outformatsName), "FORMATS%d", i);
        Node *outformatsNode = createNode(outformatsName, displayFormats, planeNode);
        addChild(planeNode, outformatsNode);

        drmModePlane *plane = drmModeGetPlane(fd, resources->planes[i]);
        if (!plane)
        {
            perror("drmModeGetPlane");
            drmModeFreePlaneResources(resources);
            close(fd);
            return 0;
        }

        for (int j = 0; j < 32; j++)
        {
            if (plane->possible_crtcs & (1 << j))
            {
                char crtcName[10];
                snprintf(crtcName, sizeof(crtcName), "CRTC%d", j);
                Node *crtcNode = createNode(crtcName, displayCrtc, planeNode);
                addChild(planeNode, crtcNode);
            }
        }

        drmModeFreePlane(plane);
        addChild(planePages, planeNode);
    }

    drmModeFreePlaneResources(resources);
    close(fd);

    return planeCount;
}

void initializedisplayConfig()
{
    displayConfig = createNode("Display Configuration", NULL, root);
    if (!displayConfig)
    {
        perror("malloc");
        free(crtcPages);
        return;
    }

    initializePlanes();
    addChild(displayConfig, planePages);
    
    initializeCrtcPages();
    addChild(displayConfig, crtcPages);

    initializeConnectorPages();
    addChild(displayConfig, connectorPages);

    initializeEncoderPages();
    addChild(displayConfig, encoderPages);

    initializeFrameBuffers();
    addChild(displayConfig, framebufferPages);

    Node *OtherInfo = createNode("Other Display Info", NULL, displayConfig);
    if (!OtherInfo)
    {
        perror("malloc");
        free(crtcPages);
        return;
    }

    addChild(displayConfig, OtherInfo);
}

void addRootChildren()
{
    root = createNode("Display Top", displaySummary, NULL);

    initializedisplayConfig();
    addChild(root, displayConfig);

    addChild(root, createNode("GPU Information", NULL, root));

    addChild(root, createNode("Power Management", NULL, root));

    addChild(root, createNode("Display Capabilities", NULL, root));

    addChild(root, createNode("Performance Tuning", NULL, root));

    addChild(root, createNode("Debugging and Error Info", NULL, root));

    addChild(root, createNode("Driver and Module Info", NULL, root));

    addChild(root, createNode("Internal and Client Info", NULL, root));

    addChild(root, createNode("System State", NULL, root));
}
