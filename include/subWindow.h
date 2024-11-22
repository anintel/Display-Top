#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <ncurses.h>

WINDOW *createSubWindow(char *title, char **items, int itemCount, int *level);

#endif
