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
    map<string, pair<int, bool>> symbol_table;
    bool is_module = false;
    map<string, int> definition_table;

    while (getline(&linha, &len, file) != -1) {
        vector<char*> tokens = split_line(linha);
        if (is_label(tokens[0])) {
            if (validate_symbol(tokens[0], true)) {
                tokens[0] = to_upper(tokens[0]);
                string label = tokens[0];
                if (symbol_table.find(label) == symbol_table.end()) {
                    symbol_table[label].first = contador_posicao;
                    if (is_extern(tokens[1]) && is_module)
                        symbol_table[label].second = true;
                        // ISSUE: considerar um continue aqui
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
            if (is_label(tokens[0])) {
                printf("ERRO SINTÁTICO (label dobrada na mesma linha): linha %d\n", contador_linha);
                // TODO: erro sintático
            }
            else {
                if (INSTRUCTIONS_TABLE.find(tokens[0]) != INSTRUCTIONS_TABLE.end()) {
                    contador_posicao += INSTRUCTIONS_TABLE[tokens[0]].second;
                }
                else {
                    if (is_directive(tokens[0])) {
                        contador_posicao += get_directive_size(tokens);
                        if (is_begin(tokens[0])) {
                            is_module = true;
                        }
                        else if (is_public(tokens[0])) {
                            definition_table[to_upper(tokens[1])] = -1;
                        }
                    }
                    else {
                        printf("ERRO SINTÁTICO (operação não existe): linha %d\n", contador_linha);
                        // TODO: erro sintático
                    }
                }
            }
        }
        contador_linha++;
    }

    fclose(file);

    for (auto it = definition_table.begin(); it != definition_table.end(); it++) {
        definition_table[it->first] = symbol_table[it->first].first;
    }


    for (auto it = definition_table.begin(); it != definition_table.end(); it++) {
        printf("definition_table[%s]: %d\n", it->first.c_str(), it->second);
    }

    for (auto it = symbol_table.begin(); it != symbol_table.end(); it++) {
        printf("symbol_table[%s]: %d, %d\n", it->first.c_str(), it->second.first, it->second.second);
    }





    // SEGUNDA PASSAGEM
    file = open_file(file_name);
    linha = NULL;
    len = 0;
    contador_posicao = 0;
    contador_linha = 1;
    vector<string> obj_code;
    vector<pair<string, int>> use_table;
    vector<string> reloc_bit_map;

    while (getline(&linha, &len, file) != -1) {
        vector<char*> tokens = split_line(linha);
        if (is_label(tokens[0])) tokens.erase(tokens.begin());
        if (tokens.size() > 0) {
            tokens[0] = to_upper(tokens[0]);
            if (INSTRUCTIONS_TABLE.find(tokens[0]) != INSTRUCTIONS_TABLE.end()) {
                contador_posicao += INSTRUCTIONS_TABLE[tokens[0]].second;
                if (tokens.size() == INSTRUCTIONS_TABLE[tokens[0]].second) {
                    obj_code.push_back(INSTRUCTIONS_TABLE[tokens[0]].first);
                    reloc_bit_map.push_back("0");
                    for (size_t i = 1; i < tokens.size(); i++) {
                        if (validate_symbol(tokens[i], false)) {
                            tokens[i] = to_upper(tokens[i]);
                            if (symbol_table.find(tokens[i]) != symbol_table.end()) {
                                obj_code.push_back(int_to_string(symbol_table[tokens[i]].first));
                                reloc_bit_map.push_back("1");
                                if (symbol_table[tokens[i]].second) {
                                    // se é externo, adiciona na use_table
                                    if (i == 1 && INSTRUCTIONS_TABLE[tokens[0]].second == 3)
                                        use_table.push_back(make_pair(tokens[i], contador_posicao - 2));
                                    else
                                        use_table.push_back(make_pair(tokens[i], contador_posicao - 1));
                                }
                                //printf("symbol_table[%s]: %d\n", tokens[i], symbol_table[tokens[i]]);
                            }
                            else if (strchr(tokens[i], '+')) {
                                char *ptr = strchr(tokens[i], '+');
                                *ptr = '\0';
                                string symbol = tokens[i];
                                string number = ptr + 1;
                                if (symbol_table.find(symbol) != symbol_table.end()) {
                                    obj_code.push_back(int_to_string(symbol_table[symbol].first + atoi(number.c_str())));
                                    reloc_bit_map.push_back("1");
                                    if (symbol_table[symbol].second) {
                                        // se é externo, adiciona na use_table
                                        if (i == 1 && INSTRUCTIONS_TABLE[tokens[0]].second == 3)
                                            use_table.push_back(make_pair(symbol, contador_posicao - 2));
                                        else
                                            use_table.push_back(make_pair(symbol, contador_posicao - 1));
                                    }
                                }
                                else {
                                    printf("ERRO SEMÂNTICO (símbolo não definido): linha %d\n", contador_linha);
                                    // TODO: erro semântico
                                }
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
                    for (size_t i = 0; i < obj.size(); i++) {
                        reloc_bit_map.push_back("0");
                    }
                }
                else {
                    printf("ERRO SINTÁTICO (operação não existe): linha %d\n", contador_linha);
                }
            }
        }   
        contador_linha++;
    }

    for (size_t i = 0; i < use_table.size(); i++) {
        printf("use_table[%s]: %d\n", use_table[i].first.c_str(), use_table[i].second);
    }   

    FILE *obj_file = create_file(obj_file_name);
    char space[] = " ";
    char new_line[] = "\n";
    if (is_module) {
        char d[] = "D, ";
        for (auto it = definition_table.begin(); it != definition_table.end(); it++) {
            write_file(obj_file, d);
            write_file(obj_file, (char*)it->first.c_str());
            write_file(obj_file, space);
            write_file(obj_file, int_to_string(it->second));
            write_file(obj_file, new_line);
        }
        char u[] = "U, ";
        for (size_t i = 0; i < use_table.size(); i++) {
            write_file(obj_file, u);
            write_file(obj_file, (char*)use_table[i].first.c_str());
            write_file(obj_file, space);
            write_file(obj_file, int_to_string(use_table[i].second));
            write_file(obj_file, new_line);
        }
        char r[] = "R, ";
        write_file(obj_file, r);
        for (size_t i = 0; i < reloc_bit_map.size(); i++) {
            write_file(obj_file, (char*)reloc_bit_map[i].c_str());
            write_file(obj_file, space);
        }
        write_file(obj_file, new_line);
    }
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

bool validate_symbol(char *symbol, bool is_label) {
    if (is_label) {
        symbol[strlen(symbol) - 1] = '\0';
        if (!isalpha(symbol[0]) && symbol[0] != '_') return false;

        for (char *ptr = symbol; *ptr != '\0'; ptr++) {
            if (!isalnum(*ptr) && *ptr != '_') return false;
        }
        return true;
    }
    char *copy = strdup(symbol);
    if (!strchr(copy, '+')) {
        if (!isalpha(symbol[0]) && symbol[0] != '_') return false;

        for (char *ptr = symbol; *ptr != '\0'; ptr++) {
            if (!isalnum(*ptr) && *ptr != '_') return false;
        }
        return true;
    }
    // verifica se antes do + é um símbolo válido e se depois é um número
    char *ptr = strchr(copy, '+');
    *ptr = '\0';
    if (!validate_symbol(copy, false)) {
        free(copy);
        return false;
    }
    if (!isdigit(*(ptr + 1))) {
        free(copy);
        return false;
    }
    while (*++ptr != '\0') {
        if (!isdigit(*ptr)) {
            free(copy);
            return false;
        }
    }
    free(copy);
    return true;

}

bool is_label(char *token) {
    return token[strlen(token) - 1] == ':';
}

