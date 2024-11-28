#include "data.h"
#include "display.h"

Node crtcPages[] = {
    {"CRTC1", "page", displayCrtc, NULL, 0},
    {"CRTC2", "page", displayCrtc, NULL, 0},
    {"CRTC3", "page", displayCrtc, NULL, 0},
    {"CRTC4", "page", displayCrtc, NULL, 0}};

// Node *crtcPages;

Node connectorPages[] = {
    {"Connector1", "page", displayConnector, NULL, 0},
    {"Connector2", "page", displayConnector, NULL, 0},
    {"Connector3", "page", displayConnector, NULL, 0},
    {"Connector4", "page", displayConnector, NULL, 0}};

Node encoderPages[] = {
    {"Encoder1", "page", displayEncoder, NULL, 0},
    {"Encoder2", "page", displayEncoder, NULL, 0},
    {"Encoder3", "page", displayEncoder, NULL, 0},
    {"Encoder4", "page", displayEncoder, NULL, 0}};

Node framebufferPages[] = {
    {"Framebuffer1", "page", NULL, NULL, 0},
    {"Framebuffer2", "page", NULL, NULL, 0},
    {"Framebuffer3", "page", NULL, NULL, 0},
    {"Framebuffer4", "page", NULL, NULL, 0}};

Node displayConfigMenu[] = {
    {"CRTCs", "menu", NULL, crtcPages, sizeof(crtcPages) / sizeof(Node)},
    {"Connectors", "menu", NULL, connectorPages, sizeof(connectorPages) / sizeof(Node)},
    {"Encoders", "menu", NULL, encoderPages, sizeof(encoderPages) / sizeof(Node)},
    {"Framebuffers", "menu", NULL, framebufferPages, sizeof(framebufferPages) / sizeof(Node)},
    {"Other Display Info", "page", NULL, NULL, 0}};

// Node *displayConfigMenu;

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

Node mainMenu[] = {
    {"Display Configuration", "menu", NULL, displayConfigMenu, sizeof(displayConfigMenu) / sizeof(Node)},
    {"GPU Information", "menu", NULL, gpuInfoPages, sizeof(gpuInfoPages) / sizeof(Node)},
    {"Power Management", "menu", NULL, powerManagementPages, sizeof(powerManagementPages) / sizeof(Node)},
    {"Display Capabilities", "menu", NULL, displayCapabilitiesPages, sizeof(displayCapabilitiesPages) / sizeof(Node)},
    {"Performance Tuning", "menu", NULL, performanceTuningPages, sizeof(performanceTuningPages) / sizeof(Node)},
    {"Debugging and Error Information", "menu", NULL, debuggingErrorInfoPages, sizeof(debuggingErrorInfoPages) / sizeof(Node)},
    {"Driver and Module Information", "menu", NULL, driverModuleInfoPages, sizeof(driverModuleInfoPages) / sizeof(Node)},
    {"Internal and Client Info", "menu", NULL, internalClientInfoPages, sizeof(internalClientInfoPages) / sizeof(Node)},
    {"System State", "menu", NULL, systemStatePages, sizeof(systemStatePages) / sizeof(Node)}};

// Node *mainMenu;

size_t mainMenuSize = sizeof(mainMenu) / sizeof(Node);