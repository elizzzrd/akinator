#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#include "utils.h"
#include "tree.h"
#include "errors.h"


void print_node(Node_t * node) 
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

void free_path(char ** path, size_t size)
{
    if (!path) return;
    
    for (size_t i = 0; i < size; i++)
    {
        if (path[i]) free(path[i]);
    }
    free(path);
}


char * read_line(void)
{
    char * line = NULL;
    size_t bufsize = 0;

    ssize_t len = getline(&line, &bufsize, stdin);
    if (len < 0)
    {
        free(line);
        return NULL;
    }

    if (len > 0 && line[len - 1] == '\n')   line[len-1] = '\0'; 
    return line;
}

Answer get_answer(void) 
{
    char ans[8];
    while(1)
    {
        printf("(yes/no): ");
        if (scanf("%7s", ans) != 1) 
            continue;
        to_upper_str(ans);
        
        if (strcmp(ans, "YES") == 0 || strcmp(ans, "Y") == 0)         return YES;
        else if (strcmp(ans, "NO") == 0 || strcmp(ans, "N") == 0)     return NO;
        else
            puts("Please type yes or no.\n");
    }
}


void to_upper_str(char * input)
{
    int i = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        input[i] = (char)toupper((unsigned char) input[i]);
    }
    input[i] = '\0';
}

size_t get_file_size(const char * filename) 
{
    assert(filename != NULL);
    
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) 
    {
        return (size_t)-1;
    }

    return (size_t)(file_stat.st_size);
}

char * initialize_buffer(size_t file_size)
{
    assert(file_size > 0);

    char * buffer = (char *)calloc(file_size + 1, sizeof(char));
    if (buffer == NULL) 
    {
        ErrorCode error;
        ERROR_MESSAGE(TREE_MEMORY_ALLOCATION_ERROR, error);
        return NULL;
    }
    return buffer;
}
