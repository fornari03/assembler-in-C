#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* to_upper(char *str) {
    for (char *ptr = str; *ptr != '\0'; ptr++) {
        *ptr = toupper(*ptr);
    }
    return str;
}

char* int_to_string(int value) {
    size_t size = 12;
    char* buffer = (char*)malloc(size);

    if (buffer == NULL)
        return NULL;

    sprintf(buffer, "%d", value);

    return buffer;
}