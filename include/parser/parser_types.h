#ifndef PARSER_TYPES_H
#define PARSER_TYPES_H

#include "ast.h"

typedef struct TypeBodyElem 
{
    int kind;       /* 0 = atributo (SymbolBinding*), 1 = método (ASTNode*) */
    union 
    {
        SymbolBinding* attr;
        ASTNode* method;
    } data;
    
} TypeBodyElem;

typedef struct TypeInheritance 
{
    char* father_name;   /* nombre del tipo padre (NULL si no hay herencia) */
    List* father_args;   /* lista de ASTNode* con los argumentos (NULL si no hay herencia) */
} TypeInheritance;

#endif