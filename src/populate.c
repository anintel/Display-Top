#include "populate.h"

#define DRM_DEVICE "/dev/dri/card0"

void initializeDPCD()
{
    Node *DPCD = createNode("DPCD Registers", NULL, root);
    if (!DPCD)
    {
        perror("malloc");
        return;
    }

    addChild(root, DPCD);
}

void populateData()
{
    root = createNode("Display Top", displaySummary, NULL);

    initializeDisplayConfig();
    initializeDisplayDebugfs();
    initializeDPCD();

    addChild(root, createNode("MMIO Registers", NULL, root));
    // initializeDPCD();
}
