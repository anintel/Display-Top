#ifndef MENU_H
#define MENU_H

// WINDOW * display_menu(int *level);
void CreateMainMenu(WINDOW *menu, WINDOW *summary, char *options[], int n_options);
void CreateMenu(WINDOW *win, char *options[], int n_options, char* title);

#endif
