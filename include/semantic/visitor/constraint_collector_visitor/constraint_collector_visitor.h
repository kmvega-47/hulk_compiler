#ifndef CONSTRAINT_COLLECTOR_VISITOR_H
#define CONSTRAINT_COLLECTOR_VISITOR_H

#include "scope.h"
#include "visitor.h"
#include "type_constraint.h"

/**
 * @brief Visitor recolector de restricciones de tipo para parámetros sin anotación.
 *
 * Recorre el AST y genera restricciones de tipo (TypeConstraint) para aquellos
 * parámetros de funciones o tipos que no tienen anotación explícita.
 * Las restricciones se van acumulando y mezclando conforme se detectan usos
 * del parámetro en el cuerpo de la función o en los inicializadores de atributos.
 *
 * Campos:
 *   - base:          Visitor base.
 *   - constraints:   Lista de TypeConstraint* acumuladas.
 *   - current_scope: Scope actual durante la recolección.
 */
typedef struct ConstraintCollectorVisitor
{
    Visitor base;
    List *constraints;
    Scope *current_scope;
} ConstraintCollectorVisitor;

/**
 * @brief Crea un nuevo visitor recolector de restricciones.
 *
 * @param initial_scope      Scope inicial para búsqueda de símbolos.
 * @param unannotated_params Lista de nombres de parámetros sin anotación (char*) a vigilar.
 * @return Puntero al visitor creado, o NULL si falla la asignación.
 */
ConstraintCollectorVisitor *constraint_collector_create(Scope *initial_scope, List *unannotated_params);

/**
 * @brief Destruye el visitor recolector de restricciones.
 *
 * @param visitor Puntero al visitor a destruir.
 */
void constraint_collector_destroy(ConstraintCollectorVisitor *visitor);

/**
 * @brief Obtiene la restricción acumulada para un parámetro.
 *
 * @param visitor    Visitor recolector.
 * @param param_name Nombre del parámetro.
 * @return Puntero a la restricción si existe, NULL en caso contrario.
 */
TypeConstraint *constraint_collector_get(ConstraintCollectorVisitor *visitor, const char *param_name);

/**
 * @brief Agrega o mezcla una restricción para un parámetro.
 *
 * Si el parámetro no está en la lista de vigilados, la restricción se destruye.
 *
 * @param visitor    Visitor recolector.
 * @param constraint Restricción a agregar (toma ownership).
 */
void constraint_collector_add(ConstraintCollectorVisitor *visitor, TypeConstraint *constraint);

/**
 * @brief Resuelve todas las restricciones acumuladas y devuelve una lista de tipos.
 *
 * @param visitor Visitor recolector con las restricciones acumuladas.
 * @return Lista de TypeDescriptor* con los tipos resueltos.
 */
List *constraint_collector_resolve(ConstraintCollectorVisitor *visitor);

#endif