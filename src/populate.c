#include "populate.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#define DRM_DEVICE "/dev/dri/card0"

void populateData()
{
    initializeMainMenu();
}

void setString(char *dest, const char *src, size_t size)
{
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
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

int initializeConnectorPages(){
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
    // if (framebuffersSize == 0)
    //     return NULL;
    // else
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