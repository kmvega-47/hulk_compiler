#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_variable_defined(void)
{
    printf("\n\n------------ Inicio test: test_inference_variable_defined ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x definida en scope (linea 1, columna 1)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    TypeDescriptor *num_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    scope_add_variable(iv->current_scope, "x", num_type);

    VariableReferenceNode *node = ast_variable_reference_create("x", 1, 1);
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, num_type);
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    type_inference_visitor_destroy(iv);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_variable_undefined(void)
{
    printf("\n\n------------ Inicio test: test_inference_variable_undefined ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: variable no definida (linea 2, columna 3)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    VariableReferenceNode *node = ast_variable_reference_create("undef", 2, 3);
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
    CU_ASSERT_PTR_NULL(node->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1)\n", dm_error_count(dm_global));

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    type_inference_visitor_destroy(iv);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_variable_undefined_no_duplicate_error(void)
{
    printf("\n\n------------ Inicio test: test_inference_variable_undefined_no_duplicate_error ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: misma variable indefinida referenciada dos veces (linea 3)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    VariableReferenceNode *node1 = ast_variable_reference_create("undef", 3, 5);
    ast_accept((ASTNode *)node1, (Visitor *)iv);

    VariableReferenceNode *node2 = ast_variable_reference_create("undef", 3, 15);
    ast_accept((ASTNode *)node2, (Visitor *)iv);

    CU_ASSERT_PTR_NULL(node1->base.return_type);
    CU_ASSERT_PTR_NULL(node2->base.return_type);
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1, no duplicado)\n", dm_error_count(dm_global));

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node1, (Visitor *)fv);
    ast_accept((ASTNode *)node2, (Visitor *)fv);
    free_visitor_destroy(fv);

    type_inference_visitor_destroy(iv);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_variable_in_parent_scope(void)
{
    printf("\n\n------------ Inicio test: test_inference_variable_in_parent_scope ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: variable en scope padre (linea 4, columna 5)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    TypeDescriptor *str_type = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    scope_add_variable(iv->current_scope, "msg", str_type);

    Scope *child_scope = scope_create(iv->current_scope);
    Scope *old_scope = iv->current_scope;
    iv->current_scope = child_scope;

    VariableReferenceNode *node = ast_variable_reference_create("msg", 4, 5);
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: String)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, str_type);
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    iv->current_scope = old_scope;
    scope_destroy(child_scope);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    type_inference_visitor_destroy(iv);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_variable_shadowed(void)
{
    printf("\n\n------------ Inicio test: test_inference_variable_shadowed ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: variable sombreada en scope hijo (linea 5, columna 5)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    TypeDescriptor *num_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str_type = type_table_lookup_by_tag(global_type_table, HULK_STRING);

    scope_add_variable(iv->current_scope, "x", num_type);

    Scope *child_scope = scope_create(iv->current_scope);
    Scope *old_scope = iv->current_scope;
    iv->current_scope = child_scope;
    scope_add_variable(iv->current_scope, "x", str_type);

    VariableReferenceNode *node = ast_variable_reference_create("x", 5, 5);
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido en scope hijo: %s (esperado: String, sombreado)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, str_type);
    CU_ASSERT_FALSE(dm_has_errors(dm_global));

    iv->current_scope = old_scope;
    scope_destroy(child_scope);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    type_inference_visitor_destroy(iv);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de VariableReferenceNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("VariableReferenceNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_variable_defined", test_inference_variable_defined);
    CU_add_test(suite, "test_inference_variable_undefined", test_inference_variable_undefined);
    CU_add_test(suite, "test_inference_variable_undefined_no_duplicate_error", test_inference_variable_undefined_no_duplicate_error);
    CU_add_test(suite, "test_inference_variable_in_parent_scope", test_inference_variable_in_parent_scope);
    CU_add_test(suite, "test_inference_variable_shadowed", test_inference_variable_shadowed);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}