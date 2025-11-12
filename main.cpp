#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "errors.h"
#include "akinator.h"


int main(void)
{
    Tree_t tree = {};
    init_tree(&tree);
    ErrorCode error = SUCCESS;

    /*
    int values[] = {20, 10, 30, 25, 37, 5, 11};
    for (size_t i = 0; i < sizeof(values)/ sizeof(values[0]); i++)
    {
        add_item(values[i], &tree, &error);
        if (error != SUCCESS)
        {
            fprintf(stderr, "Error adding %d: %s\n", values[i], tree_error_string(error));
            error = SUCCESS;
        }
        else
        {
            fprintf(stderr, "Added %d successfully\n", values[i]);
        }
    }

    printf("Tree size after insertion: %d\n", tree.tree_size);

    printf("Searching elements\n");
    int search_values[] = {5,11,30};
    for (int i = 0; i < 3; i++)
    {
        Node_t * found = seek_item(search_values[i], &tree, &error);
        if (found && error == SUCCESS)  printf("Found node with data %d in %p\n", found->data, (void*)found);
        else fprintf(stderr, "Element %d has not found (%s)\n", search_values[i], tree_error_string(error));

        error = SUCCESS;
    }
    */

    char str1[] = "Animal?";
    char str2[] = "Matrusha?";
    char str3[] = "Math teacher?";
    char str4[] = "Petrovich?";
    Node_t * node_1 = init_node(str1, &error);
    Node_t * node_2 = init_node(str2, &error);
    Node_t * node_3 = init_node(str3, &error);
    Node_t * node_4 = init_node(str4, &error);
    
    if (error != SUCCESS) 
    {
        printf("%s\n", tree_error_string(error));
        if (node_1) free(node_1->data), free(node_1);
        if (node_2) free(node_2->data), free(node_2);
        if (node_3) free(node_3->data), free(node_3);
        if (node_4) free(node_4->data), free(node_4);
        return -1;
    }
    
    tree.root = node_1;
    node_1->left = node_2;
    node_1->right = node_3;
    node_3->left = node_4;
    tree.tree_size = 4;
    tree_graph_dump(&tree, "logger/tree1.dot", "logger/graph1.png");
    

    int mode = 0;
    puts("============== AKINATOR ============== \n");
    puts("Hello, this is Akinator!");
    puts("Choose mode:\n1 - Guess\n2 - Define\n3 - Compare\n");
    scanf("%d" , &mode);

    if (mode == 1)      guess_object(&tree, &error);
    else if (mode == 2)
    {
        char name[64] = {};
        printf("Enter object name: ");
        scanf("%63s", name);
        define_object(&tree, name, &error);
    }
    else if (mode == 3)
    {
        char name1[64] = {};
        char name2[64] = {};
        printf("Enter two objects for comparison: ");
        scanf("%63s %63s", name1, name2);
        compare_objects(&tree, name1, name2, &error);
    }

    tree_graph_dump(&tree, "logger/tree2.dot", "logger/graph2.png");
        
    destroy_tree(&tree, &error);
    if (error != SUCCESS)   fprintf(stderr, "Tree destroy failed: %s", tree_error_string(error));

    make_html();
    printf("Programm is finished\n");
    return 0;
}







