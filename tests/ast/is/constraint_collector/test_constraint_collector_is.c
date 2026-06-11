#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_is_no_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_is_no_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A {} f(x) => 42 is A  -->  x Object (sin restriccion)\n");

    UserTypeDescriptor *a_type = user_type_create("A",
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT),
        list_create(0, NULL, NULL, NULL, NULL));
    type_table_insert(global_type_table, a_type);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *body = (ASTNode *)ast_is_create((ASTNode *)ast_number_literal_create(42.0, 1, 15), "A", 1, 18);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para x: %s (esperado: Object)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
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

void test_collector_is_undefined_type(void)
{
    printf("\n\n------------ Inicio test: test_collector_is_undefined_type ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => x is NoExiste  -->  error, x Object\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 15);
    ASTNode *body = (ASTNode *)ast_is_create(x_ref, "NoExiste", 1, 17);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para x: %s (esperado: Object)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu (esperado: >= 1)\n", dm_error_count(dm_global));
    CU_ASSERT(dm_error_count(dm_global) >= 1);

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
    printf("\n=== Iniciando tests de ConstraintCollector (is) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Is Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_is_no_param", test_collector_is_no_param);
    CU_add_test(suite, "test_collector_is_undefined_type", test_collector_is_undefined_type);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}