#include "free_visitor.h"

static void *visit_literal_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    LiteralNode *literal = (LiteralNode *)node;

    if (literal->base.return_type == type_table_lookup_by_tag(global_type_table, HULK_STRING))
        free(literal->value.string_value);

    free(literal);
    return NULL;
}

static void *visit_unary_operation_node(Visitor *visitor, ASTNode *node)
{
    UnaryOperationNode *unary = (UnaryOperationNode *)node;

    ast_accept(unary->operand, visitor);
    free(unary);

    return NULL;
}

static void *visit_binary_operation_node(Visitor *visitor, ASTNode *node)
{
    BinaryOperationNode *bin = (BinaryOperationNode *)node;

    ast_accept(bin->left, visitor);
    ast_accept(bin->right, visitor);
    free(bin);

    return NULL;
}

static void *visit_expression_block_node(Visitor *visitor, ASTNode *node)
{
    ExpressionBlockNode *block = (ExpressionBlockNode *)node;

    for (size_t i = 0; i < list_count(block->expressions); i++)
    {
        ASTNode *expr = (ASTNode *)list_get(block->expressions, i);
        ast_accept(expr, visitor);
    }

    list_destroy(block->expressions);
    free(block);

    return NULL;
}

static void *visit_conditional_node(Visitor *visitor, ASTNode *node)
{
    ConditionalNode *cond = (ConditionalNode *)node;

    ast_accept(cond->condition, visitor);
    ast_accept(cond->then_branch, visitor);

    if (cond->else_branch)
        ast_accept(cond->else_branch, visitor);

    free(cond);
    return NULL;
}

static void *visit_while_loop_node(Visitor *visitor, ASTNode *node)
{
    WhileLoopNode *loop = (WhileLoopNode *)node;

    ast_accept(loop->condition, visitor);

    if (loop->body)
        ast_accept(loop->body, visitor);

    free(loop);
    return NULL;
}

static void *visit_let_in_node(Visitor *visitor, ASTNode *node)
{
    LetInNode *let_in = (LetInNode *)node;

    for (size_t i = 0; i < list_count(let_in->bindings); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(let_in->bindings, i);

        if (binding->initializer)
            ast_accept(binding->initializer, visitor);
    }

    list_destroy(let_in->bindings);

    ast_accept(let_in->body, visitor);
    free(let_in);

    return NULL;
}

static void *visit_variable_reference_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    VariableReferenceNode *var_ref = (VariableReferenceNode *)node;

    free(var_ref->name);
    free(var_ref);

    return NULL;
}

static void *visit_reassignment_node(Visitor *visitor, ASTNode *node)
{
    ReassignmentNode *reassign = (ReassignmentNode *)node;

    ast_accept(reassign->target, visitor);
    ast_accept(reassign->value, visitor);
    free(reassign);

    return NULL;
}

static void *visit_function_definition_node(Visitor *visitor, ASTNode *node)
{
    FunctionDefinitionNode *func = (FunctionDefinitionNode *)node;

    for (size_t i = 0; i < list_count(func->params); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(func->params, i);

        if (binding->initializer)
            ast_accept(binding->initializer, visitor);
    }
    
    list_destroy(func->params);

    ast_accept(func->body, visitor);

    free(func->name);
    free(func->return_type_annotation);
    free(func);

    return NULL;
}

static void *visit_function_call_node(Visitor *visitor, ASTNode *node)
{
    FunctionCallNode *func = (FunctionCallNode *)node;

    for (size_t i = 0; i < list_count(func->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(func->args, i);
        ast_accept(arg, visitor);
    }

    list_destroy(func->args);

    free(func->name);
    free(func);

    return NULL;
}

static void *visit_type_definition_node(Visitor *visitor, ASTNode *node)
{
    TypeDefinitionNode *type = (TypeDefinitionNode *)node;

    list_destroy(type->init_params);

    for (size_t i = 0; i < list_count(type->father_init_args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(type->father_init_args, i);
        ast_accept(arg, visitor);
    }

    list_destroy(type->father_init_args);

    for (size_t i = 0; i < list_count(type->attributes); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(type->attributes, i);

        if (binding->initializer)
            ast_accept(binding->initializer, visitor);
    }

    list_destroy(type->attributes);

    for (size_t i = 0; i < list_count(type->methods); i++)
    {
        ASTNode *method = (ASTNode *)list_get(type->methods, i);
        ast_accept(method, visitor);
    }

    list_destroy(type->methods);

    free(type->name);
    free(type->father_name);
    free(type);

    return NULL;
}

static void *visit_attribute_access_node(Visitor *visitor, ASTNode *node)
{
    AttributeAccessNode *attr = (AttributeAccessNode *)node;

    ast_accept(attr->target, visitor);
    free(attr->attribute_name);
    free(attr);

    return NULL;
}

static void *visit_method_access_node(Visitor *visitor, ASTNode *node)
{
    MethodAccessNode *method = (MethodAccessNode *)node;

    ast_accept(method->target, visitor);

    for (size_t i = 0; i < list_count(method->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(method->args, i);
        ast_accept(arg, visitor);
    }

    list_destroy(method->args);

    free(method->method_name);
    free(method);

    return NULL;
}

static void *visit_type_instanciation_node(Visitor *visitor, ASTNode *node)
{
    TypeInstanciationNode *inst = (TypeInstanciationNode *)node;

    for (size_t i = 0; i < list_count(inst->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(inst->args, i);
        ast_accept(arg, visitor);
    }

    list_destroy(inst->args);

    free(inst->type_name);
    free(inst);

    return NULL;
}

static void *visit_program_node(Visitor *visitor, ASTNode *node)
{
    ProgramNode *program = (ProgramNode *)node;

    ast_accept(program->root, visitor);

    for (size_t i = 0; i < list_count(program->function_definitions); i++)
    {
        ASTNode *def = (ASTNode *)list_get(program->function_definitions, i);
        ast_accept(def, visitor);
    }

    list_destroy(program->function_definitions);

    for (size_t i = 0; i < list_count(program->type_definitions); i++)
    {
        ASTNode *type_def = (ASTNode *)list_get(program->type_definitions, i);
        ast_accept(type_def, visitor);
    }

    list_destroy(program->type_definitions);

    free(program);
    return NULL;
}

static void *visit_base_call_node(Visitor *visitor, ASTNode *node)
{
    BaseCallNode *base_call = (BaseCallNode *)node;

    for (size_t i = 0; i < list_count(base_call->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(base_call->args, i);
        ast_accept(arg, visitor);
    }
    list_destroy(base_call->args);

    free(base_call->method_name);
    free(base_call->type_name);
    free(base_call);

    return NULL;
}

static void *visit_is_node(Visitor *visitor, ASTNode *node)
{
    IsNode *is_node = (IsNode *)node;

    ast_accept(is_node->target, visitor);
    free(is_node->type_name);
    free(is_node);

    return NULL;
}

static void *visit_as_node(Visitor *visitor, ASTNode *node)
{
    AsNode *as_node = (AsNode *)node;

    ast_accept(as_node->target, visitor);
    free(as_node->type_name);
    free(as_node);

    return NULL;
}

FreeVisitor *free_visitor_create(void)
{
    FreeVisitor *visitor = (FreeVisitor *)malloc(sizeof(FreeVisitor));
    if (!visitor)
        return NULL;

    visitor_init(&visitor->base);

    VisitorFunc funcs[] = { (VisitorFunc)visit_literal_node, (VisitorFunc)visit_unary_operation_node, (VisitorFunc)visit_binary_operation_node, (VisitorFunc)visit_expression_block_node, (VisitorFunc)visit_conditional_node, (VisitorFunc)visit_while_loop_node, (VisitorFunc)visit_let_in_node, (VisitorFunc)visit_variable_reference_node, (VisitorFunc)visit_reassignment_node, (VisitorFunc)visit_function_definition_node, (VisitorFunc)visit_function_call_node, (VisitorFunc)visit_type_definition_node, (VisitorFunc)visit_attribute_access_node, (VisitorFunc)visit_method_access_node, (VisitorFunc)visit_type_instanciation_node, (VisitorFunc)visit_base_call_node, (VisitorFunc)visit_is_node, (VisitorFunc)visit_as_node, (VisitorFunc)visit_program_node };
    visitor_register_functions(&visitor->base, funcs);

    return visitor;
}

void free_visitor_destroy(FreeVisitor *visitor)
{
    if (!visitor)
        return;

    visitor_destroy(&visitor->base);
    free(visitor);
}