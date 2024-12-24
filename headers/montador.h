#ifndef MONTADOR_H
#define MONTADOR_H

int main(int argc, char *argv[]);
void assemble(char *file_name);
bool validate_symbol(char *symbol, bool is_label);
bool is_label(char *token);

#endif