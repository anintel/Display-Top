#include <ncurses.h>
#include <menu.h>

#include "node.h"
#include "display.h"

#include "data.h"

int display_random_stuff(WINDOW *win)
{
    int lines_printed = 5;
    while(lines_printed < 25){
        mvwprintw(win, lines_printed++, 1, "Random stuff %d", lines_printed);
    }
    wrefresh(win);
    return lines_printed;
}

int main()
{

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    int height, width;
    getmaxyx(stdscr, height, width);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    init_pair(5, COLOR_BLACK, COLOR_BLUE);

    int level = 0;

    populateData();
    Node mmenu;
    setString(mmenu.name, "DISPLAY TOP", sizeof(mmenu.name));
    setString(mmenu.type, "menu", sizeof(mmenu.type));
    mmenu.displayFunction = displaySummary;
    mmenu.submenu = mainMenu;
    mmenu.submenuSize = mainMenuSize;

    doublePage(stdscr, &mmenu);

    endwin();
    return 0;
}
