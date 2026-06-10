#ifndef HULK_TYPE_CONSTRAINT_H
#define HULK_TYPE_CONSTRAINT_H

#include "enums.h"
#include "global_tables.h"

/**
 * @brief Restricción de tipo para un parámetro sin anotación.
 *
 * Campos:
 * 
 *   - param_name:    Nombre del parámetro al que se aplica la restricción.
 * 
 *   - kind:          Tipo de restricción (EQUAL, CONFORMS o CONFLICT).
 * 
 *   - allowed_types: Lista de TypeDescriptor* con los tipos permitidos.
 * 
 *   - lines:         Lista de líneas (int como void*) donde se generó la restricción.
 */
typedef struct TypeConstraint
{
    char *param_name;
    ConstraintKind kind;
    List *allowed_types;
    List *lines;
} TypeConstraint;

/**
 * @brief Crea una restricción a partir de un uso detectado.
 *
 * @param param_name Nombre del parámetro.
 * @param kind       Tipo de restricción (EQUAL o CONFORMS).
 * @param types      Array de tipos permitidos.
 * @param count      Número de tipos en el array.
 * @param line       Línea donde se detectó el uso (-1 si no se quiere especificar).
 * @return Puntero a la restricción creada, o NULL si falla.
 */
TypeConstraint *constraint_create(const char *param_name, ConstraintKind kind, TypeDescriptor **types, size_t count, int line);

/**
 * @brief Crea una restricción inicial para un parámetro sin anotación.
 *
 * La restricción inicial es CONFORMS a Object (todos los tipos conforman).
 *
 * @param param_name Nombre del parámetro.
 * @return Puntero a la restricción creada, o NULL si falla.
 */
TypeConstraint *constraint_create_initial(const char *param_name);

/**
 * @brief Destruye una restricción y libera su memoria.
 *
 * @param constraint Restricción a destruir.
 */
void constraint_destroy(TypeConstraint *constraint);

/**
 * @brief Crea una copia profunda de una restricción.
 *
 * @param c Restricción a clonar.
 * @return Puntero a la nueva restricción, o NULL si falla.
 */
TypeConstraint *constraint_clone(const TypeConstraint *c);

/**
 * @brief Verifica si una restricción está en conflicto.
 *
 * @param c Restricción a verificar.
 * @return true si está en conflicto, false en caso contrario.
 */
bool constraint_is_conflicted(const TypeConstraint *c);

/**
 * @brief Obtiene la última línea donde se generó una restricción.
 *
 * @param c Restricción.
 * @return Número de la última línea, o -1 si no hay.
 */
int constraint_last_line(const TypeConstraint *c);

/**
 * @brief Compara dos restricciones por nombre de parámetro.
 *
 * @param a Primera restricción.
 * @param b Segunda restricción.
 * @return 0 si tienen el mismo nombre, != 0 en caso contrario.
 */
int constraint_cmp_by_name(const TypeConstraint *a, const TypeConstraint *b);

/**
 * @brief Mezcla dos restricciones en una sola.
 *
 * @param a Primera restricción.
 * @param b Segunda restricción.
 * @return Nueva restricción con el resultado de la mezcla, o NULL si falla.
 *
 * @note La nueva restricción contiene la unión de las líneas de a y b.
 * @note Si la mezcla resulta en conflicto, retorna una restricción de tipo CONFLICT.
 */
TypeConstraint *constraint_merge(const TypeConstraint *a, const TypeConstraint *b);

/**
 * @brief Intenta resolver una restricción a un único tipo.
 *
 * @param c         Restricción a resolver.
 * @param out_type  Puntero donde se devuelve el tipo resultante (NULL si falla).
 * @return 0 si se resolvió a un único tipo, 1 si hay conflicto, 2 si hay múltiples tipos posibles.
 */
int constraint_resolve_type(const TypeConstraint *c, TypeDescriptor **out_type);

/**
 * @brief Convierte una restricción a su representación como string.
 *
 * @param c Restricción a convertir.
 * @return String con la representación (debe liberarse con free).
 */
char *constraint_to_string(const TypeConstraint *c);

/**
 * @brief Convierte un ConstraintKind a string.
 */
const char *constraint_kind_to_string(ConstraintKind kind);

#endif