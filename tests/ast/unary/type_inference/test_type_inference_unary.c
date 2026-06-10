#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_unary_not(void)
{
    printf("\n\n------------ Inicio test: test_inference_unary_not ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: !true (linea 1, columna 1)\n");

    LiteralNode *operand = ast_bool_literal_create(true, 1, 2);
    UnaryOperationNode *node = ast_unary_create(OP_NOT, (ASTNode *)operand, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));

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

void test_inference_unary_negative(void)
{
    printf("\n\n------------ Inicio test: test_inference_unary_negative ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: -42 (linea 2, columna 3)\n");

    LiteralNode *operand = ast_number_literal_create(42.0, 2, 4);
    UnaryOperationNode *node = ast_unary_create(OP_SUB, (ASTNode *)operand, 2, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
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

void test_inference_unary_nested(void)
{
    printf("\n\n------------ Inicio test: test_inference_unary_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: ---47 (linea 3, columna 1)\n");

    LiteralNode *inner = ast_number_literal_create(47.0, 3, 4);
    UnaryOperationNode *neg1 = ast_unary_create(OP_SUB, (ASTNode *)inner, 3, 3);
    UnaryOperationNode *neg2 = ast_unary_create(OP_SUB, (ASTNode *)neg1, 3, 2);
    UnaryOperationNode *neg3 = ast_unary_create(OP_SUB, (ASTNode *)neg2, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)neg3, (Visitor *)iv);

    printf("Tipos: neg3=%s, neg2=%s, neg1=%s, inner=%s\n",
           neg3->base.return_type->name,
           neg2->base.return_type->name,
           neg1->base.return_type->name,
           inner->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(neg3->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(neg2->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(neg1->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)neg3, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de UnaryOperationNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("UnaryOperationNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_unary_not", test_inference_unary_not);
    CU_add_test(suite, "test_inference_unary_negative", test_inference_unary_negative);
    CU_add_test(suite, "test_inference_unary_nested", test_inference_unary_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}