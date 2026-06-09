#ifndef HULK_VISITOR_H
#define HULK_VISITOR_H

#include "ast.h"

/**
 * @brief Estructura base del Visitor.
 *
 * Contiene una lista de funciones de visita indexadas por ASTNodeType.
 * Cada visitor concreto asigna sus propias funciones durante la creación.
 */
typedef struct Visitor {
    List* visit_funcs;      /**< Lista de VisitorFunc (índice = ASTNodeType). */
} Visitor;

/**
 * @brief Tipo de función de visita para un nodo del AST.
 *
 * @param visitor Visitor que ejecuta la visita.
 * @param node    Nodo del AST a visitar.
 * @return Resultado dependiente del contexto, o NULL.
 */
typedef void* (*VisitorFunc)(Visitor* visitor, ASTNode* node);

/**
 * @brief Inicializa la estructura base de un visitor.
 *
 * @param visitor Puntero al visitor a inicializar.
 */
void visitor_init(Visitor* visitor);

/**
 * @brief Libera los recursos de la estructura base del visitor.
 *
 * @param visitor Puntero al visitor cuyo base se destruirá.
 */
void visitor_destroy(Visitor* visitor);

/**
 * @brief Registra un arreglo de funciones de visita en el orden natural del enum ASTNodeType.
 *
 * @param visitor Visitor donde se registrarán las funciones.
 * @param funcs   Arreglo de VisitorFunc con exactamente AST_NODE_TYPE_COUNT elementos.
 *                El índice i corresponde al ASTNodeType con valor i.
 *
 * @note El visitor debe haber sido inicializado previamente con visitor_init.
 * @note Si el visitor o su lista interna son NULL, la función no hace nada.
 * @note Las funciones se almacenan directamente; no se copian.
 */
void visitor_register_functions(Visitor* visitor, VisitorFunc funcs[]);

/**
 * @brief Ejecuta el patrón visitor sobre un nodo AST.
 *
 * @param node    Puntero al nodo a visitar.
 * @param visitor Puntero al visitor que procesará el nodo.
 * @return Puntero al resultado de la visita (depende del visitor), o NULL si no hay función registrada.
 */
void* ast_accept(ASTNode* node, Visitor* visitor);

#endif