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

    bool found = false;
    TypeDescriptor *type = scope_lookup(infer->current_scope, var_ref->name, &found);

    if (!found)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, var_ref->base.line, var_ref->base.column, "Undefined variable '%s'", var_ref->name);
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

    for (size_t i = 0; i < list_count(call->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(call->args, i);
        ast_accept(arg, visitor);
    }

    bool found = false;
    TypeDescriptor *return_type = function_table_get_return_type(global_function_table, call->name, &found);

    if (!found)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, call->base.line, call->base.column, "Function '%s' is not defined", call->name);
    }

    else
    {
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
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;
    TypeDefinitionNode *type_def = (TypeDefinitionNode *)node;

    // El tipo ya debe estar registrado en la tabla global (se hizo al visitar el program)
    TypeDescriptor *type = type_table_lookup_by_name(global_type_table, type_def->name);
    UserTypeDescriptor *user_type = type_to_user_defined(type);

    // Scope para init_params y father_init_args
    Scope *old_scope = infer->current_scope;
    infer->current_scope = scope_create(old_scope);

    // Registrar init_params: los tipos ya están en user_type->param_types
    List *param_types = user_type_get_param_types(user_type);

    for (size_t i = 0; i < list_count(type_def->init_params); i++)
    {
        SymbolBinding *b = (SymbolBinding *)list_get(type_def->init_params, i);
        TypeDescriptor *ptype = (TypeDescriptor *)list_get(param_types, i);

        b->return_type = ptype;
        scope_add_parameter(infer->current_scope, b->name, ptype);
    }

    // Inferir father_init_args
    for (size_t i = 0; i < list_count(type_def->father_init_args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(type_def->father_init_args, i);
        ast_accept(arg, visitor);
    }

    // Inferir atributos y registrarlos en el tipo
    for (size_t i = 0; i < list_count(type_def->attributes); i++)
    {
        SymbolBinding *b = (SymbolBinding *)list_get(type_def->attributes, i);

        if (b->initializer)
            ast_accept(b->initializer, visitor);

        TypeDescriptor *attr_type = NULL;
        if (b->type_name)
        {
            attr_type = type_table_lookup_by_name(global_type_table, b->type_name);

            if (!attr_type)
            {
                dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, b->line, b->column, "Type '%s' for attribute '%s' is not defined", b->type_name, b->name);
            }
        }

        else
            attr_type = b->initializer->return_type;

        // Guardar el tipo en el binding y registrar/actualizar en el tipo
        b->return_type = attr_type;
        user_type_update_attribute(user_type, b->name, attr_type);
    }

    // Destruir scope temporal
    scope_destroy(infer->current_scope);
    infer->current_scope = old_scope;

    // Establecer el tipo actual para que los métodos sepan en qué tipo están
    TypeDescriptor *old_type = infer->current_type;
    infer->current_type = type;

    // Los métodos se visitan externamente (desde program node)

    // Restaurar tipo actual
    infer->current_type = old_type;

    type_def->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_VOID);
    return type_def->base.return_type;
}

static void *visit_attribute_access_node(Visitor *visitor, ASTNode *node)
{
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;
    AttributeAccessNode *attr = (AttributeAccessNode *)node;

    // Verificar que el target sea 'self' (atributos privados, solo acceso vía self)
    if (attr->target->node_type != NODE_VARIABLE_REFERENCE || !scope_is_self_instance(infer->current_scope, ((VariableReferenceNode *)attr->target)->name))
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, attr->base.line, attr->base.column, "Attribute access is private. (only trough self referring to the type instance)");
        attr->base.return_type = NULL;
        return NULL;
    }

    // Inferir el target
    ast_accept(attr->target, visitor);

    // Obtener el tipo de 'self' desde el scope
    VariableReferenceNode *var_ref = (VariableReferenceNode *)attr->target;
    TypeDescriptor *self_type = scope_lookup(infer->current_scope, var_ref->name, NULL);

    // Buscar el atributo en el tipo
    UserTypeDescriptor *user_type = (UserTypeDescriptor *)self_type;
    TypeDescriptor *attr_type = user_type_lookup_attribute(user_type, attr->attribute_name);

    if (!attr_type)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, attr->base.line, attr->base.column, "Attribute '%s' is not defined in type '%s'", attr->attribute_name, user_type->base.name);
    }

    attr->base.return_type = attr_type;
    return attr_type;
}

static void *visit_method_access_node(Visitor *visitor, ASTNode *node)
{
    MethodAccessNode *method = (MethodAccessNode *)node;

    // Inferir el target
    ast_accept(method->target, visitor);

    // Inferir los argumentos
    for (size_t i = 0; i < list_count(method->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(method->args, i);
        ast_accept(arg, visitor);
    }

    // Obtener tipo del target
    TypeDescriptor *target_type = method->target->return_type;

    if (!target_type)
    {
        method->base.return_type = NULL;
        return NULL;
    }

    // Solo tipos usuario tienen métodos
    if (target_type->tag != HULK_USER_DEFINED)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, method->base.line, method->base.column, "Type '%s' does not have methods", target_type->name);
        method->base.return_type = NULL;
        return NULL;
    }

    UserTypeDescriptor *user_type = (UserTypeDescriptor *)target_type;

    // Buscar el tipo que realmente define el método (el propio o un ancestro)
    UserTypeDescriptor *owner = user_type_find_type_with_method(user_type, method->method_name);

    if (!owner)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, method->base.line, method->base.column, "Method '%s' is not defined in type '%s' or its ancestors", method->method_name, user_type->base.name);
        method->base.return_type = NULL;
        return NULL;
    }

    // Construir nombre compuesto del método y buscar en la tabla global
    char *full_name = function_table_build_method_name(owner->base.name, method->method_name);
    bool found = false;

    TypeDescriptor *return_type = function_table_get_return_type(global_function_table, full_name, &found);
    
    free(full_name);

    if (!found)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, method->base.line, method->base.column, "Method '%s' is not defined in type '%s' or its ancestors", method->method_name, user_type->base.name);
    }

    method->base.return_type = return_type;
    return return_type;
}

static void *visit_type_instanciation_node(Visitor *visitor, ASTNode *node)
{
    TypeInstanciationNode *inst = (TypeInstanciationNode *)node;

    // Buscar el tipo a instanciar
    TypeDescriptor *type = type_table_lookup_by_name(global_type_table, inst->type_name);
    if (!type)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, inst->base.line, inst->base.column, "Undefined type '%s'", inst->type_name);
        inst->base.return_type = NULL;
        return NULL;
    }

    if (type->tag != HULK_USER_DEFINED)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, inst->base.line, inst->base.column, "Cannot instantiate built-in type '%s'", inst->type_name);
        inst->base.return_type = NULL;
        return NULL;
    }

    // Inferir recursivamente los argumentos
    for (size_t i = 0; i < list_count(inst->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(inst->args, i);
        ast_accept(arg, visitor);
    }

    inst->base.return_type = type;
    return type;
}

static void *visit_base_call_node(Visitor *visitor, ASTNode *node)
{
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;
    BaseCallNode *base_call = (BaseCallNode *)node;

    if (!infer->current_type || !infer->current_method_name)
    {
        if(!base_call->setted)
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, base_call->base.line, base_call->base.column, "'base()' can only be used inside a method of a user-defined type");
        }

        base_call->setted = true;
        base_call->base.return_type = NULL;
        
        return NULL;
    }

    // Guardar el tipo y método actual en el nodo para uso en codegen
    if(!base_call->type_name)
        base_call->type_name = strdup(infer->current_type->name);

    if(!base_call->method_name)
        base_call->method_name = strdup(infer->current_method_name);

    if(base_call->type_name && base_call->method_name)
    {
        base_call->setted = true;
    }

    // Inferir los argumentos
    for (size_t i = 0; i < list_count(base_call->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(base_call->args, i);
        ast_accept(arg, visitor);
    }

    // Buscar el ancestro que tenga el método (empezando desde el padre)
    UserTypeDescriptor *user_type = type_to_user_defined(infer->current_type);
    UserTypeDescriptor *parent_user = type_to_user_defined(user_type->base.parent);
    UserTypeDescriptor *ancestor = user_type_find_type_with_method(parent_user, infer->current_method_name);

    if (!ancestor)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, base_call->base.line, base_call->base.column, "No ancestor of '%s' implements method '%s'", infer->current_type->name, infer->current_method_name);
        base_call->base.return_type = NULL;
        return NULL;
    }

    // Buscar el método en la tabla global con el nombre del ancestro
    char *full_name = function_table_build_method_name(ancestor->base.name, infer->current_method_name);
    bool found = false;

    TypeDescriptor *return_type = function_table_get_return_type(global_function_table, full_name, &found);

    free(full_name);

    if (!found)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, base_call->base.line, base_call->base.column, "Method '%s' not found in ancestor '%s'", infer->current_method_name, ancestor->base.name);
    }

    base_call->base.return_type = return_type;
    return return_type;
}

static void *visit_is_node(Visitor *visitor, ASTNode *node)
{
    IsNode *is_node = (IsNode *)node;

    // Inferir el target
    ast_accept(is_node->target, visitor);

    // Buscar el tipo en la tabla
    TypeDescriptor *type = type_table_lookup_by_name(global_type_table, is_node->type_name);
    if (!type)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, is_node->base.line, is_node->base.column, "Undefined type '%s'", is_node->type_name);
    }

    // is siempre retorna Bool
    is_node->base.return_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);
    return is_node->base.return_type;
}

static void *visit_as_node(Visitor *visitor, ASTNode *node)
{
    AsNode *as_node = (AsNode *)node;

    // Inferir el target
    ast_accept(as_node->target, visitor);

    // Buscar el tipo en la tabla
    TypeDescriptor *type = type_table_lookup_by_name(global_type_table, as_node->type_name);
    if (!type)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, as_node->base.line, as_node->base.column, "Undefined type '%s'", as_node->type_name);
    }

    // as retorna el tipo al que se convierte
    as_node->base.return_type = type;
    return type;
}

static void register_function(FunctionDefinitionNode *node, TypeDescriptor *current_type)
{
    // Construir tipos de parámetros desde las anotaciones
    List *param_types = build_param_types(node->params, dm_global);

    if (!param_types)
        return;

    // Resolver tipo de retorno desde la anotación
    TypeDescriptor *return_type = get_type_from_annotation(node->return_type_annotation);

    if (!return_type && node->return_type_annotation)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Undefined type '%s'", node->return_type_annotation);
    }

    if (current_type)
    {
        // Método

        char *full_name = function_table_build_method_name(current_type->name, node->name);

        if (!function_table_insert(global_function_table, full_name, return_type, param_types))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Method '%s' is already defined in type '%s'", node->name, current_type->name);
        }

        free(full_name);

        // Agregar el nombre del método a la lista del tipo
        UserTypeDescriptor *utype = (UserTypeDescriptor *)current_type;
        user_type_add_method(utype, node->name);
    }
    else
    {
        // Función
        if (!function_table_insert(global_function_table, node->name, return_type, param_types))
        {
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Function '%s' is already defined", node->name);
        }
    }
}

static void register_type(TypeDefinitionNode *node)
{
    TypeDescriptor *object_type = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);

    // Resolver tipo padre
    TypeDescriptor *parent_type = get_type_from_annotation(node->father_name);

    if (!parent_type)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Undefined type '%s'", node->father_name);
        parent_type = object_type;
    }

    else if (parent_type->tag != HULK_OBJECT && parent_type->tag != HULK_USER_DEFINED)
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Cannot inherit from built-in type '%s'", parent_type->name);
        parent_type = object_type;
    }

    // Construir tipos de parámetros de inicialización
    List *param_types = build_param_types(node->init_params, dm_global);

    // Crear el UserTypeDescriptor
    UserTypeDescriptor *utype = user_type_create(node->name, parent_type, param_types);
    if (!utype)
        return;

    // Insertar en la tabla global
    if (!type_table_insert(global_type_table, utype))
    {
        dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, node->base.line, node->base.column, "Type '%s' is already defined", node->name);
        user_type_destroy(utype);
        return;
    }

    // Registrar métodos del tipo
    for (size_t i = 0; i < list_count(node->methods); i++)
    {
        FunctionDefinitionNode *method = (FunctionDefinitionNode *)list_get(node->methods, i);
        register_function(method, (TypeDescriptor *)utype);
    }
}

static List *resolve_unknown_params(List *current_params, List *resolved_types)
{
    List *new_params = list_create(0, NULL, NULL, NULL, NULL);
    size_t resolved_idx = 0;

    for (size_t j = 0; j < list_count(current_params); j++)
    {
        TypeDescriptor *current = (TypeDescriptor *)list_get(current_params, j);

        if (current == NULL)
        {
            TypeDescriptor *resolved = (TypeDescriptor *)list_get(resolved_types, resolved_idx);
            list_append(new_params, resolved);
            resolved_idx++;
        }

        else
        {
            list_append(new_params, current);
        }
    }

    return new_params;
}

static void infer_function(TypeInferenceVisitor *infer, FunctionDefinitionNode *func_def, TypeDescriptor *current_type)
{
    Visitor *visitor = (Visitor *)infer;

    // Si es un método, establecer el nombre del método actual (para base())
    if (current_type)
        infer->current_method_name = func_def->name;

    // Primera pasada de inferencia (con parámetros posiblemente NULL)
    ast_accept((ASTNode *)func_def, visitor);

    List *unannotated = get_unannotated_params(func_def->params);

    // Si no hay parámetros sin anotar, no hay más que hacer
    if (!unannotated || list_count(unannotated) == 0)
    {
        if (unannotated)
            list_destroy(unannotated);

        infer->current_method_name = NULL;
        return;
    }

    // Recolectar restricciones sobre los parámetros sin anotar
    ConstraintCollectorVisitor *collector = constraint_collector_create(infer->current_scope, unannotated);
    ast_accept((ASTNode *)func_def, (Visitor *)collector);

    // Resolver restricciones a tipos concretos
    List *resolved_types = constraint_collector_resolve(collector);

    if (!resolved_types)
    {
        constraint_collector_destroy(collector);
        list_destroy(unannotated);

        infer->current_method_name = NULL;
        return;
    }

    // Construir lista actual de tipos de parámetros (con NULL para los sin anotar)
    List *current_params = build_param_types(func_def->params, dm_global);

    // Reemplazar los NULL con los tipos resueltos
    List *new_params = resolve_unknown_params(current_params, resolved_types);

    // Actualizar la tabla de funciones con los nuevos tipos
    const char *func_name;
    char *full_name = NULL;

    if (current_type)
    {
        full_name = function_table_build_method_name(current_type->name, func_def->name);
        func_name = full_name;
    }
    else
    {
        func_name = func_def->name;
    }

    function_table_update(global_function_table, func_name, NULL, new_params);

    if (full_name)
        free(full_name);

    // Segunda pasada de inferencia con los tipos ya resueltos
    ast_accept((ASTNode *)func_def, visitor);

    // Limpiar método actual
    infer->current_method_name = NULL;

    // Limpieza
    list_destroy(current_params);
    list_destroy(resolved_types);
    list_destroy(unannotated);

    constraint_collector_destroy(collector);
}

static void infer_type(TypeInferenceVisitor *infer, TypeDefinitionNode *type_def)
{
    Visitor *visitor = (Visitor *)infer;
    UserTypeDescriptor *utype = (UserTypeDescriptor *)type_table_lookup_by_name(global_type_table, type_def->name);

    if (!utype)
        return;

    // Guardar y establecer el tipo actual para los métodos
    TypeDescriptor *old_type = infer->current_type;
    infer->current_type = (TypeDescriptor *)utype;

    // Primera pasada de inferencia sobre el tipo
    ast_accept((ASTNode *)type_def, visitor);

    // Resolver init_params sin anotar
    List *unannotated = get_unannotated_params(type_def->init_params);

    if (unannotated && list_count(unannotated) > 0)
    {
        ConstraintCollectorVisitor *collector = constraint_collector_create(infer->current_scope, unannotated);
        ast_accept((ASTNode *)type_def, (Visitor *)collector);

        List *resolved_types = constraint_collector_resolve(collector);

        if (resolved_types)
        {
            // Obtener los tipos actuales (con NULL) y reemplazar por los resueltos
            List *current = user_type_get_param_types(utype);
            List *new_params = resolve_unknown_params(current, resolved_types);

            // Reemplazar la lista de parámetros en el tipo
            list_destroy(current);
            utype->param_types = new_params;

            list_destroy(resolved_types);
        }

        constraint_collector_destroy(collector);

        // Segunda pasada de inferencia con los nuevos tipos
        ast_accept((ASTNode *)type_def, visitor);
    }

    if (unannotated)
        list_destroy(unannotated);

    // Inferir métodos del tipo
    for (size_t m = 0; m < list_count(type_def->methods); m++)
    {
        FunctionDefinitionNode *method = (FunctionDefinitionNode *)list_get(type_def->methods, m);
        infer_function(infer, method, (TypeDescriptor *)utype);
    }

    // Restaurar tipo anterior
    infer->current_type = old_type;
}


static void *visit_program_node(Visitor *visitor, ASTNode *node)
{
    TypeInferenceVisitor *infer = (TypeInferenceVisitor *)visitor;
    ProgramNode *program = (ProgramNode *)node;

    // Registrar tipos
    for (size_t i = 0; i < list_count(program->type_definitions); i++)
    {
        TypeDefinitionNode *type_def = (TypeDefinitionNode *)list_get(program->type_definitions, i);
        register_type(type_def);
    }

    // Registrar funciones globales
    for (size_t i = 0; i < list_count(program->function_definitions); i++)
    {
        FunctionDefinitionNode *func_def = (FunctionDefinitionNode *)list_get(program->function_definitions, i);
        register_function(func_def, NULL);
    }

    // Inferir definiciones de tipos
    for (size_t i = 0; i < list_count(program->type_definitions); i++)
    {
        TypeDefinitionNode *type_def = (TypeDefinitionNode *)list_get(program->type_definitions, i);
        infer_type(infer, type_def);
    }

    // Inferir definiciones de funciones globales
    for (size_t i = 0; i < list_count(program->function_definitions); i++)
    {
        FunctionDefinitionNode *func_def = (FunctionDefinitionNode *)list_get(program->function_definitions, i);
        infer_function(infer, func_def, NULL);
    }

    // Inferir expresión principal
    TypeDescriptor *root_type = ast_accept(program->root, visitor);
    program->base.return_type = root_type;

    return root_type;
}

TypeInferenceVisitor *type_inference_visitor_create(void)
{
    TypeInferenceVisitor *visitor = (TypeInferenceVisitor *)malloc(sizeof(TypeInferenceVisitor));
    if (!visitor)
        return NULL;

    visitor_init(&visitor->base);

    VisitorFunc funcs[] = { (VisitorFunc)visit_literal_node, (VisitorFunc)visit_unary_operation_node, (VisitorFunc)visit_binary_operation_node, (VisitorFunc)visit_expression_block_node, (VisitorFunc)visit_conditional_node, (VisitorFunc)visit_while_loop_node, (VisitorFunc)visit_let_in_node, (VisitorFunc)visit_variable_reference_node, (VisitorFunc)visit_reassignment_node, (VisitorFunc)visit_function_definition_node, (VisitorFunc)visit_function_call_node, (VisitorFunc)visit_type_definition_node, (VisitorFunc)visit_attribute_access_node, (VisitorFunc)visit_method_access_node, (VisitorFunc)visit_type_instanciation_node, (VisitorFunc)visit_base_call_node, (VisitorFunc)visit_is_node, (VisitorFunc)visit_as_node, (VisitorFunc)visit_program_node };
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