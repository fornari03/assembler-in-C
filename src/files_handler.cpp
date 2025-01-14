#include <string.h>
#include <stdlib.h>
#include "files_handler.h"

/*Verifica a extensão no nome do arquivo*/
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

/*Altera a extensão no nome do arquivo*/
char* change_file_extension(char *file, char *new_extension) {
    char *extensao = strrchr(file, '.');
    char *new_file = (char*) malloc(strlen(file) + strlen(new_extension) + 1);

    strncpy(new_file, file, extensao - file);
    new_file[extensao - file] = '\0';
    strcat(new_file, new_extension);

    return new_file;
}

/*Abre um arquivo*/
FILE* open_file(char *file) {
    FILE* opened_file = fopen(file, "r");

    if (opened_file == NULL)
        printf("Erro em manuseio de arquivo\n");

    return opened_file;
}

/*Cria um arquivo*/
FILE* create_file(char *file) {
    FILE* created_file = fopen(file, "w");

    if (created_file == NULL)
        printf("Erro em manuseio de arquivo\n");

    return created_file;
}

/*Escreve um texto em um arquivo*/
void write_file(FILE* file, char *code) {
    fputs(code, file);
}   

/*Fecha um arquivo*/
void delete_file(char *file) {
    int ret = remove(file);
    if (ret) printf("Erro em manuseio de arquivo\n");
}