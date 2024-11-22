#include <ncurses.h>
#include "menu.h"
#include "summary.h"
#include "breadcrumb.h"

int main() {
    WINDOW *summary, *menu;

    initscr();
    start_color(); 
    cbreak();
    noecho();    
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLACK); 
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));

    int level = 0;
    char **path;
    summary = display_summary(level);

    
    wrefresh(summary);

    const char *choices[] = {
        "Display Configuration",
        "GPU Information",
        "Power Management",
        "Display Capabilities",
        "Performance Tuning",
        "Debugging and Error Information",
        "Driver and Module Information",
        "Internal and Client Info",
        "System State",
        "Read Registers",
        "Get MMIO",
        "Trigger a Sequence"
    };

    int n_choices = sizeof(choices) / sizeof(choices[0]);

    WINDOW *win = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), getmaxy(stdscr) / 2, 0);
    box(win, 0, 0);
    keypad(win, TRUE);
    mvwprintw(win, 1, 1, "USER MENU");
    wrefresh(win);

    CreateMenu(win, choices, n_choices);

    endwin();
    return 0;
}
