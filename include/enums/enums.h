#ifndef HULK_ENUMS_H
#define HULK_ENUMS_H

#include "common.h"

/**
 * @brief Tipos de error del compilador HULK.
 */
typedef enum
{
    ERROR_TYPE_LEXICAL,
    ERROR_TYPE_SYNTACTIC,
    ERROR_TYPE_SEMANTIC
} error_type_t;

/**
 * @brief Tipos de datos del lenguaje HULK.
 */
typedef enum
{
    HULK_OBJECT,
    HULK_NUMBER, HULK_STRING, HULK_BOOL, HULK_VOID,
    HULK_USER_DEFINED,
} HulkType;

/**
 * @brief Tipos de nodos del AST.
 */
typedef enum
{
    NODE_PROGRAM,
    NODE_LITERAL, NODE_UNARY_OPERATION, NODE_BINARY_OPERATION, 
    NODE_EXPRESSION_BLOCK, NODE_CONDITIONAL, NODE_WHILE_LOOP,
    NODE_LET_IN, NODE_VARIABLE_REFERENCE, NODE_REASSIGNMENT,
    NODE_FUNCTION_DEFINITION, NODE_FUNCTION_CALL, NODE_TYPE_DEFINITION,
    NODE_ATTRIBUTE_ACCESS, NODE_TYPE_INSTANCIATION, NODE_METHOD_ACCESS
} ASTNodeType;

/**
 * @brief Operadores del lenguaje HULK.
 */
typedef enum
{
    OP_EQUAL, OP_NOT_EQUAL, OP_LESS, OP_LESS_EQUAL, OP_GREATER, OP_GREATER_EQUAL,
    OP_NOT,
    OP_ADD, OP_SUB, OP_MULT, OP_DIV, OP_MOD, OP_EXP,
    OP_AND, OP_OR,
    OP_CONCAT, OP_CONCAT_WS
} HulkOperator;

/**
 * @brief Convierte un error_type_t a su representación en string.
 */
const char *error_type_to_string(error_type_t type);

/**
 * @brief Convierte un HulkType a su representación en string.
 */
const char *hulk_type_to_string(HulkType type);

/**
 * @brief Convierte un tipo de nodo a su representación como string.
 * 
 * @param type El tipo a convertir.
 * @return Representación en texto del tipo.
 */
const char* ast_node_type_to_string(ASTNodeType type);

/**
 * @brief Convierte un operador a su representación como string.
 * 
 * @param op El operador a convertir.
 * @return const char* Representación en texto del operador.
 */
const char* operator_to_string(HulkOperator op);

/**
 * @brief Convierte un operador a su representación como string.
 * 
 * @param op El operador a convertir.
 * @return const char* Representación en texto del operador.
 */
const char* operator_to_string(HulkOperator op);

/**
 * @brief Determina si un operador es aritmético.
 * 
 * @param op El operador a verificar.
 * @return true si el operador es aritmético, false en otro caso.
 */
bool is_arithmetic_operator(HulkOperator op);

/**
 * @brief Determina si un operador es de comparación.
 * 
 * @param op El operador a verificar.
 * @return true si el operador es de comparación, false en otro caso.
 */
bool is_comparison_operator(HulkOperator op);

/**
 * @brief Determina si un operador es lógico.
 * 
 * @param op El operador a verificar.
 * @return true si el operador es lógico, false en otro caso.
 */
bool is_logical_operator(HulkOperator op);

/**
 * @brief Determina si un operador es de concatenación de strings.
 * 
 * @param op El operador a verificar.
 * @return true si el operador es de concatenación, false en otro caso.
 */
bool is_string_operator(HulkOperator op);

#endif