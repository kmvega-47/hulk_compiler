#include "codegen_utils.h"
#include "global_tables.h"

LLVMTypeRef code_gen_get_llvm_type(LLVMContextRef context, TypeDescriptor *type)
{
    if (!type)
        return LLVMVoidTypeInContext(context);

    switch (type->tag)
    {
        case HULK_NUMBER:
            return LLVMDoubleTypeInContext(context);

        case HULK_BOOL:
            return LLVMInt1TypeInContext(context);

        case HULK_STRING:
            return LLVMPointerType(LLVMInt8TypeInContext(context), 0);

        case HULK_VOID:
            return LLVMVoidTypeInContext(context);

        case HULK_OBJECT:
        case HULK_USER_DEFINED:
            return LLVMPointerType(LLVMInt8TypeInContext(context), 0);

        default:
            return LLVMVoidTypeInContext(context);
    }
}

void code_gen_register_builtins(LLVMModuleRef module, LLVMContextRef context)
{
    LLVMTypeRef double_type = LLVMDoubleTypeInContext(context);
    LLVMTypeRef void_type   = LLVMVoidTypeInContext(context);
    LLVMTypeRef i8_ptr_type = LLVMPointerType(LLVMInt8TypeInContext(context), 0);
    LLVMTypeRef i32_type    = LLVMInt32TypeInContext(context);
    LLVMTypeRef bool_type   = LLVMInt1TypeInContext(context);

    // sqrt
    LLVMTypeRef sqrt_params[] = {double_type};
    LLVMTypeRef sqrt_type = LLVMFunctionType(double_type, sqrt_params, 1, 0);
    LLVMValueRef sqrt_fn = LLVMAddFunction(module, "_hulk_sqrt", sqrt_type);
    function_table_set_fn(global_function_table, "sqrt", sqrt_fn);
    function_table_set_func_type(global_function_table, "sqrt", sqrt_type);

    // sin
    LLVMTypeRef sin_params[] = {double_type};
    LLVMTypeRef sin_type = LLVMFunctionType(double_type, sin_params, 1, 0);
    LLVMValueRef sin_fn = LLVMAddFunction(module, "_hulk_sin", sin_type);
    function_table_set_fn(global_function_table, "sin", sin_fn);
    function_table_set_func_type(global_function_table, "sin", sin_type);

    // cos
    LLVMTypeRef cos_params[] = {double_type};
    LLVMTypeRef cos_type = LLVMFunctionType(double_type, cos_params, 1, 0);
    LLVMValueRef cos_fn = LLVMAddFunction(module, "_hulk_cos", cos_type);
    function_table_set_fn(global_function_table, "cos", cos_fn);
    function_table_set_func_type(global_function_table, "cos", cos_type);

    // exp
    LLVMTypeRef exp_params[] = {double_type};
    LLVMTypeRef exp_type = LLVMFunctionType(double_type, exp_params, 1, 0);
    LLVMValueRef exp_fn = LLVMAddFunction(module, "_hulk_exp", exp_type);
    function_table_set_fn(global_function_table, "exp", exp_fn);
    function_table_set_func_type(global_function_table, "exp", exp_type);

    // log
    LLVMTypeRef log_params[] = {double_type, double_type};
    LLVMTypeRef log_type = LLVMFunctionType(double_type, log_params, 2, 0);
    LLVMValueRef log_fn = LLVMAddFunction(module, "_hulk_log", log_type);
    function_table_set_fn(global_function_table, "log", log_fn);
    function_table_set_func_type(global_function_table, "log", log_type);

    // rand
    LLVMTypeRef rand_type = LLVMFunctionType(double_type, NULL, 0, 0);
    LLVMValueRef rand_fn = LLVMAddFunction(module, "_hulk_rand", rand_type);
    function_table_set_fn(global_function_table, "rand", rand_fn);
    function_table_set_func_type(global_function_table, "rand", rand_type);

    // pow
    LLVMTypeRef pow_params[] = {double_type, double_type};
    LLVMTypeRef pow_type = LLVMFunctionType(double_type, pow_params, 2, 0);
    LLVMValueRef pow_fn = LLVMAddFunction(module, "_hulk_pow", pow_type);
    function_table_set_fn(global_function_table, "_pow", pow_fn);
    function_table_set_func_type(global_function_table, "_pow", pow_type);

    // print_number
    LLVMTypeRef print_num_params[] = {double_type};
    LLVMTypeRef print_num_type = LLVMFunctionType(void_type, print_num_params, 1, 0);
    LLVMValueRef print_num_fn = LLVMAddFunction(module, "_hulk_print_number", print_num_type);
    function_table_set_fn(global_function_table, "_print_number", print_num_fn);
    function_table_set_func_type(global_function_table, "_print_number", print_num_type);

    // print_string
    LLVMTypeRef print_str_params[] = {i8_ptr_type};
    LLVMTypeRef print_str_type = LLVMFunctionType(void_type, print_str_params, 1, 0);
    LLVMValueRef print_str_fn = LLVMAddFunction(module, "_hulk_print_string", print_str_type);
    function_table_set_fn(global_function_table, "_print_string", print_str_fn);
    function_table_set_func_type(global_function_table, "_print_string", print_str_type);

    // print_bool
    LLVMTypeRef print_bool_params[] = {bool_type};
    LLVMTypeRef print_bool_type = LLVMFunctionType(void_type, print_bool_params, 1, 0);
    LLVMValueRef print_bool_fn = LLVMAddFunction(module, "_hulk_print_bool", print_bool_type);
    function_table_set_fn(global_function_table, "_print_bool", print_bool_fn);
    function_table_set_func_type(global_function_table, "_print_bool", print_bool_type);

    // concat
    LLVMTypeRef concat_params[] = {i8_ptr_type, i8_ptr_type, i32_type};
    LLVMTypeRef concat_type = LLVMFunctionType(i8_ptr_type, concat_params, 3, 0);
    LLVMValueRef concat_fn = LLVMAddFunction(module, "_hulk_concat", concat_type);
    function_table_set_fn(global_function_table, "_concat", concat_fn);
    function_table_set_func_type(global_function_table, "_concat", concat_type);

    // alloc
    LLVMTypeRef alloc_params[] = { i32_type };
    LLVMTypeRef alloc_type = LLVMFunctionType(i8_ptr_type, alloc_params, 1, 0);
    LLVMValueRef alloc_fn = LLVMAddFunction(module, "_hulk_alloc", alloc_type);
    function_table_set_fn(global_function_table, "_hulk_alloc", alloc_fn);
    function_table_set_func_type(global_function_table, "_hulk_alloc", alloc_type);

    // free
    LLVMTypeRef free_params[] = { i8_ptr_type };
    LLVMTypeRef free_type = LLVMFunctionType(void_type, free_params, 1, 0);
    LLVMValueRef free_fn = LLVMAddFunction(module, "_hulk_free", free_type);
    function_table_set_fn(global_function_table, "_hulk_free", free_fn);
    function_table_set_func_type(global_function_table, "_hulk_free", free_type);

    // number_to_string
    LLVMTypeRef num_to_str_params[] = { double_type };  
    LLVMTypeRef num_to_str_type = LLVMFunctionType(i8_ptr_type, num_to_str_params, 1, 0);
    LLVMValueRef num_to_str_fn = LLVMAddFunction(module, "_hulk_number_to_string", num_to_str_type);
    function_table_set_fn(global_function_table, "_number_to_string", num_to_str_fn);
    function_table_set_func_type(global_function_table, "_number_to_string", num_to_str_type);
}

const char *code_gen_resolve_function_name(const char *name, TypeDescriptor *arg_type)
{
    // Solo "print" necesita despacho por tipo
    if (strcmp(name, "print") != 0 || !arg_type)
        return name;

    // Despachar según el tipo del argumento
    if (arg_type->tag == HULK_NUMBER)
        return "_print_number";

    if (arg_type->tag == HULK_STRING)
        return "_print_string";

    if (arg_type->tag == HULK_BOOL)
        return "_print_bool";

    return name;
}

LLVMValueRef code_gen_build_call(LLVMBuilderRef builder, const char *name, LLVMValueRef *args, size_t arg_count)
{
    LLVMValueRef fn = function_table_get_fn(global_function_table, name);

    if (!fn)
        return NULL;

    LLVMTypeRef func_type = function_table_get_func_type(global_function_table, name);

    if (!func_type)
        return NULL;

    LLVMValueRef result = LLVMBuildCall2(builder, func_type, fn, args, arg_count, "");

    if (LLVMGetReturnType(func_type) == LLVMVoidType())
        return NULL;

    return result;
}

LLVMValueRef code_gen_build_comparison(LLVMBuilderRef builder, HulkOperator op, LLVMValueRef left, LLVMValueRef right)
{
    switch (op)
    {
        case OP_EQUAL:
            return LLVMBuildFCmp(builder, LLVMRealOEQ, left, right, "eq");

        case OP_NOT_EQUAL:
            return LLVMBuildFCmp(builder, LLVMRealONE, left, right, "ne");

        case OP_LESS:
            return LLVMBuildFCmp(builder, LLVMRealOLT, left, right, "lt");

        case OP_LESS_EQUAL:
            return LLVMBuildFCmp(builder, LLVMRealOLE, left, right, "le");

        case OP_GREATER:
            return LLVMBuildFCmp(builder, LLVMRealOGT, left, right, "gt");

        case OP_GREATER_EQUAL:
            return LLVMBuildFCmp(builder, LLVMRealOGE, left, right, "ge");

        default:
            return NULL;
    }
}

LLVMValueRef code_gen_build_binary_op(LLVMBuilderRef builder, HulkOperator op, LLVMValueRef left, LLVMValueRef right)
{
    switch (op)
    {
        // Aritméticos: operaciones directas de LLVM sobre double
        case OP_ADD:
            return LLVMBuildFAdd(builder, left, right, "add");

        case OP_SUB:
            return LLVMBuildFSub(builder, left, right, "sub");

        case OP_MULT:
            return LLVMBuildFMul(builder, left, right, "mul");

        case OP_DIV:
            return LLVMBuildFDiv(builder, left, right, "div");

        case OP_MOD:
            return LLVMBuildFRem(builder, left, right, "mod");

        // Potencia: llamada a función runtime
        case OP_EXP:
        {
            LLVMValueRef args[] = {left, right};
            return code_gen_build_call(builder, "_pow", args, 2);
        }

        // Comparaciones
        case OP_EQUAL:
        case OP_NOT_EQUAL:
        case OP_LESS:
        case OP_LESS_EQUAL:
        case OP_GREATER:
        case OP_GREATER_EQUAL:
            return code_gen_build_comparison(builder, op, left, right);

        // Lógicos: operaciones directas de LLVM sobre i1
        case OP_AND:
            return LLVMBuildAnd(builder, left, right, "and");

        case OP_OR:
            return LLVMBuildOr(builder, left, right, "or");

        // Concatenación: llamada a función runtime con flag de espacio
        case OP_CONCAT:
        case OP_CONCAT_WS:
        {
            // Si right es Number, convertir a string
            LLVMValueRef right_str = right;
            if (LLVMGetTypeKind(LLVMTypeOf(right)) == LLVMDoubleTypeKind)
            {
                LLVMValueRef conv_args[] = { right };
                right_str = code_gen_build_call(builder, "_number_to_string", conv_args, 1);
            }

            LLVMValueRef space_val = LLVMConstInt(LLVMInt32TypeInContext(LLVMGetGlobalContext()), op == OP_CONCAT_WS ? 1 : 0, 0);
            LLVMValueRef concat_args[] = {left, right_str, space_val};
            return code_gen_build_call(builder, "_concat", concat_args, 3);
        }

        default:
            return NULL;
    }
}

LLVMValueRef code_gen_get_default_value(LLVMContextRef context, TypeDescriptor *type)
{
    if (!type)
        return NULL;

    switch (type->tag)
    {
        case HULK_NUMBER:
            return LLVMConstReal(LLVMDoubleTypeInContext(context), 0.0);

        case HULK_BOOL:
            return LLVMConstInt(LLVMInt1TypeInContext(context), 0, 0);

        case HULK_STRING:
        case HULK_OBJECT:
        case HULK_USER_DEFINED:
            return LLVMConstPointerNull(LLVMPointerType(LLVMInt8TypeInContext(context), 0));

        case HULK_VOID:
        default:
            return NULL;
    }
}

LLVMValueRef code_gen_build_conditional_phi(LLVMBuilderRef builder, LLVMContextRef context, TypeDescriptor *hulk_type, LLVMValueRef then_val, LLVMBasicBlockRef end_then, LLVMValueRef else_val, LLVMBasicBlockRef end_else, LLVMBasicBlockRef start_block, bool has_else)
{
    // Si el tipo es Void, no se necesita PHI
    if (!hulk_type || hulk_type->tag == HULK_VOID)
        return NULL;

    LLVMTypeRef llvm_type = code_gen_get_llvm_type(context, hulk_type);
    LLVMValueRef phi = LLVMBuildPhi(builder, llvm_type, "if.result");

    LLVMValueRef values[2];
    LLVMBasicBlockRef blocks[2];

    // Rama then
    values[0] = then_val ? then_val : code_gen_get_default_value(context, hulk_type);
    blocks[0] = end_then;

    // Rama else (o valor por defecto si no hay else)
    if (has_else)
    {
        values[1] = else_val ? else_val : code_gen_get_default_value(context, hulk_type);
        blocks[1] = end_else;
    }

    else
    {
        values[1] = code_gen_get_default_value(context, hulk_type);
        blocks[1] = start_block;
    }

    LLVMAddIncoming(phi, values, blocks, 2);

    return phi;
}

LLVMValueRef code_gen_build_while_phi(LLVMBuilderRef builder, LLVMContextRef context, TypeDescriptor *hulk_type, LLVMBasicBlockRef start_block)
{
    // Void no necesita PHI
    if (!hulk_type || hulk_type->tag == HULK_VOID)
        return NULL;

    LLVMTypeRef llvm_type = code_gen_get_llvm_type(context, hulk_type);
    LLVMValueRef phi = LLVMBuildPhi(builder, llvm_type, "while.val");

    // Valor inicial por defecto desde el bloque de entrada
    LLVMValueRef default_val = code_gen_get_default_value(context, hulk_type);

    if (default_val)
    {
        LLVMValueRef values[] = {default_val};
        LLVMBasicBlockRef blocks[] = {start_block};
        LLVMAddIncoming(phi, values, blocks, 1);
    }

    return phi;
}

void code_gen_add_while_phi_incoming(LLVMValueRef phi, LLVMValueRef body_val, LLVMBasicBlockRef body_end)
{
    if (!phi || !body_end)
        return;

    LLVMValueRef val = body_val ? body_val : LLVMGetUndef(LLVMTypeOf(LLVMGetIncomingValue(phi, 0)));
    LLVMValueRef values[] = {val};
    LLVMBasicBlockRef blocks[] = {body_end};
    LLVMAddIncoming(phi, values, blocks, 1);
}

void code_gen_register_constants(Scope *scope, LLVMBuilderRef builder, LLVMContextRef context)
{
    LLVMTypeRef double_type = LLVMDoubleTypeInContext(context);

    // PI
    LLVMValueRef pi_alloca = LLVMBuildAlloca(builder, double_type, "PI");
    LLVMBuildStore(builder, LLVMConstReal(double_type, 3.141592653589793), pi_alloca);
    scope_add_constant(scope, "PI", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    scope_set_alloca(scope, "PI", pi_alloca);

    // E
    LLVMValueRef e_alloca = LLVMBuildAlloca(builder, double_type, "E");
    LLVMBuildStore(builder, LLVMConstReal(double_type, 2.718281828459045), e_alloca);
    scope_add_constant(scope, "E", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    scope_set_alloca(scope, "E", e_alloca);
}

void code_gen_register_user_function(LLVMModuleRef module, LLVMContextRef context, const char *name)
{
    TypeDescriptor *hulk_return = function_table_get_return_type(global_function_table, name, NULL);
    List *hulk_params = function_table_get_params_types(global_function_table, name);

    if (!hulk_return || !hulk_params)
        return;

    LLVMTypeRef llvm_return = code_gen_get_llvm_type(context, hulk_return);

    size_t param_count = list_count(hulk_params);
    LLVMTypeRef *llvm_params = (LLVMTypeRef *)malloc(param_count * sizeof(LLVMTypeRef));

    for (size_t i = 0; i < param_count; i++)
    {
        TypeDescriptor *pt = (TypeDescriptor *)list_get(hulk_params, i);
        llvm_params[i] = code_gen_get_llvm_type(context, pt);
    }

    LLVMTypeRef func_type = LLVMFunctionType(llvm_return, llvm_params, param_count, 0);
    free(llvm_params);

    LLVMValueRef fn = LLVMAddFunction(module, name, func_type);
    function_table_set_fn(global_function_table, name, fn);
    function_table_set_func_type(global_function_table, name, func_type);
}

LLVMTypeRef code_gen_build_struct_type(LLVMContextRef context, UserTypeDescriptor *user_type)
{
    if (!user_type)
        return NULL;

    // Obtener campos del padre recursivamente
    size_t parent_count = 0;
    LLVMTypeRef *parent_fields = NULL;

    UserTypeDescriptor *parent_user = type_to_user_defined(user_type->base.parent);

    if (parent_user && parent_user->struct_type)
    {
        parent_count = LLVMCountStructElementTypes(parent_user->struct_type);
        parent_fields = (LLVMTypeRef *)malloc(parent_count * sizeof(LLVMTypeRef));
        LLVMGetStructElementTypes(parent_user->struct_type, parent_fields);
    }

    // Obtener tipos de atributos propios
    size_t own_count = list_count(user_type->attribute_types);

    // Campos del padre sin su type_id
    size_t parent_fields_count = parent_count > 0 ? parent_count - 1 : 0;

    // total = type_id + campos_padre_sin_type_id + atributos_propios
    size_t total = 1 + parent_fields_count + own_count;

    LLVMTypeRef *field_types = (LLVMTypeRef *)malloc(total * sizeof(LLVMTypeRef));

    // Primer campo: type_id del hijo
    field_types[0] = LLVMInt32TypeInContext(context);

    // Copiar campos del padre (saltando el type_id del padre, índice 0)
    for (size_t i = 0; i < parent_fields_count; i++)
        field_types[1 + i] = parent_fields[1 + i];

    // Agregar campos propios
    for (size_t i = 0; i < own_count; i++)
    {
        TypeDescriptor *attr_type = (TypeDescriptor *)list_get(user_type->attribute_types, i);
        field_types[1 + parent_fields_count + i] = code_gen_get_llvm_type(context, attr_type);
    }

    // Crear struct con nombre
    LLVMTypeRef struct_type = LLVMStructCreateNamed(context, user_type->base.name);
    LLVMStructSetBody(struct_type, field_types, total, 0);

    free(field_types);

    if (parent_fields)
        free(parent_fields);

    return struct_type;
}

int code_gen_get_attribute_index(UserTypeDescriptor *user_type, const char *attr_name)
{
    if (!user_type || !attr_name)
        return -1;

    // Buscar en los atributos propios
    for (size_t i = 0; i < list_count(user_type->attribute_names); i++)
    {
        char *name = (char *)list_get(user_type->attribute_names, i);

        if (strcmp(name, attr_name) == 0)
            return user_type->offset + (int)i;
    }

    return -1;
}