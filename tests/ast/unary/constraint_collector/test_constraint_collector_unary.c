#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_unary_not(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_not ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => !x  -->  x debe ser Bool\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 15);
    ASTNode *body = (ASTNode *)ast_unary_create(OP_NOT, x_ref, 1, 14);
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

    printf("Errores reportados:\n");
    dm_print_errors(dm_global);

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

void test_collector_unary_negative(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_negative ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => -x  -->  x debe ser Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 15);
    ASTNode *body = (ASTNode *)ast_unary_create(OP_SUB, x_ref, 1, 14);
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

void test_collector_unary_multiple_not(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_multiple_not ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => !!x  -->  x sigue siendo Bool\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 17);
    ASTNode *not1 = (ASTNode *)ast_unary_create(OP_NOT, x_ref, 1, 16);
    ASTNode *body = (ASTNode *)ast_unary_create(OP_NOT, not1, 1, 15);
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

void test_collector_unary_multiple_negative(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_multiple_negative ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => --x  -->  x Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 17);
    ASTNode *neg1 = (ASTNode *)ast_unary_create(OP_SUB, x_ref, 1, 16);
    ASTNode *body = (ASTNode *)ast_unary_create(OP_SUB, neg1, 1, 15);
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

void test_collector_unary_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { !x; -x; }  -->  conflicto\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *not_x = (ASTNode *)ast_unary_create(OP_NOT, x1, 1, 21);
    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 27);
    ASTNode *neg_x = (ASTNode *)ast_unary_create(OP_SUB, x2, 1, 26);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, not_x);
    list_append(body_exprs, neg_x);
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

void test_collector_unary_multiple_params_all_correct(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_multiple_params_all_correct ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x, y) => { !x; -y; }  -->  x Bool, y Number\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 8));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 23);
    ASTNode *not_x = (ASTNode *)ast_unary_create(OP_NOT, x_ref, 1, 22);
    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 1, 28);
    ASTNode *neg_y = (ASTNode *)ast_unary_create(OP_SUB, y_ref, 1, 27);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, not_x);
    list_append(body_exprs, neg_y);
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

    printf("Parametro 'x' resuelto a: %s (esperado: Bool)\n", x_type ? x_type->name : "NULL");
    printf("Parametro 'y' resuelto a: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_unary_multiple_params_one_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_multiple_params_one_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x, y) => { !x; -x; !y; }  -->  x conflicto, y Bool\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 8));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 24);
    ASTNode *not_x = (ASTNode *)ast_unary_create(OP_NOT, x1, 1, 23);
    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 29);
    ASTNode *neg_x = (ASTNode *)ast_unary_create(OP_SUB, x2, 1, 28);
    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 1, 33);
    ASTNode *not_y = (ASTNode *)ast_unary_create(OP_NOT, y_ref, 1, 32);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, not_x);
    list_append(body_exprs, neg_x);
    list_append(body_exprs, not_y);
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

void test_collector_unary_nested(void)
{
    printf("\n\n------------ Inicio test: test_collector_unary_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => !(-x)  -->  x Number (el NOT no afecta a x)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 18);
    ASTNode *neg_x = (ASTNode *)ast_unary_create(OP_SUB, x_ref, 1, 17);
    ASTNode *body = (ASTNode *)ast_unary_create(OP_NOT, neg_x, 1, 16);
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

int main(void)
{
    printf("\n=== Iniciando tests de ConstraintCollector (unary) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Unary Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_unary_not", test_collector_unary_not);
    CU_add_test(suite, "test_collector_unary_negative", test_collector_unary_negative);
    CU_add_test(suite, "test_collector_unary_multiple_not", test_collector_unary_multiple_not);
    CU_add_test(suite, "test_collector_unary_multiple_negative", test_collector_unary_multiple_negative);
    CU_add_test(suite, "test_collector_unary_conflict", test_collector_unary_conflict);
    CU_add_test(suite, "test_collector_unary_multiple_params_all_correct", test_collector_unary_multiple_params_all_correct);
    CU_add_test(suite, "test_collector_unary_multiple_params_one_conflict", test_collector_unary_multiple_params_one_conflict);
    CU_add_test(suite, "test_collector_unary_nested", test_collector_unary_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}