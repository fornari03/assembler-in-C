#include <string.h>
#include "files_handler.h"

int check_file_extension(char *file) {
    char *extensao = strrchr(file, '.');

    if (strcmp(extensao, ".asm") == 0) {
        return 1;
    }

    else if (strcmp(extensao, ".pre") == 0) {
        return 2;
    }

    else {
        return 0;
    }
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