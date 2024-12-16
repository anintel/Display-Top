#include "populate.h"

void initializeDisplayDebugfs()
{
    displayDebugfs = createNode("Display Debugfs", NULL, root);

    addChild(displayDebugfs, createNode("GPU Information", NULL, displayDebugfs));
    addChild(displayDebugfs, createNode("Power Management", NULL, displayDebugfs));
    addChild(displayDebugfs, createNode("Display Capabilities", NULL, displayDebugfs));
    addChild(displayDebugfs, createNode("Performance Tuning", NULL, displayDebugfs));
    addChild(displayDebugfs, createNode("Debugging and Error Info", NULL, displayDebugfs));
    addChild(displayDebugfs, createNode("Driver and Module Info", NULL, displayDebugfs));
    addChild(displayDebugfs, createNode("Internal and Client Info", NULL, displayDebugfs));
    addChild(displayDebugfs, createNode("System State", NULL, displayDebugfs));

    addChild(root, displayDebugfs);
}
