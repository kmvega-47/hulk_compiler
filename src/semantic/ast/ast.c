#include "ast.h"

void ast_base_init(ASTNode *node, ASTNodeType node_type, int line, int column)
{
    if (!node)
        return;

    node->node_type = node_type;
    node->return_type = NULL;
    node->line = line;
    node->column = column;
}

LiteralNode *ast_number_literal_create(double value, int line, int column)
{
    LiteralNode *node = (LiteralNode *)malloc(sizeof(LiteralNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_LITERAL, line, column);
    node->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    node->value.number_value = value;
    return node;
}

LiteralNode *ast_string_literal_create(const char *value, int line, int column)
{
    if (!value)
        return NULL;

    LiteralNode *node = (LiteralNode *)malloc(sizeof(LiteralNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_LITERAL, line, column);
    node->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_STRING);

    node->value.string_value = strdup(value);
    if (!node->value.string_value)
    {
        free(node);
        return NULL;
    }

    return node;
}

LiteralNode *ast_bool_literal_create(bool value, int line, int column)
{
    LiteralNode *node = (LiteralNode *)malloc(sizeof(LiteralNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_LITERAL, line, column);
    node->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);
    node->value.bool_value = value;
    return node;
}

UnaryOperationNode *ast_unary_create(HulkOperator op, ASTNode *operand, int line, int column)
{
    if (!operand)
        return NULL;

    UnaryOperationNode *node = (UnaryOperationNode *)malloc(sizeof(UnaryOperationNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_UNARY_OPERATION, line, column);
    node->operator = op;
    node->operand = operand;

    return node;
}

BinaryOperationNode *ast_binary_create(HulkOperator op, ASTNode *left, ASTNode *right, int line, int column)
{
    if (!left || !right)
        return NULL;

    BinaryOperationNode *node = (BinaryOperationNode *)malloc(sizeof(BinaryOperationNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_BINARY_OPERATION, line, column);
    node->operator = op;
    node->left = left;
    node->right = right;

    return node;
}

ExpressionBlockNode *ast_block_create(List *expressions, int line, int column)
{
    if (!expressions)
        return NULL;

    ExpressionBlockNode *node = (ExpressionBlockNode *)malloc(sizeof(ExpressionBlockNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_EXPRESSION_BLOCK, line, column);
    node->expressions = expressions;

    return node;
}

ConditionalNode *ast_conditional_create(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int line, int column)
{
    if (!condition || !then_branch)
        return NULL;

    ConditionalNode *node = (ConditionalNode *)malloc(sizeof(ConditionalNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_CONDITIONAL, line, column);
    node->condition = condition;
    node->then_branch = then_branch;
    node->else_branch = else_branch;

    return node;
}

WhileLoopNode *ast_while_create(ASTNode *condition, ASTNode *body, int line, int column)
{
    if (!condition)
        return NULL;

    WhileLoopNode *node = (WhileLoopNode *)malloc(sizeof(WhileLoopNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_WHILE_LOOP, line, column);
    node->condition = condition;
    node->body = body;

    return node;
}

LetInNode *ast_let_create(List *bindings, ASTNode *body, int line, int column)
{
    if (!bindings || list_count(bindings) == 0 || !body)
        return NULL;

    LetInNode *node = (LetInNode *)malloc(sizeof(LetInNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_LET_IN, line, column);
    node->bindings = bindings;
    node->body = body;

    return node;
}

VariableReferenceNode *ast_variable_reference_create(const char *name, int line, int column)
{
    if (!name)
        return NULL;

    VariableReferenceNode *node = (VariableReferenceNode *)malloc(sizeof(VariableReferenceNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_VARIABLE_REFERENCE, line, column);

    node->name = strdup(name);
    if (!node->name)
    {
        free(node);
        return NULL;
    }

    return node;
}

ReassignmentNode *ast_reassignment_create(ASTNode *target, ASTNode *value, int line, int column)
{
    if (!target || !value)
        return NULL;

    ReassignmentNode *node = (ReassignmentNode *)malloc(sizeof(ReassignmentNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_REASSIGNMENT, line, column);
    node->target = target;
    node->value = value;

    return node;
}

FunctionDefinitionNode *ast_function_definition_create(const char *name, List *params, const char *return_type_annotation, ASTNode *body, int line, int column)
{
    if (!name || !params || !body)
        return NULL;

    FunctionDefinitionNode *node = (FunctionDefinitionNode *)malloc(sizeof(FunctionDefinitionNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_FUNCTION_DEFINITION, line, column);

    node->name = strdup(name);
    if (!node->name)
    {
        free(node);
        return NULL;
    }

    node->params = params;

    if (return_type_annotation)
    {
        node->return_type_annotation = strdup(return_type_annotation);
        if (!node->return_type_annotation)
        {
            free(node->name);
            free(node);
            return NULL;
        }
    }
    else
    {
        node->return_type_annotation = NULL;
    }

    node->body = body;

    return node;
}

FunctionCallNode *ast_function_call_create(const char *name, List *args, int line, int column)
{
    if (!name || !args)
        return NULL;

    FunctionCallNode *node = (FunctionCallNode *)malloc(sizeof(FunctionCallNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_FUNCTION_CALL, line, column);

    node->name = strdup(name);
    if (!node->name)
    {
        free(node);
        return NULL;
    }

    node->args = args;

    return node;
}

TypeDefinitionNode *ast_type_definition_create(const char *name, List *init_params, const char *father_name, List *father_init_args, List *attributes, List *methods, int line, int column)
{
    if (!name || !init_params || !attributes || !methods)
        return NULL;

    if (!father_init_args)
        father_init_args = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = (TypeDefinitionNode *)malloc(sizeof(TypeDefinitionNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_TYPE_DEFINITION, line, column);

    node->name = strdup(name);

    node->init_params = init_params;
    node->father_init_args = father_init_args;
    node->attributes = attributes;
    node->methods = methods;

    const char *father = father_name ? father_name : "Object";
    node->father_name = strdup(father);

    return node;
}

AttributeAccessNode *ast_attribute_access_create(ASTNode *target, const char *attribute_name, int line, int column)
{
    if (!target || !attribute_name)
        return NULL;

    AttributeAccessNode *node = (AttributeAccessNode *)malloc(sizeof(AttributeAccessNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_ATTRIBUTE_ACCESS, line, column);

    node->target = target;

    node->attribute_name = strdup(attribute_name);
    if (!node->attribute_name)
    {
        free(node);
        return NULL;
    }

    return node;
}

MethodAccessNode *ast_method_access_create(ASTNode *target, const char *method_name, List *args, int line, int column)
{
    if (!target || !method_name || !args)
        return NULL;

    MethodAccessNode *node = (MethodAccessNode *)malloc(sizeof(MethodAccessNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_METHOD_ACCESS, line, column);

    node->target = target;
    node->args = args;

    node->method_name = strdup(method_name);
    if (!node->method_name)
    {
        free(node);
        return NULL;
    }

    return node;
}

TypeInstanciationNode *ast_type_instanciation_create(const char *type_name, List *args, int line, int column)
{
    if (!type_name || !args)
        return NULL;

    TypeInstanciationNode *node = (TypeInstanciationNode *)malloc(sizeof(TypeInstanciationNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_TYPE_INSTANCIATION, line, column);

    node->type_name = strdup(type_name);
    node->args = args;

    return node;
}

ProgramNode *ast_program_create(ASTNode *root, List *function_definitions, List *type_definitions, int line, int column)
{
    if (!root || !function_definitions || !type_definitions)
        return NULL;

    ProgramNode *node = (ProgramNode *)malloc(sizeof(ProgramNode));
    if (!node)
        return NULL;

    ast_base_init(&node->base, NODE_PROGRAM, line, column);

    node->root = root;
    node->function_definitions = function_definitions;
    node->type_definitions = type_definitions;

    return node;
}