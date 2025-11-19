#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "akinator.h"
#include "menu.h"
#include "errors.h"
#include "utils.h"
#include "database.h"

void show_menu()
{
    printf("\nChoose mode:\n"
            "1 - Guess\n"
            "2 - Define\n"
            "3 - Compare\n"
            "4 - Show tree\n"
            "5 - Quit\n");
}

Akinator_mode get_user_choice()
{
    int choice = -1;
    while(1)
    {
        printf("(Choose mode): ");
        if (scanf("%d", &choice) != 1) 
            continue;
        switch (choice)
        {
            case 1:     return GUESS;
            case 2:     return DEFINE;
            case 3:     return COMPARE;
            case 4:     return SHOW_TREE;
            case 5:     return QUIT;
            default:    printf("Invalid mode. Please try again.\n"); continue;
        }
    }
}

void execute_mode(Akinator_mode mode, Tree_t * tree)
{
    switch (mode)
    {
        case GUESS:
        {
            guess_object(tree);
            break;
        }
        case DEFINE:
        {
            char name[64] = {};
            printf("Enter object name: ");
            scanf("%63s", name);
            define_object(tree, name);
            break;
        }
        case COMPARE:
        {
            char name1[64] = {};
            char name2[64] = {};
            printf("Enter two objects for comparison: ");
            scanf("%63s %63s", name1, name2);
            compare_objects(tree, name1, name2);
            break;
        }
        case SHOW_TREE:
        {
            extern int graph_dump_count;
            char command[60];
            sprintf(command, "xdg-open logger/graph%d.png", graph_dump_count);
            system(command);
            break;
        }
        default: printf("Unknown mode\n"); break;
    }
}


void ask_for_quit(Tree_t * tree)
{
    ErrorCode error = SUCCESS;
    printf("Do you want to save tree?");
    Answer ans = get_answer();
    if (ans == YES)     
    {
        if (save_database(tree) == SUCCESS)
                printf("The database has successfully loaded to the disk\n");
        else    
        {
            ERROR_MESSAGE(TREE_SAVING_DATABASE_ERROR, error);
        }
        printf("Goodbye!\n");
    puts("============== AKINATOR ============== \n");
    }
}


