#ifndef HULK_ENUMS_H
#define HULK_ENUMS_H

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
 * @brief Convierte un error_type_t a su representación en string.
 */
const char *error_type_to_string(error_type_t type);

/**
 * @brief Convierte un HulkType a su representación en string.
 */
const char *hulk_type_to_string(HulkType type);

#endif