#ifndef DATA_H
#define DATA_H

#include "node.h"

// extern Node *crtcPages;
// extern Node *displayConfigMenu; 
// extern Node *mainMenu;

// extern Node crtcPages[];
extern Node connectorPages[];
extern Node encoderPages[];
extern Node framebufferPages[];

extern Node displayConfigPages[];
extern Node gpuInfoPages[];
extern Node powerManagementPages[];
extern Node displayConfigMenu[];
extern Node gpuInfoMenu[];
extern Node powerManagementMenu[];
extern Node mainMenu[];

extern size_t mainMenuSize;
void initializeCrtcPages();

#endif 