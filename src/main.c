#include <ncurses.h>
#include "node.h"
#include "display.h"

#include "data.h"

int main()
{

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int height, width;
    getmaxyx(stdscr, height, width);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));

    int level = 0;

    WINDOW *summary = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), 0, 0);
    keypad(summary, TRUE);
    scrollok(summary, TRUE);
    wrefresh(summary);

    WINDOW *menu = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), getmaxy(stdscr) / 2, 0);
    keypad(menu, TRUE);
    mvwprintw(menu, 1, 1, "MAIN MENU");
    wrefresh(menu);

    populateData();

    displaySummary(summary);
    displayMainMenu(menu);

    delwin(menu);
    delwin(summary);
    endwin();
    return 0;
}