#pragma once

#include <stdio.h>
#include "tree.h"

#define GRAPH_DUMP(count) do { \
    char png_file[100], dot_file[100]; \
    sprintf(png_file, "logger/graph%d.png", count); \
    sprintf(dot_file, "logger/tree%d.dot", count); \
    tree_graph_dump(tree, dot_file, png_file); \
} while(0)


// ErrorCode node_verify(Node_t * node);
// ErrorCode tree_verify(Tree_t * tree);

void print_node(Node_t * node);
const char * tree_error_string(ErrorCode error); 

void tree_graph_dump(Tree_t * tree, const char * filename_dot, const char * filename_png);
void tree_graph_dump_nodes(FILE * dot_fp, const Node_t * node);
void tree_graph_dump_edges(FILE * dot_fp, const Node_t * node);

void make_html(void);