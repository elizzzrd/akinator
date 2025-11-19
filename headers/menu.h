#pragma once

#include "tree.h"

typedef enum
{
    GUESS = 1,
    DEFINE,
    COMPARE,
    SHOW_TREE,
    QUIT
} Akinator_mode;


void show_menu();
Akinator_mode get_user_choice();
void execute_mode(Akinator_mode mode, Tree_t * tree);
void ask_for_quit(Tree_t * tree);
