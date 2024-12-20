#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H
#include <cstdio>

void pre_process(char *file_name);
void expand_macros(char *str);
char* remove_spaces(char *str, bool last_line);
char* remove_comments(char *str);

#endif