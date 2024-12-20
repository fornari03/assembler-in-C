#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "montador.h"
#include "pre_process.h"
#include "files_handler.h"

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

    while (getline(&linha, &len, file) != -1) {
        continue;
        // TODO: implementar a montagem
    }
    free(linha);

    fclose(file);

    // TODO: implementar a escrita do arquivo objeto (.obj)
}