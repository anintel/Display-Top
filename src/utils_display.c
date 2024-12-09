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

void setString(char *dest, const char *src, size_t size)
{
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

int check_size_change(WINDOW *win, int *height, int *width)
{
    int new_height, new_width;
    getmaxyx(win, new_height, new_width);
    if (new_height != *height || new_width != *width)
    {
        *height = new_height;
        *width = new_width;
        return 1;
    }
    return 0;
}
