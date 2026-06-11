#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_binary_single_arithmetic(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_single_arithmetic ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => x + 1  -->  x Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 18);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 22);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_ADD, x_ref, one, 1, 20);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_multiple_arithmetic(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_multiple_arithmetic ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { x + 1; x * 2; }  -->  x Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 26);
    ASTNode *add = (ASTNode *)ast_binary_create(OP_ADD, x1, one, 1, 24);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 31);
    ASTNode *two = (ASTNode *)ast_number_literal_create(2.0, 1, 35);
    ASTNode *mult = (ASTNode *)ast_binary_create(OP_MULT, x2, two, 1, 33);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, add);
    list_append(body_exprs, mult);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_single_comparison_number(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_single_comparison_number ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => x >= 5  -->  x Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 18);
    ASTNode *five = (ASTNode *)ast_number_literal_create(5.0, 1, 23);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_GREATER_EQUAL, x_ref, five, 1, 20);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_single_comparison_string(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_single_comparison_string ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => x != \"hello\"  -->  x String\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 18);
    ASTNode *hello = (ASTNode *)ast_string_literal_create("hello", 1, 23);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_NOT_EQUAL, x_ref, hello, 1, 20);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: String)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_single_logic(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_single_logic ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => x & true  -->  x Bool\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 18);
    ASTNode *t = (ASTNode *)ast_bool_literal_create(true, 1, 22);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_AND, x_ref, t, 1, 20);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Bool)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_multiple_logic(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_multiple_logic ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { x & true; x | false; }  -->  x Bool\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *t = (ASTNode *)ast_bool_literal_create(true, 1, 26);
    ASTNode *and_op = (ASTNode *)ast_binary_create(OP_AND, x1, t, 1, 24);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 31);
    ASTNode *f_bool = (ASTNode *)ast_bool_literal_create(false, 1, 35);
    ASTNode *or_op = (ASTNode *)ast_binary_create(OP_OR, x2, f_bool, 1, 33);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, and_op);
    list_append(body_exprs, or_op);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Bool)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_concat_left_only(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_concat_left_only ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => x @ \".\"  -->  x String\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 18);
    ASTNode *dot = (ASTNode *)ast_string_literal_create(".", 1, 22);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_CONCAT, x_ref, dot, 1, 20);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: String)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_concat_right_plus_string_use(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_concat_right_plus_string_use ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { \"Hola\" @@ x;  x != \"mundo\"; }  -->  x String\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *hello = (ASTNode *)ast_string_literal_create("Hola", 1, 23);
    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 32);
    ASTNode *concat = (ASTNode *)ast_binary_create(OP_CONCAT_WS, hello, x1, 1, 30);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 37);
    ASTNode *mundo = (ASTNode *)ast_string_literal_create("mundo", 1, 42);
    ASTNode *neq = (ASTNode *)ast_binary_create(OP_NOT_EQUAL, x2, mundo, 1, 39);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, concat);
    list_append(body_exprs, neq);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: String)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_concat_right_plus_number_use(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_concat_right_plus_number_use ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { \"Expedition\" @ x;  x - 1; }  -->  x Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *expedition = (ASTNode *)ast_string_literal_create("Expedition", 1, 23);
    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 37);
    ASTNode *concat = (ASTNode *)ast_binary_create(OP_CONCAT, expedition, x1, 1, 35);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 42);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 46);
    ASTNode *sub = (ASTNode *)ast_binary_create(OP_SUB, x2, one, 1, 44);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, concat);
    list_append(body_exprs, sub);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_concat_ambiguous_only(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_concat_ambiguous_only ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => \"\" @@ x  -->  x multiple (String o Number)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *empty = (ASTNode *)ast_string_literal_create("", 1, 18);
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 23);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_CONCAT_WS, empty, x_ref, 1, 21);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: NULL - multiple)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_NULL(x_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_multiple_expressions_correct(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_multiple_expressions_correct ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { x + 1; x * 2; }  -->  x Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 26);
    ASTNode *add = (ASTNode *)ast_binary_create(OP_ADD, x1, one, 1, 24);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 31);
    ASTNode *two = (ASTNode *)ast_number_literal_create(2.0, 1, 35);
    ASTNode *mult = (ASTNode *)ast_binary_create(OP_MULT, x2, two, 1, 33);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, add);
    list_append(body_exprs, mult);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_multiple_expressions_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_multiple_expressions_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { x + 1; x @ \"s\"; }  -->  conflicto\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 26);
    ASTNode *add = (ASTNode *)ast_binary_create(OP_ADD, x1, one, 1, 24);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 31);
    ASTNode *s = (ASTNode *)ast_string_literal_create("s", 1, 35);
    ASTNode *concat = (ASTNode *)ast_binary_create(OP_CONCAT, x2, s, 1, 33);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, add);
    list_append(body_exprs, concat);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: NULL - conflicto)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_NULL(x_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_nested(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => (x + 1) * (x + 2)  -->  x Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 19);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 23);
    ASTNode *add1 = (ASTNode *)ast_binary_create(OP_ADD, x1, one, 1, 21);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 28);
    ASTNode *two = (ASTNode *)ast_number_literal_create(2.0, 1, 32);
    ASTNode *add2 = (ASTNode *)ast_binary_create(OP_ADD, x2, two, 1, 30);

    ASTNode *body = (ASTNode *)ast_binary_create(OP_MULT, add1, add2, 1, 25);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_multiple_params_correct(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_multiple_params_correct ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x, y) => { x + 1; y @ \"s\"; }  -->  x Number, y String\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 8));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 23);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 27);
    ASTNode *add = (ASTNode *)ast_binary_create(OP_ADD, x1, one, 1, 25);

    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 1, 32);
    ASTNode *s = (ASTNode *)ast_string_literal_create("s", 1, 36);
    ASTNode *concat = (ASTNode *)ast_binary_create(OP_CONCAT, y_ref, s, 1, 34);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, add);
    list_append(body_exprs, concat);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 19);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 2);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 1);

    printf("Parametro 'x' resuelto a: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    printf("Parametro 'y' resuelto a: %s (esperado: String)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_binary_multiple_params_one_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_binary_multiple_params_one_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x, y) => { x + 1; x @ \"s\"; y & true; }  -->  x conflicto, y Bool\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 8));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 24);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 28);
    ASTNode *add = (ASTNode *)ast_binary_create(OP_ADD, x1, one, 1, 26);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 33);
    ASTNode *s = (ASTNode *)ast_string_literal_create("s", 1, 37);
    ASTNode *concat = (ASTNode *)ast_binary_create(OP_CONCAT, x2, s, 1, 35);

    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 1, 42);
    ASTNode *t = (ASTNode *)ast_bool_literal_create(true, 1, 46);
    ASTNode *and_op = (ASTNode *)ast_binary_create(OP_AND, y_ref, t, 1, 44);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, add);
    list_append(body_exprs, concat);
    list_append(body_exprs, and_op);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 20);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 2);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 1);

    printf("Parametro 'x' resuelto a: %s (esperado: NULL - conflicto)\n", x_type ? x_type->name : "NULL");
    printf("Parametro 'y' resuelto a: %s (esperado: Bool)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_NULL(x_type);
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de ConstraintCollector (binary) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Binary Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_binary_single_arithmetic", test_collector_binary_single_arithmetic);
    CU_add_test(suite, "test_collector_binary_multiple_arithmetic", test_collector_binary_multiple_arithmetic);
    CU_add_test(suite, "test_collector_binary_single_comparison_number", test_collector_binary_single_comparison_number);
    CU_add_test(suite, "test_collector_binary_single_comparison_string", test_collector_binary_single_comparison_string);
    CU_add_test(suite, "test_collector_binary_single_logic", test_collector_binary_single_logic);
    CU_add_test(suite, "test_collector_binary_multiple_logic", test_collector_binary_multiple_logic);
    CU_add_test(suite, "test_collector_binary_concat_left_only", test_collector_binary_concat_left_only);
    CU_add_test(suite, "test_collector_binary_concat_right_plus_string_use", test_collector_binary_concat_right_plus_string_use);
    CU_add_test(suite, "test_collector_binary_concat_right_plus_number_use", test_collector_binary_concat_right_plus_number_use);
    CU_add_test(suite, "test_collector_binary_concat_ambiguous_only", test_collector_binary_concat_ambiguous_only);
    CU_add_test(suite, "test_collector_binary_multiple_expressions_correct", test_collector_binary_multiple_expressions_correct);
    CU_add_test(suite, "test_collector_binary_multiple_expressions_conflict", test_collector_binary_multiple_expressions_conflict);
    CU_add_test(suite, "test_collector_binary_nested", test_collector_binary_nested);
    CU_add_test(suite, "test_collector_binary_multiple_params_correct", test_collector_binary_multiple_params_correct);
    CU_add_test(suite, "test_collector_binary_multiple_params_one_conflict", test_collector_binary_multiple_params_one_conflict);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}