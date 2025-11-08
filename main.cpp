#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "errors.h"


int main(void)
{
    Tree_t tree = {};
    init_tree(&tree);
    ErrorCode error = SUCCESS;

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

    destroy_tree(&tree, &error);
    if (error != SUCCESS)   fprintf(stderr, "Tree destroy failed: %s", tree_error_string(error));

    make_html();
    printf("Programm is finished\n");
    return 0;
}
