#include "utils.h"

void print_bold_text(WINDOW *win, int line, int col, const char *text)
{
    if (line >= 0 && col >= 0)
    {
        wattron(win, A_BOLD);
        mvwprintw(win, line, col, "%s", text);
        wattroff(win, A_BOLD);
    }
    else
    {
        wattron(win, A_BOLD);
        wprintw(win, "%s", text);
        wattroff(win, A_BOLD);
    }
}

void print_dim_text(WINDOW *win, int line, int col, const char *text)
{
    if (line >= 0 && col >= 0)
    {
        wattron(win, A_DIM);
        mvwprintw(win, line, col, "%s", text);
        wattroff(win, A_DIM);
    }
    else
    {
        wattron(win, A_DIM);
        wprintw(win, "%s", text);
        wattroff(win, A_DIM);
    }
}

void print_red_text(WINDOW *win, int line, int col, const char *text)
{
    if (line >= 0 && col >= 0)
    {
        wattron(win, COLOR_PAIR(3));
        mvwprintw(win, line, col, "%s", text);
        wattroff(win, COLOR_PAIR(3));
    }
    else
    {
        wattron(win, COLOR_PAIR(3));
        wprintw(win, "%s", text);
        wattroff(win, COLOR_PAIR(3));
    }
}

void print_green_text(WINDOW *win, int line, int col, const char *text)
{
    if (line >= 0 && col >= 0)
    {
        wattron(win, COLOR_PAIR(2));
        mvwprintw(win, line, col, "%s", text);
        wattroff(win, COLOR_PAIR(2));
    }
    else
    {
        wattron(win, COLOR_PAIR(2));
        wprintw(win, "%s", text);
        wattroff(win, COLOR_PAIR(2));
    }
}