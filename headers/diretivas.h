#ifndef DIRETIVAS_H
#define DIRETIVAS_H

#include <vector>
using namespace std;

bool is_directive(char *token);
int get_directive_size(vector<char*> tokens);
vector<char*> execute_directive(vector<char*> tokens);

#endif