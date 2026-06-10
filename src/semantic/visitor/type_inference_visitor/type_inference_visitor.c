#include "type_inference_visitor.h"

static void *visit_literal_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    return node->return_type;
}

static void *visit_unary_operation_node(Visitor *visitor, ASTNode *node)
{
    UnaryOperationNode *unary = (UnaryOperationNode *)node;

    ast_accept(unary->operand, visitor);

    if (unary->operator == OP_NOT)
        unary->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);

    else if (unary->operator == OP_SUB)
        unary->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);

    return unary->base.return_type;
}


static void *visit_binary_operation_node(Visitor *visitor, ASTNode *node)
{
    BinaryOperationNode *bin = (BinaryOperationNode *)node;

    ast_accept(bin->left, visitor);
    ast_accept(bin->right, visitor);

    if (is_arithmetic_operator(bin->operator))
        bin->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);

    else if (is_comparison_operator(bin->operator) || is_logical_operator(bin->operator))
        bin->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);

    else if (is_string_operator(bin->operator))
        bin->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_STRING);

    return bin->base.return_type;
}

static void *visit_expression_block_node(Visitor *visitor, ASTNode *node)
{
    ExpressionBlockNode *block = (ExpressionBlockNode *)node;

    if (list_count(block->expressions) == 0)
    {
        block->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_VOID);
        return block->base.return_type;
    }

    TypeDescriptor *last_type = NULL;

    for (size_t i = 0; i < list_count(block->expressions); i++)
    {
        ASTNode *expr = (ASTNode *)list_get(block->expressions, i);
        last_type = ast_accept(expr, visitor);
    }

    block->base.return_type = last_type;
    return last_type;
}

static void *visit_conditional_node(Visitor *visitor, ASTNode *node)
{
    ConditionalNode *cond = (ConditionalNode *)node;

    ast_accept(cond->condition, visitor);
    TypeDescriptor *then_type = ast_accept(cond->then_branch, visitor);

    if (!cond->else_branch)
    {
        cond->base.return_type = then_type;
        return then_type;
    }

    TypeDescriptor *else_type = ast_accept(cond->else_branch, visitor);

    cond->base.return_type = type_lca(then_type, else_type);
    return cond->base.return_type;
}

static void *visit_while_loop_node(Visitor *visitor, ASTNode *node)
{
    WhileLoopNode *loop = (WhileLoopNode *)node;

    ast_accept(loop->condition, visitor);

    TypeDescriptor *body_type = NULL;
    if (loop->body)
        body_type = ast_accept(loop->body, visitor);

    loop->base.return_type = body_type ? body_type : type_table_lookup_by_tag(global_type_table, HULK_VOID);
    return loop->base.return_type;
}


static void *visit_let_in_node(Visitor *visitor, ASTNode *node)
{
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;
    LetInNode *let_in = (LetInNode *)node;

    // Crear un nuevo scope hijo del actual
    Scope *new_scope = scope_create(infer->current_scope);

    Scope *old_scope = infer->current_scope;
    Scope *current = new_scope;

    infer->current_scope = current;

    // Procesar cada binding
    for (size_t i = 0; i < list_count(let_in->bindings); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(let_in->bindings, i);

        // Si el símbolo ya existe en este scope, crear un nuevo scope anidado (shadowing)
        if (scope_lookup_current(current, binding->name))
        {
            Scope *extra = scope_create(current);
            current = extra;
            infer->current_scope = current;
        }

        // Inferir el tipo del inicializador
        ast_accept(binding->initializer, visitor);

        TypeDescriptor *var_type = NULL;

        // Si tiene anotación de tipo, buscarlo en la tabla
        if (binding->type_name)
        {
            var_type = type_table_lookup_by_name(global_type_table, binding->type_name);

            if (!var_type)
            {
                dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, binding->line, binding->column, "Type '%s' is not defined", binding->type_name);
            }
        }

        else
        {
            // Sin anotación, usar el tipo inferido del inicializador
            var_type = binding->initializer->return_type;
        }

        // Guardar el tipo resuelto en el binding y registrar en el scope
        binding->return_type = var_type;
        scope_add_variable(current, binding->name, var_type);
    }

    // Inferir el cuerpo del let con el nuevo scope
    TypeDescriptor *body_type = ast_accept(let_in->body, visitor);

    // Restaurar el scope original y destruir los scopes creados
    infer->current_scope = old_scope;
    scope_destroy_until(current, old_scope);

    // El tipo del let es el tipo de su cuerpo
    let_in->base.return_type = body_type;
    return body_type;
}

static void *visit_variable_reference_node(Visitor *visitor, ASTNode *node)
{
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;
    VariableReferenceNode *var_ref = (VariableReferenceNode *)node;

    TypeDescriptor *type = scope_lookup(infer->current_scope, var_ref->name);

    if (!type && !scope_is_error_flag(infer->current_scope, var_ref->name))
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, var_ref->base.line, var_ref->base.column, "Undefined variable '%s'", var_ref->name);

        // Marca la variable como no definida la primera vez(para evitar reportarlo multiples veces)
        scope_mark_variable_error(infer->current_scope, var_ref->name);
    }

    var_ref->base.return_type = type;
    return type;
}

static void *visit_reassignment_node(Visitor *visitor, ASTNode *node)
{
    ReassignmentNode *reassign = (ReassignmentNode *)node;
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;

    // Inferir el tipo del target
    TypeDescriptor *target_type = ast_accept(reassign->target, visitor);

    // Inferir el tipo del valor
    ast_accept(reassign->value, visitor);

    // Verificar que el target sea válido para reasignación
    if (reassign->target->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *var_ref = (VariableReferenceNode *)reassign->target;

        if (scope_is_self_instance(infer->current_scope, var_ref->name))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC,
                reassign->base.line, reassign->base.column, "'self' is not a valid assignment target when referring to type's instance");
        }

        if (scope_is_constant(infer->current_scope, var_ref->name))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, reassign->base.line, reassign->base.column, "Cannot reassign constant '%s'", var_ref->name);
        }
    }

    // El tipo del reassignment es el tipo del target
    reassign->base.return_type = target_type;
    return target_type;
}

static void *visit_function_definition_node(Visitor *visitor, ASTNode *node)
{
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;
    FunctionDefinitionNode *func = (FunctionDefinitionNode *)node;

    // Construir el nombre de la función/método según corresponda
    const char *func_name;

    if (infer->current_type)
    {
        // Es un método: guardar nombre para base() y usar nombre compuesto
        infer->current_method_name = func->name;
        char *method_name = function_table_build_method_name(infer->current_type->name, func->name);
        func_name = method_name;
    }

    else
    {
        func_name = func->name;
    }

    // Obtener tipos de parámetros desde la tabla
    List *param_types = function_table_get_params_types(global_function_table, func_name);

    if (!param_types)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, func->base.line, func->base.column, "Undefined function '%s'", func->name);
        func->base.return_type = NULL;
        return NULL;
    }

    // Crear scope para parámetros y cuerpo
    Scope *new_scope = scope_create(infer->current_scope);
    Scope *old_scope = infer->current_scope;
    infer->current_scope = new_scope;

    // Registrar self en el scope si es un método
    if (infer->current_type)
        scope_add_self_instance(new_scope, infer->current_type);

    // Registrar parámetros en el scope
    for (size_t i = 0; i < list_count(func->params); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(func->params, i);
        TypeDescriptor *param_type = (TypeDescriptor *)list_get(param_types, i);

        binding->return_type = param_type;
        scope_add_parameter(new_scope, binding->name, param_type);
    }

    // Inferir el cuerpo
    TypeDescriptor *body_type = ast_accept(func->body, visitor);

    // Restaurar scope
    infer->current_scope = old_scope;
    scope_destroy(new_scope);

    // Limpiar método actual al salir
    infer->current_method_name = NULL;

    // Actualizar tipo de retorno en la tabla si no tiene anotación
    if (!func->return_type_annotation)
        function_table_update(global_function_table, func_name, body_type, NULL);

    // Liberar nombre compuesto si se creó
    if (infer->current_type)
        free((void *)func_name);

    // El tipo del nodo de definición es Void
    func->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_VOID);
    return func->base.return_type;
}

static void *visit_function_call_node(Visitor *visitor, ASTNode *node)
{
    FunctionCallNode *call = (FunctionCallNode *)node;

    // Inferir recursivamente cada argumento
    for (size_t i = 0; i < list_count(call->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(call->args, i);
        ast_accept(arg, visitor);
    }

    // Buscar el tipo de retorno de la función en la tabla global
    TypeDescriptor *return_type = function_table_get_return_type(global_function_table, call->name);

    if (!return_type)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC,
            call->base.line, call->base.column,
            "Function '%s' is not defined", call->name);
    }

    else
    {
        // Verificar cantidad de argumentos
        List *param_types = function_table_get_params_types(global_function_table, call->name);
        if (list_count(call->args) != list_count(param_types))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC,
                call->base.line, call->base.column, "Function '%s' expects %zu arguments but got %zu", call->name, list_count(param_types), list_count(call->args));
        }
    }

    call->base.return_type = return_type;
    return return_type;
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