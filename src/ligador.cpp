#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include "ligador.h"
#include "files_handler.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Tem que passar pelo menos um arquivo .obj\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        char *file_name = argv[i];
        int extensao = check_file_extension(file_name);
        if (extensao != 3) {
            printf("Formato do arquivo inválido\n");
            return 1;
        }
    }

    link(argv + 1);


    return 0;
}


void link (char** files_names) {
    map<string, int> global_definition_table;
    vector<pair<string, int>> global_use_table;
    vector<int> fator_correcao = {0};
    vector<string> global_reloc_bit_map;
    vector<int> linked_code;

    for (int i = 0; files_names[i] != NULL; i++) {
        FILE *file = open_file(files_names[i]);
        char *line = NULL;
        size_t len = 0;
        int code_size = linked_code.size();
        while (getline(&line, &len, file) != -1) {
            if (line[0] == 'D') {
                vector<char*> label_def = split_line(line);
                global_definition_table[label_def[1]] = atoi(label_def[2])+fator_correcao[i]; // ja aplica o fator de correção na tabela de definições
            }
            else if (line[0] == 'U') {
                vector<char*> symbol_use = split_line(line);
                global_use_table.push_back(make_pair(symbol_use[1], atoi(symbol_use[2])+fator_correcao[i])); // ja aplica o fator de correção na tabela de uso
            }
            else if (line[0] == 'R') {
                vector<char*> nums = split_line(line);
                for (size_t j = 1; j < nums.size(); j++) {
                    global_reloc_bit_map.push_back(nums[j]);
                }
            }
            else {
                vector<char*> nums = split_line(line);
                fator_correcao.push_back((int)nums.size());
                for (size_t j = 0; j < nums.size(); j++) {
                    if (global_reloc_bit_map[(int)j+code_size] == "1") {
                        linked_code.push_back(atoi(nums[j])+fator_correcao[i]); // ja aplica o fator de correção pra endereços relativos
                    }
                    else
                        linked_code.push_back(atoi(nums[j]));
                }
            }
        }
    }
    fator_correcao.pop_back();

    for (int i = 0; files_names[i] != NULL; i++) {
        printf("Fator de correção %s: %d\n", files_names[i], fator_correcao[i]);
    }

    for (auto use : global_use_table) {
        linked_code[use.second] += global_definition_table[use.first];
    }

    char ext[] = ".e";
    char *linked_file_name = change_file_extension(files_names[0], ext);
    FILE *linked_file = create_file(linked_file_name);

    char space[] = " ";
    for (int num : linked_code) {
        write_file(linked_file, int_to_string(num));
        write_file(linked_file, space);
    }

    fclose(linked_file);
}