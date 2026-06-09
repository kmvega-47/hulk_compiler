#ifndef HULK_TYPE_TABLE_H
#define HULK_TYPE_TABLE_H
#define BUILTIN_TYPES_COUNT 5

#include "user_type_descriptor.h"

/**
 * @brief Tabla de tipos para el compilador HULK.
 *
 * Almacena todos los tipos disponibles en el programa: los tipos built-in
 * (Object, Number, String, Bool, Void) y los tipos definidos por el usuario
 * mediante la keyword 'type'.
 *
 * Campos:
 *   - builtin_types: Vector de TypeDescriptor* de tamaño fijo (BUILTIN_TYPES_COUNT).
 *                    El índice coincide con el valor del enum HulkType.
 *
 *   - user_types:    Lista de UserTypeDescriptor* con los tipos definidos por el usuario.
 */
typedef struct TypeTable
{
    Vector *builtin_types;
    List *user_types;
} TypeTable;

/**
 * @brief Crea e inicializa una nueva tabla de tipos con los tipos built-in.
 *
 * @return Puntero a la TypeTable creada, o NULL si falla la asignación.
 */
TypeTable *type_table_create(void);

/**
 * @brief Destruye la tabla de tipos y libera toda su memoria.
 *
 * @param table Puntero a la tabla a destruir. Si es NULL, no hace nada.
 */
void type_table_destroy(TypeTable *table);

/**
 * @brief Inserta un tipo definido por el usuario en la tabla.
 *
 * @param table Tabla de tipos.
 * @param type  UserTypeDescriptor a insertar (toma ownership).
 * @return true si se insertó correctamente, false si ya existía un tipo con el mismo nombre
 *         o si el tipo es NULL.
 */
bool type_table_insert(TypeTable *table, UserTypeDescriptor *type);

/**
 * @brief Busca un tipo built-in por su tag.
 *
 * @param table Tabla de tipos.
 * @param tag   Tag del tipo (HULK_NUMBER, HULK_STRING, etc.). No debe ser HULK_USER_DEFINED.
 * @return Puntero al TypeDescriptor si existe, NULL si el tag está fuera de rango.
 */
TypeDescriptor *type_table_lookup_by_tag(TypeTable *table, HulkType tag);

/**
 * @brief Busca un tipo por su nombre (case-sensitive).
 *
 * Busca primero entre los built-in y luego entre los tipos definidos por el usuario.
 *
 * @param table Tabla de tipos.
 * @param name  Nombre del tipo.
 * @return Puntero al TypeDescriptor si existe, NULL en caso contrario.
 */
TypeDescriptor *type_table_lookup_by_name(TypeTable *table, const char *name);

#endif