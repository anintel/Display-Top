#include "node.h"

Node *createNode(const char *name, int (*displayFunction)(WINDOW *, ...), Node *parent)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    newNode->displayFunction = displayFunction;
    newNode->parent = parent;
    newNode->children = NULL;
    newNode->childrenSize = 0;
    return newNode;
}

void addChild(Node *parent, Node *child)
{
    parent->children = (Node *)realloc(parent->children, sizeof(Node) * (parent->childrenSize + 1));
    parent->children[parent->childrenSize] = *child;
    parent->childrenSize++;
}

void freeTree(Node *root)
{
    if (!root)
        return;
    for (int i = 0; i < root->childrenSize; i++)
    {
        freeTree(&root->children[i]);
    }
    free(root->children);
    free(root);
}

void printPath(Node *node)
{
    if (node->parent)
    {
        printPath(node->parent);
        printw(" > %s", node->name);
    }
    else
    {
        printw("%s", node->name);
    }
}

void traverseAndPrint(Node *node, int depth)
{
    if (node == NULL)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        printw("-------");
    }

    if (node->parent == NULL)
    {
        printw("Node %s has no parent\n", node->name);
    }
    else
    {
        printw("Node %s is a child of %s\n", node->name, node->parent->name);
    }

    for (int i = 0; i < node->childrenSize; i++)
    {
        traverseAndPrint(&node->children[i], depth + 1);
    }
}