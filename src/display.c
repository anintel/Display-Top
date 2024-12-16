#include "display.h"
#include "data.h"
#include "utils.h"

#define DRM_DEVICE "/dev/dri/card0"

void displayWin(WINDOW *win, Node *node)
{
    Node *head = node;
    int highlight = 0;
    int running;
    bool focus_on_top = false;
    bool start = true;

    int win_height, win_width;

    int pad_height;
    int common_pad_width;
    int display_pad_height;
    int menu_pad_height;
    int display_pad_pos;

    int mpy;
    int dpy;
    int count = 100;

    WINDOW *display_pad = NULL;
    WINDOW *menu_pad = NULL;

    int ch;

    while (1)
    {
        wclear(win);
        wbkgd(win, COLOR_PAIR(2));
        box(win, 0, 0);
        move(1, 2);
        printPath(head);
        mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);

        mpy = 4;
        dpy = 4;
        running = 1;
        pad_height = 100;
        display_pad_pos = 0;

        if (head->displayFunction == NULL && head->childrenSize == 0)
        {
            mvwprintw(win, 4, 2, "It's so Empty at %s :(", head->name);
            mvwprintw(win, 8, 2, "Press 'e' to go back.");
            wrefresh(win);
        }

        getmaxyx(win, win_height, win_width);
        common_pad_width = win_width - 2;

        while (running)
        {
            if (check_size_change(win, &win_height, &win_width) == 1 || start)
            {
                start = false;
                wclear(win);
                box(win, 0, 0);
                move(1, 2);
                wattron(win, A_BOLD);
                printPath(head);
                wattroff(win, A_BOLD);
                mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
                wrefresh(win);

                // when we have both display and menu
                if (head->displayFunction != NULL && head->childrenSize > 0)
                {
                    menu_pad_height = head->childrenSize + 2;
                    mpy = win_height - 1 - head->childrenSize;
                }

                if (head->displayFunction != NULL)
                {
                    focus_on_top = true;

                    // set the height to take as much as it can
                    display_pad_height = win_height - 6;
                    display_pad_height -= menu_pad_height;
                    if (display_pad != NULL)
                    {
                        delwin(display_pad);
                        display_pad = NULL;
                    }
                    display_pad = newpad(pad_height, common_pad_width);
                    count = pad_height;

                    // add content to the display pad
                    head->displayFunction(display_pad, head->name, &count);

                    // adjust display pad size to fit the content only
                    if (count < pad_height)
                        pad_height = count;
                    if (count < display_pad_height)
                    {
                        display_pad_height = pad_height + 1;
                        mpy = dpy + display_pad_height + 3;
                    }

                    // update & refresh the pad
                    prefresh(display_pad, display_pad_pos, 0, dpy, 1, 4 + display_pad_height, common_pad_width);
                }

                if (head->childrenSize > 0)
                {
                    menu_pad_height = head->childrenSize;
                    focus_on_top = false;
                    if (menu_pad != NULL)
                    {
                        delwin(menu_pad);
                        menu_pad = NULL;
                    }

                    if (head->displayFunction != NULL)
                        mvwhline(win, mpy - 1, 1, ACS_HLINE, common_pad_width);

                    menu_pad = newpad(head->childrenSize, common_pad_width);
                    prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, common_pad_width);
                }
            }

            if (head->displayFunction != NULL)
            {
                if (display_pad_pos > 0)
                    print_bold_text(win, 3, 1, "...");
                else
                    mvwprintw(win, 3, 1, "   ");

                if (display_pad_pos < pad_height - display_pad_height)
                    print_bold_text(win, dpy + display_pad_height + 1, 1, "...");
                else
                    mvwprintw(win, dpy + display_pad_height + 1, 1, "   ");

                prefresh(display_pad, display_pad_pos, 0, 4, 1, 4 + display_pad_height, common_pad_width);
            }

            if (head->childrenSize > 0 && head->displayFunction != NULL)
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

            if (head->childrenSize > 0)
            {
                for (int i = 0; i < head->childrenSize; ++i)
                {
                    if (highlight == i && !focus_on_top)
                    {
                        wattron(menu_pad, A_REVERSE);
                    }
                    mvwprintw(menu_pad, i, 1, head->children[i].name);
                    wattroff(menu_pad, A_REVERSE);
                }
                prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, common_pad_width);
            }

            ch = wgetch(win);
            switch (ch)
            {
            case KEY_UP:
                if (focus_on_top && display_pad_pos > 0)
                {
                    display_pad_pos--;
                }
                else if (!focus_on_top)
                {
                    highlight = (highlight == 0) ? head->childrenSize - 1 : highlight - 1;
                }
                break;

            case KEY_DOWN:
                if (focus_on_top && display_pad_pos < pad_height - display_pad_height)
                {
                    display_pad_pos++;
                }
                else if (!focus_on_top)
                {
                    highlight = (highlight == head->childrenSize - 1) ? 0 : highlight + 1;
                }
                break;
            case 'e':
                if (head != NULL && head->parent != NULL)
                {
                    head = head->parent;
                    focus_on_top = false;
                    highlight = 0;
                    display_pad_pos = 0;
                    running = 0;
                }
                else
                {
                    return;
                }
                break;

            case '\n':
                if (!focus_on_top && head != NULL && head->childrenSize > 0)
                {
                    if (highlight >= 0 && highlight < head->childrenSize)
                    {
                        head = &head->children[highlight];
                        focus_on_top = false;
                        highlight = 0;
                        display_pad_pos = 0;
                        running = 0;
                        start = true;
                    }
                    else
                    {
                        printf("Invalid highlight index: %d\n", highlight); // Debug information
                    }
                }
                break;

            case '\t':
                if (head->childrenSize > 0)
                {
                    focus_on_top = !focus_on_top;
                }
                break;
            }
        }

        start = true;

        if (display_pad != NULL)
        {
            delwin(display_pad);
            display_pad = NULL;
        }
        if (menu_pad != NULL)
        {
            delwin(menu_pad);
            menu_pad = NULL;
        }
    }
}