#pragma once

#include <stdio.h>
#include <tree.h>

typedef enum
{
    YES,
    NO
} Answer;

void print_node(Node_t * node); 
void to_upper_str(char * input);
Answer get_answer(void);
char * read_line(void);
void free_path(char ** path, size_t size);
size_t get_file_size(const char * filename); 
char * initialize_buffer(size_t file_size);