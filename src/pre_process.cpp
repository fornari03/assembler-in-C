#include <stdlib.h>
#include <ctype.h>
#include "pre_process.h"
#include "files_handler.h"

void pre_process(char *file_name) {
    FILE *file = open_file(file_name);
    char *linha = NULL; // ponteiro para a linha
    size_t len = 0;

    char ext[] = ".pre";
    char* pre_file_name = change_file_extension(file_name, ext);
    FILE *pre_file = create_file(pre_file_name);

    while (getline(&linha, &len, file) != -1) {
        // TODO: expand_macros(linha);
        linha = remove_comments(linha);
        linha = remove_spaces(linha);
        write_file(pre_file, linha);
    }
    free(linha);

    fclose(file);
    fclose(pre_file);
}

void expand_macros(char *file) {

}

char* remove_spaces(char *str) {
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
            if (!space_found) {
                *write++ = ' '; // deixa só 1 espaço
                space_found = true;
            }
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

    *write++ = '\n';
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