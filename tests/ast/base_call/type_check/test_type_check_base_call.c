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

void test_check_base_no_args(void)
{
    printf("\n\n------------ Inicio test: test_check_base_no_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m() => 1; } type B inherits A { m() => base(); }\n");

    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *a_body = (ASTNode *)ast_number_literal_create(1.0, 1, 25);
    FunctionDefinitionNode *a_method = ast_function_definition_create("m", a_mp, NULL, a_body, 1, 15);
    list_append(a_methods, a_method);

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *b_body = (ASTNode *)ast_base_call_create(list_create(0, NULL, NULL, NULL, NULL), 1, 35);
    FunctionDefinitionNode *b_method = ast_function_definition_create("m", b_mp, NULL, b_body, 1, 30);
    list_append(b_methods, b_method);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("B", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de base(): %s\n", b_body->return_type ? b_body->return_type->name : "NULL");
    printf("Tipo de B.m(): %s\n", call->return_type ? call->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_base_with_args(void)
{
    printf("\n\n------------ Inicio test: test_check_base_with_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { sum(x: Number, y: Number) => x + y; } type B inherits A { sum(x: Number, y: Number) => base(x, y) * 2; }\n");

    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_params, symbol_binding_create("x", "Number", NULL, 1, 16));
    list_append(a_params, symbol_binding_create("y", "Number", NULL, 1, 28));
    ASTNode *a_x = (ASTNode *)ast_variable_reference_create("x", 1, 35);
    ASTNode *a_y = (ASTNode *)ast_variable_reference_create("y", 1, 37);
    ASTNode *a_body = (ASTNode *)ast_binary_create(OP_ADD, a_x, a_y, 1, 36);
    FunctionDefinitionNode *a_method = ast_function_definition_create("sum", a_params, NULL, a_body, 1, 15);
    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(a_methods, a_method);

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(b_params, symbol_binding_create("x", "Number", NULL, 1, 62));
    list_append(b_params, symbol_binding_create("y", "Number", NULL, 1, 74));

    List *base_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(base_args, ast_variable_reference_create("x", 1, 85));
    list_append(base_args, ast_variable_reference_create("y", 1, 88));
    ASTNode *base_call = (ASTNode *)ast_base_call_create(base_args, 1, 82);
    ASTNode *b_body = (ASTNode *)ast_binary_create(OP_MULT, base_call, (ASTNode *)ast_number_literal_create(2.0, 1, 93), 1, 91);
    FunctionDefinitionNode *b_method = ast_function_definition_create("sum", b_params, NULL, b_body, 1, 58);
    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(b_methods, b_method);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("B", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(3.0, 2, 12));
    list_append(args, ast_number_literal_create(4.0, 2, 15));
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "sum", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de base(x, y): %s\n", base_call->return_type ? base_call->return_type->name : "NULL");
    printf("Tipo de B.sum(): %s\n", call->return_type ? call->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_base_wrong_arg_type(void)
{
    printf("\n\n------------ Inicio test: test_check_base_wrong_arg_type ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m(x: Number) => x; } type B inherits A { m(x: Number) => base(\"hello\"); } -- error\n");

    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_params, symbol_binding_create("x", "Number", NULL, 1, 16));
    ASTNode *a_x = (ASTNode *)ast_variable_reference_create("x", 1, 30);
    FunctionDefinitionNode *a_method = ast_function_definition_create("m", a_params, NULL, a_x, 1, 15);
    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(a_methods, a_method);

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(b_params, symbol_binding_create("x", "Number", NULL, 1, 54));

    List *base_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(base_args, ast_string_literal_create("hello", 1, 70));
    ASTNode *base_call = (ASTNode *)ast_base_call_create(base_args, 1, 67);
    FunctionDefinitionNode *b_method = ast_function_definition_create("m", b_params, NULL, base_call, 1, 50);
    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(b_methods, b_method);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("B", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(42.0, 2, 12));
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo del argumento: String\n");
    printf("Tipo esperado: Number\n");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_base_wrong_arg_count(void)
{
    printf("\n\n------------ Inicio test: test_check_base_wrong_arg_count ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m(x: Number) => x; } type B inherits A { m(x: Number) => base(x, x); } -- error\n");

    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_params, symbol_binding_create("x", "Number", NULL, 1, 16));
    ASTNode *a_x = (ASTNode *)ast_variable_reference_create("x", 1, 30);
    FunctionDefinitionNode *a_method = ast_function_definition_create("m", a_params, NULL, a_x, 1, 15);
    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(a_methods, a_method);

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(b_params, symbol_binding_create("x", "Number", NULL, 1, 54));

    List *base_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(base_args, ast_variable_reference_create("x", 1, 70));
    list_append(base_args, ast_variable_reference_create("x", 1, 73));
    ASTNode *base_call = (ASTNode *)ast_base_call_create(base_args, 1, 67);
    FunctionDefinitionNode *b_method = ast_function_definition_create("m", b_params, NULL, base_call, 1, 50);
    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(b_methods, b_method);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("B", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(42.0, 2, 12));
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Argumentos pasados a base: 2\n");
    printf("Argumentos esperados: 1\n");

    dm_print_errors(dm_global);
    CU_ASSERT(errors >= 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_base_chain(void)
{
    printf("\n\n------------ Inicio test: test_check_base_chain ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { m() => 1; } type B inherits A { m() => base() + 1; } type C inherits B { m() => base() + 1; }\n");

    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *a_body = (ASTNode *)ast_number_literal_create(1.0, 1, 25);
    FunctionDefinitionNode *a_method = ast_function_definition_create("m", a_mp, NULL, a_body, 1, 15);
    list_append(a_methods, a_method);

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *b_base = (ASTNode *)ast_base_call_create(list_create(0, NULL, NULL, NULL, NULL), 1, 55);
    ASTNode *b_body = (ASTNode *)ast_binary_create(OP_ADD, b_base, (ASTNode *)ast_number_literal_create(1.0, 1, 64), 1, 62);
    FunctionDefinitionNode *b_method = ast_function_definition_create("m", b_mp, NULL, b_body, 1, 50);
    list_append(b_methods, b_method);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 1, 1);

    List *c_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *c_mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *c_base = (ASTNode *)ast_base_call_create(list_create(0, NULL, NULL, NULL, NULL), 1, 85);
    ASTNode *c_body = (ASTNode *)ast_binary_create(OP_ADD, c_base, (ASTNode *)ast_number_literal_create(1.0, 1, 94), 1, 92);
    FunctionDefinitionNode *c_method = ast_function_definition_create("m", c_mp, NULL, c_body, 1, 80);
    list_append(c_methods, c_method);

    TypeDefinitionNode *c_def = ast_type_definition_create("C",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "B",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        c_methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);
    list_append(types, c_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("C", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "m", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de base() en B: %s\n", b_base->return_type ? b_base->return_type->name : "NULL");
    printf("Tipo de base() en C: %s\n", c_base->return_type ? c_base->return_type->name : "NULL");
    printf("Tipo de C.m(): %s\n", call->return_type ? call->return_type->name : "NULL");

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
    printf("\n=== Iniciando tests de BaseCallNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BaseCallNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_base_no_args", test_check_base_no_args);
    CU_add_test(suite, "test_check_base_with_args", test_check_base_with_args);
    CU_add_test(suite, "test_check_base_wrong_arg_type", test_check_base_wrong_arg_type);
    CU_add_test(suite, "test_check_base_wrong_arg_count", test_check_base_wrong_arg_count);
    CU_add_test(suite, "test_check_base_chain", test_check_base_chain);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}