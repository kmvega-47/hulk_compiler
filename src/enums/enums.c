#include "enums.h"

const char *error_type_to_string(error_type_t type)
{
    switch (type)
    {
        case ERROR_TYPE_LEXICAL:   
            return "LEXICAL";

        case ERROR_TYPE_SYNTACTIC: 
            return "SYNTACTIC";

        case ERROR_TYPE_SEMANTIC:  
            return "SEMANTIC";

        default:                   
            return "UNKNOWN";
    }
}