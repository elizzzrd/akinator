#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"
#include "errors.h"

#define HTML_FILE "page.html"

const char * tree_error_string(ErrorCode error) 
{
    static const char * tree_error_strings[] = 
    {
        "Success",
        "Null pointer provided", 
        "Memory allocation failed",
        "Dublicate key adding",
        "Deletion failed",
        "Invalid key",
        "Node data corruption",
        "Tree is empty"
    };
    
    if (error < SUCCESS || error > TREE_EMPTY_TREE) return "Unknown error";
    
    return tree_error_strings[error];
}



void print_node(Node_t * node) //INORDER    
{
    printf("(");
    if (node -> left)
        print_node(node -> left);
    printf("%d", node -> data);
    if (node -> right)
        print_node(node -> right);
    printf(")");
}

void tree_graph_dump_nodes(FILE * dot_fp, const Node_t * node)
{
    if (!node) return;
    const char * fillcolor = "#d1c5efff";

    if (node->left != NULL && node->right != NULL)
    {
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <ptr> %p | <d> %d | {<l> %p |<r> %p}}\"];\n", 
        (const void*)node, fillcolor, (const void*)node, node->data, (const void*)node->left, (const void*)node->right);
    }
    else if (node->left == NULL && node->right != NULL)
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <ptr> %p | <d> %d | {<l> %s |<r> %p}}\"];\n", 
        (const void*)node, fillcolor, (const void*)node, node->data, "NULL", (const void*)node->right);
    else if (node->right == NULL && node->left != NULL)
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <ptr> %p | <d> %d | {<l> %p |<r> %s}}\"];\n", 
        (const void*)node, fillcolor, (const void*)node, node->data, (const void*)node->left, "NULL");
    else
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <ptr> %p | <d> %d | {<l> %s |<r> %s}}\"];\n", 
        (const void*)node, fillcolor, (const void*)node, node->data, "NULL", "NULL");
    

    if (node -> left)
        tree_graph_dump_nodes(dot_fp, node -> left);
    if (node -> right)
        tree_graph_dump_nodes(dot_fp, node -> right);    
}

void tree_graph_dump_edges(FILE * dot_fp, const Node_t * node)
{
    if (!node) return;

    if (node->left)
        fprintf(dot_fp, 
        "    node_%p:l -> node_%p [color=\"#f570dfff\", penwidth=1];\n",
    (const void*)node, (const void*)node->left);

    
    if (node->right)
        fprintf(dot_fp, 
        "    node_%p:r -> node_%p [color=\"#f570dfff\", penwidth=1];\n",
    (const void*)node, (const void*)node->right);

    tree_graph_dump_edges(dot_fp, node->left);
    tree_graph_dump_edges(dot_fp, node->right);
}

void tree_graph_dump(Tree_t * tree, const char *filename_dot, const char *filename_png)
{
    if (!tree)
    {
        fprintf(stderr, "Tree NULL pointer\n");
        return;
    }

    FILE * dot_fp = fopen(filename_dot, "w");
    if (!dot_fp)
    {
        fprintf(stderr, "Cannot open file %s\n", filename_dot);
        return;
    }

    fprintf(dot_fp,
        "digraph TreeGraph {\n"
        "    rankdir=TB"
        "    bgcolor=\"#ffffff\";\n"
        "    fontname=\"Consolas\";\n"
        "    nodesep=0.6;\n"
        "    node [shape=record, style=filled, fontname=\"Consolas\", margin=0.1, width=1.3, height=0.8];\n"
        "    edge [fontname=\"Consolas\", arrowsize=0.8];\n\n");
    fprintf(dot_fp, "\n");

    
    tree_graph_dump_nodes(dot_fp, tree->root);
    fprintf(dot_fp, 
        "    node_%p [fillcolor = \"#f89898ff\", label=\"{<r> root| tree_size = %d}\"];\n", 
        (const void*)tree, tree->tree_size);
    fprintf(dot_fp, "\n");

    tree_graph_dump_edges(dot_fp, tree->root);
    if (tree->root)
    {
        fprintf(dot_fp, 
            "    node_%p -> node_%p [color = \"#f89898ff\", penwidth=1];\n",
        (const void*)tree, (const void*)tree->root);
    }

    fprintf(dot_fp, "}\n");
    fclose(dot_fp);

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "dot -Tpng \"%s\" -o \"%s\"", filename_dot, filename_png);
    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "Error: Graphviz execution failed (code=%d)\n", ret);
    }
}



void make_html()
{
    FILE * html_fp = fopen(HTML_FILE, "w");
    if (!html_fp)
        {
        fprintf(stderr, "Cannot open file %s\n", HTML_FILE);
        return;
    }

    fprintf(html_fp, 
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "\t<meta charset=\"UTF-8\">\n"
            "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
            "\t<title>Struct of data: trees</title>\n"
            "\t<style>\n"
            "\timg {\n"
            "\t\tmax-width: 100%%;\n"
            "\t\theight: auto;\n"
            "\t\tdisplay: block;\n"
            "\t}\n"
            "\t</style>\n"
            "</head>\n"
            "<body>\n"
            "\t<h1>Debug page</h1>\n");

    extern int count;        
    for (int i = 1; i <= count; i++)
    {
        fprintf(html_fp, "\t\t<p>dump call: %d</p>\n", i);
        fprintf(html_fp, "\t\t<img src=\"logger/graph%d.png\" alt=\"graph%d\" class=\"img\">\n", i, i);
    }

    fprintf(html_fp, "</body>\n</html>\n");
    fclose(html_fp);
}

