#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H
#include <cstdio>

void pre_process(char *file_name);
char* remove_spaces(char *str, bool last_line);
char* remove_comments(char *str);
void expand_macro(FILE *file, char *line);
bool is_macro(char *str);
bool is_macro_call(char *str);
bool is_end_macro(char *str);

#endif