#include "constraint_collector_visitor.h"

static int constraint_cmp_by_name_for_search(const void *a, const void *b)
{
    const TypeConstraint *constraint = (const TypeConstraint *)a;
    const char *name = (const char *)b;
    return strcmp(constraint->param_name, name);
}

static void* visit_literal_node(Visitor* visitor, ASTNode* node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

static void add_unary_operator_constraint(ConstraintCollectorVisitor *collector, VariableReferenceNode *var_ref, HulkOperator op, int line)
{
    TypeDescriptor *expected_type = NULL;

    if (op == OP_NOT)
        expected_type = type_table_lookup_by_tag(global_type_table, HULK_BOOL);

    else if (op == OP_SUB)
        expected_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);

    else
        return;

    TypeDescriptor *types[] = { expected_type };
    TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, line);
    constraint_collector_add(collector, c);
}

static void *visit_unary_operation_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    UnaryOperationNode *unary = (UnaryOperationNode *)node;

    ast_accept(unary->operand, visitor);

    if (unary->operand->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *var_ref = (VariableReferenceNode *)unary->operand;

        if (constraint_collector_get(collector, var_ref->name))
            add_unary_operator_constraint(collector, var_ref, unary->operator, unary->base.line);
    }

    return NULL;
}

static void add_binary_op_constraint(ConstraintCollectorVisitor *collector, const char *param_name, HulkOperator op, bool is_left, TypeDescriptor *other_type, int line)
{
    TypeDescriptor *number_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *string_type = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *bool_type   = type_table_lookup_by_tag(global_type_table, HULK_BOOL);

    TypeDescriptor *types[2];
    size_t types_count = 0;

    if (is_arithmetic_operator(op))
    {
        types[0] = number_type;
        types_count = 1;
    }

    else if (is_comparison_operator(op))
    {
        if (other_type == number_type)
        {
            types[0] = number_type;
            types_count = 1;
        }

        else if (other_type == string_type)
        {
            types[0] = string_type;
            types_count = 1;
        }

        else
        {
            types[0] = number_type;
            types[1] = string_type;
            types_count = 2;
        }
    }

    else if (is_logical_operator(op))
    {
        types[0] = bool_type;
        types_count = 1;
    }

    else if (is_string_operator(op))
    {
        if (is_left)
        {
            types[0] = string_type;
            types_count = 1;
        }

        else
        {
            types[0] = string_type;
            types[1] = number_type;
            types_count = 2;
        }
    }

    else
        return;

    TypeConstraint *c = constraint_create(param_name, CONSTRAINT_CONFORMS, types, types_count, line);
    constraint_collector_add(collector, c);
}

static void check_binary_operand(ConstraintCollectorVisitor *collector, ASTNode *operand, TypeDescriptor *other_type, HulkOperator op, bool is_left, int line)
{
    if (!operand || operand->node_type != NODE_VARIABLE_REFERENCE)
        return;

    VariableReferenceNode *var_ref = (VariableReferenceNode *)operand;

    if (constraint_collector_get(collector, var_ref->name))
        add_binary_op_constraint(collector, var_ref->name, op, is_left, other_type, line);
}

static void *visit_binary_operation_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    BinaryOperationNode *bin = (BinaryOperationNode *)node;

    ast_accept(bin->left, visitor);
    ast_accept(bin->right, visitor);

    check_binary_operand(collector, bin->left, bin->right->return_type, bin->operator, true, bin->base.line);
    check_binary_operand(collector, bin->right, bin->left->return_type, bin->operator, false, bin->base.line);

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

    return NULL;
}

static void *visit_while_loop_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    WhileLoopNode *loop = (WhileLoopNode *)node;

    ast_accept(loop->condition, visitor);

    if (loop->condition->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *var_ref = (VariableReferenceNode *)loop->condition;

        if (constraint_collector_get(collector, var_ref->name))
        {
            TypeDescriptor *types[] = { type_table_lookup_by_tag(global_type_table, HULK_BOOL) };
            TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, loop->base.line);
            constraint_collector_add(collector, c);
        }
    }

    if (loop->body)
        ast_accept(loop->body, visitor);

    return NULL;
}

static void constrain_branch_to_type(ConstraintCollectorVisitor *collector, ASTNode *branch, TypeDescriptor *target_type, int line)
{
    if (!branch || !target_type)
        return;

    // Recorrer bloques anidados hasta encontrar la última expresión
    ASTNode *current = branch;
    while (current->node_type == NODE_EXPRESSION_BLOCK)
    {
        ExpressionBlockNode *block = (ExpressionBlockNode *)current;
        size_t count = list_count(block->expressions);
        if (count == 0)
            return;

        current = (ASTNode *)list_get(block->expressions, count - 1);
    }

    // Verificar si es una referencia a un parámetro sin anotar
    if (current->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *var_ref = (VariableReferenceNode *)current;
        if (constraint_collector_get(collector, var_ref->name))
        {
            TypeDescriptor *types[] = { target_type };
            TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, line);
            constraint_collector_add(collector, c);
        }
    }
}

static void check_condition_constraint(ConstraintCollectorVisitor *collector, ASTNode *condition, int line)
{
    if (condition->node_type != NODE_VARIABLE_REFERENCE)
        return;

    VariableReferenceNode *var_ref = (VariableReferenceNode *)condition;
    if (constraint_collector_get(collector, var_ref->name))
    {
        TypeDescriptor *types[] = { type_table_lookup_by_tag(global_type_table, HULK_BOOL) };
        TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, line);
        constraint_collector_add(collector, c);
    }
}

static void *visit_conditional_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    ConditionalNode *cond = (ConditionalNode *)node;

    ast_accept(cond->condition, visitor);
    check_condition_constraint(collector, cond->condition, cond->base.line);

    ast_accept(cond->then_branch, visitor);

    if (cond->else_branch)
        ast_accept(cond->else_branch, visitor);

    TypeDescriptor *then_type = cond->then_branch->return_type;
    TypeDescriptor *else_type = cond->else_branch ? cond->else_branch->return_type : NULL;

    constrain_branch_to_type(collector, cond->else_branch, then_type, cond->base.line);
    constrain_branch_to_type(collector, cond->then_branch, else_type, cond->base.line);

    return NULL;
}


static void *visit_let_in_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    LetInNode *let_in = (LetInNode *)node;

    for (size_t i = 0; i < list_count(let_in->bindings); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(let_in->bindings, i);

        ast_accept(binding->initializer, visitor);

        if (binding->type_name && binding->initializer->node_type == NODE_VARIABLE_REFERENCE)
        {
            VariableReferenceNode *var_ref = (VariableReferenceNode *)binding->initializer;

            if (constraint_collector_get(collector, var_ref->name))
            {
                TypeDescriptor *annotated_type = type_table_lookup_by_name(global_type_table, binding->type_name);

                if (annotated_type)
                {
                    TypeDescriptor *types[] = { annotated_type };
                    TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, let_in->base.line);
                    constraint_collector_add(collector, c);
                }
            }
        }
    }

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
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    ReassignmentNode *reassign = (ReassignmentNode *)node;

    ast_accept(reassign->target, visitor);
    ast_accept(reassign->value, visitor);

    // Restricción sobre el target si es un parámetro sin anotar
    if (reassign->target->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *target_ref = (VariableReferenceNode *)reassign->target;

        if (constraint_collector_get(collector, target_ref->name))
        {
            TypeDescriptor *value_type = reassign->value->return_type;

            if (value_type)
            {
                TypeDescriptor *types[] = { value_type };
                TypeConstraint *c = constraint_create(target_ref->name, CONSTRAINT_EQUAL, types, 1, reassign->base.line);
                constraint_collector_add(collector, c);
            }
        }
    }

    // Restricción sobre el valor si es un parámetro sin anotar
    if (reassign->value->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *value_ref = (VariableReferenceNode *)reassign->value;

        if (constraint_collector_get(collector, value_ref->name))
        {
            TypeDescriptor *target_type = reassign->target->return_type;

            if (target_type)
            {
                TypeDescriptor *types[] = { target_type };
                TypeConstraint *c = constraint_create(value_ref->name, CONSTRAINT_CONFORMS, types, 1, reassign->base.line);
                constraint_collector_add(collector, c);
            }
        }
    }

    return NULL;
}

static void *visit_function_definition_node(Visitor *visitor, ASTNode *node)
{
    FunctionDefinitionNode *func = (FunctionDefinitionNode *)node;
    ast_accept(func->body, visitor);
    return NULL;
}

static void *visit_type_definition_node(Visitor *visitor, ASTNode *node)
{
    TypeDefinitionNode *type_def = (TypeDefinitionNode *)node;

    // Visitar argumentos de inicialización del padre
    for (size_t i = 0; i < list_count(type_def->father_init_args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(type_def->father_init_args, i);
        ast_accept(arg, visitor);
    }

    // Visitar inicializadores de atributos
    for (size_t i = 0; i < list_count(type_def->attributes); i++)
    {
        SymbolBinding *b = (SymbolBinding *)list_get(type_def->attributes, i);
        ast_accept(b->initializer, visitor);
    }

    return NULL;
}

static void *visit_function_call_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    FunctionCallNode *call = (FunctionCallNode *)node;

    // Visitar recursivamente los argumentos
    for (size_t i = 0; i < list_count(call->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(call->args, i);
        ast_accept(arg, visitor);
    }

    // Obtener los tipos de los parámetros de la función
    List *param_types = function_table_get_params_types(global_function_table, call->name);

    if (!param_types)
        return NULL;

    // Para cada argumento que sea un parámetro sin anotar, generar restricción
    size_t num_args = list_count(call->args);

    for (size_t i = 0; i < num_args && i < list_count(param_types); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(call->args, i);

        if (arg->node_type == NODE_VARIABLE_REFERENCE)
        {
            VariableReferenceNode *var_ref = (VariableReferenceNode *)arg;

            if (constraint_collector_get(collector, var_ref->name))
            {
                TypeDescriptor *param_type = (TypeDescriptor *)list_get(param_types, i);

                if (param_type)
                {
                    TypeDescriptor *types[] = { param_type };
                    TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, call->base.line);
                    constraint_collector_add(collector, c);
                }
            }
        }
    }

    return NULL;
}

static void *visit_attribute_access_node(Visitor *visitor, ASTNode *node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

/**
 * @brief Filtro: retorna true si el tipo es más general que algún otro en la lista.
 *        (existe otro tipo que conforma a este, por tanto este es ancestro y se descarta).
 */
static bool is_more_specific_than_any(const void *element, void *context)
{
    const TypeDescriptor *type = (const TypeDescriptor *)element;
    const List *candidates = (const List *)context;

    for (size_t i = 0; i < list_count(candidates); i++)
    {
        const TypeDescriptor *other = (const TypeDescriptor *)list_get(candidates, i);

        if (type != other && type_conforms_to(other, type))
            return true;
    }

    return false;
}

/**
 * @brief Busca los tipos más específicos que tienen un método con el nombre dado.
 *
 * @param method_name Nombre del método.
 * @return Lista de TypeDescriptor* con los tipos más específicos (debe liberarse con list_destroy).
 */
static List *find_specific_types_with_method(const char *method_name)
{
    List *candidate_types = list_create(0, NULL, NULL, NULL, NULL);

    for (size_t i = 0; i < list_count(global_type_table->user_types); i++)
    {
        UserTypeDescriptor *ut = (UserTypeDescriptor *)list_get(global_type_table->user_types, i);

        if (user_type_has_method(ut, method_name))
            list_append(candidate_types, (TypeDescriptor *)ut);
    }

    List *specific_types = list_filter(candidate_types, is_more_specific_than_any, candidate_types);
    list_destroy(candidate_types);

    return specific_types;
}

/**
 * @brief Genera restricciones para los argumentos de una llamada a método.
 *
 * @param collector  Visitor recolector.
 * @param method_acc Nodo de acceso a método.
 * @param full_name  Nombre completo del método en la tabla global (_<tipo>_<metodo>).
 */
static void constrain_method_args(ConstraintCollectorVisitor *collector, MethodAccessNode *method_acc, const char *full_name)
{
    List *param_types = function_table_get_params_types(global_function_table, full_name);

    if (!param_types)
        return;

    for (size_t i = 0; i < list_count(method_acc->args) && i < list_count(param_types); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(method_acc->args, i);

        if (arg->node_type == NODE_VARIABLE_REFERENCE)
        {
            VariableReferenceNode *arg_ref = (VariableReferenceNode *)arg;

            if (constraint_collector_get(collector, arg_ref->name))
            {
                TypeDescriptor *ptype = (TypeDescriptor *)list_get(param_types, i);
                TypeDescriptor *types[] = { ptype };
                TypeConstraint *c = constraint_create(arg_ref->name, CONSTRAINT_CONFORMS, types, 1, method_acc->base.line);
                constraint_collector_add(collector, c);
            }
        }
    }
}

static void *visit_method_access_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    MethodAccessNode *method_acc = (MethodAccessNode *)node;

    // Visitar target y argumentos
    ast_accept(method_acc->target, visitor);

    for (size_t i = 0; i < list_count(method_acc->args); i++)
    {
        ast_accept((ASTNode *)list_get(method_acc->args, i), visitor);
    }

    // Si el target es un parámetro sin anotar, inferir su tipo buscando tipos con este método
    if (method_acc->target->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *var_ref = (VariableReferenceNode *)method_acc->target;
        if (constraint_collector_get(collector, var_ref->name))
        {
            List *specific_types = find_specific_types_with_method(method_acc->method_name);
            if (list_count(specific_types) > 0)
            {
                TypeDescriptor **types_array = malloc(list_count(specific_types) * sizeof(TypeDescriptor *));

                for (size_t i = 0; i < list_count(specific_types); i++)
                {
                    types_array[i] = (TypeDescriptor *)list_get(specific_types, i);
                }

                TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types_array, list_count(specific_types), method_acc->base.line);
                constraint_collector_add(collector, c);
                free(types_array);
            }

            list_destroy(specific_types);
        }
    }

    // Si el target tiene tipo definido, restringir los argumentos según el método
    TypeDescriptor *target_type = method_acc->target->return_type;

    if (target_type && target_type->tag == HULK_USER_DEFINED)
    {
        UserTypeDescriptor *user_type = (UserTypeDescriptor *)target_type;
        char *full_name = function_table_build_method_name(user_type->base.name, method_acc->method_name);

        constrain_method_args(collector, method_acc, full_name);

        free(full_name);
    }

    return NULL;
}

static void *visit_type_instanciation_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    TypeInstanciationNode *inst = (TypeInstanciationNode *)node;

    // Buscar el tipo a instanciar
    TypeDescriptor *type = type_table_lookup_by_name(global_type_table, inst->type_name);

    if (!type || type->tag != HULK_USER_DEFINED)
        return NULL;

    // Obtener los tipos de los parámetros de inicialización del tipo
    List *param_types = user_type_get_param_types((UserTypeDescriptor *)type);

    // Visitar cada argumento y generar restricciones si es un parámetro sin anotar
    for (size_t i = 0; i < list_count(inst->args); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(inst->args, i);
        ast_accept(arg, visitor);

        // Si el argumento es una variable sin anotar, restringirla al tipo del parámetro
        if (arg->node_type == NODE_VARIABLE_REFERENCE)
        {
            VariableReferenceNode *var_ref = (VariableReferenceNode *)arg;

            if (constraint_collector_get(collector, var_ref->name) && i < list_count(param_types))
            {
                TypeDescriptor *param_type = (TypeDescriptor *)list_get(param_types, i);
                TypeDescriptor *types[] = { param_type };
                TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, inst->base.line);
                constraint_collector_add(collector, c);
            }
        }
    }

    return NULL;
}


static void *visit_base_call_node(Visitor *visitor, ASTNode *node)
{
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    BaseCallNode *base_call = (BaseCallNode *)node;

    // Visitar los argumentos
    for (size_t i = 0; i < list_count(base_call->args); i++)
    {
        ast_accept((ASTNode *)list_get(base_call->args, i), visitor);
    }

    // Si no tenemos type_name o method_name, no podemos resolver
    if (!base_call->type_name || !base_call->method_name)
    {
        printf("No tipo o no metodo\n");
        return NULL;
    }

    // Obtener el nombre del padre
    UserTypeDescriptor *utype = type_to_user_defined(type_table_lookup_by_name(global_type_table, base_call->type_name));

    if (!utype)
        return NULL;

    char *father_name = utype->base.parent->name;

    // Buscar el método en la tabla global con el nombre compuesto
    char *full_name = function_table_build_method_name(father_name, base_call->method_name);
    List *param_types = function_table_get_params_types(global_function_table, full_name);

    printf("Nombre resuelto : %s\n" , full_name);

    free(full_name);

    if (!param_types)
        return NULL;

    // Generar restricciones para los argumentos que sean parámetros sin anotar
    for (size_t i = 0; i < list_count(base_call->args) && i < list_count(param_types); i++)
    {
        ASTNode *arg = (ASTNode *)list_get(base_call->args, i);

        if (arg->node_type == NODE_VARIABLE_REFERENCE)
        {
            VariableReferenceNode *arg_ref = (VariableReferenceNode *)arg;

            if (constraint_collector_get(collector, arg_ref->name))
            {
                TypeDescriptor *ptype = (TypeDescriptor *)list_get(param_types, i);
                printf("NULL param_type : %s\n", ptype == NULL ? "true" : "false");
                
                if(ptype)
                {
                    TypeDescriptor *types[] = { ptype };
                    TypeConstraint *c = constraint_create(arg_ref->name, CONSTRAINT_CONFORMS, types, 1, base_call->base.line);
                    constraint_collector_add(collector, c);
                }
                
            }
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
    ConstraintCollectorVisitor *collector = (ConstraintCollectorVisitor *)visitor;
    AsNode *as_node = (AsNode *)node;

    // Visitar el target recursivamente
    ast_accept(as_node->target, visitor);

    // Si el target es un parámetro sin anotar, restringirlo al tipo del as
    if (as_node->target->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *var_ref = (VariableReferenceNode *)as_node->target;

        if (constraint_collector_get(collector, var_ref->name))
        {
            TypeDescriptor *as_type = type_table_lookup_by_name(global_type_table, as_node->type_name);

            if (as_type)
            {
                TypeDescriptor *types[] = { as_type };
                TypeConstraint *c = constraint_create(var_ref->name, CONSTRAINT_CONFORMS, types, 1, as_node->base.line);
                constraint_collector_add(collector, c);
            }
        }
    }

    return NULL;
}

static void* visit_program_node(Visitor* visitor, VariableReferenceNode* node)
{
    (void)visitor;
    (void)node;
    return NULL;
}

ConstraintCollectorVisitor *constraint_collector_create(Scope *initial_scope, List *unannotated_params)
{
    ConstraintCollectorVisitor *visitor = (ConstraintCollectorVisitor *)malloc(sizeof(ConstraintCollectorVisitor));
    
    if (!visitor)
        return NULL;

    visitor_init(&visitor->base);

    VisitorFunc funcs[] = { (VisitorFunc)visit_literal_node, (VisitorFunc)visit_unary_operation_node, (VisitorFunc)visit_binary_operation_node, (VisitorFunc)visit_expression_block_node, (VisitorFunc)visit_conditional_node, (VisitorFunc)visit_while_loop_node, (VisitorFunc)visit_let_in_node, (VisitorFunc)visit_variable_reference_node, (VisitorFunc)visit_reassignment_node, (VisitorFunc)visit_function_definition_node, (VisitorFunc)visit_function_call_node, (VisitorFunc)visit_type_definition_node, (VisitorFunc)visit_attribute_access_node, (VisitorFunc)visit_method_access_node, (VisitorFunc)visit_type_instanciation_node, (VisitorFunc)visit_base_call_node, (VisitorFunc)visit_is_node, (VisitorFunc)visit_as_node, (VisitorFunc)visit_program_node };
    visitor_register_functions(&visitor->base, funcs);

    visitor->constraints = list_create(0, NULL, (free_func)constraint_destroy, constraint_cmp_by_name_for_search, NULL);
    
    if (!visitor->constraints)
    {
        free(visitor);
        return NULL;
    }

    if (unannotated_params)
    {
        for (size_t i = 0; i < list_count(unannotated_params); i++)
        {
            char *param_name = (char *)list_get(unannotated_params, i);
            TypeConstraint *initial = constraint_create_initial(param_name);

            if (initial)
                list_append(visitor->constraints, initial);
        }
    }

    visitor->current_scope = initial_scope;
    return visitor;
}

void constraint_collector_destroy(ConstraintCollectorVisitor *visitor)
{
    if (!visitor)
        return;

    if (visitor->constraints)
        list_destroy(visitor->constraints);

    visitor_destroy(&visitor->base);
    free(visitor);
}

TypeConstraint *constraint_collector_get(ConstraintCollectorVisitor *visitor, const char *param_name)
{
    if (!visitor || !param_name)
        return NULL;

    int index = list_index_of(visitor->constraints, param_name);
    if (index == -1)
        return NULL;

    return (TypeConstraint *)list_get(visitor->constraints, index);
}

void constraint_collector_add(ConstraintCollectorVisitor *visitor, TypeConstraint *constraint)
{
    if (!visitor || !constraint)
        return;

    int idx = list_index_of(visitor->constraints, constraint->param_name);
    if (idx == -1)
    {
        constraint_destroy(constraint);
        return;
    }

    TypeConstraint *existing = (TypeConstraint *)list_get(visitor->constraints, idx);
    TypeConstraint *merged = constraint_merge(existing, constraint);
    constraint_destroy(constraint);

    if (merged)
        list_set(visitor->constraints, idx, merged);
}

List *constraint_collector_resolve(ConstraintCollectorVisitor *visitor)
{
    if (!visitor)
        return NULL;

    List *resolved_types = list_create(0, NULL, NULL, NULL, NULL);
    if (!resolved_types)
        return NULL;

    for (size_t i = 0; i < list_count(visitor->constraints); i++)
    {
        TypeConstraint *c = (TypeConstraint *)list_get(visitor->constraints, i);
        TypeDescriptor *inferred = NULL;
        int result = constraint_resolve_type(c, &inferred);
        char *cstr = constraint_to_string(c);

        if (result == 1)
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, constraint_last_line(c), 0,
                "Conflicting inferred types for unannotated param '%s': %s", c->param_name, cstr);

        else if (result == 2)
            dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, constraint_last_line(c), 0,
                "Unannotated param '%s' is consistent with multiple types: %s, please add type annotation", c->param_name, cstr);

        list_append(resolved_types, inferred);
        free(cstr);
    }

    return resolved_types;
}