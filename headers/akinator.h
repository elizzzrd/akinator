#pragma once
#include "tree.h"



ErrorCode guess_object(Tree_t * tree);
ErrorCode define_object(Tree_t * tree, char * name);
ErrorCode compare_objects(Tree_t * tree, const char * name1, const char * name2);

void find_common_features(char ** path1, char ** path2, const char * name1, const char * name2);
void find_differences(char ** path1, char ** path2, const char * name1, const char * name2);

