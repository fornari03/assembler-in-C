#include <string.h>
#include "pre_process.h"
#include "files_handler.h"
#include "utils.h"
#include <map>
#include <string>

using namespace std;

map<string, int> macro_name_table;
vector<char*> macro_definition_table = {(char*)""};

/*Realiza todo o processo de pré-processamento do programa .asm e cria um arquivo .pre*/
void pre_process(char *file_name) {
    FILE *file = open_file(file_name);
    char *linha = NULL; // ponteiro para a linha
    size_t len = 0;
    bool last_line = false;

    char ext[] = ".pre";
    char* pre_file_name = change_file_extension(file_name, ext);
    if (!strcmp(pre_file_name, file_name)) {
        strcat(pre_file_name, "1");
    }
    FILE *pre_file = create_file(pre_file_name);
    char section_text[] = "SECTION TEXT";
    char section_data[] = "SECTION DATA";
    bool sections_found = false;
    bool data_first = false;
    bool section_text_found = false;
    vector<char*> data_lines;
    bool macro = false;

    while (getline(&linha, &len, file) != -1) {
        off_t current_pos = ftell(file);
        char *next_line = NULL;
        size_t next_len = 0;

        if (strstr(to_upper(linha), section_text) && !sections_found) {
            sections_found = true;
            // continue;
        }

        if (strstr(to_upper(linha), section_data) && !sections_found) {
            sections_found = true;
            data_first = true;
            // continue;
        }

        if (data_first && !strstr(to_upper(linha), section_text) && !section_text_found) {
            char *aux = new char[strlen(linha) + 1];
            strcpy(aux, linha);
            data_lines.push_back(aux);
        }
        else {
            if (strstr(to_upper(linha), section_text)) {
                section_text_found = true;
                // continue;
            }
            if (strstr(to_upper(linha), section_data)) {
                // continue;
            }
            if (getline(&next_line, &next_len, file) == -1 && !data_first) {
                last_line = true;
            } 
            else
                fseek(file, current_pos, SEEK_SET);
            free(next_line);

            if (is_end_macro(linha)) {
                macro = false;
                char* copy = strdup(linha);
                macro_definition_table.push_back(copy);
                continue;
            }
            if (is_macro(linha)) {
                macro = true;
                char *ptr = linha;
                while (*++ptr != ':') {}
                *ptr = '\0';
                string macro_name = to_upper(linha);
                macro_name_table[macro_name] = macro_definition_table.size();
                continue;
            }
            if (macro) {
                linha = remove_comments(linha);
                linha = remove_spaces(linha, last_line);
                linha = is_just_label(linha, last_line);
                if (!isspace(*linha)) {
                    char* copy = strdup(linha);
                    macro_definition_table.push_back(copy);
                }
                continue;
            }
            linha = remove_comments(linha);
            linha = remove_spaces(linha, last_line);
            linha = is_just_label(linha, last_line);
            if (!isspace(*linha) && is_macro_call(linha)) {
                char* copy = strdup(linha);
                expand_macro(pre_file, copy);
            }
            else if (!isspace(*linha)) write_file(pre_file, linha);
        }

    }

    if (data_first) {
        for (size_t i = 0; i < data_lines.size(); i++) {
            linha = remove_comments(data_lines[i]);
            linha = remove_spaces(linha, data_lines.size()-1 == i);
            linha = is_just_label(linha, data_lines.size()-1 == i);
            if (!isspace(*linha) && is_macro_call(linha)) {
                char* copy = strdup(linha);
                expand_macro(pre_file, copy);
            }
            else if (!isspace(*linha)) write_file(pre_file, linha);
        }
        for (char* line : data_lines) {
            free(line);
        }
    }

    fclose(file);
    fclose(pre_file);
}

char* remove_spaces(char *str, bool last_line) {
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
            if (!space_found && *(read + 1) != ',' && *(write - 1) != ',' && *(read + 1) != '+' && *(write - 1) != '+') {
                *write++ = ' '; // deixa só um espaço se não for vírgula
                space_found = true;
            }
        } 
        else if (*read == ',') {
            // tira todos espaços antes da vírgula
            while (write > str && isspace(*(write - 1))) {
                write--;
            }
            *write++ = ',';
            space_found = true;
        } 
        else if (*read == '+') {
            // tira todos espaços antes do +
            while (write > str && isspace(*(write - 1))) {
                write--;
            }
            *write++ = '+';
            space_found = true;
        }
        else if (*read == ':') {
            // tira todos espaços antes do :
            while (write > str && isspace(*(write - 1))) {
                write--;
            }
            *write++ = ':';
            space_found = false;
        }
        else {
            *write++ = *read;
            space_found = false;
        }
    }

    // tira espaços finais
    if (write > str && isspace(*(write - 1))) {
        write--;
    }

    if (!last_line) *write++ = '\n';
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

void expand_macro(FILE *file, char *line) {
    if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';
    for (int i = macro_name_table[to_upper(line)]; strcmp(to_upper(macro_definition_table[i]), (char*)"ENDMACRO\n"); i++) {
        if (is_macro_call(to_upper(macro_definition_table[i]))) {
            char* copy = strdup(macro_definition_table[i]);
            expand_macro(file, copy);
        }
        else
            write_file(file, macro_definition_table[i]);
    }

}

bool is_macro(char *str) {
    return strstr(to_upper(str), "MACRO");
}

bool is_macro_call(char *str) {
    char* copy = strdup(str);
    if (copy[strlen(copy)-1] == '\n') copy[strlen(copy)-1] = '\0';
    string s = to_upper(copy);
    return macro_name_table.find(to_upper(copy)) != macro_name_table.end();
}

bool is_end_macro(char *str) {
    return strstr(to_upper(str), "ENDMACRO");
}

char* is_just_label(char *str, bool last_line) {
    for (char *ptr = str; *ptr != '\0' && *ptr != '\n'; ptr++) {
        if (*ptr == ':' && (*(ptr+1) == '\n' || *(ptr+1) == '\0')) {
            if (!last_line) *(ptr+1) = ' ';
            else *(ptr+1) = '\0';
            break;
        }
    }
    return str;
}