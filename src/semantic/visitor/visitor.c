#include "visitor.h"

void visitor_init(Visitor* visitor)
{
    if (!visitor)
        return;

    // Crear lista con capacidad para todos los tipos de nodo, inicialmente con NULLs
    visitor->visit_funcs = list_create(AST_NODE_TYPE_COUNT, NULL, NULL, NULL, NULL);
}

void visitor_destroy(Visitor* visitor)
{
    if (!visitor)
        return;

    if (visitor->visit_funcs)
        list_destroy(visitor->visit_funcs);
}

void visitor_register_functions(Visitor* visitor, VisitorFunc funcs[])
{
    if (!visitor || !visitor->visit_funcs || !funcs)
        return;

    for (int i = 0; i < AST_NODE_TYPE_COUNT; i++)
    {
        list_set(visitor->visit_funcs, i, funcs[i]);
    }
}

void* ast_accept(ASTNode* node, Visitor* visitor)
{
    if (!node || !visitor || !visitor->visit_funcs)
        return NULL;

    VisitorFunc func = (VisitorFunc)(intptr_t)list_get(visitor->visit_funcs, node->node_type);
    if (!func)
        return NULL;

    return func(visitor, node);
}