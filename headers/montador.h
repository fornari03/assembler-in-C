#ifndef MONTADOR_H
#define MONTADOR_H
#include <vector>
using namespace std;

int main(int argc, char *argv[]);
void assemble(char *file_name);
vector<char*> split_line(char* line);
bool validate_label(char *label);
bool is_label(char *token);

#endif