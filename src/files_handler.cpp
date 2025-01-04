#include <string.h>
#include <stdlib.h>
#include "files_handler.h"

int check_file_extension(char *file) {
    char *extensao = strrchr(file, '.');

    if (strcmp(extensao, ".asm") == 0) {
        return 1;
    }

    else if (strcmp(extensao, ".pre") == 0) {
        return 2;
    }

    else if (strcmp(extensao, ".obj") == 0) {
        return 3;
    }

    else {
        return 0;
    }
}

char* change_file_extension(char *file, char *new_extension) {
    char *extensao = strrchr(file, '.');
    char *new_file = (char*) malloc(strlen(file) + strlen(new_extension) + 1);

    strncpy(new_file, file, extensao - file);
    new_file[extensao - file] = '\0';
    strcat(new_file, new_extension);

    return new_file;
}


FILE* open_file(char *file) {
    FILE* opened_file = fopen(file, "r");

    if (opened_file == NULL) {
        printf("Erro\n");
    }

    else {
        printf("Abriu arquivo %s\n", file);
    }
    return opened_file;
}

FILE* create_file(char *file) {
    FILE* created_file = fopen(file, "w");

    if (created_file == NULL) {
        printf("Erro\n");
    }

    else {
        printf("Criou arquivo %s\n", file);
    }
    return created_file;
}

void write_file(FILE* file, char *code) {
    fputs(code, file);
}   

void delete_file(char *file) {
    int ret = remove(file);
    if (ret) printf("Erro em manuseio de arquivo\n");
}