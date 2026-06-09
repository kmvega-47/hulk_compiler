#ifndef HULK_SYMBOL_BINDING_H
#define HULK_SYMBOL_BINDING_H

#include "global_tables.h"
#include "diagnostic_manager.h"

typedef struct ASTNode ASTNode;

/**
 * @brief Representa la declaración de una variable o parámetro.
 *
 * Campos:
 * 
 *   - name:        Nombre de la variable/parámetro.
 * 
 *   - type_name:   Nombre del tipo anotado, o NULL si no tiene anotación.
 * 
 *   - initializer: Expresión de inicialización (toma ownership del ASTNode),
 *                  o NULL para parámetros.
 *   - return_type: Tipo resuelto del binding (NULL hasta que se infiera/resuelva).
 * 
 *   - line:        Número de línea donde se declaró.
 * 
 *   - column:      Número de columna donde se declaró.
 */
typedef struct SymbolBinding
{
    char *name;
    char *type_name;
    ASTNode *initializer;
    TypeDescriptor *return_type;
    int line;
    int column;
} SymbolBinding;

/**
 * @brief Crea una nueva declaración de símbolo.
 *
 * @param name        Nombre del símbolo. No puede ser NULL.
 * @param type_name   Tipo anotado (se copia internamente). Puede ser NULL.
 * @param initializer Nodo AST de inicialización (toma ownership). Puede ser NULL.
 * @param line        Número de línea de la declaración.
 * @param column      Número de columna de la declaración.
 * @return Puntero al SymbolBinding creado, o NULL si falla la asignación o name es NULL.
 */
SymbolBinding *symbol_binding_create(const char *name, const char *type_name, ASTNode *initializer, int line, int column);

/**
 * @brief Destruye una declaración de símbolo.
 *
 * @param binding Puntero al SymbolBinding a destruir. Si es NULL, no hace nada.
 *
 * @note No libera el nodo `initializer` del AST ni `return_type`.
 */
void symbol_binding_destroy(SymbolBinding *binding);

/**
 * @brief Obtiene una lista con los nombres de los parámetros sin anotación.
 *
 * @param bindings Lista de SymbolBinding*.
 * @return Lista de char* con los nombres de los SymbolBinding sin type_name.
 *         La lista debe liberarse con list_destroy (no se liberan los nombres).
 */
List *get_unannotated_params(List *bindings);

/**
 * @brief Construye una lista de TypeDescriptor a partir de una lista de SymbolBinding.
 *
 * Para cada binding, obtiene el tipo desde la anotación. Si no tiene anotación,
 * retorna NULL para ese parámetro (el caller deberá inferirlo luego).
 * Si la anotación no existe en la tabla, reporta error y retorna NULL para ese parámetro.
 *
 * @param bindings Lista de SymbolBinding*.
 * @param dm       Gestor de diagnósticos para reportar errores.
 * @return Lista de TypeDescriptor* creada, o NULL si falla la asignación.
 */
List *build_param_types(List *bindings, diagnostic_manager_t *dm);

#endif