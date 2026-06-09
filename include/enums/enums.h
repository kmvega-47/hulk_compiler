
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
 *
 * @param type Tipo de error.
 * @return String con el nombre del tipo ("LEXICAL", "SYNTACTIC", "SEMANTIC").
 */
const char *error_type_to_string(error_type_t type);

/**
 * @brief Convierte un tipo HULK a su representación como string.
 * 
 * @param type El tipo a convertir.
 * @return Representación en texto del tipo.
 */
const char* hulk_type_to_string(HulkType type);