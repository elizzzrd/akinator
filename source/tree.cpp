#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "errors.h"

int count = 0;


void init_tree(Tree_t * tree)
{
    tree->root = NULL;
    tree->tree_size = 0;
}

bool tree_is_empty(const Tree_t * tree)
{
    if (tree->root == NULL)
        return true;
    else
        return false;
}

Node_t * init_node(TreeElem_t data, ErrorCode * error)
{
    Node_t * new_node = (Node_t *) calloc(1, sizeof(Node_t));
    if (!new_node)
    {
        *error = TREE_MEMORY_ALLOCATION_ERROR; 
        return NULL;
    }
    else
    {
        new_node->data = strdup(data);
        if (!new_node->data)
        {
            free(new_node);
            *error = TREE_MEMORY_ALLOCATION_ERROR;
            return NULL;
        }
        new_node->left = NULL;
        new_node->right = NULL;
    }
    *error = SUCCESS;
    return new_node;
}

Node_t * seek_item(const TreeElem_t data, const Tree_t * tree, ErrorCode * error) 
{
    if (!tree)
    {
        *error = TREE_NULL_POINTER;
        return NULL;
    } 

    if(!tree->root)
    {
        *error = TREE_EMPTY_TREE;
        return NULL;
    }

    Node_t * current_node = tree->root;

    while (current_node)
    {
        if (ToLeft(data, current_node->data))   
            current_node = current_node->left;
        else if (ToRight(data, current_node->data))   
            current_node = current_node->right;
        else 
        {
            *error = SUCCESS;
            return current_node;
        }
    }

    //printf("element %d not found in tree\n", data);
    //*error = TREE_INVALID_KEY;
    return NULL;
}

void add_node(Node_t * node, Node_t * root, ErrorCode * error)
{
    if (!node || !root)
    {
        *error = TREE_NULL_POINTER;
        return;
    }
    
    if (ToLeft(node->data, root->data))
    {
        if (root->left == NULL)     
        {
            root->left = node;
            return;
        }
        else 
        add_node(node, root->left, error);
    }
    else if (ToRight(node->data, root->data))
    {
        if (root->right == NULL)    
        {
            root->right = node;
            return;
        }
        else                        
            add_node(node, root->right, error);
    }
    else
    {
        *error = TREE_DUPLICATE_VALUE;
    }
}

Node_t * add_item(TreeElem_t data, Tree_t * tree, ErrorCode * error)
{
    assert(*error == SUCCESS);
    
    if (!tree)
    {
        *error = TREE_NULL_POINTER;
        return NULL;
    }
    if (tree->root && (seek_item(data, tree, error) != NULL))
    {
        *error = TREE_DUPLICATE_VALUE;
        fprintf(stderr, "An attempt to add an already existing element"); // возврат из функцией с просьбой повторить попытку
        return NULL;
    }
    
    *error = SUCCESS;
    Node_t * new_node = init_node(data, error);
    if (*error != SUCCESS)
    {
        fprintf(stderr, "Could not create node\n");
        return NULL;
    }
    
    if (tree->root == NULL)         tree->root = new_node;
    else                            add_node(new_node, tree->root, error);
    
    if (*error == SUCCESS) tree->tree_size++;
    count++;
    GRAPH_DUMP(count);
    return new_node;
}


void destroy_node(Node_t * node, ErrorCode * error, Tree_t * tree)
{
    if (node == NULL) 
    {
        //*error = TREE_NULL_POINTER;
        return;
    }
    
    if (node->left)
    destroy_node(node->left, error, tree);
    if (node->right)
    destroy_node(node->right, error, tree);
    
    node->left = NULL;
    node->right = NULL;  
    if (node->data)
        {
            free(node->data);
            node->data = NULL; 
        }
    free(node);
    
    if (error)
    *error = SUCCESS;
    return;
}


void destroy_tree(Tree_t * tree, ErrorCode * error)
{
    if (!tree)
    {
        *error = TREE_NULL_POINTER;
        return;
    }
    GRAPH_DUMP(count);
    
    destroy_node(tree->root, error,tree);
    tree->tree_size = 0;
    
    *error = SUCCESS;
    count++;
    //GRAPH_DUMP(count);
    return;
}


// bool ToLeft(const Node_t node_to_add, const Node_t existing_node)
// {
//     printf("%s %s\n", node_to_add.data, existing_node.data);
//     char option[10]= {};
    
// }


// bool ToRight(const TreeElem_t data1, const TreeElem_t data2)
// {
//     if (data1 > data2)
//         return true;
//     else
//         return false;
// }


bool ToLeft(const TreeElem_t i1, const TreeElem_t i2)
{
    int comp1;
    if ((comp1 = strcmp(i1, i2)) < 0)
        return true;
    else if (comp1 == 0 && strcmp(i1, i2) < 0)
        return true;
    else
        return false;
}

bool ToRight(const TreeElem_t i1, const TreeElem_t i2)
{
    int comp1;
    if ((comp1 = strcmp(i1, i2)) > 0)
        return true;
    else if (comp1 == 0 && strcmp(i1, i2) > 0)
        return true;
    else
        return false;
}

