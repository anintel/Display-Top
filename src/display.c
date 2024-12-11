#include "display.h"
#include "data.h"
#include "utils.h"

#define DRM_DEVICE "/dev/dri/card0"

void doublePage(WINDOW *win, Node *node)
{
    int win_height, win_width;
    getmaxyx(win, win_height, win_width);
    wclear(win);
    wbkgd(win, COLOR_PAIR(2));
    box(win, 0, 0);

    print_bold_text(win, 1, 1, node->name);
    mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
    wrefresh(win);

    bool focus_on_top = false;

    int pad_height = 100;
    int pad_width = getmaxx(win) - 2;

    int display_pad_height = win_height - 8;
    int display_pad_pos = 0;
    int highlight = 0;

    WINDOW *display_pad = NULL;
    WINDOW *menu_pad = NULL;

    int mpy = 0;
    int dpy = 0;

    if (node->displayFunction != NULL && node->submenuSize > 0)
    {
        mpy = win_height - 1 - node->submenuSize;
        dpy = 4;
    }
    else if (node->displayFunction != NULL)
    {
        focus_on_top = true;
        dpy = 4;
    }
    else if (node->submenuSize > 0)
    {
        mpy = 3;
    }
    else if (node->displayFunction == NULL && node->submenuSize == 0)
    {
        mvwprintw(win, 4, 2, "It's so Empty at %s :(", node->name);
        mvwprintw(win, 6, 2, "Press 'e' to go back.");
        wrefresh(win);

        int ch;
        while ((ch = wgetch(win)) != 'e')
        {
        }
        return;
    }

    if (node->displayFunction != NULL)
    {
        display_pad_height -= node->submenuSize;
        display_pad = newpad(pad_height, pad_width);
        int count = 100;
        node->displayFunction(display_pad, node->name, &count);
        if (count < pad_height)
        {
            pad_height = count;
        }
        prefresh(display_pad, display_pad_pos, 0, dpy, 1, 4 + display_pad_height, pad_width);
        mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);
    }

    if (node->submenuSize > 0)
    {
        menu_pad = newpad(node->submenuSize, pad_width);
        prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, pad_width);
    }

    while (1)
    {
        if (check_size_change(win, &win_height, &win_width) == 1)
        {
            wclear(win);
            box(win, 0, 0);
            print_bold_text(win, 1, 1, node->name);
            mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
            wrefresh(win);

            if (node->displayFunction != NULL && node->submenuSize > 0)
            {
                mpy = win_height - 1 - node->submenuSize;
                dpy = 4;
            }
            else if (node->displayFunction != NULL)
            {
                dpy = 4;
            }
            else if (node->submenuSize > 0)
            {
                mpy = 3;
            }

            display_pad_height = win_height - 8;
            if (node->displayFunction != NULL)
            {
                display_pad_height -= node->submenuSize;
                mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);
                prefresh(display_pad, display_pad_pos, 0, 4, 1, 4 + display_pad_height, pad_width);
            }
            if (node->submenuSize > 0)
            {
                prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, pad_width);
            }
        }

        if (display_pad != NULL)
        {
            if (display_pad_pos > 0)
                print_bold_text(win, 3, 1, "...");
            else
                mvwprintw(win, 3, 1, "   ");

            if (display_pad_pos < pad_height - display_pad_height)
                print_bold_text(win, dpy + display_pad_height + 1, 1, "...");
            else
                mvwprintw(win, dpy + display_pad_height + 1, 1, "   ");
        }
        if (node->submenuSize > 0 && node->displayFunction != NULL)
        {
            if (focus_on_top)
            {
                print_bold_text(win, dpy, win_width - 6, "***");
            }
            else
            {
                print_bold_text(win, mpy, win_width - 6, "***");
            }
        }

        if (node->submenuSize > 0)
        {
            for (int i = 0; i < node->submenuSize; ++i)
            {
                if (highlight == i && !focus_on_top)
                {
                    wattron(menu_pad, A_REVERSE);
                }
                mvwprintw(menu_pad, i, 1, node->submenu[i].name);
                wattroff(menu_pad, A_REVERSE);
            }
            prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, pad_width);
        }

        if (node->displayFunction != NULL)
        {
            prefresh(display_pad, display_pad_pos, 0, 4, 1, 4 + display_pad_height, pad_width);
        }

        int ch = wgetch(win);
        switch (ch)
        {
        case KEY_UP:
            if (focus_on_top && display_pad_pos > 0)
            {
                display_pad_pos--;
            }
            else if (!focus_on_top)
            {
                highlight = (highlight == 0) ? node->submenuSize - 1 : highlight - 1;
            }
            break;

        case KEY_DOWN:
            if (focus_on_top && display_pad_pos < pad_height - display_pad_height)
            {
                display_pad_pos++;
            }
            else if (!focus_on_top)
            {
                highlight = (highlight == node->submenuSize - 1) ? 0 : highlight + 1;
            }
            break;
        case 'e':
            return;
        case '\n':
            if (!focus_on_top)
            {
                doublePage(win, &node->submenu[highlight]);

                wclear(win);
                box(win, 0, 0);
                print_bold_text(win, 1, 1, node->name);
                mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
                if (node->displayFunction != NULL)
                {
                    mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);
                }
                wrefresh(win);
            }
            break;

        case '\t':
            if (node->submenuSize > 0)
            {
                focus_on_top = !focus_on_top;
            }
            break;
        }
    }

    if (display_pad != NULL)
    {
        delwin(display_pad);
    }
    if (menu_pad != NULL)
    {
        delwin(menu_pad);
    }
}
