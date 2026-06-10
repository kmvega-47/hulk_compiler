#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_conditional_if_without_else(void)
{
    printf("\n\n------------ Inicio test: test_inference_conditional_if_without_else ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: if (true) 42 (linea 1, columna 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 12);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, NULL, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo del condicional: %s (esperado: Number)\n", node->base.return_type->name);
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

void test_inference_conditional_if_with_else(void)
{
    printf("\n\n------------ Inicio test: test_inference_conditional_if_with_else ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: if (true) 42 else 0 (linea 2, columna 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 2, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 2, 12);
    LiteralNode *else_branch = ast_number_literal_create(0.0, 2, 20);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 2, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo rama else: %s\n", else_branch->base.return_type->name);
    printf("Tipo del condicional: %s (esperado: Number)\n", node->base.return_type->name);
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

void test_inference_conditional_if_else_if_else(void)
{
    printf("\n\n------------ Inicio test: test_inference_conditional_if_else_if_else ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: if (true) 1 else if (false) 2 else 3 (linea 3, columna 1)\n");

    LiteralNode *cond1 = ast_bool_literal_create(true, 3, 5);
    LiteralNode *then1 = ast_number_literal_create(1.0, 3, 12);
    LiteralNode *cond2 = ast_bool_literal_create(false, 3, 22);
    LiteralNode *then2 = ast_number_literal_create(2.0, 3, 30);
    LiteralNode *else_b = ast_number_literal_create(3.0, 3, 37);

    ConditionalNode *inner = ast_conditional_create((ASTNode *)cond2, (ASTNode *)then2, (ASTNode *)else_b, 3, 19);
    ConditionalNode *outer = ast_conditional_create((ASTNode *)cond1, (ASTNode *)then1, (ASTNode *)inner, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)outer, (Visitor *)iv);

    printf("Tipo condicional interno: %s\n", inner->base.return_type->name);
    printf("Tipo del condicional externo: %s (esperado: Number)\n", outer->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(inner->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(outer->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

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

void test_inference_conditional_block_branches(void)
{
    printf("\n\n------------ Inicio test: test_inference_conditional_block_branches ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: if (true) { 1; \"hello\" } else { 2; false } (linea 4, columna 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 4, 5);

    List *then_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(then_exprs, ast_number_literal_create(1.0, 4, 15));
    list_append(then_exprs, ast_string_literal_create("hello", 4, 18));
    ExpressionBlockNode *then_block = ast_block_create(then_exprs, 4, 12);

    List *else_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(else_exprs, ast_number_literal_create(2.0, 4, 32));
    list_append(else_exprs, ast_bool_literal_create(false, 4, 35));
    ExpressionBlockNode *else_block = ast_block_create(else_exprs, 4, 29);

    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_block, (ASTNode *)else_block, 4, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo bloque then: %s\n", then_block->base.return_type->name);
    printf("Tipo bloque else: %s\n", else_block->base.return_type->name);
    printf("Tipo del condicional: %s (esperado: Object)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));

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

void test_inference_conditional_lca_resolution(void)
{
    printf("\n\n------------ Inicio test: test_inference_conditional_lca_resolution ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: if (true) \"hello\" else 42 (linea 5, columna 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 5, 5);
    LiteralNode *then_branch = ast_string_literal_create("hello", 5, 12);
    LiteralNode *else_branch = ast_number_literal_create(42.0, 5, 25);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 5, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo rama else: %s\n", else_branch->base.return_type->name);
    printf("Tipo del condicional (LCA): %s (esperado: Object)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));

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

int main(void)
{
    printf("\n=== Iniciando tests de ConditionalNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConditionalNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_conditional_if_without_else", test_inference_conditional_if_without_else);
    CU_add_test(suite, "test_inference_conditional_if_with_else", test_inference_conditional_if_with_else);
    CU_add_test(suite, "test_inference_conditional_if_else_if_else", test_inference_conditional_if_else_if_else);
    CU_add_test(suite, "test_inference_conditional_block_branches", test_inference_conditional_block_branches);
    CU_add_test(suite, "test_inference_conditional_lca_resolution", test_inference_conditional_lca_resolution);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}