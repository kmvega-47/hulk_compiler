#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_binary_arithmetic(void)
{
    printf("\n\n------------ Inicio test: test_inference_binary_arithmetic ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: 1 + 2 (linea 1, columna 3)\n");

    LiteralNode *left = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 5);
    BinaryOperationNode *node = ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 1, 3);

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

void test_inference_binary_comparison(void)
{
    printf("\n\n------------ Inicio test: test_inference_binary_comparison ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: 1 == 2 (linea 2, columna 4)\n");

    LiteralNode *left = ast_number_literal_create(1.0, 2, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 2, 6);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 2, 4);

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

void test_inference_binary_logical(void)
{
    printf("\n\n------------ Inicio test: test_inference_binary_logical ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: true & false (linea 3, columna 6)\n");

    LiteralNode *left = ast_bool_literal_create(true, 3, 1);
    LiteralNode *right = ast_bool_literal_create(false, 3, 8);
    BinaryOperationNode *node = ast_binary_create(OP_AND, (ASTNode *)left, (ASTNode *)right, 3, 6);

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

void test_inference_binary_string(void)
{
    printf("\n\n------------ Inicio test: test_inference_binary_string ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: \"Hello\" @ \"World\" (linea 4, columna 9)\n");

    LiteralNode *left = ast_string_literal_create("Hello", 4, 1);
    LiteralNode *right = ast_string_literal_create("World", 4, 10);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 4, 9);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: String)\n", node->base.return_type->name);
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

void test_inference_binary_nested(void)
{
    printf("\n\n------------ Inicio test: test_inference_binary_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: (1 + 2) * (3 + 4) (linea 5, columna 1)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 5, 1);
    LiteralNode *b = ast_number_literal_create(2.0, 5, 3);
    LiteralNode *c = ast_number_literal_create(3.0, 5, 7);
    LiteralNode *d = ast_number_literal_create(4.0, 5, 9);
    BinaryOperationNode *add1 = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 5, 2);
    BinaryOperationNode *add2 = ast_binary_create(OP_ADD, (ASTNode *)c, (ASTNode *)d, 5, 8);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)add1, (ASTNode *)add2, 5, 5);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)iv);

    printf("Tipos: mult=%s, add1=%s, add2=%s\n",
           mult->base.return_type->name,
           add1->base.return_type->name,
           add2->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(mult->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(add1->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(add2->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de BinaryOperationNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BinaryOperationNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_binary_arithmetic", test_inference_binary_arithmetic);
    CU_add_test(suite, "test_inference_binary_comparison", test_inference_binary_comparison);
    CU_add_test(suite, "test_inference_binary_logical", test_inference_binary_logical);
    CU_add_test(suite, "test_inference_binary_string", test_inference_binary_string);
    CU_add_test(suite, "test_inference_binary_nested", test_inference_binary_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}