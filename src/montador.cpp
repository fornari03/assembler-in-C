#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <map>
#include <string>
#include "montador.h"
#include "pre_process.h"
#include "files_handler.h"
#include "opcodes.h"
#include "diretivas.h"
#include "utils.h"

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
    map<string, int> symbol_table;

    while (getline(&linha, &len, file) != -1) {
        vector<char*> tokens = split_line(linha);
        if (is_label(tokens[0])) {
            if (validate_symbol(tokens[0])) {
                tokens[0] = to_upper(tokens[0]);
                string label = tokens[0];
                if (symbol_table.find(label) == symbol_table.end()) {
                    symbol_table[label] = contador_posicao;
                }
                else {
                    printf("ERRO SEMÂNTICO (label redefinida): linha %d\n", contador_linha);
                    // TODO: erro semântico
                }
            }
            else {
                printf("ERRO SEMÂNTICO: linha %d\n", contador_linha);
                // TODO: erro semântico
            }
            tokens.erase(tokens.begin());
        }
        if (tokens.size() > 0) { // se linha não é só a label
            tokens[0] = to_upper(tokens[0]);
            if (INSTRUCTIONS_TABLE.find(tokens[0]) != INSTRUCTIONS_TABLE.end()) {
                contador_posicao += INSTRUCTIONS_TABLE[tokens[0]].second;
            }
            else {
                if (is_directive(tokens[0])) {
                    contador_posicao += get_directive_size(tokens);
                }
                else {
                    printf("ERRO SINTÁTICO (operação não existe): linha %d\n", contador_linha);
                    // TODO: erro sintático
                }
            }
        }
        contador_linha++;
    }

    fclose(file);


    // SEGUNDA PASSAGEM
    file = open_file(file_name);
    linha = NULL;
    len = 0;
    contador_posicao = 0;
    contador_linha = 1;
    vector<string> obj_code;

    while (getline(&linha, &len, file) != -1) {
        vector<char*> tokens = split_line(linha);
        if (is_label(tokens[0])) tokens.erase(tokens.begin());
        if (tokens.size() > 0) {
            tokens[0] = to_upper(tokens[0]);
            if (INSTRUCTIONS_TABLE.find(tokens[0]) != INSTRUCTIONS_TABLE.end()) {
                contador_posicao += INSTRUCTIONS_TABLE[tokens[0]].second;
                if (tokens.size() == INSTRUCTIONS_TABLE[tokens[0]].second) {
                    obj_code.push_back(INSTRUCTIONS_TABLE[tokens[0]].first);
                    for (size_t i = 1; i < tokens.size(); i++) {
                        if (validate_symbol(tokens[i])) {
                            tokens[i] = to_upper(tokens[i]);
                            if (symbol_table.find(tokens[i]) != symbol_table.end()) {
                                obj_code.push_back(int_to_string(symbol_table[tokens[i]]));
                            }
                            else {
                                printf("ERRO SEMÂNTICO (símbolo não definido): linha %d\n", contador_linha);
                                // TODO: erro semântico
                            }
                        }
                        else {
                            printf("ERRO LÉXICO (símbolo inválido): linha %d\n", contador_linha);
                            // TODO: erro léxico
                        }
                    }
                }
                else {
                    printf("ERRO SINTÁTICO (número de operandos incorreto): linha %d\n", contador_linha);
                    // TODO: erro sintático
                }
            }
            else {
                if (is_directive(tokens[0])) {
                    contador_posicao += get_directive_size(tokens);
                    vector<char*> obj = execute_directive(tokens);
                    obj_code.insert(obj_code.end(), obj.begin(), obj.end());
                }
                else {
                    printf("ERRO SINTÁTICO (operação não existe): linha %d\n", contador_linha);
                }
            }
        }
        contador_linha++;
    }

    FILE *obj_file = create_file(obj_file_name);
    char space[] = " ";
    for (size_t i = 0; i < obj_code.size(); i++) {
        write_file(obj_file, (char*)obj_code[i].c_str());
        write_file(obj_file, space);
    }
}

vector<char*> split_line(char *line) {
    vector<char*> tokens;
    char *tok = strtok(line, " \n");
    if (!strcmp(to_upper(tok), (char*)"COPY")) {
        while (tok != NULL) {
            tokens.push_back(tok);
            tok = strtok(NULL, ", \n");
        }
    }
    else {
        while (tok != NULL) {
            tokens.push_back(tok);
            tok = strtok(NULL, " \n");
        }
    }
    return tokens;
}

bool validate_symbol(char *symbol) {
    if (is_label(symbol))
        symbol[strlen(symbol) - 1] = '\0';
    if (!isalpha(symbol[0]) && symbol[0] != '_') return false;

    for (char *ptr = symbol; *ptr != '\0'; ptr++) {
        if (!isalnum(*ptr) && *ptr != '_') return false;
    }
    return true;
}

bool is_label(char *token) {
    return token[strlen(token) - 1] == ':';
}

