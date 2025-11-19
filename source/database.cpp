#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "tree.h"
#include "database.h"
#include "errors.h"
#include "utils.h"


Node_result_t fill_new_node(char * new_object, char * new_question, Node_t * current, Tree_t * tree)
{
    assert(new_object);
    assert(new_question);

    Node_result_t res = {.error = SUCCESS};
    
    Node_result_t new_node_yes = init_node(new_object, tree); 
    free(new_object);
    if (new_node_yes.error != SUCCESS)
    {
        ERROR_MESSAGE(TREE_ADDING_TO_DATABASE_ERROR, new_node_yes.error);
        res.node = NULL;
        return res;
    }

    Node_result_t new_node_no = init_node(current->data, tree);
    if (new_node_no.error != SUCCESS)
    {
        if (new_node_yes.node->data) free(new_node_yes.node->data);
        free(new_node_yes.node);
        ERROR_MESSAGE(TREE_ADDING_TO_DATABASE_ERROR, new_node_no.error);
        res.node = NULL;
        return res;
    }

    if (current -> data)
        free(current->data);

    current -> data = new_question;
    current -> left = new_node_yes.node;
    current -> right = new_node_no.node;

    new_node_yes.node->prev = current;
    new_node_no.node->prev = current;

    res.node = current;
    res.error = SUCCESS;
    return res;
}


Node_result_t add_new_object_in_database(Tree_t * tree, Node_t * current)
{
    assert(tree);
    assert(current);
    
    Node_result_t res = {.node = NULL, .error = SUCCESS};

    char * new_object = NULL;
    char * new_question = NULL;

    while (1)
    {
        puts("\nWhat it is?");
        new_object = read_line();
        if (!new_object || strlen(new_object) == 0)
        {
            printf("Invalid object. Please try again.\n");
            free(new_object);
            new_object = NULL;
            continue;
        }
        printf("\nHow %s differ from %s: ", new_object, current->data);
        new_question = read_line();
        if (!new_question || strlen(new_question) == 0)
        {
            printf("Invalid feature. Please try again.\n");
            free(new_object);
            new_object = NULL;
            free(new_question);
            new_question = NULL;
            continue;
        }
        break;
    }

    res = fill_new_node(new_object, new_question, current, tree);
    if (res.error == SUCCESS)   
    {
        tree->tree_size++;   
        ErrorCode error = build_parent_links(tree);
        if (error != SUCCESS)
        {
            res.error = error;
        }
    }
    return res;
}


Node_t * find_node_by_name(Node_t * node, const char * target, int * count)
{
    assert(target);
    if (!node)  return NULL;

    if (node->data && strcmp(node->data, target) == 0)   
        return node;

    if (node->left) 
    {
        Node_t * found = find_node_by_name(node->left, target, count);
        if (found) 
        {
            (*count)++;
            return found;
        }
    }

    if (node->right) 
    {
        Node_t * found = find_node_by_name(node->right, target, count);
        if (found) 
        {
            (*count)++;
            return found;
        }
    }

    return NULL;
}


Node_result_t seek_item(const char * data, const Tree_t * tree,  int * count)
{
    assert(tree);
    assert(data);

    Node_result_t res = {.node = NULL, .error = SUCCESS};
    if (!tree->root)    
    {
        ERROR_MESSAGE(TREE_EMPTY_TREE, res.error);
        return res;
    }

    Node_t * found = find_node_by_name(tree->root, data, count);
    if (found)     
    {
        res.node = found;
        return res;
    }
    else 
    {
        ERROR_MESSAGE(TREE_INVALID_KEY, res.error);
        return res;
    }
}

char ** collect_path(Node_t * node, size_t tree_size, size_t * path_size, Tree_t * tree)
{       
    assert(node);
    assert(tree_size > 0);
    assert(path_size);

    char ** path = (char **) calloc(tree_size, sizeof(char *)); 
    ErrorCode error = SUCCESS;
    if (!path)
    {
        ERROR_MESSAGE(TREE_MEMORY_ALLOCATION_ERROR, error);
        return NULL;
    } 

    Node_t * current = node -> prev;
    Node_t * go_from = node;
    size_t i = 0;
    

    printf("in function collect_path\n");
    while (current && current -> data)
    {        
        const char * option = NULL;
        if (current->left == go_from)           option = "YES";
        else if (current->right == go_from)     option = "NO";
        else                                    break;
        if (!option)                            break;
        
        size_t len_need = strlen(current->data) + strlen(option) + 8;
        path[i] = (char *) calloc(len_need, sizeof(char));
        if (!path[i])
        {
            for (int j = 0; j < i; j++)     
            free(path[j]);
            free(path);
            ERROR_MESSAGE(TREE_MEMORY_ALLOCATION_ERROR, error);
            return NULL;
        }
        
        snprintf(path[i++], len_need, "%s: %s", option, current->data);
        if (current->prev == tree->root)
            break;


        go_from = current;
        current = current -> prev;
    }

    *path_size = i;
    return path;
}


void savenode(Node_t * node, FILE * file_ptr)
{
    assert(file_ptr);

    fprintf(file_ptr, "(");
    if (node -> data)
        fprintf(file_ptr, "\"%s\"", node->data);

    if (!node -> left && !node -> right)
        fprintf(file_ptr, " nil nil");

    if (node -> left)    
        savenode(node -> left, file_ptr);
    if (node -> right)
        savenode(node -> right, file_ptr);

    fprintf(file_ptr, ")");
}


ErrorCode save_database(Tree_t * tree)
{
    assert(tree);

    ErrorCode error = SUCCESS;

    FILE * file_ptr = fopen(DATABASE_OUTPUT, "w");
    if (!file_ptr)
    {
        ERROR_MESSAGE(TREE_OPENING_FILE_ERROR, error);
        return error;
    }

    savenode(tree->root->right, file_ptr);

    GRAPH_DUMP(tree);
    fclose(file_ptr);
    return error;
}


ErrorCode load_database(Tree_t * tree, const char * filename)
{
    assert(tree);
    assert(filename);

    ErrorCode error = SUCCESS;

    size_t file_size = get_file_size(filename);
    char * buffer = initialize_buffer(file_size);
    if (!buffer)
    {
        ERROR_MESSAGE(TREE_LOADING_DATABASE_ERROR, error);
        return error;
    }

    FILE * file_ptr = fopen(filename, "r");
    if (!file_ptr)
    {
        ERROR_MESSAGE(TREE_OPENING_FILE_ERROR, error);
        return error;
    }

    size_t read_symbols = fread(buffer, sizeof(char), file_size, file_ptr);
    if (read_symbols <= file_size)
        buffer[read_symbols] = '\0';
    else    
        buffer[file_size] = '\0';


    size_t pos = 0;
    Node_t * first_node = read_node(buffer, &pos, tree);
    if (!first_node)
    {
        ERROR_MESSAGE(TREE_LOADING_DATABASE_ERROR, error);
        free(buffer);
        return error;
    }

    tree -> root -> right = first_node;
    error = build_parent_links(tree);
    if (error != SUCCESS)
    {
        ERROR_MESSAGE(TREE_LOADING_DATABASE_ERROR, error);
        free(buffer);
        return error;
    }
    GRAPH_DUMP(tree);
    
    free(buffer);
    return error;
}



Node_t * read_node(char * buffer, size_t * pos, Tree_t * tree) 
{
    while (isspace(buffer[*pos])) 
        (*pos)++;
    
    if (buffer[*pos] == '(') 
    {
        (*pos)++;                                                   // пропускаем '('
        
        while (isspace(buffer[*pos])) (*pos)++;                     // пропускаем пробелы после '('
                                                            
        if (buffer[*pos] == '"') 
            (*pos)++;                                               // пропускаем '"'
        
        char data[128] = {};                                        // читаем до закрывающей кавычки
        int read_chars = 0;

        if (sscanf(buffer + *pos, "%127[^\"]%n", data, &read_chars) == 1) 
            *pos += read_chars;
        else 
            data[0] = '\0';
    
        
        if (buffer[*pos] == '"')                                  // пропускаем закрывающую кавычку
            (*pos)++;


        Node_result_t current = init_node(data, tree);
        if (current.error != SUCCESS) 
            return NULL;    
        tree->tree_size++;

        current.node->left = read_node(buffer, pos, tree);
        current.node->right = read_node(buffer, pos, tree);
        
        
        while (isspace(buffer[*pos]))                             // пробелы перед закрывающей скобкой
            (*pos)++;
        
        if (buffer[*pos] == ')') 
            (*pos)++;
        
        return current.node;
    }
    else if (strncmp(buffer + *pos, "nil", 3) == 0) 
    {
        *pos += 3;
        return NULL;
    }
    return NULL;
}