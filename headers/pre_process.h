#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H
#include <cstdio>

void pre_process(FILE *file);
void expand_macros(char *file);
char* remove_spaces(char *str);
char* remove_comments(char *str);

#endif