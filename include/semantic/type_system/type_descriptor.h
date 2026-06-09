#ifndef HULK_TYPE_DESCRIPTOR_H
#define HULK_TYPE_DESCRIPTOR_H

#include "hulk_common.h"
#include "enums.h"

/**
 * @brief Cantidad de tipos built-in (OBJECT, NUMBER, STRING, BOOL, VOID, más
 *        los que se agreguen).
 */
#define BUILTIN_TYPES_COUNT 5

/**
 * @brief Descriptor de un tipo en HULK.
 *
 * Soporta tanto tipos built-in como tipos definidos por el usuario.
 *
 * Campos:
 *   - tag:    Tipo HULK (OBJECT, NUMBER, STRING, BOOL, VOID, USER_DEFINED).
 *   - name:   Nombre del tipo (ej. "Number", "Object"). El descriptor toma
 *             posesión de esta cadena y la libera al destruirse.
 *   - parent: Puntero al tipo del que hereda (NULL para Object o built-in).
 */
typedef struct TypeDescriptor
{
    HulkType tag;
    char *name;
    struct TypeDescriptor *parent;
} TypeDescriptor;

/**
 * @brief Crea un nuevo descriptor de tipo.
 *
 * @param tag    Tipo HULK.
 * @param name   Nombre del tipo. Se almacena una copia interna (strdup).
 * @param parent Tipo del que hereda (puede ser NULL).
 * @return Puntero al TypeDescriptor creado, o NULL si falla la asignación.
 */
TypeDescriptor *type_create(HulkType tag, const char *name, TypeDescriptor *parent);

/**
 * @brief Destruye un descriptor de tipo y libera su memoria.
 *
 * @param type Puntero al tipo a destruir. Si es NULL, no hace nada.
 */
void type_destroy(TypeDescriptor *type);

/**
 * @brief Verifica si un tipo A conforma a un tipo B (A <= B).
 *
 * Un tipo A conforma a B si:
 *   - A y B son el mismo tipo (misma instancia).
 *   - A hereda de B (directa o indirectamente).
 *
 * @param a Tipo A.
 * @param b Tipo B.
 * @return true si A conforma a B, false si alguno es NULL o no conforma.
 */
bool type_conforms_to(const TypeDescriptor *a, const TypeDescriptor *b);

/**
 * @brief Obtiene el menor ancestro común (LCA) de dos tipos.
 *
 * @param a Tipo A.
 * @param b Tipo B.
 * @return LCA de a y b, o NULL si alguno es NULL.
 */
TypeDescriptor *type_lca(TypeDescriptor *a, TypeDescriptor *b);

/**
 * @brief Compara dos TypeDescriptor por igualdad de punteros.
 *
 * @param a Primer TypeDescriptor.
 * @param b Segundo TypeDescriptor.
 * @return 0 si son la misma instancia, != 0 en caso contrario.
 */
int type_cmp(const TypeDescriptor *a, const TypeDescriptor *b);

#endif