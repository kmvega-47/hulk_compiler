#include <CUnit/Basic.h>
#include "type_check_visitor.h"
#include "type_inference_visitor.h"
#include "free_visitor.h"
#include "function_table.h"

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

void test_check_function_call_no_args(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_no_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: rand()\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_function_call_create("rand", args, 1, 1);
    ProgramNode *program = ast_program_create(root,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de retorno: %s\n", root->return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_with_args(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_with_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: sqrt(16)\n");

    ASTNode *arg = (ASTNode *)ast_number_literal_create(16.0, 1, 6);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg);
    ASTNode *root = (ASTNode *)ast_function_call_create("sqrt", args, 1, 1);
    ProgramNode *program = ast_program_create(root,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo del argumento: %s\n", arg->return_type->name);
    printf("Tipo de retorno: %s\n", root->return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_wrong_argument_count(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_wrong_argument_count ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: sqrt() sin argumentos\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_function_call_create("sqrt", args, 1, 1);
    ProgramNode *program = ast_program_create(root,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de retorno: %s\n", root->return_type ? root->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_wrong_signature(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_wrong_signature ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: sqrt(\"hello\")\n");

    ASTNode *arg = (ASTNode *)ast_string_literal_create("hello", 1, 6);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg);
    ASTNode *root = (ASTNode *)ast_function_call_create("sqrt", args, 1, 1);
    ProgramNode *program = ast_program_create(root,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo del argumento: %s\n", arg->return_type->name);
    printf("Tipo de retorno: %s\n", root->return_type ? root->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_undefined(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_undefined ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: nonexistent()\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_function_call_create("nonexistent", args, 1, 1);
    ProgramNode *program = ast_program_create(root,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de retorno: %s\n", root->return_type ? root->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT(errors >= 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_error_in_args(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_error_in_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: print(true + 1)\n");

    LiteralNode *bad_l = ast_bool_literal_create(true, 1, 7);
    LiteralNode *bad_r = ast_number_literal_create(1.0, 1, 14);
    ASTNode *bad_arg = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)bad_l, (ASTNode *)bad_r, 1, 12);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, bad_arg);
    ASTNode *root = (ASTNode *)ast_function_call_create("print", args, 1, 1);
    ProgramNode *program = ast_program_create(root,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo del argumento: %s\n", bad_arg->return_type->name);
    printf("Tipo de retorno: %s\n", root->return_type ? root->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_multiple_errors_in_args(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_multiple_errors_in_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: log(true + 1, \"a\" - 2)\n");

    LiteralNode *e1_l = ast_bool_literal_create(true, 1, 5);
    LiteralNode *e1_r = ast_number_literal_create(1.0, 1, 12);
    ASTNode *err1 = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)e1_l, (ASTNode *)e1_r, 1, 10);

    LiteralNode *e2_l = ast_string_literal_create("a", 1, 16);
    LiteralNode *e2_r = ast_number_literal_create(2.0, 1, 21);
    ASTNode *err2 = (ASTNode *)ast_binary_create(OP_SUB, (ASTNode *)e2_l, (ASTNode *)e2_r, 1, 18);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, err1);
    list_append(args, err2);
    ASTNode *root = (ASTNode *)ast_function_call_create("log", args, 1, 1);
    ProgramNode *program = ast_program_create(root,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo err1: %s\n", err1->return_type->name);
    printf("Tipo err2: %s\n", err2->return_type->name);
    printf("Tipo de retorno: %s\n", root->return_type ? root->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 2);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_incorrect_use(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_incorrect_use ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: function get_str() => \"hello\"; get_str() + 1\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_string_literal_create("hello", 1, 30);
    FunctionDefinitionNode *get_str = ast_function_definition_create("get_str", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, get_str);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_function_call_create("get_str", args, 2, 1);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 2, 13);
    ASTNode *root = (ASTNode *)ast_binary_create(OP_ADD, call, one, 2, 11);

    ProgramNode *program = ast_program_create(root, funcs,
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de get_str(): %s\n", call->return_type->name);
    printf("Tipo de get_str() + 1: %s\n", root->return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_call_inferred_params(void)
{
    printf("\n\n------------ Inicio test: test_check_function_call_inferred_params ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: function f(x) => x + 1; f(2)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 13));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 20);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 24);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_ADD, x_ref, one, 1, 22);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(2.0, 2, 3));
    ASTNode *root = (ASTNode *)ast_function_call_create("f", args, 2, 1);

    ProgramNode *program = ast_program_create(root, funcs,
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    TypeDescriptor *ret = function_table_get_return_type(global_function_table, "f", NULL);
    printf("Tipo de retorno de f: %s\n", ret->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de FunctionCallNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionCallNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_function_call_no_args", test_check_function_call_no_args);
    CU_add_test(suite, "test_check_function_call_with_args", test_check_function_call_with_args);
    CU_add_test(suite, "test_check_function_call_wrong_argument_count", test_check_function_call_wrong_argument_count);
    CU_add_test(suite, "test_check_function_call_wrong_signature", test_check_function_call_wrong_signature);
    CU_add_test(suite, "test_check_function_call_undefined", test_check_function_call_undefined);
    CU_add_test(suite, "test_check_function_call_error_in_args", test_check_function_call_error_in_args);
    CU_add_test(suite, "test_check_function_call_multiple_errors_in_args", test_check_function_call_multiple_errors_in_args);
    CU_add_test(suite, "test_check_function_call_incorrect_use", test_check_function_call_incorrect_use);
    CU_add_test(suite, "test_check_function_call_inferred_params", test_check_function_call_inferred_params);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}