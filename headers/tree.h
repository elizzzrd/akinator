#pragma once

typedef char * TreeElem_t; 

typedef enum 
{
    SUCCESS = 0,

    TREE_NULL_POINTER,
    TREE_MEMORY_ALLOCATION_ERROR,
    TREE_DUPLICATE_VALUE,
    TREE_INVALID_KEY,
    TREE_EMPTY_TREE,

    TREE_ADDING_TO_DATABASE_ERROR,
    TREE_SAVING_DATABASE_ERROR,
    TREE_OPENING_FILE_ERROR,
    TREE_LOADING_DATABASE_ERROR

} ErrorCode;


typedef struct treenode 
{
    TreeElem_t data;
    struct treenode * left; // ответ да
    struct treenode * right; // ответ нет
    struct treenode * prev;
} Node_t;

typedef struct 
{
    Node_t * root;
    int tree_size;
} Tree_t;

typedef struct 
{
    Node_t * node;
    ErrorCode error;
} Node_result_t; 


ErrorCode init_tree(Tree_t * tree);
Node_result_t init_node(TreeElem_t data, Tree_t * tree);

ErrorCode add_node(Node_t * node, Node_t * root);
Node_result_t add_item(TreeElem_t data, Tree_t * tree);

Node_result_t seek_digit(const TreeElem_t data, const Tree_t * tree);

void destroy_node(Node_t * node); 
void destroy_tree(Tree_t * tree);

ErrorCode build_parent_links_recursive(Node_t * node, Tree_t * tree);
ErrorCode build_parent_links(Tree_t * tree);

bool ToLeft(const TreeElem_t data1, const TreeElem_t data2);
bool ToRight(const TreeElem_t data1, const TreeElem_t data2);