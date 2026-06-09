#ifndef HULK_FREE_VISITOR_H
#define HULK_FREE_VISITOR_H

#include "visitor.h"

/**
 * @brief Visitor para liberar la memoria del AST.
 *
 * Recorre el AST y libera todos los nodos y sus recursos asociados.
 */
typedef struct FreeVisitor
{
    Visitor base;
} FreeVisitor;

/**
 * @brief Crea un nuevo visitor de liberación.
 *
 * @return Puntero al visitor creado, o NULL si falla la asignación.
 */
FreeVisitor *free_visitor_create(void);

/**
 * @brief Destruye el visitor de liberación.
 *
 * @param visitor Puntero al visitor a destruir.
 */
void free_visitor_destroy(FreeVisitor *visitor);

#endif