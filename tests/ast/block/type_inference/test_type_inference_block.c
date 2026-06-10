#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_block_empty(void)
{
    printf("\n\n------------ Inicio test: test_inference_block_empty ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: { } (linea 1, columna 1)\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido del bloque: %s (esperado: Void)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_block_single(void)
{
    printf("\n\n------------ Inicio test: test_inference_block_single ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: { 42 } (linea 2, columna 1)\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_number_literal_create(42.0, 2, 3));
    ExpressionBlockNode *node = ast_block_create(exprs, 2, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    ASTNode *inner = (ASTNode *)list_get(node->expressions, 0);
    printf("Tipo expresion interna: %s\n", inner->return_type->name);
    printf("Tipo inferido del bloque: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_block_multiple(void)
{
    printf("\n\n------------ Inicio test: test_inference_block_multiple ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: { 1; true; \"hello\" } (linea 3, columna 1)\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_number_literal_create(1.0, 3, 3));
    list_append(exprs, ast_bool_literal_create(true, 3, 6));
    list_append(exprs, ast_string_literal_create("hello", 3, 12));
    ExpressionBlockNode *node = ast_block_create(exprs, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    for (size_t i = 0; i < list_count(node->expressions); i++)
    {
        ASTNode *expr = (ASTNode *)list_get(node->expressions, i);
        printf("Tipo expr[%zu]: %s\n", i, expr->return_type->name);
    }
    printf("Tipo inferido del bloque: %s (esperado: String)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_block_nested(void)
{
    printf("\n\n------------ Inicio test: test_inference_block_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: { { 1; true; \"hello\" } } (linea 4, columna 1)\n");

    List *inner_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(inner_exprs, ast_number_literal_create(1.0, 4, 5));
    list_append(inner_exprs, ast_bool_literal_create(true, 4, 8));
    list_append(inner_exprs, ast_string_literal_create("hello", 4, 14));
    ExpressionBlockNode *inner = ast_block_create(inner_exprs, 4, 3);

    List *outer_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(outer_exprs, inner);
    ExpressionBlockNode *outer = ast_block_create(outer_exprs, 4, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)outer, (Visitor *)iv);

    printf("Tipo bloque interno: %s (esperado: String)\n", inner->base.return_type->name);
    printf("Tipo bloque externo: %s (esperado: String)\n", outer->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(inner->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_PTR_EQUAL(outer->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de ExpressionBlockNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ExpressionBlockNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_block_empty", test_inference_block_empty);
    CU_add_test(suite, "test_inference_block_single", test_inference_block_single);
    CU_add_test(suite, "test_inference_block_multiple", test_inference_block_multiple);
    CU_add_test(suite, "test_inference_block_nested", test_inference_block_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}