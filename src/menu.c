#include <ncurses.h>
#include <string.h>
#include "menu.h"
#include "summary.h"
#include "subWindow.h"
#include "breadcrumb.h"

void menuHiglight(WINDOW *win, char *content, int index, int highlight);

void CreateMainMenu(WINDOW *menu, WINDOW *summary, char *options[], int n_options)
{
    int exitpos, highlight = 0, choice = 0, level = 0;
    exitpos = getmaxy(menu) - 2;

    while (1)
    {
        box(menu, 0, 0);
        PrintSummary(summary);
        for (int i = 0; i < n_options; ++i)
            menuHiglight(menu, options[i], i + 3, highlight + 3);

        menuHiglight(menu, "EXIT or PRESS E", exitpos, highlight);

        wrefresh(menu);

        choice = wgetch(menu);
        switch (choice)
        {
        case KEY_UP:
            if (highlight == exitpos)
                highlight = n_options - 1;
            else if (highlight == 0)
                highlight = exitpos;
            else if (highlight > 0)
                --highlight;
            break;
        case KEY_DOWN:
            if (highlight < n_options - 1)
                ++highlight;
            else if (highlight == n_options - 1)
                highlight = exitpos;
            break;
        case 'e':
            delwin(menu);
            return;
        case '\n':
            if (highlight == exitpos)
            {
                level -= 1;
                delwin(menu);
                return;
            }
            else
            {
                CreateSubMenu(options[highlight]);
            }
            break;
        }
    }
}

void CreateMenu(WINDOW *win, char *options[], int n_options, char *title)
{

    int exitpos, highlight = 0, choice = 0, level = 0;
    exitpos = getmaxy(win) - 2;

    while (1)
    {

        mvwprintw(win, 1, 1, "%s", title);

        for (int i = 0; i < n_options; ++i)
            menuHiglight(win, options[i], i + 3, highlight + 3);

        menuHiglight(win, "EXIT or PRESS E", exitpos, highlight);

        wrefresh(win);

        choice = wgetch(win);
        switch (choice)
        {
        case KEY_UP:
            if (highlight == exitpos)
                highlight = n_options - 1;
            else if (highlight == 0)
                highlight = exitpos;
            else if (highlight > 0)
                --highlight;
            break;
        case KEY_DOWN:
            if (highlight < n_options - 1)
                ++highlight;
            else if (highlight == n_options - 1)
                highlight = exitpos;
            break;
        case 'e':
            delwin(win);
            return;
        case '\n':
            if (highlight == exitpos)
            {
                level -= 1;
                delwin(win);
                return;
            }
            else
            {
                CreateSubMenu(options[highlight]);
            }
            break;
        }
    }
}

void CreateSubMenu(char *option)
{
    // clear();
    const char *choices[] = {
        "Sub Menu 1",
        "Sub Menu 2",
        "Sub Menu 3",
        "Sub Menu 4",
        "Sub Menu 5",
    };

    int n_choices = sizeof(choices) / sizeof(choices[0]);

    WINDOW *win = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
    box(win, 0, 0);
    keypad(win, TRUE);
    wrefresh(win);

    CreateMenu(win, choices, n_choices, option);
}

// Function that allows you to highlight text for key movements
void menuHiglight(WINDOW *win, char *content, int index, int highlight)
{
    if (index == highlight)
    {
        wattron(win, A_REVERSE);
        mvwprintw(win, index, 2, "%s", content);
        wattroff(win, A_REVERSE);
    }
    else
    {
        mvwprintw(win, index, 2, "%s", content);
    }
}
