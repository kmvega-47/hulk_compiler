#ifndef HULK_GLOBAL_TABLES_H
#define HULK_GLOBAL_TABLES_H

#include "type_table.h"
#include "function_table.h"

/**
 * @brief Tabla global de tipos (builtins + definidos por el usuario).
 */
extern TypeTable *global_type_table;

/**
 * @brief Tabla global de funciones y métodos (builtins + definidas por el usuario).
 */
extern FunctionTable *global_function_table;

/**
 * @brief Inicializa las tablas globales.
 *
 * Crea la TypeTable con los tipos builtin y la FunctionTable
 * con las funciones builtin.
 */
void global_tables_init(void);

/**
 * @brief Destruye las tablas globales y libera su memoria.
 */
void global_tables_destroy(void);

/**
 * @brief Obtiene el TypeDescriptor correspondiente a una anotación de tipo
 *        buscando en la tabla de tipos global.
 *
 * @param type_name Nombre del tipo anotado (puede ser NULL).
 * @return TypeDescriptor* correspondiente, NULL si no hay anotación o no existe.
 */
TypeDescriptor *get_type_from_annotation(const char *type_name);

#endif