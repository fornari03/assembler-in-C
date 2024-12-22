#ifndef OPCODES_H
#define OPCODES_H

#include <string>
#include <map>
using namespace std;

map<string, pair<string, int>> INSTRUCTIONS_TABLE = {
    {"ADD", {"00", 2}},
    {"SUB", {"01", 2}},
    {"MUL", {"03", 2}},
    {"DIV", {"04", 2}},
    {"JMP", {"05", 2}},
    {"JMPN", {"06", 2}},
    {"JMPP", {"07", 2}},
    {"JMPZ", {"08", 2}},
    {"COPY", {"09", 3}},
    {"LOAD", {"10", 2}},
    {"STORE", {"11", 2}},
    {"INPUT", {"12", 2}},
    {"OUTPUT", {"13", 2}},
    {"STOP", {"14", 1}}
};

#endif