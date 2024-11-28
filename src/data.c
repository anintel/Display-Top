#include "data.h"
#include "display.h"

Node *crtcPages;
Node *connectorPages;
Node *encoderPages;
Node *framebufferPages;

Node *displayConfigMenu;

Node gpuInfoPages[] = {
    {"GPU General Information", "page", NULL, NULL, 0},
    {"Engine Information", "page", NULL, NULL, 0},
    {"GEM Objects", "page", NULL, NULL, 0}};

Node powerManagementPages[] = {
    {"Runtime Power Management", "page", NULL, NULL, 0},
    {"Low Power Modes", "page", NULL, NULL, 0}};

Node displayCapabilitiesPages[] = {
    {"Supported Features", "page", NULL, NULL, 0},
    {"DMC and PSR", "page", NULL, NULL, 0}};

Node performanceTuningPages[] = {
    {"GPU Frequency and Boosting", "page", NULL, NULL, 0},
    {"Watermark Latencies", "page", NULL, NULL, 0}};

Node debuggingErrorInfoPages[] = {
    {"Error State", "page", NULL, NULL, 0},
    {"Debugging Utilities", "page", NULL, NULL, 0},
    {"Frontbuffer and Swizzle Info", "page", NULL, NULL, 0}};

Node driverModuleInfoPages[] = {
    {"Driver Parameters", "page", NULL, NULL, 0},
    {"Capabilities and OpRegion", "page", NULL, NULL, 0},
    {"Shared DPLLs and Workarounds", "page", NULL, NULL, 0}};

Node internalClientInfoPages[] = {
    {"Internal Clients", "page", NULL, NULL, 0},
    {"External Clients", "page", NULL, NULL, 0}};

Node systemStatePages[] = {
    {"Overall State", "page", NULL, NULL, 0},
    {"Hotplug Detect Control", "page", NULL, NULL, 0},
    {"SAGV and IPC Status", "page", NULL, NULL, 0}};

size_t gpuInfoPagesSize = sizeof(gpuInfoPages) / sizeof(Node);
size_t powerManagementPagesSize = sizeof(powerManagementPages) / sizeof(Node);
size_t displayCapabilitiesPagesSize = sizeof(displayCapabilitiesPages) / sizeof(Node);
size_t performanceTuningPagesSize = sizeof(performanceTuningPages) / sizeof(Node);
size_t debuggingErrorInfoPagesSize = sizeof(debuggingErrorInfoPages) / sizeof(Node);
size_t driverModuleInfoPagesSize = sizeof(driverModuleInfoPages) / sizeof(Node);
size_t internalClientInfoPagesSize = sizeof(internalClientInfoPages) / sizeof(Node);
size_t systemStatePagesSize = sizeof(systemStatePages) / sizeof(Node);

Node *mainMenu;

size_t mainMenuSize = 0;