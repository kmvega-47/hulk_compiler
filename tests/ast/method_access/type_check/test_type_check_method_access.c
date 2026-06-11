#include <CUnit/Basic.h>
#include "type_check_visitor.h"
#include "type_inference_visitor.h"
#include "free_visitor.h"

static size_t run_inference_and_check(ASTNode *node)
{
    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept(node, (Visitor *)iv);
    type_inference_visitor_destroy(iv);

    if (!dm_has_errors(dm_global))
    {
        TypeCheckVisitor *cv = type_check_visitor_create();
        ast_accept(node, (Visitor *)cv);
        type_check_visitor_destroy(cv);
    }

    return dm_error_count(dm_global);
}

void test_check_method_access_no_args(void)
{
    printf("\n\n------------ Inicio test: test_check_method_access_no_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m() => 42; } new A().m()\n");

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 25);
    FunctionDefinitionNode *method = ast_function_definition_create("m", mp, NULL, body, 1, 15);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de retorno: %s\n", call->return_type ? call->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_method_access_correct_args(void)
{
    printf("\n\n------------ Inicio test: test_check_method_access_correct_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { add(x: Number, y: Number) => x + y; } new A().add(3, 4)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 1, 20));
    list_append(params, symbol_binding_create("y", "Number", NULL, 1, 32));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 40);
    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 1, 42);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_ADD, x_ref, y_ref, 1, 41);
    FunctionDefinitionNode *method = ast_function_definition_create("add", params, NULL, body, 1, 15);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(3.0, 2, 12));
    list_append(args, ast_number_literal_create(4.0, 2, 15));
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "add", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de retorno: %s\n", call->return_type ? call->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_method_access_inherited(void)
{
    printf("\n\n------------ Inicio test: test_check_method_access_inherited ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type Base { m() => 1; } type Derived inherits Base {} new Derived().m()\n");

    List *base_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 1, 30);
    FunctionDefinitionNode *method = ast_function_definition_create("m", mp, NULL, body, 1, 15);
    list_append(base_methods, method);

    TypeDefinitionNode *base_def = ast_type_definition_create("Base",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        base_methods, 1, 1);

    TypeDefinitionNode *derived_def = ast_type_definition_create("Derived",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "Base",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, base_def);
    list_append(types, derived_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("Derived", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de retorno: %s\n", call->return_type ? call->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_method_access_fewer_args(void)
{
    printf("\n\n------------ Inicio test: test_check_method_access_fewer_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m(x: Number) => x; } new A().m() -- error\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 1, 20));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 30);
    FunctionDefinitionNode *method = ast_function_definition_create("m", params, NULL, x_ref, 1, 15);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_method_access_more_args(void)
{
    printf("\n\n------------ Inicio test: test_check_method_access_more_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m(x: Number) => x; } new A().m(1, 2) -- error\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 1, 20));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 30);
    FunctionDefinitionNode *method = ast_function_definition_create("m", params, NULL, x_ref, 1, 15);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(1.0, 2, 12));
    list_append(args, ast_number_literal_create(2.0, 2, 15));
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_method_access_wrong_arg_type(void)
{
    printf("\n\n------------ Inicio test: test_check_method_access_wrong_arg_type ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m(x: Number) => x; } new A().m(\"hello\") -- error\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 1, 20));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 30);
    FunctionDefinitionNode *method = ast_function_definition_create("m", params, NULL, x_ref, 1, 15);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_string_literal_create("hello", 2, 12));
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de MethodAccessNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("MethodAccessNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_method_access_no_args", test_check_method_access_no_args);
    CU_add_test(suite, "test_check_method_access_correct_args", test_check_method_access_correct_args);
    CU_add_test(suite, "test_check_method_access_inherited", test_check_method_access_inherited);
    CU_add_test(suite, "test_check_method_access_fewer_args", test_check_method_access_fewer_args);
    CU_add_test(suite, "test_check_method_access_more_args", test_check_method_access_more_args);
    CU_add_test(suite, "test_check_method_access_wrong_arg_type", test_check_method_access_wrong_arg_type);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}