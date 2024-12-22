#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "montador.h"
#include "pre_process.h"
#include "files_handler.h"

using namespace std;

int main(int argc, char *argv[]) {
    // TODO: argc deve ser usado para a ligação
    if (argc != 2) {
        printf("tem que passar um arquivo .asm ou .pre\n");
        return 1;
    }

    char *file_name = argv[1];

    int extensao = check_file_extension(file_name); // 1 = .asm, 2 = .pre, 0 = inválido

    if (extensao == 1) pre_process(file_name);

    else if (extensao == 2) assemble(file_name);

    else if (!extensao) {
        printf("Formato do arquivo inválido\n");
        return 1;
    }

    return 0;
}


void assemble(char *file_name) {
    FILE *file = open_file(file_name);
    char *linha = NULL; // ponteiro para a linha
    size_t len = 0;

    char ext[] = ".obj";
    char* obj_file_name = change_file_extension(file_name, ext);

    // PRIMEIRA PASSAGEM
    int contador_posicao = 0;
    int contador_linha = 1;
    while (getline(&linha, &len, file) != -1) {
        vector<char*> toks = split_line(linha, &contador_posicao);
        if (is_label(toks[0])) {
            if (validate_label(toks[0])) {
                // TODO: coloca label na tabela de símbolos
            }
            else {
                printf("ERRO SEMÂNTICO: linha %d\n", contador_linha);
                // TODO: erro semântico
            }
        }
        contador_linha++;
    }

    free(linha);

    fclose(file);

    // TODO: implementar a escrita do arquivo objeto (.obj)
}

vector<char*> split_line(char *line, int *count) {
    vector<char*> tokens;
    char *tok = strtok(line, " ,\n");
    while (tok != NULL) {
        tokens.push_back(tok);
        tok = strtok(NULL, " ,\n");
        (*count)++;
    }
    return tokens;
}

bool validate_label(char *label) {
    label[strlen(label) - 1] = '\0';
    if (!isalpha(label[0]) && label[0] != '_') return false;

    for (char *ptr = label; *ptr != '\0'; ptr++) {
        if (!isalnum(*ptr) && *ptr != '_') return false;
    }
    return true;
}

bool is_label(char *token) {
    return token[strlen(token) - 1] == ':';
}