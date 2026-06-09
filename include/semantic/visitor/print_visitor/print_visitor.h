#ifndef HULK_PRINT_VISITOR_H
#define HULK_PRINT_VISITOR_H

#include "visitor.h"

/**
 * @brief Visitor para imprimir el AST en formato legible.
 *
 * Recorre el AST y lo imprime con indentación para mostrar la jerarquía.
 * Útil para depuración y visualización de la estructura del programa.
 *
 * Campos:
 *   - base:         Visitor base.
 *   - indent_level: Nivel de indentación actual.
 */
typedef struct PrintVisitor
{
    Visitor base;
    int indent_level;
} PrintVisitor;

/**
 * @brief Crea un nuevo visitor de impresión.
 *
 * @return Puntero al visitor creado, o NULL si falla la asignación.
 */
PrintVisitor *print_visitor_create(void);

/**
 * @brief Destruye el visitor de impresión.
 *
 * @param visitor Puntero al visitor a destruir.
 */
void print_visitor_destroy(PrintVisitor *visitor);

#endif