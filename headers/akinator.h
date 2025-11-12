#pragma once
#include "tree.h"


void guess_object(Tree_t * tree, ErrorCode * error);
void define_object(Tree_t * tree, const char * name, ErrorCode * error);
void compare_objects(Tree_t * tree, const char * name1, const char * name2, ErrorCode * error);

void write_down_database(Tree_t * tree);

void to_upper_str(char * input);
char get_answer(void);
void add_new_object_in_database(Tree_t * tree, Node_t * current, ErrorCode * error);
int check_for_negation(char question[]);