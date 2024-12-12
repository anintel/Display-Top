#include "display.h"
#include "data.h"
#include "utils.h"

#define DRM_DEVICE "/dev/dri/card0"

void doublePage(WINDOW *win, Node *node)
{

    if (node->displayFunction == NULL && node->childrenSize == 0)
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

    int win_height, win_width;
    bool focus_on_top = false;

    getmaxyx(win, win_height, win_width);
    wclear(win);
    wbkgd(win, COLOR_PAIR(2));
    box(win, 0, 0);

    int mpy = 0;
    int dpy = 0;
    if (node->displayFunction != NULL && node->childrenSize > 0)
    {
        mpy = win_height - 1 - node->childrenSize;
        dpy = 4;
    }
    else if (node->displayFunction != NULL)
    {
        focus_on_top = true;
        dpy = 4;
        mpy = 4;
    }
    else if (node->childrenSize > 0)
    {
        mpy = 3;
    }

    int pad_height = 100;
    int pad_width = getmaxx(win) - 2;

    int display_pad_height = win_height - 8;
    int display_pad_pos = 0;
    int highlight = 0;

    WINDOW *display_pad = NULL;
    WINDOW *menu_pad = NULL;

    move(1, 2);
    printPath(node);
    mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
    wrefresh(win);

    if (node->displayFunction != NULL)
    {
        display_pad_height -= node->childrenSize;
        display_pad = newpad(pad_height, pad_width);
        int count = pad_height;
        node->displayFunction(display_pad, node->name, &count);
        if (count < pad_height)
        {
            pad_height = count;
        }
        if (count < display_pad_height)
        {
            display_pad_height = count + 1;
            mpy = dpy + display_pad_height + 2;
        }
        prefresh(display_pad, display_pad_pos, 0, dpy, 1, 4 + display_pad_height, pad_width);
    }

    if (node->childrenSize > 0)
    {
        menu_pad = newpad(node->childrenSize, pad_width);
        prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, pad_width);
    }

    while (1)
    {
        move(1, 2);
        printPath(node);
        if (check_size_change(win, &win_height, &win_width) == 1)
        {
            wclear(win);
            box(win, 0, 0);
            mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
            wrefresh(win);

            if (node->displayFunction != NULL && node->childrenSize > 0)
            {
                mpy = win_height - 1 - node->childrenSize;
                dpy = 4;
            }
            else if (node->displayFunction != NULL)
            {
                dpy = 4;
            }
            else if (node->childrenSize > 0)
            {
                mpy = 3;
            }

            display_pad_height = win_height - 8;
            if (node->displayFunction != NULL)
            {
                display_pad_height -= node->childrenSize;

                prefresh(display_pad, display_pad_pos, 0, 4, 1, 4 + display_pad_height, pad_width);
            }
            if (node->childrenSize > 0)
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
        if (node->childrenSize > 0 && node->displayFunction != NULL)
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

        if (node->childrenSize > 0)
        {
            for (int i = 0; i < node->childrenSize; ++i)
            {
                if (highlight == i && !focus_on_top)
                {
                    wattron(menu_pad, A_REVERSE);
                }
                mvwprintw(menu_pad, i, 1, node->children[i].name);
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
                highlight = (highlight == 0) ? node->childrenSize - 1 : highlight - 1;
            }
            break;

        case KEY_DOWN:
            if (focus_on_top && display_pad_pos < pad_height - display_pad_height)
            {
                display_pad_pos++;
            }
            else if (!focus_on_top)
            {
                highlight = (highlight == node->childrenSize - 1) ? 0 : highlight + 1;
            }
            break;
        case 'e':
            return;
        case '\n':
            if (!focus_on_top)
            {
                doublePage(win, &node->children[highlight]);

                wclear(win);
                box(win, 0, 0);
                if (node->displayFunction != NULL)
                {
                }
                wrefresh(win);
            }
            break;

        case '\t':
            if (node->childrenSize > 0)
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

void displayWin(WINDOW *win, Node *node)
{
    Node *head = node;
    int highlight = 0;
    bool focus_on_top = false;

    int win_height, win_width;
    int pad_height = 100;
    int pad_width = getmaxx(win) - 2;

    getmaxyx(win, win_height, win_width);
    int display_pad_height = win_height - 8;
    int display_pad_pos = 0;

    int menu_pad_height = 0;

    WINDOW *display_pad = NULL;
    WINDOW *menu_pad = NULL;

    int mpy = 0;
    int dpy = 0;

    int ch;
    int running = 1;

    while (1)
    {
        wclear(win);
        wbkgd(win, COLOR_PAIR(2));
        box(win, 0, 0);
        move(1, 2);
        printPath(head);
        mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
        wrefresh(win);

        running = 1;
        pad_height = 100;

        if (head->displayFunction == NULL && head->childrenSize == 0)
        {
            mvwprintw(win, 4, 2, "It's so Empty at %s :(", head->name);
            mvwprintw(win, 6, 2, "childsize %d", head->childrenSize);
            mvwprintw(win, 8, 2, "Press 'e' to go back.");
            wrefresh(win);
        }
        else
        {
            getmaxyx(win, win_height, win_width);

            if (head->displayFunction != NULL && head->childrenSize > 0)
            {
                mpy = win_height - 1 - head->childrenSize;
                dpy = 4;
                menu_pad_height = head->childrenSize;
            }
            else if (head->displayFunction != NULL)
            {
                focus_on_top = true;
                dpy = 4;
                mpy = 3;
            }
            else if (head->childrenSize > 0)
            {
                menu_pad_height = head->childrenSize;
                mpy = 3;
            }

            if (head->displayFunction != NULL)
            {
                display_pad_height = win_height - 8;
                display_pad_height -= menu_pad_height;
                display_pad = newpad(pad_height, pad_width);
                int count = 100;
                head->displayFunction(display_pad, head->name, &count);
                if (count < pad_height)
                {
                    pad_height = count;
                }
                if (count < display_pad_height)
                {
                    display_pad_height = count + 1;
                    mpy = dpy + display_pad_height + 3;
                }
                prefresh(display_pad, display_pad_pos, 0, dpy, 1, 4 + display_pad_height, pad_width);
                mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);
            }

            if (head->childrenSize > 0)
            {
                menu_pad = newpad(head->childrenSize, pad_width);
                prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, pad_width);
            }
        }

        while (running)
        {
            if (check_size_change(win, &win_height, &win_width) == 1)
            {
                wclear(win);
                box(win, 0, 0);
                print_bold_text(win, 1, 1, head->name);
                mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
                wrefresh(win);

                if (head->displayFunction != NULL && head->childrenSize > 0)
                {
                    mpy = win_height - 1 - head->childrenSize;
                    dpy = 4;
                    menu_pad_height = head->childrenSize;
                }
                else if (head->displayFunction != NULL)
                {
                    focus_on_top = true;
                    dpy = 4;
                    mpy = 3;
                }
                else if (head->childrenSize > 0)
                {
                    menu_pad_height = head->childrenSize;
                    mpy = 3;
                }

                if (head->displayFunction != NULL)
                {
                    display_pad_height = win_height - 8;
                    display_pad_height -= menu_pad_height;
                    mvwhline(win, mpy - 1, 1, ACS_HLINE, pad_width);
                    prefresh(display_pad, display_pad_pos, 0, 4, 1, 4 + display_pad_height, pad_width);
                }
                if (head->childrenSize > 0)
                {
                    prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, pad_width);
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

                prefresh(display_pad, display_pad_pos, 0, 4, 1, 4 + display_pad_height, pad_width);
            }

            if (head->childrenSize > 0 && head->displayFunction != NULL)
            {
                if (focus_on_top)
                {
                    print_bold_text(win, dpy, win_width - 6, "***");
                }
                else
                {
                    print_bold_text(win, mpy , win_width - 6, "***");
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
                prefresh(menu_pad, 0, 0, mpy, 1, win_height - 1, pad_width);
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