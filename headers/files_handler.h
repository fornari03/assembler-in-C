#ifndef FILES_HANDLER_H
#define FILES_HANDLER_H
#include <cstdio>

int check_file_extension(char *file);
FILE* open_file(char *file);
FILE* create_file(char *file);
void write_file(FILE* file, char *code);

#endif