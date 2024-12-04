#include <ncurses.h>
#include <menu.h>

#include "node.h"
#include "display.h"

#include "data.h"

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
    init_pair(4, COLOR_WHITE, COLOR_GREEN);
    // bkgd(COLOR_PAIR());

    int level = 0;

    // WINDOW *summary = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), 0, 0);
    // keypad(summary, TRUE);
    // scrollok(summary, TRUE);
    // wrefresh(summary);

    // WINDOW *menu = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), getmaxy(stdscr) / 2, 0);
    // keypad(menu, TRUE);
    // mvwprintw(menu, 1, 1, "MAIN MENU");
    // wrefresh(menu);

    // populateData();

    // displayHomePage();


    scrollablePage(stdscr, "i915_capabilities", displayDebugfsFile);

    // ScrollablePage(stdscr, "Scrollable Page");


    

    endwin();
    return 0;
}