#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "menu.h"

WINDOW* createSubWindow(char *title, char **items, int itemCount, int *level) {
    *level += 1;
    int height = getmaxy(stdscr);
    int width = getmaxx(stdscr);
    WINDOW *win = newwin(height - 3, width, 3, 0);
    box(win, 0, 0);
    mvwprintw(win, 1, 3, "This is the subwindow %s & level - %d", title, *level);

    // Display submenu items
    for (int i = 0; i < itemCount; i++) {
        mvwprintw(win, i + 3, 3, "%d. %s", i + 1, items[i]);
    }
    mvwprintw(win, height - 5, 2, "Press 'e' to exit %s", title);
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e') {
        // Wait for the user to press 'e' to exit
    }

    // Clear the window and refresh the screen
    wclear(win);
    wrefresh(win);
    delwin(win);

    *level -= 1;
    return NULL;
}
