#pragma once

#include "tree.h"
#define DATABASE_INPUT "database_input.txt"
#define DATABASE_OUTPUT "logger/database.txt"

Node_result_t fill_new_node(char * new_object, char * new_question, Node_t * current, Tree_t * tree);
Node_result_t add_new_object_in_database(Tree_t * tree, Node_t * current);

Node_t * find_node_by_name(Node_t * node, const char * target, int * count);
Node_result_t seek_item(const char * data, const Tree_t * tree,  int * count);
char ** collect_path(Node_t * node, size_t tree_size, size_t * path_size, Tree_t * tree);

ErrorCode save_database(Tree_t * tree);
void savenode(Node_t * node, FILE * file_ptr);

ErrorCode load_database(Tree_t * tree, const char * filename);
Node_t * read_node(char * buffer, size_t * pos, Tree_t * tree);