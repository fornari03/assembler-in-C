#include <stdlib.h>
#include <ctype.h>
#include "pre_process.h"
#include "files_handler.h"

void pre_process(char *file_name) {
    FILE *file = open_file(file_name);
    char *linha = NULL; // ponteiro para a linha
    size_t len = 0;
    bool last_line = false;

    char ext[] = ".pre";
    char* pre_file_name = change_file_extension(file_name, ext);
    FILE *pre_file = create_file(pre_file_name);

    while (getline(&linha, &len, file) != -1) {
        off_t current_pos = ftell(file);
        char *next_line = NULL;
        size_t next_len = 0;

        if (getline(&next_line, &next_len, file) == -1) {
            last_line = true;
        } 
        else
            fseek(file, current_pos, SEEK_SET);
        free(next_line);

        linha = remove_comments(linha);
        linha = remove_spaces(linha, last_line);
        if (!isspace(*linha)) write_file(pre_file, linha);
    }

    free(linha);

    fclose(file);
    fclose(pre_file);
}

void expand_macros(char *file) {

}

char* remove_spaces(char *str, bool last_line) {
    if (!str) return str;

    char *read = str; // ponteiro pra leitura
    char *write = str; // ponteiro pra escrita
    bool space_found = false;

    // tira espaços iniciais
    while (isspace(*read)) {
        read++;
    }

    for (; *read != '\0'; read++) {
        if (isspace(*read)) {
            if (!space_found && *(read + 1) != ',' && *(write - 1) != ',') {
                *write++ = ' '; // deixa só um espaço se não for vírgula
                space_found = true;
            }
        } 
        else if (*read == ',') {
            // tira todos espaços antes da vírgula
            while (write > str && isspace(*(write - 1))) {
                write--;
            }
            *write++ = ','; // escreve a vírgula
            space_found = true;
        } 
        else {
            *write++ = *read; // se não é espaço, não mexe
            space_found = false;
        }
    }

    // tira espaços finais
    if (write > str && isspace(*(write - 1))) {
        write--;
    }

    if (!last_line) *write++ = '\n';
    *write = '\0';

    return str;
}

char* remove_comments(char *str) {
    if (!str) return str;

    for (char *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == ';') {
            *ptr++ = '\n';
            *ptr = '\0';
            break;
        }
    }

    return str;
}