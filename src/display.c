#include "display.h"
#include "data.h"

#define DRM_DEVICE "/dev/dri/card0"

void displayMenu(WINDOW *win, Node *nodes, int count)
{
    int highlight = 0;
    int ch;

    while (1)
    {
        wclear(win);
        box(win, 0, 0);
        for (int i = 0; i < count; ++i)
        {
            if (highlight == i)
            {
                wattron(win, A_REVERSE);
            }
            mvwprintw(win, i + 1, 1, nodes[i].name);
            wattroff(win, A_REVERSE);
        }
        mvwprintw(win, count + 2, 1, "Use Arrow Keys to Navigate, Enter to Select, 'e' to Exit");
        wrefresh(win);

        ch = wgetch(win);
        switch (ch)
        {
        case KEY_UP:
            highlight = (highlight == 0) ? count - 1 : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight == count - 1) ? 0 : highlight + 1;
            break;
        case 'e':
            return;
        case '\n':
            if (strcmp(nodes[highlight].type, "menu") == 0)
            {
                displayMenu(win, nodes[highlight].submenu, nodes[highlight].submenuSize);
            }
            else if (strcmp(nodes[highlight].type, "page") == 0)
            {
                if (nodes[highlight].displayFunction != NULL)
                {
                    nodes[highlight].displayFunction(win, nodes[highlight].name);
                }
                else
                {
                    displayPage(win, nodes[highlight].name);
                }
            }
            break;
        }
    }
}

void displayPage(WINDOW *win, const char *pageName)
{
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Page: %s", pageName);
    mvwprintw(win, 3, 1, "Press 'e' to go back.");
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e')
    {
    }
}

void displayMainMenu(WINDOW *win)
{
    displayMenu(win, mainMenu, mainMenuSize);
}

