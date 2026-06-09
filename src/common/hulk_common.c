#include "hulk_common.h"

char* str_format(const char *format, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = vsnprintf(NULL, 0, format, args_copy) + 1;
    va_end(args_copy);
    
    char *buffer = (char*)malloc(size);
    if (buffer)
    {
        vsnprintf(buffer, size, format, args);
    }
    
    return buffer;
}

char* str_toupper(char *str)
{
    if (!str) 
        return NULL;

    for (char *p = str; *p; ++p)
        *p = (char)toupper((unsigned char)*p);

    return str;
}