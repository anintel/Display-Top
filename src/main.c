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

    int level = 0;

    // uncomment these two functions for the app

    populateData();
    displayHomePage();

    // testing stuff

    // scrollablePage(stdscr, "i915_capabilities", displayDebugfsFile);

    endwin();
    return 0;
}