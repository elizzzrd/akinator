#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "errors.h"
#include "akinator.h"
#include "menu.h"
#include "utils.h"
#include "database.h"

extern int graph_dump_count;


int main(void)
{
    Tree_t tree = {};
    init_tree(&tree);
    ErrorCode error = SUCCESS;

    error = load_database(&tree, DATABASE_INPUT);
    if (error != SUCCESS)
    {
        destroy_tree(&tree);
        return 1;
    }
   
 
    tree_graph_dump(&tree, "logger/tree1.dot", "logger/graph1.png");
    
    puts("============== AKINATOR ============== \n");
    puts("Hello, this is Akinator!");
    while (1)
    {
        show_menu();
        Akinator_mode mode = get_user_choice();
        if (mode == QUIT)   break;
        else                execute_mode(mode, &tree);
    }
    ask_for_quit(&tree);
        
    destroy_tree(&tree);
    if (error != SUCCESS)   
        fprintf(stderr, "Tree destroy failed: %s", tree_error_string(error));

    make_html();
    return 0;
}


