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
        "Tree is empty",
        "Error during adding object to database"
    };
    
    if (error < SUCCESS || error > TREE_ADDING_TO_DATABASE_ERROR) return "Unknown error";
    
    return tree_error_strings[error];
}



void print_node(Node_t * node) //INORDER    
{
    if (!node)  return;

    printf("(");
    if (node -> left)
        print_node(node -> left);
    
    if (node -> data)
        printf("%s", node -> data);
    else
        printf("[NULL]");

    if (node -> right)
        print_node(node -> right);
    printf(")");
}

void tree_graph_dump_nodes(FILE * dot_fp, const Node_t * node)
{
    if (!node) return;
    const char * fillcolor = "#d1c5efff";

    const char *node_data = (node->data) ? node->data : "[NULL_DATA]";
    const char *left_label = "YES";
    const char *right_label = "NO";

    if (node->left != NULL && node->right != NULL)
    {
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <d> %s | {<l> %s |<r> %s}}\"];\n", 
        (const void*)node, fillcolor, node_data, left_label, right_label);
    }
    else if (node->left == NULL && node->right != NULL)
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <d> %s | {<l> %s |<r> %s}}\"];\n", 
        (const void*)node, fillcolor, node_data, "NULL", right_label);
    else if (node->right == NULL && node->left != NULL)
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <d> %s | {<l> %s |<r> %s}}\"];\n", 
        (const void*)node, fillcolor, node_data, left_label, "NULL");
    else
        fprintf(dot_fp, 
        "    node_%p [fillcolor = \"%s\", label=\"{ <d> %s | {<l> %s |<r> %s}}\"];\n", 
        (const void*)node, fillcolor, node_data, "NULL", "NULL");
    

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

    if (tree -> root)
    {
        tree_graph_dump_nodes(dot_fp, tree->root);
        fprintf(dot_fp, 
            "    node_%p [fillcolor = \"#f89898ff\", label=\"{<r> root| tree_size = %d}\"];\n", 
            (const void*)tree, tree->tree_size);
        fprintf(dot_fp, "\n");
    
        tree_graph_dump_edges(dot_fp, tree->root);
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
    FILE *html_fp = fopen(HTML_FILE, "w");
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
        "\tbody {\n"
        "\t\tfont-family: Consolas, monospace;\n"
        "\t\tbackground-color: #f4f4f8;\n"
        "\t\tmargin: 0;\n"
        "\t\tpadding: 20px;\n"
        "\t\tcolor: #333;\n"
        "\t}\n"
        "\th1 {\n"
        "\t\ttext-align: center;\n"
        "\t\tcolor: #222;\n"
        "\t\tmargin-bottom: 30px;\n"
        "\t}\n"
        "\t.container {\n"
		"\tdisplay: flex;\n"
		"flex-direction: column;\n" 
		"align-items: center;\n"    
		"gap: 20px;\n"  
        "\t}\n"
        "\t.card {\n"
        "\t\tbackground: #fff;\n"
        "\t\tborder-radius: 10px;\n"
        "\t\tbox-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);\n"
        "\t\tpadding: 15px;\n"
        "\t\twidth: 100%%;\n"
        "\t\tmax-width: 600px;\n"
        "\t\ttext-align: center;\n"
        "\t\ttransition: transform 0.2s ease-in-out;\n"
        "\t}\n"
        "\t.card:hover { transform: scale(1.02); }\n"
        "\timg {\n"
        "\t\twidth: 100%%;\n"
		"\t\theight: auto;\n"
		"\t\tmax-height: 600px;\n"
        "\t\tobject-fit: contain;\n"
        "\t\tbackground-color: #fafafa;\n"
        "\t\tborder: 1px solid #ddd;\n"
        "\t\tborder-radius: 6px;\n"
        "\t}\n"
        "\tp {\n"
        "\t\tfont-size: 14px;\n"
        "\t\tcolor: #555;\n"
        "\t\tmargin: 10px 0;\n"
        "\t}\n"
        "\t</style>\n"
        "</head>\n"
        "<body>\n"
        "\t<h1>Debug page</h1>\n"
        "\t<div class=\"container\">\n");

    extern int count;        
    for (int i = 1; i <= count; i++)
    {
        fprintf(html_fp,
            "\t\t<div class=\"card\">\n"
            "\t\t\t<p>Dump call: %d</p>\n"
            "\t\t\t<img src=\"logger/graph%d.png\" alt=\"graph%d\">\n"
            "\t\t</div>\n",
            i, i, i);
    }

    fprintf(html_fp,
        "\t</div>\n"
        "</body>\n"
        "</html>\n");

    fclose(html_fp);
}
