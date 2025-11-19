#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "akinator.h"
#include "errors.h"
#include "utils.h"
#include "database.h"

#define MAX_LENGTH 64


ErrorCode guess_object(Tree_t * tree)
{
    assert(tree);
    ErrorCode error = SUCCESS;
    if (!tree->root)    
    {
        ERROR_MESSAGE(TREE_EMPTY_TREE, error);
        return error;
    }

    while (1)
    {
        puts("\n======================================");
        puts("Imagine an object, and I'll try to guess it.");
        Node_t * current = tree->root->right;

        while (current->left || current->right)
        {
            printf("\n%s\n", current->data);
            Answer ans = get_answer();

            if (ans == YES)
            {
                if (current->left) 
                    current = current -> left;
                else
                {
                    printf("No further questions for YES answer. Making guess...\n");
                    break;
                }
            } 
            else if (ans == NO)
            {
                if (current->right)  
                    current = current -> right;
                else                                // нет правого потомка - сразу переходим к угадыванию
                    break;
            } 
        }

        printf("\nIt is %s?\n", current -> data);
        Answer ans = get_answer();

        if (ans == YES) 
        {
            printf("Wow! I guessed it!\n");
            error = SUCCESS; 
            puts("\nDo you want to continue?");
            ans = get_answer();
            if (ans == YES)     continue;
            else     
            {
                puts("======================================");
                break;
            }           
        }
        else
        {
            Node_result_t added_node = add_new_object_in_database(tree, current);
            if (added_node.error != SUCCESS)
            {
                ERROR_MESSAGE(TREE_ADDING_TO_DATABASE_ERROR, error);
                return error;
            }
            puts("Okay, now I know what it is!");
            GRAPH_DUMP(tree);
        }
        puts("\nDo you want to continue?");
        ans = get_answer();
        if (ans == 'y')     
        {   
            printf("\n");
            continue;
        }
        else     
        {           
            puts("======================================");
            break;
        }
    }   
    return SUCCESS;
}


ErrorCode define_object(Tree_t * tree, char * name)
{
    assert(tree);
    assert(name);

    ErrorCode error = SUCCESS;

    if (!tree->root || !tree->root->right)
    {
        ERROR_MESSAGE(TREE_EMPTY_TREE, error);
        return error;
    }

    int count = 0;
    Node_result_t elem = seek_item(name, tree, &count);

    if (elem.error == SUCCESS)
    {
        if (!elem.node || !elem.node->data) 
        {
            ERROR_MESSAGE(TREE_NULL_POINTER, error);
            return error;
        }
 
        size_t path_size = 0;
        char ** path = collect_path(elem.node, tree -> tree_size, &path_size, tree);
        if (!path)
        {
            ERROR_MESSAGE(TREE_MEMORY_ALLOCATION_ERROR, error);
            return error;
        }
           
        printf("%s is:\n", elem.node->data);
        if (path_size > 0)
        {
            for (int j = path_size - 1; j >= 0; j--)
            {
                if (path[j])
                {
                    printf("\t%s\n", path[j]);
                    free(path[j]);
                }
            }
        }

        free(path);
    }
    else   
    {
        printf("Object '%s' not found in database\n", name);
        return elem.error;
    }

    return SUCCESS;
}


ErrorCode compare_objects(Tree_t * tree, const char * name1, const char * name2)
{
    assert(tree);
    assert(name1);
    assert(name2);

    ErrorCode error = SUCCESS;

    if (!tree->root || !tree->root->right)
    {
        ERROR_MESSAGE(TREE_EMPTY_TREE, error);
        return error;
    }

    int count1 = 0, count2 = 0;
    Node_result_t elem1 = seek_item(name1, tree, &count1);
    Node_result_t elem2 = seek_item(name2, tree, &count2);

    if (elem1.error != SUCCESS)
    {
        printf("Object '%s' not found in database\n", name1);
        return elem1.error;
    }

    if (elem2.error != SUCCESS)
    {
        printf("Object '%s' not found in database\n", name2);
        return elem2.error;
    }

    if (strcmp(name1, name2) == 0)
    {
        printf("'%s' and '%s' are the same object!\n", name1, name2);
        return SUCCESS;
    }

    size_t path_size1 = 0, path_size2 = 0;
    char ** path1 = collect_path(elem1.node, tree->tree_size, &path_size1, tree);
    char ** path2 = collect_path(elem2.node, tree->tree_size, &path_size2, tree);
    
     if (!path1 || !path2)
    {
        ERROR_MESSAGE(TREE_MEMORY_ALLOCATION_ERROR, error);
        return error;
    }

    find_common_features(path1, path2, name1, name2);
    find_differences(path1, path2, name1, name2);

    free_path(path1, tree -> tree_size);
    free_path(path2, tree -> tree_size);

    return error;
}


void find_common_features(char ** path1, char ** path2, const char * name1, const char * name2)
{
    assert(path1);
    assert(path2);
    assert(name1);
    assert(name2);

    printf("Common features:\n");
    int common_found = 0;

    for (int i = 0; path1[i] && path2[i]; i++)
    {
        char * question1 = strchr(path1[i], ':');
        char * question2 = strchr(path2[i], ':');

        if (question1 && question2 && strcmp(question1, question2) == 0)
        {
            char answer1[10] = {};
            char answer2[10] = {};
            
            sscanf(path1[i], " %9[^:]", answer1);
            sscanf(path2[i], " %9[^:]", answer2);

            if (strcmp(answer1, answer2) == 0)
            {
                printf("\tBoth are %s %s\n", 
                        strcmp(answer1, "YES") == 0 ? "": "not",
                        question1);
                common_found = 1;
            }
        }
        else    
            break;
    }

    if (!common_found)  
        printf("No common features\n");
}


void find_differences(char ** path1, char ** path2, const char * name1, const char * name2)
{
    assert(path1);
    assert(path2);
    assert(name1);
    assert(name2);

    printf("Differences:\n");
    int dif_found = 0;

    for (int i = 0; (path1[i] || path2[i]); i++)
    {
        char * question1 = path1[i] ? strchr(path1[i], ':') : NULL;
        char * question2 = path2[i] ? strchr(path2[i], ':') : NULL;
        // пути разошлись или один закончился
        if (!question1 || !question2 || strcmp(question1, question2) != 0)
        {
            if (path1[i])
            {
                printf("\t%s: %s\n", name1, path1[i]);
                dif_found = 1;
            }
            if (path2[i])
            {
                printf("\t%s: %s\n", name2, path2[i]);
                dif_found = 1;
            }
            break;
        }
        else    // вопросы совпадают
        {   
            char answer1[10] = "", answer2[10] = "";
            sscanf(path1[i], " %9[^:]", answer1);
            sscanf(path2[i], " %9[^:]", answer2);
            
            if (strcmp(answer1, answer2) != 0)
            {
                printf("\t%s is %s%s, but %s is %s%s\n",
                       name1, strcmp(answer1, "YES") == 0 ? "" : "not ",
                       question1 + 2,
                       name2, strcmp(answer2, "YES") == 0 ? "" : "not ",
                       question2 + 2);
                dif_found = 1;
                break;
            }
        }
    }

    if (!dif_found)
    {
        printf("No differences found.\n");
    }
}



