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
    init_pair(4, COLOR_BLACK, COLOR_GREEN);
    init_pair(5, COLOR_BLACK, COLOR_RED);

    int level = 0;

    populateData();
    // doublePage(stdscr, root);
    displayWin(stdscr, root);

    // Assuming root is the root node of your tree
    // traverseAndPrint(root, 0);
    // wgetch(stdscr);
    endwin();

    // freeTree(root);
    return 0;
}

