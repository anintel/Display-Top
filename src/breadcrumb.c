#include "breadcrumb.h"


WINDOW *init_breadcumb(){
    WINDOW* crumb = newwin(3, getmaxx(stdscr),0,0);
    wbkgd(crumb, COLOR_PAIR(2));
    box(crumb,0,0);
    mvwprintw(crumb, 1, 1, "HOME > ");
    wrefresh(crumb);
    return crumb;
}

void update_breadcrumb(WINDOW *crumb, const char **path, int level) {
    wclear(crumb);
    box(crumb, 0, 0);

    int x = 2;
    for (int i = 0; i < level; ++i) {
        if (i > 0) {
            mvwprintw(crumb, 1, x, " > ");
            x += 3;
        }
        mvwprintw(crumb, 1, x, "%s", path[i]);
        x += strlen(path[i]);
    }
    wrefresh(crumb);
}

// int handle_breadcrumb_click(const char **path, int depth, MEVENT *event) {
//     if (event->bstate & BUTTON1_CLICKED) {
//         int x = event->x;
//         int offset = 2; // Start offset in breadcrumb window

//         for (int i = 0; i < depth; ++i) {
//             if (x >= offset && x < offset + (int)strlen(path[i])) {
//                 return i; // Return the clicked breadcrumb index
//             }
//             offset += strlen(path[i]) + 3; // Account for " > "
//         }
//     }
//     return -1; // No valid click
// }
