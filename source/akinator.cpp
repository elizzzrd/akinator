#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "akinator.h"
#include "errors.h"


// если дошли до листа, то это объект - программа делает предлоположение, что это загаданный объект
// если программа ошиблась - мы обучаем дерево
// спрашиваем правильный объект; вопрос, который их различает; 
void guess_object(Tree_t * tree, ErrorCode * error)
{
    if (!tree)
    {
        *error = TREE_NULL_POINTER;
        printf("guess_object: %s", tree_error_string(*error));
        return;
    }

    if (!tree->root)
    {
        *error = TREE_EMPTY_TREE;
        printf("%s", tree_error_string(*error));
        return;
    }

    while (1)
    {
        puts("\nImagine an object, and I'll try to guess it.");
        Node_t * current = tree->root;

        while (current->left || current->right)
        {
            printf("\n%s\n", current->data);
            char ans = get_answer();
            if (ans == 'y' && current->left)         current = current -> left;
            else if (ans == 'n' && current->right)   current = current -> right;
        }

        printf("\nIt is %s?\n", current -> data);
        char ans = get_answer();

        if (ans == 'y') 
        {
            printf("Wow! I guessed it!\n");
            *error = SUCCESS; 
            puts("\nDo you want to continue?");
            ans = get_answer();
            if (ans == 'y')     continue;
            else                break;
        }
        else
        {
            add_new_object_in_database(tree, current, error);
            if (error && *error != SUCCESS)
            {
                if (error) *error = TREE_ADDING_TO_DATABASE_ERROR;
                printf("%s", tree_error_string(*error));
                return;
            }
            puts("Okay, now I know what it is!");
        }
        puts("Do you want to continue?");
        ans = get_answer();
        if (ans == 'y')     
        {   
            printf("\n");
            continue;
        }
        else                
            break;
    }   
    if (error && *error == SUCCESS) return;
}

void to_upper_str(char * input)
{
    int i = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        input[i] = (char)toupper((unsigned char) input[i]);
    }
    input[i] = '\0';
}

char get_answer(void)
{
    char ans[8];
    while(1)
    {
        printf("(yes/no): ");
        if (scanf("%7s", ans) != 1) 
            continue;
        to_upper_str(ans);
        
        if (strcmp(ans, "YES") == 0 || strcmp(ans, "Y") == 0)    return 'y';
        else if (strcmp(ans, "NO") == 0 || strcmp(ans, "N") == 0)     return 'n';
        else
            puts("Please type yes or no.\n");
    }
}

int check_for_negation(char question[])
{
    return 0;
}

void add_new_object_in_database(Tree_t * tree, Node_t * current, ErrorCode * error)
{
    if (!tree || !current)
    {
        *error = TREE_NULL_POINTER;
        printf("%s", tree_error_string(*error));
        return;
    }

    char new_object[128] = {};
    char new_question[128] = {};

    puts("\nWhat it is?");
    if (scanf(" %[^\n]", new_object) != 1)
    {
        *error = TREE_ADDING_TO_DATABASE_ERROR;
        printf("%s", tree_error_string(*error));
        return;
    }

    while (1)
    {
        printf("\nHow %s differ from %s: ", new_object, current->data);
        if (scanf(" %[^\n]", new_question) != 1)
        {
            *error = TREE_ADDING_TO_DATABASE_ERROR;
            printf("%s", tree_error_string(*error));
            return;
        }
        if (check_for_negation(new_question))
        {
            printf("Please don't use negative constructions. Try again.\n");
            continue;
        }
        break;
    }

    Node_t * new_node_yes = init_node(new_object, error);
    if (!new_node_yes)
    {
        *error = TREE_MEMORY_ALLOCATION_ERROR;
        printf("%s", tree_error_string(*error));
        return;
    }

    Node_t * new_node_no = init_node(current->data, error);
    if (!new_node_no)
    {
        if (new_node_yes->data) free(new_node_yes->data);
        free(new_node_yes);
        *error = TREE_MEMORY_ALLOCATION_ERROR;
        printf("%s", tree_error_string(*error));
        return;
    }

    char * question_copy = strdup(new_question);
    if (!question_copy)
    {
        if (new_node_yes->data) free(new_node_yes->data);
        free(new_node_yes);
        if (new_node_no->data) free(new_node_no->data);
        free(new_node_no);
        *error = TREE_MEMORY_ALLOCATION_ERROR;
        printf("%s", tree_error_string(*error));
        return;
    }

    if (current -> data)
        free(current->data);
    current->data = question_copy;
    current -> left = new_node_yes;
    current -> right = new_node_no;

    tree->tree_size++;
    *error = SUCCESS;
    return;
}


void define_object(Tree_t * tree, const char * name, ErrorCode * error)
{

}

void compare_objects(Tree_t * tree, const char * name1, const char * name2, ErrorCode * error)
{

}