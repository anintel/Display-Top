#include "populate.h"

int initializeCrtcPages()
{
    int fd = open_primary_drm_device();
    if (fd < 0)
    {
        perror("drmOpen");
        return 0;
    }

    drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);

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

        drmModeCrtc *crtc = drmModeGetCrtc(fd, resources->crtcs[i]);
        if (!crtc)
        {
            perror("drmModeGetCrtc");
            drmModeFreeResources(resources);
            close(fd);
            return 0;
        }

        drmModePlaneRes *planeResources = drmModeGetPlaneResources(fd);
        if (!planeResources)
        {
            perror("drmModeGetPlaneResources");
            drmModeFreeCrtc(crtc);
            drmModeFreeResources(resources);
            close(fd);
            return 0;
        }

        int planeCount = planeResources->count_planes;

        for (int j = 0; j < planeCount; ++j)
        {
            drmModePlane *plane = drmModeGetPlane(fd, planeResources->planes[j]);
            if (!plane)
            {
                perror("drmModeGetPlane");
                drmModeFreeResources(resources);
                close(fd);
                return 0;
            }

            if (plane->possible_crtcs & (1 << i))
            {
                char planeName[10];
                snprintf(planeName, sizeof(planeName), "Plane%d", j);
                Node *planeNode = createNode(planeName, displayPlane, crtcNode);
                
                char informatsName[15];
                snprintf(informatsName, sizeof(informatsName), "IN_FORMATS%d", j);
                Node *informatsNode = createNode(informatsName, displayInformats, planeNode);
                addChild(planeNode, informatsNode);

                char outformatsName[15];
                snprintf(outformatsName, sizeof(outformatsName), "FORMATS%d", i);
                Node *outformatsNode = createNode(outformatsName, displayFormats, planeNode);
                addChild(planeNode, outformatsNode);

                addChild(crtcNode, planeNode);
            }
            drmModeFreePlane(plane);
        }

        for (int j = 0; j < resources->count_encoders; ++j)
        {
            drmModeEncoder *encoder = drmModeGetEncoder(fd, resources->encoders[j]);
            if (!encoder)
            {
                perror("drmModeGetEncoder");
                drmModeFreeResources(resources);
                close(fd);
                return 0;
            }

            if (encoder->crtc_id == crtc->crtc_id)
            {
                char encoderName[15];
                snprintf(encoderName, sizeof(encoderName), "Encoder%d", j);
                Node *encoderNode = createNode(encoderName, displayEncoder, crtcNode);
                addChild(crtcNode, encoderNode);
            }

            drmModeFreeEncoder(encoder);
        }

        for (int j = 0; j < resources->count_connectors; ++j)
        {
            drmModeConnector *connector = drmModeGetConnector(fd, resources->connectors[j]);
            if (!connector)
            {
                fprintf(stderr, "Failed to get connector %d", j);
                continue;
            }

            if (connector->encoder_id)
            {
                drmModeEncoder *encoder = drmModeGetEncoder(fd, connector->encoder_id);
                if (encoder)
                {
                    if (encoder->crtc_id == crtc->crtc_id)
                    {
                        char connectorName[15];
                        snprintf(connectorName, sizeof(connectorName), "Connector%d", j);
                        Node *connectorNode = createNode(connectorName, displayConnector, crtcNode);
                        addChild(crtcNode, connectorNode);
                    }

                    drmModeFreeEncoder(encoder);
                }
                else
                {
                    fprintf(stderr, "Failed to get encoder for connector %d", j);
                }
            }

            drmModeFreeConnector(connector);
        }

        addChild(crtcPages, crtcNode);

        drmModeFreeCrtc(crtc);
    }

    drmModeFreeResources(resources);
    close(fd);

    return crtcCount;
}

int initializeConnectorPages()
{
    int fd = open_primary_drm_device();
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
    int fd = open_primary_drm_device();
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
    int fd = open_primary_drm_device();
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
    int fd = open_primary_drm_device();
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

// Todo: fix up the unwanted opening and closing of drm and streamline the node
// creation for the connectors, encoders, framebuffers, and planes
void initializeDisplayConfig()
{
    displayConfig = createNode("Display Configuration", NULL, root);
    if (!displayConfig)
    {
        perror("malloc");
        free(crtcPages);
        return;
    }

    initializeCrtcPages();
    addChild(displayConfig, crtcPages);

    initializePlanes();
    addChild(displayConfig, planePages);

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
    
    addChild(root, displayConfig);
}