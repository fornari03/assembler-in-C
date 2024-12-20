#include "utils.h"
#include <ctype.h>
#include <string.h>

char* to_upper(char *str) {
    for (char *ptr = str; *ptr != '\0'; ptr++) {
        *ptr = toupper(*ptr);
    }
    return str;
}