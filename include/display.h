#ifndef DISPLAY_H
#define DISPLAY_H

#include "node.h"

void displayMenu(WINDOW *win, Node *nodes, int count);

void displayPage(WINDOW *win, const char *pageName);

void displayMainMenu(WINDOW *win);

void displaySummary(WINDOW *win);

int getCrtcCount();
void updateCrtcList(Node **nodes, int *count);

// displayConfiguration pages
void displayCrtc(WINDOW *win, const char *page_name);
void displayConnector(WINDOW *win, const char *page_name);
void displayEncoder(WINDOW *win, const char *page_name);

#endif