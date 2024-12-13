#include <stdlib.h>
#include "pre_process.h"

void pre_process(FILE *file) {
    char *linha = NULL; // ponteiro para a linha
    size_t len = 0;

    // TODO: fazer coleção pra armazenar as linhas

    while (getline(&linha, &len, file) != -1) {
        linha = remove_comments(linha);
        printf("%s", linha);
        // linha = remove_spaces(linha);
    }
    free(linha);

    // TODO: escrever as linhas no novo arquivo (.pre)
}

void expand_macros(char *file) {

}

char* remove_spaces(char *str) {
    return str;
}

char* remove_comments(char *str) {
    if (!str) return str;

    for (char *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == ';') {
            *ptr = '\0';
            break;
        }
    }

    return str;
}