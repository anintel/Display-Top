#ifndef DISPLAY_H
#define DISPLAY_H

#include "node.h"
#include "utils.h"


void displayWin(WINDOW *win, Node *node);

void doublePage(WINDOW *win, Node *node);
void displaySummary(WINDOW *pad, const char *name, int *content_line);


// displayConfiguration pages
void displayCrtc(WINDOW *pad, const char *page_name, int *content_line);
void displayConnector(WINDOW *win, const char *page_name, int *content_line);
void displayEncoder(WINDOW *win, const char *page_name, int *content_line);
void displayFramebuffer(WINDOW *win, const char *page_name, int *content_line);
void displayPlane(WINDOW *win, Node *node, int *content_line);

void displayInformats(WINDOW *pad, Node* node, int *content_line);
void displayFormats(WINDOW *pad, Node *node, int *content_line);
void gotoCrtc(WINDOW *pad, const char *page_name, int *content_line);

void displayDebugfsFile(WINDOW *pad, const char *file_name, int *content_line);


#endif