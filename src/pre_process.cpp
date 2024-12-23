#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "pre_process.h"
#include "files_handler.h"
#include "utils.h"
#include <vector>

using namespace std;

void pre_process(char *file_name) {
    FILE *file = open_file(file_name);
    char *linha = NULL; // ponteiro para a linha
    size_t len = 0;
    bool last_line = false;

    char ext[] = ".pre";
    char* pre_file_name = change_file_extension(file_name, ext);
    FILE *pre_file = create_file(pre_file_name);
    char section_text[] = "SECTION TEXT";
    char section_data[] = "SECTION DATA";
    bool sections_found = false;
    bool data_first = false;
    bool section_text_found = false;
    vector<char*> data_lines;

    while (getline(&linha, &len, file) != -1) {
        off_t current_pos = ftell(file);
        char *next_line = NULL;
        size_t next_len = 0;

        if (strstr(to_upper(linha), section_text) && !sections_found) {
            printf("section text found\n");
            sections_found = true;
            continue;
        }

        if (strstr(to_upper(linha), section_data) && !sections_found) {
            printf("section data found\n");
            sections_found = true;
            data_first = true;
            continue;
        }

        if (data_first && !strstr(to_upper(linha), section_text) && !section_text_found) {
            printf("data line found\n");
            char *aux = new char[strlen(linha) + 1];
            strcpy(aux, linha);
            data_lines.push_back(aux);
        }
        else {
            printf("else\n");
            if (strstr(to_upper(linha), section_text)) {
                section_text_found = true;
                continue;
            }
            if (strstr(to_upper(linha), section_data)) {
                continue;
            }
            if (getline(&next_line, &next_len, file) == -1 && !data_first) {
                last_line = true;
            } 
            else
                fseek(file, current_pos, SEEK_SET);
            free(next_line);

            linha = remove_comments(linha);
            linha = remove_spaces(linha, last_line);
            if (!isspace(*linha)) write_file(pre_file, linha);
        }

    }

    if (data_first) {
        for (size_t i = 0; i < data_lines.size(); i++) {
            printf("%s\n", linha);
            linha = remove_comments(data_lines[i]);
            printf("%s\n", linha);
            linha = remove_spaces(linha, data_lines.size()-1 == i);
            if (!isspace(*linha)) write_file(pre_file, linha);
        }
        for (char* line : data_lines) {
            free(line);
        }
    }

    fclose(file);
    fclose(pre_file);
}

void expand_macros(char *file) {

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