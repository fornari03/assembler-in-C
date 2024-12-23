#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* to_upper(char *str) {
    char *copy = (char*)malloc(strlen(str) + 1);
    for (size_t i = 0; str[i] != '\0'; i++) {
        copy[i] = toupper((unsigned char)str[i]);
    }
    copy[strlen(str)] = '\0';

    return copy;
}

char* int_to_string(int value) {
    size_t size = 12;
    char* buffer = (char*)malloc(size);

    if (buffer == NULL)
        return NULL;

    sprintf(buffer, "%d", value);

    return buffer;
}