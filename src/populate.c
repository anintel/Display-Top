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
    initializeMainMenu();
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
    crtcPages = (Node *)malloc(crtcCount * sizeof(Node));
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
        snprintf(crtcName, sizeof(crtcName), "CRTC%d", i + 1);
        setString(crtcPages[i].name, crtcName, sizeof(crtcPages[i].name));
        setString(crtcPages[i].type, "page", sizeof(crtcPages[i].type));
        crtcPages[i].displayFunction = displayCrtc;
        crtcPages[i].submenu = NULL;
        crtcPages[i].submenuSize = 0;
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
    connectorPages = (Node *)malloc(connectorCount * sizeof(Node));
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
        snprintf(connectorName, sizeof(connectorName), "Connector%d", i + 1);
        setString(connectorPages[i].name, connectorName, sizeof(connectorPages[i].name));
        setString(connectorPages[i].type, "page", sizeof(connectorPages[i].type));
        connectorPages[i].displayFunction = displayConnector;
        connectorPages[i].submenu = NULL;
        connectorPages[i].submenuSize = 0;
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
    encoderPages = (Node *)malloc(encoderCount * sizeof(Node));
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
        snprintf(encoderName, sizeof(encoderName), "Encoder%d", i + 1);
        setString(encoderPages[i].name, encoderName, sizeof(encoderPages[i].name));
        setString(encoderPages[i].type, "page", sizeof(encoderPages[i].type));
        encoderPages[i].displayFunction = displayEncoder;
        encoderPages[i].submenu = NULL;
        encoderPages[i].submenuSize = 0;
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
    framebufferPages = (Node *)malloc(framebufferCount * sizeof(Node));
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
        snprintf(framebufferName, sizeof(framebufferName), "Framebuffer%d", i + 1);
        setString(framebufferPages[i].name, framebufferName, sizeof(framebufferPages[i].name));
        setString(framebufferPages[i].type, "page", sizeof(framebufferPages[i].type));
        framebufferPages[i].displayFunction = displayFramebuffer;
        framebufferPages[i].submenu = NULL;
        framebufferPages[i].submenuSize = 0;
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
        perror("drmModeGetResources");
        close(fd);
        return 0;
    }

    int planeCount = -1;
    planeCount = resources->count_planes;
    planePages = (Node *)malloc(planeCount * sizeof(Node));
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
        setString(planePages[i].name, planeName, sizeof(planePages[i].name));
        setString(planePages[i].type, "menu", sizeof(planePages[i].type));
        planePages[i].displayFunction = displayPlane;
        planePages[i].submenu = NULL;
        planePages[i].submenuSize = 0;
        
        planePages[i].submenu = (Node *)malloc(2 * sizeof(Node));
        if (!planePages[i].submenu)
        {
            perror("malloc");
            drmModeFreePlaneResources(resources);
            close(fd);
            return 0;
        }

        char informatsName[15];
        snprintf(informatsName, sizeof(informatsName), "IN_FORMATS%d", i);
        setString(planePages[i].submenu[0].name, informatsName, sizeof(planePages[i].submenu[0].name));
        setString(planePages[i].submenu[0].type, "page", sizeof(planePages[i].submenu[0].type));
        planePages[i].submenu[0].displayFunction = displayInformats;
        planePages[i].submenu[0].submenu = NULL;
        planePages[i].submenu[0].submenuSize = 0;

        setString(planePages[i].submenu[1].name, "Goto CRTC", sizeof(planePages[i].submenu[1].name));
        setString(planePages[i].submenu[1].type, "page", sizeof(planePages[i].submenu[1].type));
        planePages[i].submenu[1].displayFunction = gotoCrtc;
        planePages[i].submenu[1].submenu = NULL;
        planePages[i].submenu[1].submenuSize = 0;

        planePages[i].submenuSize = 2;

    }

    drmModeFreePlaneResources(resources);
    close(fd);

    return planeCount;
}

int initializeDisplayConfigMenu()
{
    int count = 0;
    int index = 0;

    int crtcsSize = initializeCrtcPages();
    // todo: don't check for zero check for -1. update the init functions aswell.
    if (crtcsSize == 0)
        return NULL;
    else
        count++;

    int planesSize = initializePlanes();
    if (planesSize == 0)
        // return NULL;
        count++;
    else
        count++;

    int connectorsSize = initializeConnectorPages();
    if (connectorsSize == 0)
        return NULL;
    else
        count++;

    int encodersSize = initializeEncoderPages();
    if (encodersSize == 0)
        return NULL;
    else
        count++;

    int framebuffersSize = initializeFrameBuffers();
    if (framebuffersSize == 0)
        // return NULL;
        count++;
    else
        count++;

    count++;
    count++;

    displayConfigMenu = (Node *)malloc(count * sizeof(Node));
    if (!displayConfigMenu)
    {
        perror("malloc");
        free(crtcPages);
        return;
    }

    setString(displayConfigMenu[index].name, "CRTCs", sizeof(displayConfigMenu[index].name));
    setString(displayConfigMenu[index].type, "menu", sizeof(displayConfigMenu[index].type));
    displayConfigMenu[index].displayFunction = NULL;
    displayConfigMenu[index].submenu = crtcPages;
    displayConfigMenu[index].submenuSize = crtcsSize;
    index++;

    setString(displayConfigMenu[index].name, "Planes", sizeof(displayConfigMenu[index].name));
    setString(displayConfigMenu[index].type, "menu", sizeof(displayConfigMenu[index].type));
    displayConfigMenu[index].displayFunction = NULL;
    displayConfigMenu[index].submenu = planePages;
    displayConfigMenu[index].submenuSize = planesSize;
    index++;

    setString(displayConfigMenu[index].name, "Connectors", sizeof(displayConfigMenu[index].name));
    setString(displayConfigMenu[index].type, "menu", sizeof(displayConfigMenu[index].type));
    displayConfigMenu[index].displayFunction = NULL;
    displayConfigMenu[index].submenu = connectorPages;
    displayConfigMenu[index].submenuSize = connectorsSize;
    index++;

    setString(displayConfigMenu[index].name, "Encoders", sizeof(displayConfigMenu[index].name));
    setString(displayConfigMenu[index].type, "menu", sizeof(displayConfigMenu[index].type));
    displayConfigMenu[index].displayFunction = NULL;
    displayConfigMenu[index].submenu = encoderPages;
    displayConfigMenu[index].submenuSize = encodersSize;
    index++;

    setString(displayConfigMenu[index].name, "Framebuffers", sizeof(displayConfigMenu[index].name));
    setString(displayConfigMenu[index].type, "menu", sizeof(displayConfigMenu[index].type));
    displayConfigMenu[index].displayFunction = NULL;
    displayConfigMenu[index].submenu = framebufferPages;
    displayConfigMenu[index].submenuSize = framebuffersSize;
    index++;

    setString(displayConfigMenu[index].name, "Other Display Info", sizeof(displayConfigMenu[index].name));
    setString(displayConfigMenu[index].type, "page", sizeof(displayConfigMenu[index].type));
    displayConfigMenu[index].displayFunction = NULL;
    displayConfigMenu[index].submenu = NULL;
    displayConfigMenu[index].submenuSize = 0;
    index++;

    return count;
}

// int initializeGpuGeneralInfo()
// {
//     gpuGeneralInfoPages = (Node *)malloc(1 * sizeof(Node));
//     if (!gpuGeneralInfoPages)
//     {
//         perror("malloc");
//         return 0;
//     }

//     setString(gpuGeneralInfoPages[0].name, "i915_gpu_info", sizeof(gpuGeneralInfoPages[0].name));
//     setString(gpuGeneralInfoPages[0].type, "page", sizeof(gpuGeneralInfoPages[0].type));
//     gpuGeneralInfoPages[0].displayFunction = displayDebugfsFile;
//     gpuGeneralInfoPages[0].submenu = NULL;
//     gpuGeneralInfoPages[0].submenuSize = 0;

//     return 1;
// }

// int initializeEngineInfo()
// {
//     engineInfoPages = (Node *)malloc(1 * sizeof(Node));
//     if (!engineInfoPages)
//     {
//         perror("malloc");
//         return 0;
//     }

//     setString(engineInfoPages[0].name, "i915_engine_info", sizeof(engineInfoPages[0].name));
//     setString(engineInfoPages[0].type, "page", sizeof(engineInfoPages[0].type));
//     engineInfoPages[0].displayFunction = displayDebugfsFile;
//     engineInfoPages[0].submenu = NULL;
//     engineInfoPages[0].submenuSize = 0;

//     return 1;
// }

// int initializeGemObjects()
// {
//     gemObjectsPages = (Node *)malloc(3 * sizeof(Node));
//     if (!gemObjectsPages)
//     {
//         perror("malloc");
//         return 0;
//     }

//     setString(gemObjectsPages[0].name, "gem_names", sizeof(gemObjectsPages[0].name));
//     setString(gemObjectsPages[0].type, "page", sizeof(gemObjectsPages[0].type));
//     gemObjectsPages[0].displayFunction = displayDebugfsFile;
//     gemObjectsPages[0].submenu = NULL;
//     gemObjectsPages[0].submenuSize = 0;

//     setString(gemObjectsPages[1].name, "i915_gem_objects", sizeof(gemObjectsPages[1].name));
//     setString(gemObjectsPages[1].type, "page", sizeof(gemObjectsPages[1].type));
//     gemObjectsPages[1].displayFunction = displayDebugfsFile;
//     gemObjectsPages[1].submenu = NULL;
//     gemObjectsPages[1].submenuSize = 0;

//     setString(gemObjectsPages[2].name, "i915_gem_framebuffer", sizeof(gemObjectsPages[2].name));
//     setString(gemObjectsPages[2].type, "page", sizeof(gemObjectsPages[2].type));
//     gemObjectsPages[2].displayFunction = displayDebugfsFile;
//     gemObjectsPages[2].submenu = NULL;
//     gemObjectsPages[2].submenuSize = 0;

//     return 3;
// }

// int initializeGpuInfoMenu()
// {
//     int count = 0;
//     int index = 0;

//     gpuInfoMenu = (Node *)malloc(3 * sizeof(Node));
//     if (!gpuInfoMenu)
//     {
//         perror("malloc");
//         return 0;
//     }

//     setString(gpuInfoMenu[index].name, "GPU General Information", sizeof(gpuInfoMenu[index].name));
//     setString(gpuInfoMenu[index].type, "page", sizeof(gpuInfoMenu[index].type));
//     gpuInfoMenu[index].displayFunction = displayDebugfsFile;
//     gpuInfoMenu[index].submenu = NULL;
//     gpuInfoMenu[index].submenuSize = 0;
//     index++;

//     setString(gpuInfoMenu[index].name, "Engine Information", sizeof(gpuInfoMenu[index].name));
//     setString(gpuInfoMenu[index].type, "page", sizeof(gpuInfoMenu[index].type));
//     gpuInfoMenu[index].displayFunction = displayDebugfsFile;
//     gpuInfoMenu[index].submenu = NULL;
//     gpuInfoMenu[index].submenuSize = 0;
//     index++;

//     setString(gpuInfoMenu[index].name, "GEM Objects", sizeof(gpuInfoMenu[index].name));
//     setString(gpuInfoMenu[index].type, "page", sizeof(gpuInfoMenu[index].type));
//     gpuInfoMenu[index].displayFunction = displayDebugfsFile;
//     gpuInfoMenu[index].submenu = NULL;
//     gpuInfoMenu[index].submenuSize = 0;
//     index++;

//     return 3;
// }

void initializeMainMenu()
{
    int count = 0;
    int displayConfigCount = initializeDisplayConfigMenu();

    if (!displayConfigMenu)
    {
        printf("Failed to initialize displayConfigMenu\n");
        return;
    }
    mainMenu = (Node *)malloc(9 * sizeof(Node));
    if (!mainMenu)
    {
        perror("malloc");
        free(displayConfigMenu);
        return;
    }

    setString(mainMenu[count].name, "Display Configuration", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = displayConfigMenu;
    mainMenu[count].submenuSize = displayConfigCount;
    count++;

    setString(mainMenu[count].name, "GPU Information", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = gpuInfoPages;
    mainMenu[count].submenuSize = gpuInfoPagesSize;
    count++;

    setString(mainMenu[count].name, "Power Management", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = powerManagementPages;
    mainMenu[count].submenuSize = powerManagementPagesSize;
    count++;

    setString(mainMenu[count].name, "Display Capabilities", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = displayCapabilitiesPages;
    mainMenu[count].submenuSize = displayCapabilitiesPagesSize;
    count++;

    setString(mainMenu[count].name, "Performance Tuning", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = performanceTuningPages;
    mainMenu[count].submenuSize = performanceTuningPagesSize;
    count++;

    setString(mainMenu[count].name, "Debugging and Error Info", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = debuggingErrorInfoPages;
    mainMenu[count].submenuSize = debuggingErrorInfoPagesSize;
    count++;

    setString(mainMenu[count].name, "Driver and Module Info", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = driverModuleInfoPages;
    mainMenu[count].submenuSize = driverModuleInfoPagesSize;
    count++;

    setString(mainMenu[count].name, "Internal and Client Info", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = internalClientInfoPages;
    mainMenu[count].submenuSize = internalClientInfoPagesSize;
    count++;

    setString(mainMenu[count].name, "System State", sizeof(mainMenu[count].name));
    setString(mainMenu[count].type, "menu", sizeof(mainMenu[count].type));
    mainMenu[count].displayFunction = NULL;
    mainMenu[count].submenu = systemStatePages;
    mainMenu[count].submenuSize = systemStatePagesSize;
    count++;

    mainMenuSize = count;
}