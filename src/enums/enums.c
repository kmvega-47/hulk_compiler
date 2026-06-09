#include "enums.h"

static const char *ERROR_TYPE_STRINGS[] = {
    [ERROR_TYPE_LEXICAL]  = "LEXICAL",
    [ERROR_TYPE_SYNTACTIC]   = "SYNTACTIC",
    [ERROR_TYPE_SEMANTIC] = "SEMANTIC",
};

static const char *HULK_TYPE_STRINGS[] = {
    [HULK_OBJECT]       = "Object",
    [HULK_NUMBER]       = "Number",
    [HULK_STRING]       = "String",
    [HULK_BOOL]         = "Bool",
    [HULK_VOID]         = "Void",
    [HULK_USER_DEFINED] = "UserDefined",
};

const char* error_type_to_string(error_type_t type)
{
    if (type < 0 || type >= (int)(sizeof(ERROR_TYPE_STRINGS) / sizeof(ERROR_TYPE_STRINGS[0])))
        return "UNKNOWN";
        
    return ERROR_TYPE_STRINGS[type];
}

const char* hulk_type_to_string(HulkType type)
{
    if (type < 0 || type >= (int)(sizeof(HULK_TYPE_STRINGS) / sizeof(HULK_TYPE_STRINGS[0])))
        return "Undefined Type";
        
    return HULK_TYPE_STRINGS[type];
}