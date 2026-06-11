#include "type_check_visitor.h"

static void *visit_literal_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static bool check_unary_operator(UnaryOperationNode *node, TypeDescriptor *operand_type)
{
    TypeDescriptor *bool_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);
    TypeDescriptor *number_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);

    if (node->operator == OP_NOT)
    {
        if (!type_conforms_to(operand_type, bool_type))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Operator '!' expected Bool, but got '%s'", operand_type->name);
            return false;
        }
    }

    else if (node->operator == OP_SUB)
    {
        if (!type_conforms_to(operand_type, number_type))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Operator '-' expected Number, but got '%s'", operand_type->name);
            return false;
        }
    }

    return true;
}

static void *visit_unary_operation_node(Visitor *visitor, ASTNode *node)
{
    UnaryOperationNode *unary = (UnaryOperationNode *)node;

    if (!unary->operand)
        return NULL;

    ast_accept(unary->operand, visitor);

    TypeDescriptor *operand_type = unary->operand->return_type;

    if (operand_type)
        check_unary_operator(unary, operand_type);

    return NULL;
}

static bool check_binary_operator(BinaryOperationNode *node, TypeDescriptor *left_type, TypeDescriptor *right_type)
{
    TypeDescriptor *number_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *string_type = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *bool_type   = type_table_lookup_by_tag(global_type_table, HULK_BOOL);

    const char *error_msg = NULL;

    if (is_arithmetic_operator(node->operator))
    {
        if (!type_conforms_to(left_type, number_type) || !type_conforms_to(right_type, number_type))
            error_msg = "Arithmetic operator '%s' expected Number and Number";
    }

    else if (is_comparison_operator(node->operator))
    {
        if (!type_conforms_to(left_type, number_type) || !type_conforms_to(right_type, number_type))
            error_msg = "Comparison operator '%s' expected Number and Number";
    }

    else if (is_logical_operator(node->operator))
    {
        if (!type_conforms_to(left_type, bool_type) || !type_conforms_to(right_type, bool_type))
            error_msg = "Logical operator '%s' expected Bool and Bool";
    }

    else if (is_string_operator(node->operator))
    {
        bool right_ok = type_conforms_to(right_type, string_type) || type_conforms_to(right_type, number_type);

        if (!type_conforms_to(left_type, string_type) || !right_ok)
            error_msg = "Concatenation operator '%s' expected String and String or Number";
    }

    if (error_msg)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column,
            error_msg, operator_to_string(node->operator), left_type->name, right_type->name);
        return false;
    }

    return true;
}

static void *visit_binary_operation_node(Visitor *visitor, ASTNode *node)
{
    BinaryOperationNode *bin = (BinaryOperationNode *)node;

    if (!bin->left || !bin->right)
        return NULL;

    ast_accept(bin->left, visitor);
    ast_accept(bin->right, visitor);

    TypeDescriptor *left_type = bin->left->return_type;
    TypeDescriptor *right_type = bin->right->return_type;

    if (left_type && right_type)
        check_binary_operator(bin, left_type, right_type);

    return NULL;
}

static void *visit_expression_block_node(Visitor *visitor, ASTNode *node)
{
    ExpressionBlockNode *block = (ExpressionBlockNode *)node;

    if (!block->expressions)
        return NULL;

    for (size_t i = 0; i < list_count(block->expressions); i++)
    {
        ASTNode *expr = (ASTNode *)list_get(block->expressions, i);

        if (expr)
            ast_accept(expr, visitor);
    }

    return NULL;
}

static void *visit_conditional_node(Visitor *visitor, ASTNode *node)
{
    ConditionalNode *cond = (ConditionalNode *)node;

    if (!cond->condition || !cond->then_branch)
        return NULL;

    ast_accept(cond->condition, visitor);
    ast_accept(cond->then_branch, visitor);

    if (cond->else_branch)
        ast_accept(cond->else_branch, visitor);

    TypeDescriptor *cond_type = cond->condition->return_type;
    TypeDescriptor *bool_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);

    if (cond_type && !type_conforms_to(cond_type, bool_type))
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, cond->base.line, cond->base.column, "If condition expected Bool, but got '%s'", cond_type->name);
    }

    return NULL;
}

static void *visit_while_loop_node(Visitor *visitor, ASTNode *node)
{
    WhileLoopNode *loop = (WhileLoopNode *)node;

    if (!loop->condition)
        return NULL;

    ast_accept(loop->condition, visitor);

    if (loop->body)
        ast_accept(loop->body, visitor);

    TypeDescriptor *cond_type = loop->condition->return_type;
    TypeDescriptor *bool_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);

    if (cond_type && !type_conforms_to(cond_type, bool_type))
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, loop->base.line, loop->base.column, "While loop condition expected Bool, but got '%s'", cond_type->name);
    }

    return NULL;
}

static void *visit_let_in_node(Visitor *visitor, ASTNode *node)
{
    LetInNode *let_in = (LetInNode *)node;

    if (!let_in->bindings)
        return NULL;

    for (size_t i = 0; i < list_count(let_in->bindings); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(let_in->bindings, i);

        if (!binding)
            continue;

        if (binding->initializer)
            ast_accept(binding->initializer, visitor);

        if (binding->type_name && binding->initializer)
        {
            TypeDescriptor *init_type = binding->initializer->return_type;
            TypeDescriptor *annotated_type = type_table_lookup_by_name(global_type_table, binding->type_name);

            if (annotated_type && init_type && !type_conforms_to(init_type, annotated_type))
            {
                dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, binding->line, binding->column, "Type mismatch for '%s': expected '%s', but got '%s'", binding->name, annotated_type->name, init_type->name);
            }
        }
    }

    if (let_in->body)
        ast_accept(let_in->body, visitor);

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
    ReassignmentNode *reassign = (ReassignmentNode *)node;

    ast_accept(reassign->target, visitor);
    ast_accept(reassign->value, visitor);

    TypeDescriptor *target_type = reassign->target->return_type;
    TypeDescriptor *value_type = reassign->value->return_type;

    if (target_type && value_type && !type_conforms_to(value_type, target_type))
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, reassign->base.line, reassign->base.column,
            "Cannot reassign type '%s' to type '%s'", value_type->name, target_type->name);
    }

    return NULL;
}

static void *visit_function_definition_node(Visitor *visitor, ASTNode *node)
{
    FunctionDefinitionNode *func = (FunctionDefinitionNode *)node;

    if (!func->body)
        return NULL;

    ast_accept(func->body, visitor);

    TypeDescriptor *body_type = func->body->return_type;

    if (func->return_type_annotation && body_type)
    {
        TypeDescriptor *annotated = type_table_lookup_by_name(global_type_table, func->return_type_annotation);

        printf("DEBUG: body_type=%s, annotated=%s, conforms=%d\n", body_type->name, annotated->name, type_conforms_to(body_type, annotated));

        if (annotated && !type_conforms_to(body_type, annotated))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, func->base.line, func->base.column,
                "Function '%s' return type mismatch: expected '%s', but got '%s'", func->name, annotated->name, body_type->name);
        }
    }

    return NULL;
}

static void *visit_function_call_node(Visitor *visitor, ASTNode *node)
{
    FunctionCallNode *call = (FunctionCallNode *)node;

    for (size_t i = 0; i < list_count(call->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(call->args, i);

        if (arg)
            ast_accept(arg, visitor);
    }

    List *param_types = function_table_get_params_types(global_function_table, call->name);

    if (!param_types)
        return NULL;

    size_t num_args = list_count(call->args);

    for (size_t i = 0; i < num_args && i < list_count(param_types); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(call->args, i);

        TypeDescriptor *arg_type = arg ? arg->return_type : NULL;
        TypeDescriptor *param_type = (TypeDescriptor *)list_get(param_types, i);

        if (!arg_type || !param_type)
            continue;

        if (!type_conforms_to(arg_type, param_type))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, call->base.line, call->base.column,
                "Argument %zu of '%s' expects type '%s', but got '%s'", i + 1, call->name, param_type->name, arg_type->name);
        }
    }

    return NULL;
}

static void *visit_type_definition_node(Visitor *visitor, ASTNode *node)
{
    TypeDefinitionNode *type_def = (TypeDefinitionNode *)node;

    TypeDescriptor *father_type = type_table_lookup_by_name(global_type_table, type_def->father_name);

    size_t expected = 0;

    if (father_type && father_type->tag == HULK_USER_DEFINED)
        expected = list_count(user_type_get_param_types((UserTypeDescriptor *)father_type));

    // Visitar y verificar argumentos de inicialización del padre
    for (size_t i = 0; i < list_count(type_def->father_init_args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(type_def->father_init_args, i);
        ast_accept(arg, visitor);

        if (father_type && i < expected)
        {
            List *father_params = user_type_get_param_types((UserTypeDescriptor *)father_type);

            TypeDescriptor *param_type = (TypeDescriptor *)list_get(father_params, i);
            TypeDescriptor *arg_type = arg->return_type;

            if (!type_conforms_to(arg_type, param_type))
            {
                dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, type_def->base.line, type_def->base.column, 
                    "Argument type '%s' does not conform to parameter type '%s'", arg_type->name, param_type->name);
            }
        }
    }

    // Visitar y verificar atributos
    for (size_t i = 0; i < list_count(type_def->attributes); i++)
    {
        SymbolBinding *b = (SymbolBinding *)list_get(type_def->attributes, i);

        if (b->initializer)
            ast_accept(b->initializer, visitor);

        if (b->type_name && b->initializer)
        {
            TypeDescriptor *annotated = type_table_lookup_by_name(global_type_table, b->type_name);
            TypeDescriptor *init_type = b->initializer->return_type;

            if (!type_conforms_to(init_type, annotated))
            {
                dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, b->line, b->column,
                    "Attribute '%s' initializer type '%s' does not conform to annotated type '%s'", b->name, init_type->name, annotated->name);
            }
        }
    }

    // Visitar métodos
    for (size_t i = 0; i < list_count(type_def->methods); i++)
    {
        FunctionDefinitionNode *method = (FunctionDefinitionNode *)list_get(type_def->methods, i);

        if (method)
            ast_accept((ASTNode *)method, visitor);
    }

    return NULL;
}

static void *visit_attribute_access_node(Visitor *visitor, ASTNode *node)
{
    AttributeAccessNode *attr = (AttributeAccessNode *)node;

    ast_accept(attr->target, visitor);

    return NULL;
}

static void *visit_type_instanciation_node(Visitor *visitor, ASTNode *node)
{
    TypeInstanciationNode *inst = (TypeInstanciationNode *)node;

    TypeDescriptor *type = type_table_lookup_by_name(global_type_table, inst->type_name);

    // La inferencia ya reportó si el tipo no existe o no es definido por el usuario
    if (!type || type->tag != HULK_USER_DEFINED)
        return NULL;

    List *param_types = user_type_get_param_types((UserTypeDescriptor *)type);

    size_t expected = list_count(param_types);
    size_t given = list_count(inst->args);

    // Verificar cantidad de argumentos
    if (expected != given)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, inst->base.line, inst->base.column,
            "Type '%s' expects %zu initialization argument(s), but got %zu", inst->type_name, expected, given);
    }

    // Visitar y verificar conformidad de cada argumento
    for (size_t i = 0; i < given; i++)
    {
        ASTNode *arg = (ASTNode *)list_get(inst->args, i);
        ast_accept(arg, visitor);

        if (i < expected)
        {
            TypeDescriptor *param_type = (TypeDescriptor *)list_get(param_types, i);
            TypeDescriptor *arg_type = arg->return_type;

            if (arg_type && param_type && !type_conforms_to(arg_type, param_type))
            {
                dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, inst->base.line, inst->base.column,
                    "Argument type '%s' does not conform to parameter type '%s'", arg_type->name, param_type->name);
            }
        }
    }

    return NULL;
}

static void *visit_method_access_node(Visitor *visitor, ASTNode *node)
{
    MethodAccessNode *method = (MethodAccessNode *)node;

    // Visitar target
    if (method->target)
        ast_accept(method->target, visitor);

    // Visitar argumentos
    for (size_t i = 0; i < list_count(method->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(method->args, i);

        if (arg)
            ast_accept(arg, visitor);
    }

    TypeDescriptor *target_type = method->target->return_type;

    // La inferencia ya reportó si el tipo no tiene métodos
    if (!target_type || target_type->tag != HULK_USER_DEFINED)
        return NULL;

    // Buscar el tipo que realmente define el método (recorriendo ancestros)
    UserTypeDescriptor *user_type = (UserTypeDescriptor *)target_type;
    UserTypeDescriptor *owner = user_type_find_type_with_method(user_type, method->method_name);

    if (!owner)
        return NULL;

    // Obtener tipos de los parámetros desde la tabla de funciones
    char *full_name = function_table_build_method_name(owner->base.name, method->method_name);
    List *param_types = function_table_get_params_types(global_function_table, full_name);
    free(full_name);

    if (!param_types)
        return NULL;

    size_t expected = list_count(param_types);
    size_t given = list_count(method->args);

    // Verificar cantidad de argumentos
    if (expected != given)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, method->base.line, method->base.column,
            "Method '%s' expects %zu arguments, but got %zu", method->method_name, expected, given);
    }

    // Verificar conformidad de cada argumento con el parámetro correspondiente
    size_t count = expected < given ? expected : given;

    for (size_t i = 0; i < count; i++)
    {
        ASTNode *arg = (ASTNode *)list_get(method->args, i);
        TypeDescriptor *arg_type = arg->return_type;
        TypeDescriptor *param_type = (TypeDescriptor *)list_get(param_types, i);

        if (arg_type && param_type && !type_conforms_to(arg_type, param_type))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, method->base.line, method->base.column,
                "Argument %zu of method '%s' expects type '%s', but got '%s'", i + 1, method->method_name, param_type->name, arg_type->name);
        }
    }

    return NULL;
}

static void *visit_base_call_node(Visitor *visitor, ASTNode *node)
{
    BaseCallNode *base_call = (BaseCallNode *)node;

    // Visitar argumentos
    for (size_t i = 0; i < list_count(base_call->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(base_call->args, i);

        if (arg)
            ast_accept(arg, visitor);
    }

    // La inferencia ya reportó si no hay type_name o method_name
    if (!base_call->type_name || !base_call->method_name)
        return NULL;

    // Obtener el tipo padre
    UserTypeDescriptor *utype = type_to_user_defined(type_table_lookup_by_name(global_type_table, base_call->type_name));

    if (!utype || !utype->base.parent)
        return NULL;

    // Buscar el ancestro que define el método
    UserTypeDescriptor *parent_user = type_to_user_defined(utype->base.parent);
    UserTypeDescriptor *ancestor = user_type_find_type_with_method(parent_user, base_call->method_name);

    if (!ancestor)
        return NULL;

    // Obtener tipos de los parámetros del método en el ancestro
    char *full_name = function_table_build_method_name(ancestor->base.name, base_call->method_name);
    List *param_types = function_table_get_params_types(global_function_table, full_name);
    free(full_name);

    if (!param_types)
        return NULL;

    // Verificar conformidad de cada argumento
    size_t count = list_count(base_call->args) < list_count(param_types) ? list_count(base_call->args) : list_count(param_types);

    for (size_t i = 0; i < count; i++)
    {
        ASTNode *arg = (ASTNode *)list_get(base_call->args, i);
        TypeDescriptor *arg_type = arg->return_type;
        TypeDescriptor *param_type = (TypeDescriptor *)list_get(param_types, i);

        if (arg_type && param_type && !type_conforms_to(arg_type, param_type))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, base_call->base.line, base_call->base.column,
                "Argument %zu of base call '%s' expects type '%s', but got '%s'",  i + 1, base_call->method_name, param_type->name, arg_type->name);
        }
    }

    return NULL;
}

static void *visit_is_node(Visitor *visitor, ASTNode *node)
{
    IsNode *is_node = (IsNode *)node;

    ast_accept(is_node->target, visitor);

    return NULL;
}

static void *visit_as_node(Visitor *visitor, ASTNode *node)
{
    AsNode *as_node = (AsNode *)node;

    ast_accept(as_node->target, visitor);

    return NULL;
}

static void *visit_program_node(Visitor *visitor, ASTNode *node)
{
    if (!node) 
        return NULL;

    ProgramNode *program = (ProgramNode *)node;

    for (size_t i = 0; i < list_count(program->type_definitions); i++)
    {
        ASTNode *type_def = (ASTNode *)list_get(program->type_definitions, i);

        if (type_def)
            ast_accept(type_def, visitor);
    }

    for (size_t i = 0; i < list_count(program->function_definitions); i++)
    {
        ASTNode *func_def = (ASTNode *)list_get(program->function_definitions, i);

        if (func_def)
            ast_accept(func_def, visitor);
    }

    if (program->root)
        ast_accept(program->root, visitor);

    return NULL;
}

TypeCheckVisitor *type_check_visitor_create(void)
{
    TypeCheckVisitor *visitor = (TypeCheckVisitor *)malloc(sizeof(TypeCheckVisitor));
    if (!visitor)
        return NULL;

    visitor_init(&visitor->base);

    VisitorFunc funcs[] = {(VisitorFunc)visit_literal_node, (VisitorFunc)visit_unary_operation_node, (VisitorFunc)visit_binary_operation_node, (VisitorFunc)visit_expression_block_node, (VisitorFunc)visit_conditional_node, (VisitorFunc)visit_while_loop_node, (VisitorFunc)visit_let_in_node, (VisitorFunc)visit_variable_reference_node, (VisitorFunc)visit_reassignment_node, (VisitorFunc)visit_function_definition_node, (VisitorFunc)visit_function_call_node, (VisitorFunc)visit_type_definition_node, (VisitorFunc)visit_attribute_access_node, (VisitorFunc)visit_method_access_node, (VisitorFunc)visit_type_instanciation_node, (VisitorFunc)visit_base_call_node, (VisitorFunc)visit_is_node, (VisitorFunc)visit_as_node, (VisitorFunc)visit_program_node};
    visitor_register_functions(&visitor->base, funcs);

    return visitor;
}

void type_check_visitor_destroy(TypeCheckVisitor *visitor)
{
    if (!visitor)
        return;

    visitor_destroy(&visitor->base);
    free(visitor);
}