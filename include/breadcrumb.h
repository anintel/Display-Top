#ifndef BREADCRUMB_H
#define BREADCRUMB_H

#include <ncurses.h>

WINDOW *init_breadcumb();
void update_breadcrumb(WINDOW *win, const char **path, int level);
// int handle_breadcrumb_click(const char **path, int depth, MEVENT *event);

#endif
