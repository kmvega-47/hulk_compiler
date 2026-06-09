#include "common.h"

void raise_error(const char *error_name, const char *enviroment, const char *msg) 
{
    fprintf(stderr, "%s [%s] : %s\n", error_name, enviroment, msg);
    exit(EXIT_FAILURE);
}

size_t resolve_index(int index, size_t len, char* caller_name)
{
    if (len == 0)
        raise_error("IndexError", caller_name , "cannot resolve index on empty vector");

    if (index < 0)
        index = (int)len + index;

    if (index < 0 || (size_t)index >= len)
        raise_error("IndexOutOfRangeError", caller_name, "index out of range");

    return (size_t)index;
}