#ifndef HULK_TYPE_INFERENCE_VISITOR_H
#define HULK_TYPE_INFERENCE_VISITOR_H

#include "constraint_collector_visitor.h"
#include "scope.h"
#include "diagnostic_manager.h"
/**
 * @brief Visitor para inferencia de tipos.
 *
 * Recorre el AST en post-orden infiriendo los tipos de cada expresión.
 *
 * Campos:
 * 
 *   - base:               Visitor base.
 * 
 *   - current_scope:      Scope actual durante la inferencia.
 * 
 *   - current_type:       Tipo actual (al visitar un type definition).
 * 
 *   - current_method_name: Nombre del método actual (al visitar un método).
 */
typedef struct TypeInferenceVisitor
{
    Visitor base;
    Scope *current_scope;
    TypeDescriptor *current_type;
    const char *current_method_name;
} TypeInferenceVisitor;

/**
 * @brief Crea un nuevo visitor de inferencia de tipos.
 *
 * @return Puntero al visitor creado, o NULL si falla la asignación.
 */
TypeInferenceVisitor *type_inference_visitor_create(void);

/**
 * @brief Destruye el visitor de inferencia de tipos.
 *
 * @param visitor Puntero al visitor a destruir.
 */
void type_inference_visitor_destroy(TypeInferenceVisitor *visitor);

#endif