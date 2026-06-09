#ifndef HULK_FUNCTION_TABLE_H
#define HULK_FUNCTION_TABLE_H

#include "type_descriptor.h"
#include <llvm-c/Core.h>

typedef struct FunctionTable
{
    List *functions;
} FunctionTable;

/**
 * @brief Construye el nombre interno de un método a partir del nombre del tipo
 *        y del método.
 *
 * Formato: _<type_name>_<method_name>
 *
 * @param type_name   Nombre del tipo.
 * @param method_name Nombre del método.
 * @return String con el nombre interno (debe liberarse con free), o NULL si falla.
 */
char *function_table_build_method_name(const char *type_name, const char *method_name);

/**
 * @brief Crea una nueva tabla de funciones vacía.
 *
 * @return Puntero a la nueva tabla, o NULL si falla la asignación.
 */
FunctionTable *function_table_create(void);

/**
 * @brief Destruye una tabla de funciones y libera toda su memoria.
 *
 * @param table Puntero a la tabla a destruir. Si es NULL, no hace nada.
 */
void function_table_destroy(FunctionTable *table);

/**
 * @brief Verifica si existe una entrada en la tabla con el nombre especificado.
 *
 * @param table Tabla a consultar.
 * @param name  Nombre de la función buscada.
 * @return true si existe una entrada con dicho nombre, false en caso contrario.
 */
bool function_table_exists(FunctionTable *table, const char *name);

/**
 * @brief Obtiene el tipo de retorno de la función con el nombre buscado.
 *
 * @param table Tabla de funciones a consultar.
 * @param name  Nombre de la función a buscar.
 * @return TypeDescriptor* del tipo de retorno, o NULL si no se encuentra.
 */
TypeDescriptor *function_table_get_return_type(FunctionTable *table, const char *name);

/**
 * @brief Obtiene los tipos de los parámetros de la función con el nombre buscado.
 *
 * @param table Tabla de funciones a consultar.
 * @param name  Nombre de la función a buscar.
 * @return Lista de TypeDescriptor* con los tipos de los parámetros, NULL si no se encuentra.
 */
List *function_table_get_params_types(FunctionTable *table, const char *name);

/**
 * @brief Obtiene el LLVMValueRef de la función con el nombre buscado.
 *
 * @param table Tabla de funciones a consultar.
 * @param name  Nombre de la función a buscar.
 * @return LLVMValueRef de la función, o NULL si no se encuentra.
 */
LLVMValueRef function_table_get_fn(FunctionTable *table, const char *name);

/**
 * @brief Obtiene el LLVMTypeRef de la función con el nombre buscado.
 *
 * @param table Tabla de funciones a consultar.
 * @param name  Nombre de la función a buscar.
 * @return LLVMTypeRef de la función, o NULL si no se encuentra.
 */
LLVMTypeRef function_table_get_func_type(FunctionTable *table, const char *name);

/**
 * @brief Establece el LLVMValueRef de una función.
 *
 * @param table Tabla de funciones.
 * @param name  Nombre de la función.
 * @param fn    LLVMValueRef a asignar.
 */
void function_table_set_fn(FunctionTable *table, const char *name, LLVMValueRef fn);

/**
 * @brief Establece el LLVMTypeRef de una función.
 *
 * @param table     Tabla de funciones.
 * @param name      Nombre de la función.
 * @param func_type LLVMTypeRef a asignar.
 */
void function_table_set_func_type(FunctionTable *table, const char *name, LLVMTypeRef func_type);

/**
 * @brief Inserta una nueva entrada en la tabla de funciones.
 *
 * @param table        Tabla donde insertar.
 * @param name         Nombre de la función.
 * @param return_type  Tipo de retorno.
 * @param params_types Tipos de los parámetros (toma ownership).
 * @return true si se insertó correctamente, false en caso de fallo o si ya existe.
 */
bool function_table_insert(FunctionTable *table, const char *name, TypeDescriptor *return_type, List *params_types);

/**
 * @brief Actualiza la información de una entrada de función.
 *
 * @param table            Tabla de funciones.
 * @param name             Nombre de la función.
 * @param new_return_type  Nuevo tipo de retorno (NULL para no modificar).
 * @param new_params_types Nuevos tipos de parámetros (NULL para no modificar, toma ownership).
 * @return true si se actualizó correctamente, false en caso de fallo.
 */
bool function_table_update(FunctionTable *table, const char *name, TypeDescriptor *new_return_type, List *new_params_types);

#endif