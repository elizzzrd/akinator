#pragma once

typedef int TreeElem_t; 

typedef enum 
{
    SUCCESS = 0,

    TREE_NULL_POINTER,
    TREE_MEMORY_ALLOCATION_ERROR,
    TREE_DUPLICATE_VALUE,
    TREE_INVALID_KEY,
    TREE_DATA_CORRUPTION,
    TREE_EMPTY_TREE,

} ErrorCode;

typedef struct treenode 
{
    TreeElem_t data;
    struct treenode * left;
    struct treenode * right;
} Node_t;

typedef struct 
{
    Node_t * root;
    int tree_size;
} Tree_t;


void init_tree(Tree_t * tree);
Node_t * init_node(TreeElem_t data, ErrorCode * error);

bool tree_is_empty(const Tree_t * tree);

void add_node(Node_t * node, Node_t * root, ErrorCode * error);
Node_t * add_item(TreeElem_t data, Tree_t * tree, ErrorCode * error);


Node_t * seek_item(TreeElem_t data, const Tree_t * tree,  ErrorCode * error);

void destroy_node(Node_t * node, ErrorCode * error, Tree_t * tree);
void destroy_tree(Tree_t * tree, ErrorCode * error);

bool ToLeft(const TreeElem_t data1, const TreeElem_t data2);
bool ToRight(const TreeElem_t data1, const TreeElem_t data2);