#include "populate.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#define DRM_DEVICE "/dev/dri/card0"

// void populateData()
// {
//     initializeCrtcPages();
//     // initializeConnectorPages();
//     // initializeEncoderPages();
//     // initializeFramebufferPages();
//     // initializeDisplayConfigMenu();
//     // initializeGpuInfoPages();
//     // initializePowerManagementPages();
//     // initializeDisplayCapabilitiesPages();
//     // initializePerformanceTuningPages();
//     // initializeDebuggingErrorInfoPages();
//     // initializeDriverModuleInfoPages();
// }

// void initializeCrtcPages()
// {
//     if (crtcPages == NULL)
//     {
//         int fd = open(DRM_DEVICE, O_RDWR);
//         if (fd < 0)
//         {
//             perror("drmOpen");
//             return;
//         }

//         drmModeRes *resources = drmModeGetResources(fd);
//         if (!resources)
//         {
//             perror("drmModeGetResources");
//             close(fd);
//             return;
//         }

//         int crtcCount = resources->count_crtcs;
//         crtcPages = (Node *)malloc(crtcCount * sizeof(Node));
//         if (!crtcPages)
//         {
//             perror("malloc");
//             drmModeFreeResources(resources);
//             close(fd);
//             return;
//         }

//         for (int i = 0; i < crtcCount; ++i)
//         {
//             snprintf(crtcPages[i].name, sizeof(crtcPages[i].name), "CRTC %d", i + 1);
//             strncpy(crtcPages[i].type, "page", sizeof(crtcPages[i].type) - 1);
//             crtcPages[i].type[sizeof(crtcPages[i].type) - 1] = '\0';
//             crtcPages[i].displayFunction = displayCrtc;
//         }

//         drmModeFreeResources(resources);
//         close(fd);
//     }
// }

// // Node displayConfigMenu[] = {
// //     {"CRTCs", "menu", NULL, crtcPages, sizeof(crtcPages) / sizeof(Node)},
// //     {"Connectors", "menu", NULL, connectorPages, sizeof(connectorPages) / sizeof(Node)},
// //     {"Encoders", "menu", NULL, encoderPages, sizeof(encoderPages) / sizeof(Node)},
// //     {"Framebuffers", "menu", NULL, framebufferPages, sizeof(framebufferPages) / sizeof(Node)},
// //     {"Other Display Info", "page", NULL, NULL, 0}};

// void setString(char *dest, const char *src, size_t size)
// {
//     strncpy(dest, src, size - 1);
//     dest[size - 1] = '\0';
// }
// Node * crtcPages;
// Node * displayConfigMenu2 = NULL;
// void initializeDisplayConfigMenu()
// {
//     if (displayConfigMenu2 == NULL)
//     {
//         displayConfigMenu2 = (Node *)malloc(5 * sizeof(Node));
//         if (!displayConfigMenu)
//         {
//             perror("malloc");
//             return;
//         }

//         initializeCrtcPages();
//         setString(displayConfigMenu[0].name, "CRTCs", sizeof(displayConfigMenu[0].name));
//         setString(displayConfigMenu[0].type, "menu", sizeof(displayConfigMenu[0].type));
//         displayConfigMenu[0].submenu = crtcPages;
//         displayConfigMenu[0].submenuSize = sizeof(crtcPages) / sizeof(Node);

//         strncpy(displayConfigMenu[1].name, "Connectors", sizeof(displayConfigMenu[1].name) - 1);
//         displayConfigMenu[1].name[sizeof(displayConfigMenu[1].name) - 1] = '\0';
//         strncpy(displayConfigMenu[1].type, "menu", sizeof(displayConfigMenu[1].type) - 1);
//         displayConfigMenu[1].type[sizeof(displayConfigMenu[1].type) - 1] = '\0';
//         displayConfigMenu[1].submenu = connectorPages;
//         displayConfigMenu[1].submenuSize = sizeof(connectorPages) / sizeof(Node);

//         strncpy(displayConfigMenu[2].name, "Encoders", sizeof(displayConfigMenu[2].name) - 1);
//         displayConfigMenu[2].name[sizeof(displayConfigMenu[2].name) - 1] = '\0';
//         strncpy(displayConfigMenu[2].type, "menu", sizeof(displayConfigMenu[2].type) - 1);
//         displayConfigMenu[2].type[sizeof(displayConfigMenu[2].type) - 1] = '\0';
//         displayConfigMenu[2].submenu = encoderPages;
//         displayConfigMenu[2].submenuSize = sizeof(encoderPages) / sizeof(Node);

//         strncpy(displayConfigMenu[3].name, "Framebuffers", sizeof(displayConfigMenu[3].name) - 1);
//         displayConfigMenu[3].name[sizeof(displayConfigMenu[3].name) - 1] = '\0';
//         strncpy(displayConfigMenu[3].type, "menu", sizeof(displayConfigMenu[3].type) - 1);
//         displayConfigMenu[3].type[sizeof(displayConfigMenu[3].type) - 1] = '\0';
//         displayConfigMenu[3].submenu = framebufferPages;
//         displayConfigMenu[3].submenuSize = sizeof(framebufferPages) / sizeof(Node);
        
//         strncpy(displayConfigMenu[4].name, "Other Display Info", sizeof(displayConfigMenu[4].name) - 1);
//         displayConfigMenu[4].name[sizeof(displayConfigMenu[4].name) - 1] = '\0';
//         strncpy(displayConfigMenu[4].type, "page", sizeof(displayConfigMenu[4].type) - 1);
//         displayConfigMenu[4].type[sizeof(displayConfigMenu[4].type) - 1] = '\0';
//     }
// }