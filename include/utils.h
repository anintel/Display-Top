#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>

// Utility function declarations
void print_bold_text(WINDOW *win, int line, int col, const char *text);
void print_dim_text(WINDOW *win, int line, int col, const char *text);
void print_red_text(WINDOW *win, int line, int col, const char *text);
void print_green_text(WINDOW *win, int line, int col, const char *text);

#endif // UTILS_H