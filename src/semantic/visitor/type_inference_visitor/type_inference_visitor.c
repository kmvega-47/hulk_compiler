#include "type_inference_visitor.h"

static void *visit_literal_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    return node->return_type;
}

static void *visit_unary_operation_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_binary_operation_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_expression_block_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_conditional_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_while_loop_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_let_in_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_variable_reference_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_reassignment_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_function_definition_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_function_call_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_type_definition_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_attribute_access_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_method_access_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_type_instanciation_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_program_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_base_call_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_is_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void *visit_as_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

TypeInferenceVisitor *type_inference_visitor_create(void)
{
    TypeInferenceVisitor *visitor = (TypeInferenceVisitor *)malloc(sizeof(TypeInferenceVisitor));
    if (!visitor)
        return NULL;

    visitor_init(&visitor->base);

    VisitorFunc funcs[] = { (VisitorFunc)visit_literal_node, (VisitorFunc)visit_unary_operation_node, (VisitorFunc)visit_binary_operation_node, (VisitorFunc)visit_expression_block_node, (VisitorFunc)visit_conditional_node, (VisitorFunc)visit_while_loop_node, (VisitorFunc)visit_let_in_node, (VisitorFunc)visit_variable_reference_node, (VisitorFunc)visit_reassignment_node, (VisitorFunc)visit_function_definition_node, (VisitorFunc)visit_function_call_node, (VisitorFunc)visit_type_definition_node, (VisitorFunc)visit_attribute_access_node, (VisitorFunc)visit_method_access_node, (VisitorFunc)visit_type_instanciation_node, (VisitorFunc)visit_program_node, (VisitorFunc)visit_base_call_node, (VisitorFunc)visit_is_node, (VisitorFunc)visit_as_node };
    visitor_register_functions(&visitor->base, funcs);

    visitor->current_scope = scope_create(NULL);
    visitor->current_type = NULL;
    visitor->current_method_name = NULL;

    if (!visitor->current_scope)
    {
        free(visitor);
        return NULL;
    }

    scope_add_constant(visitor->current_scope, "PI", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    scope_add_constant(visitor->current_scope, "E", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    return visitor;
}

void type_inference_visitor_destroy(TypeInferenceVisitor *visitor)
{
    if (!visitor)
        return;

    if (visitor->current_scope)
        scope_destroy(visitor->current_scope);

    visitor_destroy(&visitor->base);
    free(visitor);
}