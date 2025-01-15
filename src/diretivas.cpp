#include <string.h>
#include "diretivas.h"
#include "utils.h"
#include "errors_handler.h"

/*Verifica se o token é uma diretiva*/
bool is_directive(char *token) {
    token = to_upper(token);
    return !strcmp(token, "SPACE") || !strcmp(token, "CONST") || !strcmp(token, "EXTERN") || !strcmp(token, "PUBLIC") || !strcmp(token, "BEGIN") || !strcmp(token, "END");
}

/*Verifica se o token é a diretiva BEGIN*/
bool is_begin(char *token) {
    token = to_upper(token);
    return !strcmp(token, "BEGIN");
}

/*Verifica se o token é a diretiva EXTERN*/
bool is_extern(char *token) {
    token = to_upper(token);
    return !strcmp(token, "EXTERN");
}

/*Verifica se o token é a diretiva PUBLIC*/
bool is_public(char *token) {
    token = to_upper(token);
    return !strcmp(token, "PUBLIC");
}

/*Retorna o tamanho da diretiva com os operandos esperados*/
int get_directive_size(vector<char*> tokens) {
    char* token = to_upper(tokens[0]);
    if (!strcmp(token, "SPACE")) {
        if (tokens.size() > 1) {
            return atoi(tokens[1]);
        }
        return 1;
    }
    else if (!strcmp(token, "CONST")) {
        return 1;
    }
    else if (!strcmp(token, "EXTERN") || !strcmp(token, "PUBLIC")
            || !strcmp(token, "BEGIN") || !strcmp(token, "END")) {
        return 0;
    }
    return -10000;
}

/*Executa a diretiva retornando o valor esperado*/
vector<char*> execute_directive(vector<char*> tokens, int *contador_linha) {
    vector<char*> code_obj;
    char* token = to_upper(tokens[0]);
    if (!strcmp(token, "SPACE")) {
        if (tokens.size() == 1) {
            code_obj.push_back((char*)"0");
        }
        else if (tokens.size() == 2) {
            for (int i = 0; i < atoi(tokens[1]); i++) {
                code_obj.push_back((char*)"0");
            }
        }
        else {
            // throw AssemblerError("(Linha " + to_string(*contador_linha) + ") ERRO SINTÁTICO: quantidade de operandos inválida");
            printf("(Linha %d) ERRO SINTÁTICO: número de operandos incorreto para a diretiva \"%s\". Esperado 0 ou 1, encontrou %d\n", *contador_linha, tokens[0], (int)tokens.size()-1);
        }
    }
    else if (!strcmp(token, "CONST")) {
        if (tokens.size() == 2) {
            short arg;
            if (strlen(tokens[1]) > 2 && tokens[1][0] == '0' && (tokens[1][1] == 'x' || tokens[1][1] == 'X')) {
                arg = strtol(tokens[1], NULL, 16);
                code_obj.push_back(int_to_string(arg));
            }
            else {
                arg = atoi(tokens[1]);
                if (arg == 0 && tokens[1][0] != '0') {
                    // throw AssemblerError("(Linha " + to_string(*contador_linha) + ") ERRO SINTÁTICO: operando inválido");
                    printf("(Linha %d) ERRO SINTÁTICO: operando inválido \"%s\"\n", *contador_linha, tokens[1]);
                }
                else {
                    code_obj.push_back(tokens[1]);
                }
            }
        }
        else {
            // throw AssemblerError("(Linha " + to_string(*contador_linha) + ") ERRO SINTÁTICO: quantidade de operandos inválida");
            printf("(Linha %d) ERRO SINTÁTICO: número de operandos incorreto para a diretiva \"%s\". Esperado 1, encontrou %d\n", *contador_linha, tokens[0], (int)tokens.size()-1);
        }
    }
    else if (!strcmp(token, "BEGIN") || !strcmp(token, "EXTERN")) {
        if (tokens.size() != 1) {
            // throw AssemblerError("(Linha " + to_string(*contador_linha) + ") ERRO SINTÁTICO: quantidade de operandos inválida");
            printf("(Linha %d) ERRO SINTÁTICO: número de operandos incorreto para a diretiva \"%s\". Esperado 0, encontrou %d\n", *contador_linha, tokens[0], (int)tokens.size()-1);
        }
    }
    else if (!strcmp(token, "PUBLIC")) {
        if (tokens.size() != 2) {
            // throw AssemblerError("(Linha " + to_string(*contador_linha) + ") ERRO SINTÁTICO: quantidade de operandos inválida");
            printf("(Linha %d) ERRO SINTÁTICO: número de operandos incorreto para a diretiva \"%s\". Esperado 1, encontrou %d\n", *contador_linha, tokens[0], (int)tokens.size()-1);
        }
    }
    else if (!strcmp(token, "END")) {
        if (tokens.size() != 1) {
            // throw AssemblerError("(Linha " + to_string(*contador_linha) + ") ERRO SINTÁTICO: quantidade de operandos inválida");
            printf("(Linha %d) ERRO SINTÁTICO: número de operandos incorreto para a diretiva \"%s\". Esperado 0, encontrou %d\n", *contador_linha, tokens[0], (int)tokens.size()-1);
        }
    }

    return code_obj;
}