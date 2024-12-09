#ifndef DISPLAY_H
#define DISPLAY_H

#include "node.h"
#include "utils.h"

void displayHomePage();

void doublePage(WINDOW *win, Node* node);

void displayMenu(WINDOW *win, Node *nodes, int count);
void displayMenu2(WINDOW *win, Node *nodes, int count);

void displayPage(WINDOW *win, const char *pageName);

void scrollablePage(WINDOW *win, const char *pageName, void (*displayFunction)(WINDOW *, const char *, int *));

void displayMainMenu(WINDOW *win);

void displaySummary(WINDOW *pad, int *content_line);

const char *get_connector_type_name(uint32_t connector_type);
const char *get_encoder_type_name(uint32_t encoder_type);

// displayConfiguration pages
void displayCrtc(WINDOW *pad, const char *page_name, int *content_line);
void displayConnector(WINDOW *win, const char *page_name, int *content_line);
void displayEncoder(WINDOW *win, const char *page_name, int *content_line);
void displayFramebuffer(WINDOW *win, const char *page_name, int *content_line);
void displayPlane(WINDOW *win, const char *page_name, int *content_line);

void displayDebugfsFile(WINDOW *pad, const char *file_name, int *content_line);

#endif