#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H

void pre_process_file(char *file);
void expand_macros(char *file);
void remove_spaces(char *str);
void remove_comments(char *str);

#endif