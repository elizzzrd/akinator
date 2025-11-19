#pragma once

#include <stdio.h>
#include "tree.h"

extern int graph_dump_count;

#define GRAPH_DUMP(tree) do { \
    graph_dump_count++; \
    char png_file[100], dot_file[100]; \
    sprintf(png_file, "logger/graph%d.png", graph_dump_count); \
    sprintf(dot_file, "logger/tree%d.dot", graph_dump_count); \
    tree_graph_dump(tree, dot_file, png_file); \
} while(0)


#define ERROR_MESSAGE(enum_error, error) \
    error = enum_error; \
    printf("%s:%d --- %s", __FILE__, __LINE__, tree_error_string(error));     \
    


// ErrorCode node_verify(Node_t * node);
// ErrorCode tree_verify(Tree_t * tree);

const char * tree_error_string(ErrorCode error); 

void tree_graph_dump(Tree_t * tree, const char * filename_dot, const char * filename_png);
void tree_graph_dump_nodes(FILE * dot_fp, const Node_t * node);
void tree_graph_dump_edges(FILE * dot_fp, const Node_t * node);

void make_html(void);