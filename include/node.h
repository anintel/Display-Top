#ifndef NODE_H
#define NODE_H

#include <ncurses.h>

typedef struct Node {
    char name[50];
    char type[10]; 
    void (*displayFunction)(WINDOW*, ...); 
    struct Node* submenu;            
    int submenuSize;                 
} Node;

#endif 
