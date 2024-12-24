#include <stdio.h>
#include "ligador.h"
#include "files_handler.h"

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
    for (int i = 0; files_names[i] != NULL; i++) {
        FILE *file = open_file(files_names[i]);
        char *line = NULL;
        size_t len = 0;
        while (getline(&line, &len, file) != -1) {
            continue;
            // TODO: implementar ligação
        }
    }
}