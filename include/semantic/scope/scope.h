#ifndef HULK_SCOPE_H
#define HULK_SCOPE_H

#include "type_descriptor.h"
#include <llvm-c/Core.h>

/**
 * @brief Scope léxico para variables durante el análisis semántico y codegen.
 *
 * Forma una cadena hacia `parent`. Cada scope contiene una lista de
 * `SymbolEntry*` que se destruyen al destruir el scope.
 */
typedef struct Scope
{
    struct Scope *parent;
    List *symbols;
} Scope;

/**
 * @brief Crea un nuevo scope de variables.
 *
 * @param parent Scope padre o NULL para el scope raíz.
 * @return Nuevo scope, o NULL si falla la asignación de memoria.
 */
Scope *scope_create(Scope *parent);

/**
 * @brief Destruye un scope y todas sus entradas.
 *
 * @param scope Scope a destruir. Si es NULL no hace nada.
 */
void scope_destroy(Scope *scope);

/**
 * @brief Añade una variable al scope actual.
 *
 * @param scope Scope donde añadir.
 * @param name  Nombre de la variable.
 * @param type  Tipo de la variable.
 * @return true si se añadió correctamente, false si ya existe en este scope o falló memoria.
 */
bool scope_add_variable(Scope *scope, const char *name, TypeDescriptor *type);

/**
 * @brief Añade un parámetro al scope actual.
 *
 * @param scope Scope donde añadir.
 * @param name  Nombre del parámetro.
 * @param type  Tipo del parámetro.
 * @return true si se añadió correctamente, false si ya existe o falló memoria.
 */
bool scope_add_parameter(Scope *scope, const char *name, TypeDescriptor *type);

/**
 * @brief Añade una constante al scope actual.
 *
 * @param scope Scope donde añadir.
 * @param name  Nombre de la constante.
 * @param type  Tipo de la constante.
 * @return true si se añadió correctamente, false si ya existe o falló memoria.
 */
bool scope_add_constant(Scope *scope, const char *name, TypeDescriptor *type);

/**
 * @brief Añade la instancia 'self' al scope actual.
 *
 * @param scope Scope donde añadir.
 * @param type  Tipo de 'self'.
 * @return true si se añadió correctamente, false si ya existe o falló memoria.
 */
bool scope_add_self_instance(Scope *scope, TypeDescriptor *type);

/**
 * @brief Busca un símbolo en la jerarquía completa de scopes.
 *
 * @param scope Scope donde iniciar la búsqueda (incluye ancestros).
 * @param name  Nombre del símbolo.
 * @return Tipo del símbolo si existe, NULL en caso contrario.
 */
TypeDescriptor *scope_lookup(Scope *scope, const char *name);

/**
 * @brief Verifica si un símbolo es constante.
 *
 * @param scope Scope donde iniciar la búsqueda.
 * @param name  Nombre del símbolo.
 * @return true si es constante, false si no existe o no lo es.
 */
bool scope_is_constant(Scope *scope, const char *name);

/**
 * @brief Verifica si un símbolo es un parámetro.
 *
 * @param scope Scope donde iniciar la búsqueda.
 * @param name  Nombre del símbolo.
 * @return true si es parámetro, false si no existe o no lo es.
 */
bool scope_is_parameter(Scope *scope, const char *name);

/**
 * @brief Verifica si un símbolo es la instancia 'self'.
 *
 * @param scope Scope donde iniciar la búsqueda.
 * @param name  Nombre del símbolo.
 * @return true si es 'self', false en caso contrario.
 */
bool scope_is_self_instance(Scope *scope, const char *name);

/**
 * @brief Marca una variable como errónea en el scope actual.
 *
 * Si la variable ya existe, activa su flag de error.
 * Si no existe, crea una entrada con tipo NULL y error_flag = true.
 * Esto evita reportar múltiples veces "Undefined variable" para la misma variable.
 *
 * @param scope Scope donde marcar.
 * @param name  Nombre de la variable.
 */
void scope_mark_variable_error(Scope *scope, const char *name);

/**
 * @brief Verifica si una variable tiene el flag de error activado.
 *
 * @param scope Scope donde buscar.
 * @param name  Nombre de la variable.
 * @return true si la variable está marcada como error, false en caso contrario.
 */
bool scope_is_error_flag(Scope *scope, const char *name);

/**
 * @brief Actualiza el tipo de un símbolo existente.
 *
 * @param scope Scope donde iniciar la búsqueda.
 * @param name  Nombre del símbolo.
 * @param type  Nuevo tipo a asignar.
 */
void scope_update_symbol(Scope *scope, const char *name, TypeDescriptor *type);

/**
 * @brief Verifica si un símbolo existe solo en el scope actual (no ancestros).
 *
 * @param scope Scope donde buscar.
 * @param name  Nombre del símbolo.
 * @return true si existe en este scope, false en caso contrario.
 */
bool scope_lookup_current(Scope *scope, const char *name);

/**
 * @brief Destruye una cadena de scopes hasta (sin incluir) `target`.
 *
 * @param scope  Scope inicial (se libera).
 * @param target Scope donde detener la destrucción (no se libera).
 */
void scope_destroy_until(Scope *scope, Scope *target);

/**
 * @brief Obtiene el alloca LLVM de un símbolo.
 *
 * @param scope Scope donde iniciar la búsqueda.
 * @param name  Nombre del símbolo.
 * @return LLVMValueRef del alloca, o NULL si no existe.
 */
LLVMValueRef scope_get_alloca(Scope *scope, const char *name);

/**
 * @brief Establece el alloca LLVM de un símbolo.
 *
 * @param scope  Scope donde iniciar la búsqueda.
 * @param name   Nombre del símbolo.
 * @param alloca LLVMValueRef del alloca a asignar.
 */
void scope_set_alloca(Scope *scope, const char *name, LLVMValueRef alloca);

#endif