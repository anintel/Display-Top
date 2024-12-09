#include "display.h"
#include "data.h"
#include "utils.h"

#define DRM_DEVICE "/dev/dri/card0"

void displayHomePage()
{

    WINDOW *summary = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), 0, 0);
    keypad(summary, TRUE);
    scrollok(summary, TRUE);
    wrefresh(summary);

    WINDOW *menu = newwin(getmaxy(stdscr) / 2, getmaxx(stdscr), getmaxy(stdscr) / 2, 0);
    keypad(menu, TRUE);
    mvwprintw(menu, 1, 1, "MAIN MENU");
    wrefresh(menu);

    int highlight = 0;
    int ch;

    while (1)
    {
        // displaySummary(summary);

        wclear(menu);
        wresize(menu, getmaxy(stdscr) / 2, getmaxx(stdscr));
        mvwin(menu, getmaxy(stdscr) / 2, 0);
        box(menu, 0, 0);

        print_bold_text(menu, 1, 1, "MAIN MENU");

        for (int i = 0; i < mainMenuSize; ++i)
        {
            if (highlight == i)
            {
                wattron(menu, A_REVERSE);
            }
            mvwprintw(menu, i + 3, 1, mainMenu[i].name);
            wattroff(menu, A_REVERSE);
        }
        mvwprintw(menu, mainMenuSize + 2, 1, "Use Arrow Keys to Navigate, Enter to Select, 'e' to Exit");
        wrefresh(menu);

        ch = wgetch(menu);
        switch (ch)
        {
        case KEY_UP:
            highlight = (highlight == 0) ? mainMenuSize - 1 : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight == mainMenuSize - 1) ? 0 : highlight + 1;
            break;
        case 'e':
            return;
        case '\n':
            if (strcmp(mainMenu[highlight].type, "menu") == 0)
            {
                if (mainMenu[highlight].submenuSize == 0)
                {
                    displayPage(menu, mainMenu[highlight].name);
                }
                else
                {
                    displayMenu(menu, mainMenu[highlight].submenu, mainMenu[highlight].submenuSize);
                }
            }
            else if (strcmp(mainMenu[highlight].type, "page") == 0)
            {
                if (mainMenu[highlight].displayFunction != NULL)
                {
                    mainMenu[highlight].displayFunction(menu, mainMenu[highlight].name);
                }
                else
                {
                    displayPage(menu, mainMenu[highlight].name);
                }
            }
            break;
        }
    }

    delwin(menu);
    delwin(summary);
    return;
}

void displayMenu(WINDOW *win, Node *nodes, int count)
{

    wresize(win, getmaxy(stdscr), getmaxx(stdscr));
    mvwin(win, 0, 0);

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
        mvwprintw(win, count + 2, 1, "%d Use Arrow Keys to Navigate, Enter to Select, 'e' to Exit", count);
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
                if (nodes[highlight].submenuSize == 0)
                {
                    displayPage(win, nodes[highlight].name);
                }
                else
                {
                    displayMenu(win, nodes[highlight].submenu, nodes[highlight].submenuSize);
                }
            }
            else if (strcmp(nodes[highlight].type, "page") == 0)
            {
                if (nodes[highlight].displayFunction != NULL)
                {
                    void (*displayFunc)(WINDOW *, const char *, int *) = nodes[highlight].displayFunction;
                    scrollablePage(win, nodes[highlight].name, displayFunc);
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

#include "menu.h"

void displayMenu2(WINDOW *win, Node *nodes, int count)
{

    wbkgd(win, COLOR_PAIR(3)); // Set background color
    wresize(win, getmaxy(stdscr), getmaxx(stdscr));
    mvwin(win, 0, 0);
    wclear(win);
    box(win, 0, 0);

    ITEM **items = (ITEM **)calloc(count + 1, sizeof(ITEM *));

    for (int i = 0; i < count; ++i)
    {
        items[i] = new_item(nodes[i].name, nodes[i].name);
    }
    items[count] = NULL;

    MENU *menu = new_menu((ITEM **)items);
    set_menu_win(menu, win);
    set_menu_sub(menu, derwin(win, count + 2, getmaxx(win) - 2, 1, 1));
    set_menu_mark(menu, " * ");

    post_menu(menu);
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e')
    {
        wclear(win);
        box(win, 0, 0);
        post_menu(menu);
        wrefresh(win);

        switch (ch)
        {
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case '\n':
        {
            ITEM *cur = current_item(menu);
            int index = item_index(cur);
            if (strcmp(nodes[index].type, "menu") == 0)
            {
                if (nodes[index].submenuSize == 0)
                {
                    displayPage(win, nodes[index].name);
                }
                else
                {
                    displayMenu2(win, nodes[index].submenu, nodes[index].submenuSize);
                }
            }
            else if (strcmp(nodes[index].type, "page") == 0)
            {
                if (nodes[index].displayFunction != NULL)
                {
                    nodes[index].displayFunction(win, nodes[index].name);
                }
                else
                {
                    displayPage(win, nodes[index].name);
                }
            }
            break;
        }
        }
        wrefresh(win);
    }

    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < count; ++i)
    {
        free_item(items[i]);
    }
    free(items);
}

void displayMenu3(WINDOW *win, Node *node)
{

    int highlight = 0;
    int ch, line, start;

    wresize(win, getmaxy(stdscr), getmaxx(stdscr));
    mvwin(win, 0, 0);
    wclear(win);
    box(win, 0, 0);

    line = node->displayFunction(win) + 1;
    mvhline(line++, 1, ACS_HLINE, getmaxx(win) - 2);
    start = line;

    while (1)
    {
        start = line;

        mvwprintw(win, start++, 1, "Submenu size: %d", node->submenuSize);

        // Display submenu
        for (int i = 0; i < node->submenuSize; ++i)
        {
            if (highlight == i)
            {
                wattron(win, A_REVERSE);
            }
            mvwprintw(win, start + i + 1, 1, node->submenu[i].name);
            wattroff(win, A_REVERSE);
        }
        mvwprintw(win, start + 15, 1, "Use Arrow Keys to Navigate, Enter to Select, 'e' to Exit");
        wrefresh(win);

        ch = wgetch(win);
        switch (ch)
        {
        case KEY_UP:
            highlight = (highlight == 0) ? node->submenuSize - 1 : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight == node->submenuSize - 1) ? 0 : highlight + 1;
            break;
        case 'e':
            return;
        case '\n':
            if (strcmp(node->submenu[highlight].type, "menu") == 0)
            {
                if (node->submenu[highlight].submenuSize == 0)
                {
                    displayPage(win, node->submenu[highlight].name);
                }
                else
                {
                    displayMenu(win, node->submenu[highlight].submenu, node->submenu[highlight].submenuSize);
                }
            }
            else if (strcmp(node->submenu[highlight].type, "page") == 0)
            {
                if (node->submenu[highlight].displayFunction != NULL)
                {
                    node->submenu[highlight].displayFunction(win, node->submenu[highlight].name);
                }
                else
                {
                    displayPage(win, node->submenu[highlight].name);
                    wresize(win, getmaxy(stdscr), getmaxx(stdscr));
                    mvwin(win, 0, 0);
                    wclear(win);
                    box(win, 0, 0);

                    line = node->displayFunction(win) + 1;
                    mvhline(line++, 1, ACS_HLINE, getmaxx(win) - 2);
                    start = line;
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
    mvwprintw(win, 1, 1, "Empty Page: %s", pageName);
    mvwprintw(win, 3, 1, "Press 'e' to go back.");
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != 'e')
    {
    }
}

void scrollablePage(WINDOW *win, const char *pageName, void (*displayFunction)(WINDOW *, const char *, int *))
{

    int height, width;
    getmaxyx(win, height, width);

    int pad_height = 100;
    int pad_width = getmaxx(win) - 2;
    int pad_win_height = getmaxy(win) - 3;
    int pad_win_width = getmaxx(win) - 2;
    int pad_y = 4, pad_x = 1;

    wclear(win);
    box(win, 0, 0);
    // switch to color pair 2
    wbkgd(win, COLOR_PAIR(2));
    print_bold_text(win, 1, 1, pageName);
    mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);

    WINDOW *pad = newpad(pad_height, pad_width);
    scrollok(pad, TRUE);

    int line = 0; // Use a normal integer, no need for pointer
    displayFunction(pad, pageName, &line);
    if (line < pad_height)
    {
        pad_height = line;
    }

    prefresh(pad, 0, 0, pad_y, pad_x, pad_win_height, pad_win_width);

    int ch;
    int pad_pos = 0;
    do
    {
        if (check_size_change(win, &height, &width) == 1)
        {
            wclear(win);
            box(win, 0, 0);
            print_bold_text(win, 1, 1, pageName);
            mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
            wrefresh(win);
        }

        pad_win_height = getmaxy(win) - 3;
        pad_win_width = getmaxx(win) - 2;

        if (pad_pos > 0)
            print_bold_text(win, 3, 1, "...");
        else
            mvwprintw(win, 3, 1, "   ");

        if (pad_pos < pad_height - (pad_win_height))
            print_bold_text(win, getmaxy(win) - 2, 1, "...");
        else
            mvwprintw(win, getmaxy(win) - 2, 1, "   ");

        switch (ch)
        {
        case KEY_UP:
            if (pad_pos > 0)
                pad_pos--;
            break;
        case KEY_DOWN:
            // todo: check why this 3 is needed
            if (pad_pos < pad_height - (pad_win_height) + 3)
                pad_pos++;
            break;
        }
        wrefresh(win);
        prefresh(pad, pad_pos, 0, pad_y, pad_x, pad_win_height, pad_win_width);
    } while ((ch = wgetch(win)) != 'e');

    delwin(pad);
    wbkgd(win, COLOR_PAIR(1));
}

void displayMainMenu(WINDOW *win)
{
    displayMenu(win, mainMenu, mainMenuSize);
}

void doublePage(WINDOW *win, Node *node)
{
    int height, width;
    getmaxyx(win, height, width);

    bool focus_on_top = false;

    int pad_height = 100;

    int menu_pad_height = node->submenuSize;
    int pad_width = getmaxx(win) - 2;

    int display_pad_height = height - menu_pad_height - 8;

    int dpy = 4;
    int mpy = height - 1 - menu_pad_height;

    wclear(win);
    wbkgd(win, COLOR_PAIR(2));
    box(win, 0, 0);

    print_bold_text(win, 1, 1, node->name);
    mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
    wrefresh(win);

    WINDOW *display_pad = newpad(pad_height, pad_width);
    WINDOW *menu_pad = newpad(menu_pad_height, pad_width);

    wclear(display_pad);

    int count = 100;
    node->displayFunction(display_pad, &count);
    if (count < pad_height)
    {
        pad_height = count;
    }

    wclear(menu_pad);

    mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);

    prefresh(display_pad, 0, 0, dpy, 1, dpy + display_pad_height, pad_width);
    prefresh(menu_pad, 0, 0, mpy, 1, mpy + menu_pad_height + 1, pad_width);

    int ch;
    int display_pad_pos = 0;
    int highlight = 0;

    while (1)
    {
        if (check_size_change(win, &height, &width) == 1)
        {
            wclear(win);
            box(win, 0, 0);
            print_bold_text(win, 1, 1, node->name);
            mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
            wrefresh(win);

            display_pad_height = height - menu_pad_height - 8;
            mpy = height - 1 - menu_pad_height;
            mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);
            prefresh(display_pad, display_pad_pos, 0, dpy, 1, dpy + display_pad_height, pad_width);
            prefresh(menu_pad, 0, 0, mpy, 1, mpy + menu_pad_height + 1, pad_width);
        }

        if (display_pad_pos > 0)
            print_bold_text(win, 3, 1, "...");
        else
            mvwprintw(win, 3, 1, "   ");

        if (display_pad_pos < pad_height - (display_pad_height))
            print_bold_text(win, mpy - 2, 1, "...");
        else
            mvwprintw(win, mpy - 2, 1, "   ");

        if (focus_on_top)
        {

            print_bold_text(win, dpy, width - 6, "***");
        }
        else
        {
            print_bold_text(win, mpy, width - 6, "***");
        }

        for (int i = 0; i < node->submenuSize; ++i)
        {
            if (highlight == i && !focus_on_top)
            {
                wattron(menu_pad, A_REVERSE);
            }
            mvwprintw(menu_pad, i, 1, node->submenu[i].name);
            wattroff(menu_pad, A_REVERSE);
        }

        prefresh(menu_pad, 0, 0, mpy, 1, mpy + menu_pad_height + 1, pad_width);
        prefresh(display_pad, display_pad_pos, 0, dpy, 1, dpy + display_pad_height, pad_width);

        ch = wgetch(win);
        switch (ch)
        {
        case KEY_UP:
            if (focus_on_top)
            {
                if (display_pad_pos > 0)
                    display_pad_pos--;
            }
            else
            {
                highlight = (highlight == 0) ? node->submenuSize - 1 : highlight - 1;
            }
            break;

        case KEY_DOWN:
            if (focus_on_top)
            {
                if (display_pad_pos < pad_height - (display_pad_height))
                    display_pad_pos++;
            }
            else
            {
                highlight = (highlight == node->submenuSize - 1) ? 0 : highlight + 1;
            }
            break;
        case 'e':
            return;
        case '\n':
            if (!focus_on_top)
            {
                if (strcmp(node->submenu[highlight].type, "menu") == 0)
                {
                    if (node->submenu[highlight].submenuSize == 0)
                    {
                        displayPage(win, node->submenu[highlight].name);
                    }
                    else
                    {
                        displayMenu(win, node->submenu[highlight].submenu, node->submenu[highlight].submenuSize);
                    }
                }
                else if (strcmp(node->submenu[highlight].type, "page") == 0)
                {
                    if (node->submenu[highlight].displayFunction != NULL)
                    {
                        void (*displayFunc)(WINDOW *, const char *, int *) = node->submenu[highlight].displayFunction;
                        scrollablePage(win, node->submenu[highlight].name, displayFunc);
                    }
                    else
                    {
                        displayPage(win, node->submenu[highlight].name);
                    }
                }

                wclear(win);
                box(win, 0, 0);
                print_bold_text(win, 1, 1, node->name);
                mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
                mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);
                wrefresh(win);
            }
            break;

        case '\t':
            focus_on_top = !focus_on_top;
            break;
        }
    }
}
