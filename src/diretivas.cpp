#include <string.h>
#include "diretivas.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

bool is_directive(char *token) {
    token = to_upper(token);
    return !strcmp(token, "SPACE") || !strcmp(token, "CONST");
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
    return -1;
}

vector<char*> execute_directive(vector<char*> tokens) {
    vector<char*> code_obj;
    char* token = to_upper(tokens[0]);
    if (!strcmp(token, "SPACE")) {
        if (tokens.size() > 1) {
            for (int i = 0; i < atoi(tokens[1]); i++) {
                code_obj.push_back("00");
            }
        }
        else {
            code_obj.push_back("00");
        }
    }
    else if (!strcmp(token, "CONST")) {
        if (tokens.size() == 2) {
            int arg;
            char* out = NULL;
            if (strlen(tokens[1]) > 2 && tokens[1][0] == '0' && (tokens[1][1] == 'x' || tokens[1][1] == 'X')) {
                arg = (int)strtol(tokens[1], NULL, 16);
                sprintf(out, "%d", arg);
                code_obj.push_back(out);
            }
            else {
                arg = atoi(tokens[1]);
                sprintf(out, "%d", arg);
                code_obj.push_back(out);
            }
            free(out);
        }
        else {
            printf("ERRO SINTÁTICO: quantidade de argumentos de CONST errada\n");
            // TODO: erro sintático
        }
    }

    return code_obj;
}