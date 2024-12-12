#include "populate.h"

#define DRM_DEVICE "/dev/dri/card0"

void populateData()
{
    addRootChildren();
}


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
    // initializeDPCD();
}
