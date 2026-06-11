#ifndef HULK_TYPE_CHECK_VISITOR_H
#define HULK_TYPE_CHECK_VISITOR_H

#include "visitor.h"

/**
 * @brief Visitor para verificación de tipos.
 *
 * Recorre el AST verificando la corrección semántica después de la inferencia.
 *
 * Campos:
 *   - base: Visitor base.
 */
typedef struct TypeCheckVisitor
{
    Visitor base;
} TypeCheckVisitor;

/**
 * @brief Crea un nuevo visitor de verificación de tipos.
 *
 * @return Puntero al visitor creado, o NULL si falla la asignación.
 */
TypeCheckVisitor *type_check_visitor_create(void);

/**
 * @brief Destruye el visitor de verificación de tipos.
 *
 * @param visitor Puntero al visitor a destruir.
 */
void type_check_visitor_destroy(TypeCheckVisitor *visitor);

#endif