#include <string.h>
#include "diretivas.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

bool is_directive(char *token) {
    token = to_upper(token);
    return !strcmp(token, "SPACE") || !strcmp(token, "CONST") || !strcmp(token, "EXTERN") || !strcmp(token, "PUBLIC") || !strcmp(token, "BEGIN") || !strcmp(token, "END");
}

bool is_begin(char *token) {
    token = to_upper(token);
    return !strcmp(token, "BEGIN");
}

bool is_extern(char *token) {
    token = to_upper(token);
    return !strcmp(token, "EXTERN");
}

bool is_public(char *token) {
    token = to_upper(token);
    return !strcmp(token, "PUBLIC");
}

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

vector<char*> execute_directive(vector<char*> tokens) {
    vector<char*> code_obj;
    char* token = to_upper(tokens[0]);
    if (!strcmp(token, "SPACE")) {
        if (tokens.size() > 1) {
            for (int i = 0; i < atoi(tokens[1]); i++) {
                code_obj.push_back((char*)"0");
            }
        }
        else {
            code_obj.push_back((char*)"0");
        }
    }
    else if (!strcmp(token, "CONST")) {
        if (tokens.size() == 2) {
            int arg;
            if (strlen(tokens[1]) > 2 && tokens[1][0] == '0' && (tokens[1][1] == 'x' || tokens[1][1] == 'X')) {
                arg = (int)strtol(tokens[1], NULL, 16);
                code_obj.push_back(int_to_string(arg));
            }
            else {
                arg = atoi(tokens[1]);
                if (arg == 0 && tokens[1][0] != '0') {
                    printf("ERRO SINTÁTICO: CONST tem que ser numérico\n");
                    // TODO: erro sintático
                }
                else {
                    code_obj.push_back(tokens[1]);
                }
            }
        }
        else {
            printf("ERRO SINTÁTICO: quantidade de argumentos de CONST errada\n");
            // TODO: erro sintático
        }
    }

    return code_obj;
}