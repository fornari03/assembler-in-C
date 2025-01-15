#include <string.h>
#include "montador.h"
#include "pre_process.h"
#include "files_handler.h"
#include "opcodes.h"
#include "diretivas.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("tem que passar só 1 arquivo .asm ou .pre\n");
        return 1;
    }

    char *file_name = argv[1];

    int extensao = check_file_extension(file_name); // 1 = .asm, 2 = .pre, 3 e 0 = inválido

    if (extensao == 1) pre_process(file_name);

    else if (extensao == 2) {
        pre_process(file_name);
        assemble(strcat(file_name, "1"));
        delete_file(file_name); // deleta o arquivo temporário
    }

    else if (!extensao) {
        printf("Formato do arquivo inválido\n");
        return 1;
    }

    return 0;
}


/*Realiza o processo todo de montagem do programa e cria o arquivo .obj*/
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
        if (!strcmp(to_upper(tokens[0]), "SECTION")) {
            // se é SECTION, avança pra próxima linha
            contador_linha++;
            continue;
        }
        if (is_label(tokens[0])) {
            if (validate_symbol(tokens[0], true)) {
                tokens[0] = to_upper(tokens[0]);
                if (!strcmp(tokens[0], (char*)"BEGIN")) swap(tokens[0], tokens[1]); // se é BEGIN, inverte a ordem com o próximo token
                string label = tokens[0];
                if (symbol_table.find(label) == symbol_table.end()) {
                    symbol_table[label].first = contador_posicao;
                    if (is_extern(tokens[1]) && is_module)
                        symbol_table[label].second = true;
                }
                else {
                    // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                    // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SEMÂNTICO: label redefinida");
                    printf("(Linha %d) ERRO SEMÂNTICO: rótulo redefinido \"%s\"\n", contador_linha, label.c_str());
                }
            }
            else {
                // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO LÉXICO: label inválida");
                printf("(Linha %d) ERRO LÉXICO: rótulo inválido \"%s\"\n", contador_linha, tokens[0]);
            }
            tokens.erase(tokens.begin());
        }
        if (tokens.size() > 0) { // se linha não é só a label
            tokens[0] = to_upper(tokens[0]);
            if (is_label(tokens[0])) {
                // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SINTÁTICO: label dobrada na mesma linha");
                printf("(Linha %d) ERRO SINTÁTICO: rótulo dobrado na mesma linha \"%s\"\n", contador_linha, tokens[0]);
            }
            else {
                if (INSTRUCTIONS_TABLE.find(tokens[0]) != INSTRUCTIONS_TABLE.end()) {
                    contador_posicao += INSTRUCTIONS_TABLE[tokens[0]].second;
                }
                else {
                    if (is_directive(tokens[0])) {
                        contador_posicao += get_directive_size(tokens);
                        if (is_begin(tokens[0])) {
                            // verifica que o arquivo é pra ser ligado
                            is_module = true;
                        }
                        else if (is_public(tokens[0])) {
                            definition_table[to_upper(tokens[1])] = -1;
                        }
                    }
                    else {
                        // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                        // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SINTÁTICO: operação não existe");
                        printf("(Linha %d) ERRO SINTÁTICO: instrução ou diretiva inválida \"%s\"\n", contador_linha, tokens[0]);
                    }
                }
            }
        }
        else {
            // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
            // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SEMÂNTICO: símbolo não definido");
            printf("(Linha %d) ERRO SEMÂNTICO: rótulo não definido \"%s\"\n", contador_linha, tokens[0]);
        }
        contador_linha++;
    }

    fclose(file);

    // atualiza os valores dos símbolos públicos
    for (auto it = definition_table.begin(); it != definition_table.end(); it++) {
        if (symbol_table.find(it->first) != symbol_table.end())
            definition_table[it->first] = symbol_table[it->first].first;
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
        if (is_label(tokens[0])) {
            tokens[0][strlen(tokens[0])-1] = '\0';
            if (!strcmp(to_upper(tokens[0]), (char*)"BEGIN"))
                swap(tokens[0], tokens[1]); // se é BEGIN, inverte a ordem com o próximo token
            tokens.erase(tokens.begin()); // apaga a label porque ela não tem importância na segunda passagem
        }
        if (tokens.size() > 0) {
            tokens[0] = to_upper(tokens[0]);
            if (INSTRUCTIONS_TABLE.find(tokens[0]) != INSTRUCTIONS_TABLE.end()) {
                contador_posicao += INSTRUCTIONS_TABLE[tokens[0]].second;
                if ((int)tokens.size() == INSTRUCTIONS_TABLE[tokens[0]].second) {
                    obj_code.push_back(INSTRUCTIONS_TABLE[tokens[0]].first);
                    reloc_bit_map.push_back("0");
                    for (size_t i = 1; i < tokens.size(); i++) {
                        if (validate_symbol(tokens[i], false)) {
                            tokens[i] = to_upper(tokens[i]);
                            if (symbol_table.find(tokens[i]) != symbol_table.end()) {
                                obj_code.push_back(int_to_string(symbol_table[tokens[i]].first));
                                if (symbol_table[tokens[i]].second)
                                    reloc_bit_map.push_back("0");
                                else 
                                    reloc_bit_map.push_back("1");
                                if (symbol_table[tokens[i]].second) {
                                    // se é externo, adiciona na use_table
                                    if (i == 1 && INSTRUCTIONS_TABLE[tokens[0]].second == 3)
                                        use_table.push_back(make_pair(tokens[i], contador_posicao - 2));
                                    else
                                        use_table.push_back(make_pair(tokens[i], contador_posicao - 1));
                                }
                            }
                            else if (strchr(tokens[i], '+')) {
                                char *ptr = strchr(tokens[i], '+');
                                *ptr = '\0';
                                string symbol = tokens[i];
                                string number = ptr + 1;
                                if (symbol_table.find(symbol) != symbol_table.end()) {
                                    obj_code.push_back(int_to_string(symbol_table[symbol].first + atoi(number.c_str())));
                                    if (symbol_table[symbol].second)
                                        reloc_bit_map.push_back("0");
                                    else 
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
                                    // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                                    // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SEMÂNTICO: símbolo não definido");
                                    printf("(Linha %d) ERRO SEMÂNTICO: rótulo ausente \"%s\"\n", contador_linha, symbol.c_str());
                                }
                            }
                            else {
                                // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                                // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SEMÂNTICO: símbolo não definido");
                                printf("(Linha %d) ERRO SEMÂNTICO: rótulo ausente \"%s\"\n", contador_linha, tokens[i]);
                            }
                        }
                        else {
                            // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                            // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO LÉXICO: símbolo inválido");
                            printf("(Linha %d) ERRO LÉXICO: símbolo inválido \"%s\"\n", contador_linha, tokens[i]);
                        }
                    }
                }
                else {
                    // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                    // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SINTÁTICO: número de operandos incorreto");
                    printf("(Linha %d) ERRO SINTÁTICO: número de operandos incorreto para a instrução \"%s\". Esperado %d, encontrou %d\n", contador_linha, tokens[0], INSTRUCTIONS_TABLE[tokens[0]].second, (int)tokens.size()-1);
                }
            }
            else {
                if (is_directive(tokens[0])) {
                    if (!strcmp(tokens[0], (char*)"PUBLIC")) {
                        if (symbol_table.find(to_upper(tokens[1])) == symbol_table.end())
                            // if (file_name[strlen(file_name)-1] == '1') delete_file(file_name);
                            // throw AssemblerError("(Linha " + to_string(contador_linha) + ") ERRO SEMÂNTICO: símbolo não definido");
                            printf("(Linha %d) ERRO SEMÂNTICO: rótulo ausente \"%s\"\n", contador_linha, tokens[1]);
                    }
                    contador_posicao += get_directive_size(tokens);
                    vector<char*> obj = execute_directive(tokens, &contador_linha);
                    obj_code.insert(obj_code.end(), obj.begin(), obj.end());
                    for (size_t i = 0; i < obj.size(); i++) {
                        reloc_bit_map.push_back("0");
                    }
                }
            }
        }   
        contador_linha++;
    }  

    FILE *obj_file = create_file(obj_file_name);
    char space[] = " ";
    char new_line[] = "\n";
    if (is_module) {
        // escreve a tabela de definição, uso e infos de relocação
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

/*Valida o nome do símbolo, verificando se tem algum erro léxico*/
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

/*Verifica se o token é uma label*/
bool is_label(char *token) {
    return token[strlen(token) - 1] == ':';
}

