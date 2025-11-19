#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "errors.h"

int graph_dump_count = 0;


ErrorCode init_tree(Tree_t * tree)
{
    tree -> tree_size = 1;
    char str[] = "Unknown object";
    Node_result_t root = init_node(str, tree);
    tree->root = root.node;
    GRAPH_DUMP(tree);
    return root.error;
}

Node_result_t init_node(TreeElem_t data, Tree_t * tree)
{
    assert(data);
    Node_result_t res = {.error = SUCCESS};

    Node_t * new_node = (Node_t *) calloc(1, sizeof(Node_t));
    if (!new_node) 
    {
        ERROR_MESSAGE(TREE_MEMORY_ALLOCATION_ERROR, res.error); 
        return res;
    }
    else
    {
        new_node->data = strdup(data);
        if (!new_node->data)
        {
            free(new_node);
            ERROR_MESSAGE(TREE_MEMORY_ALLOCATION_ERROR, res.error);
            return res;
        }

        new_node->left = NULL;
        new_node->right = NULL;
        new_node->prev = NULL;
    }

    res.error = SUCCESS; res.node = new_node;
    return res;
}


Node_result_t seek_digit(const TreeElem_t data, const Tree_t * tree) 
{
    assert(tree);
    Node_result_t res = {};
    if (!tree)
    {
        ERROR_MESSAGE(TREE_NULL_POINTER, res.error);
        return res;
    } 
    if(!tree->root)
    {
        ERROR_MESSAGE(TREE_EMPTY_TREE, res.error);
        return res;
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
            res.error = SUCCESS;
            res.node = current_node;
            return res;
        }
    }

    res.error = TREE_INVALID_KEY;
    res.node = NULL;
    return res;
}


ErrorCode add_node(Node_t * node, Node_t * root) // TODO: fix by adding root by default 
{                                           // TODO: use return (pass the errorcode)
    assert(node);
    assert(root);

    ErrorCode error = SUCCESS;
    if (!node)     
    {
        ERROR_MESSAGE(TREE_NULL_POINTER, error);
        return error;
    }
    if (!root)
    {
        ERROR_MESSAGE(TREE_EMPTY_TREE, error);
        return error;
    }
    
    if (ToLeft(node->data, root->data))
    {
        if (root->left == NULL)     
        {
            root->left = node;
            node->prev = root;
            return SUCCESS;
        }
        else 
            add_node(node, root->left);
    }
    else if (ToRight(node->data, root->data))
    {
        if (root->right == NULL)    
        {
            root->right = node;
            node->prev = root;
            return SUCCESS;
        }
        else                        
            add_node(node, root->right);
    }
    else    
    {
        error = TREE_DUPLICATE_VALUE;
        return error;
    }
    return error;
}


Node_result_t add_item(TreeElem_t data, Tree_t * tree)
{
    assert(tree);
    Node_result_t res = {};
    if (!tree->root)
    {
        ERROR_MESSAGE(TREE_EMPTY_TREE, res.error);
        return res;
    }
    
    Node_result_t seek = seek_digit(data, tree);
    Node_t * node_with_same_data = seek.node;
    if (tree->root && node_with_same_data != NULL)
    {
        ERROR_MESSAGE(TREE_DUPLICATE_VALUE, res.error);
        res.node = NULL;
        return res;
    }
    
    Node_result_t node_struct = init_node(data, tree);
    if (node_struct.error != SUCCESS)
    {
        res.error = node_struct.error;
        return res;
    }

    Node_t * new_node = node_struct.node;
    res.error = add_node(new_node, tree->root);
    if (res.error == SUCCESS)
        tree->tree_size++;
    else
    {
        res.node = NULL;
        return res;
    }
    
    GRAPH_DUMP(tree);
    res.node = new_node;
    return res;
}


void destroy_node(Node_t * node) 
{
    assert(node);

    if (node->left)     destroy_node(node->left);
    if (node->right)    destroy_node(node->right);
    
    node->left = NULL;
    node->right = NULL;
    node->prev = NULL;  
    if (node->data)
        {
            free(node->data);
            node->data = NULL; 
        }
    free(node);
    return;
}


void destroy_tree(Tree_t * tree)
{
    assert(tree);
    GRAPH_DUMP(tree);
    
    destroy_node(tree->root);
    tree->tree_size = 0;
    return;
}


bool ToLeft(const TreeElem_t data1, const TreeElem_t data2)
{
    if (data1 <= data2)
        return true;
    else
        return false;
}


bool ToRight(const TreeElem_t data1, const TreeElem_t data2)
{
    if (data1 > data2)
        return true;
    else
        return false;
}


ErrorCode build_parent_links(Tree_t * tree)
{
    assert(tree);
    
    if (!tree->root)
    {
        return TREE_EMPTY_TREE;
    }
    
    tree->root->prev = NULL;
    
    return build_parent_links_recursive(tree->root, tree);
}

ErrorCode build_parent_links_recursive(Node_t * node, Tree_t * tree)
{
    if (!node)
        return SUCCESS;
    
    if (node->left)
    {
        node->left->prev = node;
        ErrorCode error = build_parent_links_recursive(node->left, tree);
        if (error != SUCCESS)
            return error;
    }
    
    if (node->right)
    {
        node->right->prev = node;
        ErrorCode error = build_parent_links_recursive(node->right, tree);
        if (error != SUCCESS)
            return error;
    }
    
    return SUCCESS;
}


// bool ToLeft_str(const TreeElem_t i1, const TreeElem_t i2)
// {
//     int comp1;
//     if ((comp1 = strcmp(i1, i2)) < 0)
//         return true;
//     else if (comp1 == 0 && strcmp(i1, i2) < 0)
//         return true;
//     else
//         return false;
// }

// bool ToRight_str(const TreeElem_t i1, const TreeElem_t i2)
// {
//     int comp1;
//     if ((comp1 = strcmp(i1, i2)) > 0)
//         return true;
//     else if (comp1 == 0 && strcmp(i1, i2) > 0)
//         return true;
//     else
//         return false;
// }

