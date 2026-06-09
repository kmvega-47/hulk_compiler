
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
 * @brief Convierte un error_type_t a su representación en string.
 *
 * @param type Tipo de error.
 * @return String con el nombre del tipo ("LEXICAL", "SYNTACTIC", "SEMANTIC").
 */
const char *error_type_to_string(error_type_t type);