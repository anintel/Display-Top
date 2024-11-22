#include "utils.h"
#include <stdio.h>
#include <ncurses.h>

void pause() {
    printf("Press any key to continue...");
    getch();
}

