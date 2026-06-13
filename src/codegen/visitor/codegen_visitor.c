#include "codegen_visitor.h"

static void *visit_literal_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    LiteralNode *literal = (LiteralNode *)node;

    switch (literal->base.return_type->tag)
    {
        case HULK_NUMBER:
            return (void *)LLVMConstReal(LLVMDoubleTypeInContext(cg->context), literal->value.number_value);

        case HULK_BOOL:
            return (void *)LLVMConstInt(LLVMInt1TypeInContext(cg->context), literal->value.bool_value ? 1 : 0, 0);

        case HULK_STRING:
            return (void *)LLVMBuildGlobalStringPtr(cg->builder, literal->value.string_value, "str");

        default:
            return NULL;
    }
}

static void *visit_unary_operation_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    UnaryOperationNode *unary = (UnaryOperationNode *)node;

    LLVMValueRef operand = (LLVMValueRef)(intptr_t)ast_accept(unary->operand, visitor);

    if (!operand)
        return NULL;

    if (unary->operator == OP_NOT)
        return (void *)LLVMBuildXor(cg->builder, operand, LLVMConstInt(LLVMInt1TypeInContext(cg->context), 1, 0), "not");

    if (unary->operator == OP_SUB)
        return (void *)LLVMBuildFNeg(cg->builder, operand, "neg");

    return NULL;
}

static void *visit_binary_operation_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    BinaryOperationNode *bin = (BinaryOperationNode *)node;

    LLVMValueRef left  = (LLVMValueRef)(intptr_t)ast_accept(bin->left, visitor);
    LLVMValueRef right = (LLVMValueRef)(intptr_t)ast_accept(bin->right, visitor);

    if (!left || !right)
        return NULL;

    return (void *)code_gen_build_binary_op(cg->builder, bin->operator, left, right);
}

static void *visit_expression_block_node(Visitor *visitor, ASTNode *node)
{
    ExpressionBlockNode *block = (ExpressionBlockNode *)node;

    LLVMValueRef last_val = NULL;

    // Evaluar cada expresión en secuencia, retornar el último valor
    for (size_t i = 0; i < list_count(block->expressions); i++)
    {
        ASTNode *expr = (ASTNode *)list_get(block->expressions, i);

        if (expr)
            last_val = (LLVMValueRef)(intptr_t)ast_accept(expr, visitor);
    }

    return (void *)last_val;
}

static void *visit_conditional_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    ConditionalNode *cond = (ConditionalNode *)node;

    // Evaluar la condición
    LLVMValueRef cond_val = (LLVMValueRef)(intptr_t)ast_accept(cond->condition, visitor);

    if (!cond_val)
        return NULL;

    // Obtener la función actual y crear los bloques
    LLVMBasicBlockRef start_block = LLVMGetInsertBlock(cg->builder);
    LLVMValueRef function = LLVMGetBasicBlockParent(start_block);

    LLVMBasicBlockRef then_block  = LLVMAppendBasicBlockInContext(cg->context, function, "if.then");
    LLVMBasicBlockRef else_block  = cond->else_branch ? LLVMAppendBasicBlockInContext(cg->context, function, "if.else") : NULL;
    LLVMBasicBlockRef merge_block = LLVMAppendBasicBlockInContext(cg->context, function, "if.merge");

    // Salto condicional
    LLVMBuildCondBr(cg->builder, cond_val, then_block, else_block ? else_block : merge_block);

    // Generar rama then
    LLVMPositionBuilderAtEnd(cg->builder, then_block);
    LLVMValueRef then_val = (LLVMValueRef)(intptr_t)ast_accept(cond->then_branch, visitor);
    LLVMBuildBr(cg->builder, merge_block);
    LLVMBasicBlockRef end_then = LLVMGetInsertBlock(cg->builder);

    // Generar rama else si existe
    LLVMValueRef else_val = NULL;
    LLVMBasicBlockRef end_else = NULL;

    if (cond->else_branch)
    {
        LLVMPositionBuilderAtEnd(cg->builder, else_block);
        else_val = (LLVMValueRef)(intptr_t)ast_accept(cond->else_branch, visitor);
        LLVMBuildBr(cg->builder, merge_block);
        end_else = LLVMGetInsertBlock(cg->builder);
    }

    // Posicionar en el bloque de unión y construir PHI
    LLVMPositionBuilderAtEnd(cg->builder, merge_block);

    return (void *)code_gen_build_conditional_phi(cg->builder, cg->context, cond->base.return_type, then_val, end_then, else_val, end_else, start_block, cond->else_branch != NULL);
}

static void *visit_while_loop_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    WhileLoopNode *loop = (WhileLoopNode *)node;

    // Obtener la función actual y crear los bloques
    LLVMBasicBlockRef start_block = LLVMGetInsertBlock(cg->builder);
    LLVMValueRef function = LLVMGetBasicBlockParent(start_block);

    LLVMBasicBlockRef cond_block = LLVMAppendBasicBlockInContext(cg->context, function, "while.cond");
    LLVMBasicBlockRef body_block = LLVMAppendBasicBlockInContext(cg->context, function, "while.body");
    LLVMBasicBlockRef end_block  = LLVMAppendBasicBlockInContext(cg->context, function, "while.end");

    // Saltar a la condición
    LLVMBuildBr(cg->builder, cond_block);
    LLVMPositionBuilderAtEnd(cg->builder, cond_block);

    // Construir PHI para el valor del while (NULL si es Void)
    LLVMValueRef phi = code_gen_build_while_phi(cg->builder, cg->context, loop->base.return_type, start_block);

    // Evaluar la condición
    LLVMValueRef cond_val = (LLVMValueRef)(intptr_t)ast_accept(loop->condition, visitor);

    if (!cond_val)
        return NULL;

    // Salto condicional: cuerpo o fin
    LLVMBuildCondBr(cg->builder, cond_val, body_block, end_block);

    // Generar el cuerpo
    LLVMPositionBuilderAtEnd(cg->builder, body_block);
    LLVMValueRef body_val = loop->body ? (LLVMValueRef)(intptr_t)ast_accept(loop->body, visitor) : NULL;
    LLVMBasicBlockRef body_end = LLVMGetInsertBlock(cg->builder);

    // Añadir el valor del cuerpo al PHI
    code_gen_add_while_phi_incoming(phi, body_val, body_end);

    // Volver a la condición
    LLVMBuildBr(cg->builder, cond_block);

    // Continuar después del while
    LLVMPositionBuilderAtEnd(cg->builder, end_block);

    return (void *)phi;
}

static void *visit_let_in_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    LetInNode *let_in = (LetInNode *)node;

    // Crear nuevo scope anidado
    Scope *old_scope = cg->current_scope;
    Scope *current = scope_create(old_scope);
    cg->current_scope = current;

    // Procesar cada binding
    for (size_t i = 0; i < list_count(let_in->bindings); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(let_in->bindings, i);

        if (!binding)
            continue;

        // Si el símbolo ya existe en este scope, crear scope extra (shadowing)
        if (scope_lookup_current(current, binding->name))
        {
            Scope *extra = scope_create(current);
            current = extra;
            cg->current_scope = current;
        }

        // Evaluar el inicializador
        LLVMValueRef init_val = NULL;

        if (binding->initializer)
            init_val = (LLVMValueRef)(intptr_t)ast_accept(binding->initializer, visitor);

        // Crear alloca y almacenar el valor inicial
        TypeDescriptor *var_type = binding->return_type;
        LLVMTypeRef llvm_type = code_gen_get_llvm_type(cg->context, var_type);
        LLVMValueRef alloca = LLVMBuildAlloca(cg->builder, llvm_type, binding->name);

        if (init_val)
            LLVMBuildStore(cg->builder, init_val, alloca);

        // Insertar en el scope
        scope_add_variable_with_alloca(current, binding->name, var_type, alloca);
    }

    // Generar el cuerpo
    LLVMValueRef body_val = NULL;

    if (let_in->body)
        body_val = (LLVMValueRef)(intptr_t)ast_accept(let_in->body, visitor);

    // Restaurar scope
    scope_destroy_until(current, old_scope);
    cg->current_scope = old_scope;

    return (void *)body_val;
}

static void *visit_variable_reference_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    VariableReferenceNode *var_ref = (VariableReferenceNode *)node;

    // Obtener el alloca desde el scope
    LLVMValueRef alloca = scope_get_alloca(cg->current_scope, var_ref->name);

    if (!alloca)
        return NULL;

    // Cargar el valor desde el alloca
    LLVMTypeRef llvm_type = code_gen_get_llvm_type(cg->context, var_ref->base.return_type);

    return (void *)LLVMBuildLoad2(cg->builder, llvm_type, alloca, var_ref->name);
}

static void *visit_reassignment_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    ReassignmentNode *reassign = (ReassignmentNode *)node;

    // Evaluar la expresión del nuevo valor
    LLVMValueRef new_val = (LLVMValueRef)(intptr_t)ast_accept(reassign->value, visitor);

    if (!new_val)
        return NULL;

    // Reasignación a variable simple (x := valor)
    if (reassign->target->node_type == NODE_VARIABLE_REFERENCE)
    {
        VariableReferenceNode *var_ref = (VariableReferenceNode *)reassign->target;

        // Obtener el alloca desde el scope unificado
        LLVMValueRef alloca = scope_get_alloca(cg->current_scope, var_ref->name);

        if (!alloca)
            return NULL;

        LLVMBuildStore(cg->builder, new_val, alloca);
        return (void *)new_val;
    }

    // Reasignación a atributo (self.attr := valor)
    if (reassign->target->node_type == NODE_ATTRIBUTE_ACCESS)
    {
        AttributeAccessNode *attr = (AttributeAccessNode *)reassign->target;

        // Evaluar el target (self)
        LLVMValueRef target_val = (LLVMValueRef)(intptr_t)ast_accept(attr->target, visitor);

        if (!target_val)
            return NULL;

        // Obtener el tipo del target
        TypeDescriptor *target_type = attr->target->return_type;

        if (!target_type || target_type->tag != HULK_USER_DEFINED)
            return NULL;

        UserTypeDescriptor *user_type = (UserTypeDescriptor *)target_type;

        // Obtener índice del atributo
        int attr_index = code_gen_get_attribute_index(user_type, attr->attribute_name);

        if (attr_index < 0)
            return NULL;

        // GEP al campo y store
        LLVMValueRef field_ptr = LLVMBuildStructGEP2(cg->builder, user_type->struct_type, target_val, attr_index, attr->attribute_name);

        LLVMBuildStore(cg->builder, new_val, field_ptr);

        return (void *)new_val;
    }

    return NULL;
}

static void *visit_function_definition_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    FunctionDefinitionNode *func_def = (FunctionDefinitionNode *)node;

    // Construir nombre de búsqueda (método: _Type_method, función: name)
    char *lookup_name = NULL;

    if (cg->current_self_type)
    {
        lookup_name = function_table_build_method_name(cg->current_self_type->base.name, func_def->name);
    }
    else
    {
        lookup_name = strdup(func_def->name);
    }

    // Obtener la función LLVM desde la tabla global
    LLVMValueRef func = function_table_get_fn(global_function_table, lookup_name);

    if (!func)
    {
        free(lookup_name);
        return NULL;
    }
    
    // Crear bloque de entrada
    LLVMBasicBlockRef entry_block = LLVMAppendBasicBlockInContext(cg->context, func, "entry");
    LLVMPositionBuilderAtEnd(cg->builder, entry_block);

    // Crear nuevo scope
    Scope *old_scope = cg->current_scope;
    cg->current_scope = scope_create(old_scope);

    size_t param_offset = 0;

    // Si es método, registrar self
    if (cg->current_self_type)
    {
        LLVMValueRef self_val = LLVMGetParam(func, 0);

        LLVMTypeRef llvm_type = code_gen_get_llvm_type(cg->context, (TypeDescriptor *)cg->current_self_type);
        LLVMValueRef self_alloca = LLVMBuildAlloca(cg->builder, llvm_type, "self");

        LLVMBuildStore(cg->builder, self_val, self_alloca);

        scope_add_self_instance(cg->current_scope, (TypeDescriptor *)cg->current_self_type);
        scope_set_alloca(cg->current_scope, "self", self_alloca);

        param_offset = 1;
    }

    // Registrar parámetros
    List *param_types = function_table_get_params_types(global_function_table, lookup_name);

    for (size_t i = 0; i < list_count(func_def->params); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(func_def->params, i);
        LLVMValueRef param_val = LLVMGetParam(func, i + param_offset);

        TypeDescriptor *param_type = param_types ? (TypeDescriptor *)list_get(param_types, i) : NULL;
        LLVMTypeRef llvm_type = code_gen_get_llvm_type(cg->context, param_type);
        LLVMValueRef alloca = LLVMBuildAlloca(cg->builder, llvm_type, binding->name);

        LLVMBuildStore(cg->builder, param_val, alloca);

        scope_add_parameter(cg->current_scope, binding->name, param_type);
        scope_set_alloca(cg->current_scope, binding->name, alloca);
    }

    // Generar el cuerpo
    LLVMValueRef body_val = NULL;

    if (func_def->body)
        body_val = (LLVMValueRef)(intptr_t)ast_accept(func_def->body, visitor);

    // Generar retorno
    TypeDescriptor *return_type = function_table_get_return_type(global_function_table, lookup_name, NULL);

    if (return_type && return_type->tag == HULK_VOID)
        LLVMBuildRetVoid(cg->builder);

    else if (body_val)
        LLVMBuildRet(cg->builder, body_val);

    else
        LLVMBuildRet(cg->builder, code_gen_get_default_value(cg->context, return_type));

    // Restaurar scope
    scope_destroy(cg->current_scope);
    cg->current_scope = old_scope;

    free(lookup_name);

    return NULL;
}

static void *visit_function_call_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    FunctionCallNode *call = (FunctionCallNode *)node;

    // Evaluar argumentos
    size_t arg_count = list_count(call->args);
    LLVMValueRef *args = NULL;

    if (arg_count > 0)
    {
        args = (LLVMValueRef *)malloc(arg_count * sizeof(LLVMValueRef));

        for (size_t i = 0; i < arg_count; i++)
        {
            ASTNode *arg = (ASTNode *)list_get(call->args, i);
            args[i] = (LLVMValueRef)(intptr_t)ast_accept(arg, visitor);
        }
    }

    // Resolver nombre real (print despacha por tipo de su argumento)
    ASTNode *first_arg = (arg_count > 0) ? (ASTNode *)list_get(call->args, 0) : NULL;
    TypeDescriptor *first_arg_type = first_arg ? first_arg->return_type : NULL;
    const char *func_name = code_gen_resolve_function_name(call->name, first_arg_type);

    // Generar llamada
    LLVMValueRef result = code_gen_build_call(cg->builder, func_name, args, arg_count);

    free(args);
    return (void *)result;
}

static LLVMValueRef codegen_create_constructor(CodeGenVisitor *cg, TypeDefinitionNode *type_def, UserTypeDescriptor *user_type)
{
    LLVMTypeRef struct_ptr_type = LLVMPointerType(user_type->struct_type, 0);

    // Construir firma: self + params
    size_t param_count = list_count(user_type->param_types);
    LLVMTypeRef *param_types_arr = (LLVMTypeRef *)malloc((param_count + 1) * sizeof(LLVMTypeRef));

    param_types_arr[0] = struct_ptr_type;

    for (size_t i = 0; i < param_count; i++)
    {
        TypeDescriptor *pt = (TypeDescriptor *)list_get(user_type->param_types, i);
        param_types_arr[i + 1] = code_gen_get_llvm_type(cg->context, pt);
    }

    LLVMTypeRef func_type = LLVMFunctionType(struct_ptr_type, param_types_arr, param_count + 1, 0);
    free(param_types_arr);

    char constructor_name[256];
    snprintf(constructor_name, sizeof(constructor_name), "_new_%s", user_type->base.name);

    LLVMValueRef constructor = LLVMAddFunction(cg->module, constructor_name, func_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(cg->context, constructor, "entry");

    LLVMBasicBlockRef prev_block = LLVMGetInsertBlock(cg->builder);
    LLVMPositionBuilderAtEnd(cg->builder, entry);

    // self es el primer parámetro
    LLVMValueRef self_val = LLVMGetParam(constructor, 0);

    // Crear scope para el constructor
    Scope *old_scope = cg->current_scope;
    cg->current_scope = scope_create(old_scope);

    // Asignar type_id en el primer campo del struct
    LLVMValueRef type_id_ptr = LLVMBuildStructGEP2(cg->builder, user_type->struct_type, self_val, 0, "type_id_ptr");
    LLVMBuildStore(cg->builder, LLVMConstInt(LLVMInt32TypeInContext(cg->context), user_type->type_id, 0), type_id_ptr);

    // Registrar los init_params en el scope
    for (size_t i = 0; i < list_count(type_def->init_params); i++)
    {
        SymbolBinding *param = (SymbolBinding *)list_get(type_def->init_params, i);
        LLVMValueRef param_val = LLVMGetParam(constructor, i + 1);

        TypeDescriptor *param_type = param->return_type;

        LLVMTypeRef llvm_type = code_gen_get_llvm_type(cg->context, param_type);
        LLVMValueRef alloca = LLVMBuildAlloca(cg->builder, llvm_type, param->name);

        LLVMBuildStore(cg->builder, param_val, alloca);

        scope_add_parameter(cg->current_scope, param->name, param_type);
        scope_set_alloca(cg->current_scope, param->name, alloca);
    }

    // Llamar al constructor del padre si existe
    UserTypeDescriptor *parent_user = type_to_user_defined(user_type->base.parent);

    if (parent_user && parent_user->constructor)
    {
        size_t father_arg_count = list_count(type_def->father_init_args);
        LLVMValueRef *parent_args = (LLVMValueRef *)malloc((father_arg_count + 1) * sizeof(LLVMValueRef));

        parent_args[0] = LLVMBuildBitCast(cg->builder, self_val, LLVMPointerType(parent_user->struct_type, 0), "parent_self");

        for (size_t i = 0; i < father_arg_count; i++)
        {
            ASTNode *arg = (ASTNode *)list_get(type_def->father_init_args, i);
            parent_args[i + 1] = (LLVMValueRef)(intptr_t)ast_accept(arg, (Visitor *)cg);
        }

        LLVMBuildCall2(cg->builder, parent_user->constructor_type, parent_user->constructor, parent_args, father_arg_count + 1, "");
        free(parent_args);
    }

    // Inicializar atributos propios
    for (size_t i = 0; i < list_count(type_def->attributes); i++)
    {
        SymbolBinding *attr = (SymbolBinding *)list_get(type_def->attributes, i);

        if (!attr->initializer)
            continue;

        LLVMValueRef init_val = (LLVMValueRef)(intptr_t)ast_accept(attr->initializer, (Visitor *)cg);

        if (!init_val)
            continue;

        int field_index = user_type->offset + (int)i;
        LLVMValueRef field_ptr = LLVMBuildStructGEP2(cg->builder, user_type->struct_type, self_val, field_index, attr->name);

        LLVMBuildStore(cg->builder, init_val, field_ptr);
    }

    LLVMBuildRet(cg->builder, self_val);

    // Restaurar scope
    scope_destroy(cg->current_scope);
    cg->current_scope = old_scope;

    user_type->constructor_type = func_type;

    if (prev_block)
        LLVMPositionBuilderAtEnd(cg->builder, prev_block);

    return constructor;
}

static void *visit_type_definition_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    TypeDefinitionNode *type_def = (TypeDefinitionNode *)node;

    // Obtener el UserTypeDescriptor desde la tabla semántica
    TypeDescriptor *type_desc = type_table_lookup_by_name(global_type_table, type_def->name);
    UserTypeDescriptor *user_type = type_to_user_defined(type_desc);

    if (!user_type)
        return NULL;

    // Construir struct type y guardarlo
    user_type->struct_type = code_gen_build_struct_type(cg->context, user_type);

    // Crear constructor y guardarlo
    user_type->constructor = codegen_create_constructor(cg, type_def, user_type);

    // Guardar y establecer el tipo actual para los métodos
    UserTypeDescriptor *old_self_type = cg->current_self_type;
    cg->current_self_type = user_type;

    // Procesar métodos
    for (size_t i = 0; i < list_count(type_def->methods); i++)
    {
        FunctionDefinitionNode *method = (FunctionDefinitionNode *)list_get(type_def->methods, i);

        // Construir nombre LLVM para el método: _TypeName_methodName
        char *method_name = function_table_build_method_name(user_type->base.name, method->name);

        // Obtener tipos desde la tabla semántica
        List *hulk_params = function_table_get_params_types(global_function_table, method_name);
        TypeDescriptor *hulk_return = function_table_get_return_type(global_function_table, method_name, NULL);

        // Construir firma LLVM con self como primer parámetro
        LLVMTypeRef llvm_return = code_gen_get_llvm_type(cg->context, hulk_return);

        size_t param_count = hulk_params ? list_count(hulk_params) : 0;
        LLVMTypeRef *llvm_params = (LLVMTypeRef *)malloc((param_count + 1) * sizeof(LLVMTypeRef));

        llvm_params[0] = LLVMPointerType(user_type->struct_type, 0);

        for (size_t j = 0; j < param_count; j++)
        {
            TypeDescriptor *pt = (TypeDescriptor *)list_get(hulk_params, j);
            llvm_params[j + 1] = code_gen_get_llvm_type(cg->context, pt);
        }

        LLVMTypeRef method_func_type = LLVMFunctionType(llvm_return, llvm_params, param_count + 1, 0);
        free(llvm_params);

        // Declarar función LLVM para el método
        LLVMValueRef method_func = LLVMAddFunction(cg->module, method_name, method_func_type);

        // Guardar en la tabla global
        function_table_set_fn(global_function_table, method_name, method_func);
        function_table_set_func_type(global_function_table, method_name, method_func_type);

        free(method_name);

        // Generar cuerpo del método
        ast_accept((ASTNode *)method, visitor);
    }

    cg->current_self_type = old_self_type;

    return NULL;
}

static void *visit_type_instanciation_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    TypeInstanciationNode *inst = (TypeInstanciationNode *)node;

    TypeDescriptor *type_desc = type_table_lookup_by_name(global_type_table, inst->type_name);
    UserTypeDescriptor *user_type = type_to_user_defined(type_desc);

    if (!user_type || !user_type->constructor)
        return NULL;

    // Alocar memoria en el heap
    LLVMValueRef size = LLVMSizeOf(user_type->struct_type);
    LLVMValueRef size_i32 = LLVMBuildTrunc(cg->builder, size, LLVMInt32TypeInContext(cg->context), "size_i32");
    LLVMValueRef malloc_args[] = { size_i32 };
    LLVMValueRef raw_ptr = code_gen_build_call(cg->builder, "_hulk_alloc", malloc_args, 1);
    LLVMValueRef instance = LLVMBuildBitCast(cg->builder, raw_ptr, LLVMPointerType(user_type->struct_type, 0), "instance");

    // Guardar para liberar al final
    list_append(cg->allocated_ptrs, instance);

    // Evaluar argumentos (self + params)
    size_t arg_count = list_count(inst->args);
    LLVMValueRef *args = (LLVMValueRef *)malloc((arg_count + 1) * sizeof(LLVMValueRef));

    args[0] = instance;

    for (size_t i = 0; i < arg_count; i++)
    {
        ASTNode *arg = (ASTNode *)list_get(inst->args, i);
        args[i + 1] = (LLVMValueRef)(intptr_t)ast_accept(arg, visitor);
    }

    // Llamar al constructor
    LLVMValueRef result = LLVMBuildCall2(cg->builder, user_type->constructor_type, user_type->constructor, args, arg_count + 1, "");
    free(args);

    return (void *)result;
}

static void *visit_attribute_access_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    AttributeAccessNode *attr = (AttributeAccessNode *)node;

    // Evaluar el target (self)
    LLVMValueRef target_val = (LLVMValueRef)(intptr_t)ast_accept(attr->target, visitor);

    if (!target_val)
        return NULL;

    // Obtener el tipo del target
    TypeDescriptor *target_type = attr->target->return_type;

    if (!target_type || target_type->tag != HULK_USER_DEFINED)
        return NULL;

    UserTypeDescriptor *user_type = (UserTypeDescriptor *)target_type;

    // Obtener índice del atributo
    int attr_index = code_gen_get_attribute_index(user_type, attr->attribute_name);

    if (attr_index < 0)
        return NULL;

    // GEP al campo y cargar valor
    LLVMValueRef field_ptr = LLVMBuildStructGEP2(cg->builder, user_type->struct_type, target_val, attr_index, attr->attribute_name);

    LLVMTypeRef llvm_type = code_gen_get_llvm_type(cg->context, attr->base.return_type);

    return (void *)LLVMBuildLoad2(cg->builder, llvm_type, field_ptr, attr->attribute_name);
}

static void *visit_method_access_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    MethodAccessNode *method = (MethodAccessNode *)node;

    // Evaluar el target
    LLVMValueRef target_val = (LLVMValueRef)(intptr_t)ast_accept(method->target, visitor);

    if (!target_val)
        return NULL;

    // Obtener el tipo del target
    TypeDescriptor *target_type = method->target->return_type;

    if (!target_type || target_type->tag != HULK_USER_DEFINED)
        return NULL;

    // Buscar el tipo que define el método
    UserTypeDescriptor *user_type = (UserTypeDescriptor *)target_type;
    UserTypeDescriptor *owner = user_type_find_type_with_method(user_type, method->method_name);

    if (!owner)
        return NULL;

    // Construir nombre LLVM del método
    char *llvm_method_name = function_table_build_method_name(owner->base.name, method->method_name);

    // Obtener función y tipo desde la tabla global
    LLVMValueRef func = function_table_get_fn(global_function_table, llvm_method_name);
    LLVMTypeRef func_type = function_table_get_func_type(global_function_table, llvm_method_name);

    free(llvm_method_name);

    if (!func || !func_type)
        return NULL;

    // Evaluar argumentos (self + params)
    size_t arg_count = list_count(method->args);
    LLVMValueRef *args = (LLVMValueRef *)malloc((arg_count + 1) * sizeof(LLVMValueRef));

    args[0] = target_val;

    for (size_t i = 0; i < arg_count; i++)
    {
        ASTNode *arg = (ASTNode *)list_get(method->args, i);
        args[i + 1] = (LLVMValueRef)(intptr_t)ast_accept(arg, visitor);
    }

    // Generar llamada
    LLVMValueRef result = LLVMBuildCall2(cg->builder, func_type, func, args, arg_count + 1, "");
    free(args);

    // Si retorna void, no devolver valor
    if (LLVMGetReturnType(func_type) == LLVMVoidTypeInContext(cg->context))
        return NULL;

    return (void *)result;
}

static void *visit_base_call_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    BaseCallNode *base_call = (BaseCallNode *)node;

    // La inferencia ya validó y guardó type_name y method_name
    if (!base_call->type_name || !base_call->method_name)
        return NULL;

    // Obtener el tipo actual y subir al padre
    UserTypeDescriptor *user_type = type_to_user_defined(type_table_lookup_by_name(global_type_table, base_call->type_name));

    if (!user_type || !user_type->base.parent)
        return NULL;

    // Buscar el ancestro que implementa el método
    UserTypeDescriptor *parent_user = type_to_user_defined(user_type->base.parent);
    UserTypeDescriptor *ancestor = user_type_find_type_with_method(parent_user, base_call->method_name);

    if (!ancestor)
        return NULL;

    // Construir nombre LLVM del método ancestro
    char *llvm_method_name = function_table_build_method_name(ancestor->base.name, base_call->method_name);

    // Obtener función y tipo desde la tabla global
    LLVMValueRef func = function_table_get_fn(global_function_table, llvm_method_name);
    LLVMTypeRef func_type = function_table_get_func_type(global_function_table, llvm_method_name);

    free(llvm_method_name);

    if (!func || !func_type)
        return NULL;

    // Obtener self desde el scope actual
    LLVMValueRef self_val = (LLVMValueRef)(intptr_t)LLVMBuildLoad2(cg->builder, 
        code_gen_get_llvm_type(cg->context, (TypeDescriptor *)user_type),
        scope_get_alloca(cg->current_scope, "self"), "self");

    // Evaluar argumentos
    size_t arg_count = list_count(base_call->args);
    LLVMValueRef *args = (LLVMValueRef *)malloc((arg_count + 1) * sizeof(LLVMValueRef));

    args[0] = LLVMBuildBitCast(cg->builder, self_val, LLVMPointerType(ancestor->struct_type, 0), "parent_self");

    for (size_t i = 0; i < arg_count; i++)
    {
        ASTNode *arg = (ASTNode *)list_get(base_call->args, i);
        args[i + 1] = (LLVMValueRef)(intptr_t)ast_accept(arg, visitor);
    }

    // Generar llamada
    LLVMValueRef result = LLVMBuildCall2(cg->builder, func_type, func, args, arg_count + 1, "");
    free(args);

    if (LLVMGetReturnType(func_type) == LLVMVoidTypeInContext(cg->context))
        return NULL;

    return (void *)result;
}

static void *visit_is_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    IsNode *is_node = (IsNode *)node;

    // Evaluar el target
    LLVMValueRef target_val = (LLVMValueRef)(intptr_t)ast_accept(is_node->target, visitor);

    if (!target_val)
        return NULL;

    // Obtener el tipo esperado
    TypeDescriptor *expected_type = type_table_lookup_by_name(global_type_table, is_node->type_name);

    if (!expected_type)
        return NULL;

    // Object: siempre true
    if (expected_type->tag == HULK_OBJECT)
        return (void *)LLVMConstInt(LLVMInt1TypeInContext(cg->context), 1, 0);

    // Solo user types usan type_id
    if (expected_type->tag != HULK_USER_DEFINED)
        return (void *)LLVMConstInt(LLVMInt1TypeInContext(cg->context), 0, 0);

    UserTypeDescriptor *expected_user_type = (UserTypeDescriptor *)expected_type;

    // Obtener el tipo del target
    TypeDescriptor *target_type = is_node->target->return_type;

    // Si el target es Object, verificamos en runtime con type_id
    if (target_type && target_type->tag == HULK_OBJECT)
    {
        // Cargar el type_id del objeto (campo 0 del struct)
        LLVMValueRef target_i8 = LLVMBuildBitCast(cg->builder, target_val, LLVMPointerType(LLVMInt8TypeInContext(cg->context), 0), "obj_i8");
        LLVMValueRef type_id_ptr = LLVMBuildBitCast(cg->builder, target_i8, LLVMPointerType(LLVMInt32TypeInContext(cg->context), 0), "type_id_ptr");
        LLVMValueRef type_id = LLVMBuildLoad2(cg->builder, LLVMInt32TypeInContext(cg->context), type_id_ptr, "type_id");

        // Comparar con el type_id esperado
        LLVMValueRef expected_id = LLVMConstInt(LLVMInt32TypeInContext(cg->context), expected_user_type->type_id, 0);
        return (void *)LLVMBuildICmp(cg->builder, LLVMIntEQ, type_id, expected_id, "is_result");
    }

    // Si el target es un tipo concreto, verificamos estáticamente
    if (target_type && target_type->tag == HULK_USER_DEFINED)
    {
        bool conforms = type_conforms_to(target_type, expected_type);
        return (void *)LLVMConstInt(LLVMInt1TypeInContext(cg->context), conforms ? 1 : 0, 0);
    }

    // Otros tipos: false
    return (void *)LLVMConstInt(LLVMInt1TypeInContext(cg->context), 0, 0);
}

static void *visit_as_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    AsNode *as_node = (AsNode *)node;

    // Evaluar el target
    LLVMValueRef target_val = (LLVMValueRef)(intptr_t)ast_accept(as_node->target, visitor);

    if (!target_val)
        return NULL;

    // Obtener el tipo al que se convierte
    TypeDescriptor *as_type = type_table_lookup_by_name(global_type_table, as_node->type_name);

    if (!as_type || as_type->tag != HULK_USER_DEFINED)
        return NULL;

    UserTypeDescriptor *user_type = (UserTypeDescriptor *)as_type;

    // BitCast al tipo esperado
    return (void *)LLVMBuildBitCast(cg->builder, target_val, LLVMPointerType(user_type->struct_type, 0), "as_cast");
}
static void *visit_program_node(Visitor *visitor, ASTNode *node)
{
    CodeGenVisitor *cg = (CodeGenVisitor *)visitor;
    ProgramNode *program = (ProgramNode *)node;

    // Registrar builtins
    code_gen_register_builtins(cg->module, cg->context);

    // Procesar definiciones de tipos
    for (size_t i = 0; i < list_count(program->type_definitions); i++)
    {
        TypeDefinitionNode *type_def = (TypeDefinitionNode *)list_get(program->type_definitions, i);
        ast_accept((ASTNode *)type_def, visitor);
    }

    // Registrar y generar funciones definidas por el usuario
    for (size_t i = 0; i < list_count(program->function_definitions); i++)
    {
        FunctionDefinitionNode *func_def = (FunctionDefinitionNode *)list_get(program->function_definitions, i);
        code_gen_register_user_function(cg->module, cg->context, func_def->name);
        ast_accept((ASTNode *)func_def, visitor);
    }

    // Crear función main
    LLVMTypeRef main_type = LLVMFunctionType(LLVMInt32TypeInContext(cg->context), NULL, 0, 0);
    LLVMValueRef main_func = LLVMAddFunction(cg->module, "main", main_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(cg->context, main_func, "entry");
    LLVMPositionBuilderAtEnd(cg->builder, entry);

    // Registrar constantes PI y E
    code_gen_register_constants(cg->current_scope, cg->builder, cg->context);

    // Generar expresión raíz
    if (program->root)
        ast_accept(program->root, visitor);

    // Liberar todos los objetos alocados en el heap
    for (size_t i = 0; i < list_count(cg->allocated_ptrs); i++)
    {
        LLVMValueRef ptr = (LLVMValueRef)list_get(cg->allocated_ptrs, i);
        LLVMValueRef casted = LLVMBuildBitCast(cg->builder, ptr, LLVMPointerType(LLVMInt8TypeInContext(cg->context), 0), "");
        LLVMValueRef args[] = { casted };
        code_gen_build_call(cg->builder, "_hulk_free", args, 1);
    }

    LLVMBuildRet(cg->builder, LLVMConstInt(LLVMInt32TypeInContext(cg->context), 0, 0));

    return NULL;
}

CodeGenVisitor *code_gen_visitor_create(const char *module_name)
{
    CodeGenVisitor *visitor = (CodeGenVisitor *)malloc(sizeof(CodeGenVisitor));
    if (!visitor)
        return NULL;

    visitor_init(&visitor->base);

    VisitorFunc funcs[] = {(VisitorFunc)visit_literal_node, (VisitorFunc)visit_unary_operation_node, (VisitorFunc)visit_binary_operation_node, (VisitorFunc)visit_expression_block_node, (VisitorFunc)visit_conditional_node, (VisitorFunc)visit_while_loop_node, (VisitorFunc)visit_let_in_node, (VisitorFunc)visit_variable_reference_node, (VisitorFunc)visit_reassignment_node, (VisitorFunc)visit_function_definition_node, (VisitorFunc)visit_function_call_node, (VisitorFunc)visit_type_definition_node, (VisitorFunc)visit_attribute_access_node, (VisitorFunc)visit_method_access_node, (VisitorFunc)visit_type_instanciation_node, (VisitorFunc)visit_base_call_node, (VisitorFunc)visit_is_node, (VisitorFunc)visit_as_node, (VisitorFunc)visit_program_node};
    visitor_register_functions(&visitor->base, funcs);

    visitor->context = LLVMContextCreate();
    visitor->module = LLVMModuleCreateWithNameInContext(module_name, visitor->context);
    visitor->builder = LLVMCreateBuilderInContext(visitor->context);
    visitor->current_scope = scope_create(NULL);
    visitor->current_self_type = NULL;
    visitor->allocated_ptrs = list_create(0, NULL, NULL, NULL, NULL);

    if (!visitor->current_scope || !visitor->allocated_ptrs)
    {
        code_gen_visitor_destroy(visitor);
        return NULL;
    }

    return visitor;
}

void code_gen_visitor_destroy(CodeGenVisitor *visitor)
{
    if (!visitor)
        return;

    if (visitor->builder)
        LLVMDisposeBuilder(visitor->builder);

    if (visitor->module)
        LLVMDisposeModule(visitor->module);

    if (visitor->context)
        LLVMContextDispose(visitor->context);

    if (visitor->current_scope)
        scope_destroy(visitor->current_scope);

    if (visitor->allocated_ptrs)
        list_destroy(visitor->allocated_ptrs);

    visitor_destroy(&visitor->base);
    free(visitor);
}

void code_gen_visitor_compile(CodeGenVisitor *visitor, const char *filename)
{
    if (!visitor || !visitor->module)
        return;

    // Imprimir IR generado (temporal, para depuración)
    printf("\n=== LLVM IR ===\n");
    LLVMDumpModule(visitor->module);

    if (!filename)
        return;

    // Escribir IR a archivo .ll
    char ll_filename[256];
    snprintf(ll_filename, sizeof(ll_filename), "%s.ll", filename);

    char *error = NULL;

    if (LLVMPrintModuleToFile(visitor->module, ll_filename, &error) != 0)
    {
        fprintf(stderr, "[codegen] Error writing module: %s\n", error ? error : "unknown");

        if (error)
            LLVMDisposeMessage(error);

        return;
    }
    

    // Compilar .ll a ensamblador
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "llc %s -o /tmp/hulk_output.s 2>/dev/null", ll_filename);

    if (system(cmd) != 0)
        return;

    // Compilar ensamblador a objeto
    snprintf(cmd, sizeof(cmd), "gcc -c /tmp/hulk_output.s -o /tmp/hulk_output.o 2>/dev/null");

    if (system(cmd) != 0)
        return;

    // Compilar runtime.c a objeto
    snprintf(cmd, sizeof(cmd), "gcc -c src/runtime.c -o /tmp/hulk_runtime.o 2>/dev/null");

    if (system(cmd) != 0)
        return;

    // Linkear en el ejecutable output
    snprintf(cmd, sizeof(cmd), "gcc -no-pie /tmp/hulk_output.o /tmp/hulk_runtime.o -o output -lm 2>/dev/null");

    if (system(cmd) != 0)
        return;

    // Limpiar objetos temporales
    remove("/tmp/hulk_output.s");
    remove("/tmp/hulk_output.o");
    remove("/tmp/hulk_runtime.o");
}