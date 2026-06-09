#include <CUnit/Basic.h>
#include "ast.h"

void test_create_block_empty(void)
{
    printf("\n\n------------ Inicio test: test_create_block_empty ------------\n");
    global_tables_init();

    printf("Caso: { } (linea 1)\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_EXPRESSION_BLOCK);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_EXPRESSION_BLOCK);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->expressions, exprs);
    printf("expressions = %p (esperado: %p)\n", (void *)node->expressions, (void *)exprs);

    CU_ASSERT_EQUAL(list_count(node->expressions), 0);
    printf("expressions count = %zu (esperado: 0)\n", list_count(node->expressions));

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    list_destroy(exprs);
    free(node);
    global_tables_destroy();
}

void test_create_block_single(void)
{
    printf("\n\n------------ Inicio test: test_create_block_single ------------\n");
    global_tables_init();

    printf("Caso: { 42; } (lineas 1-3)\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    LiteralNode *lit = ast_number_literal_create(42.0, 2, 5);
    list_append(exprs, lit);

    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_EXPRESSION_BLOCK);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_EXPRESSION_BLOCK);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->expressions, exprs);
    printf("expressions = %p (esperado: %p)\n", (void *)node->expressions, (void *)exprs);

    CU_ASSERT_EQUAL(list_count(node->expressions), 1);
    printf("expressions count = %zu (esperado: 1)\n", list_count(node->expressions));

    free(lit);
    list_destroy(exprs);
    free(node);
    global_tables_destroy();
}

void test_create_block_multiple(void)
{
    printf("\n\n------------ Inicio test: test_create_block_multiple ------------\n");
    global_tables_init();

    printf("Caso: { 1 + 2; !true; \"hello\" @ \" world\"; -5; 3 * 4; } (lineas 1-7)\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);

    LiteralNode *a = ast_number_literal_create(1.0, 2, 5);
    LiteralNode *b = ast_number_literal_create(2.0, 2, 9);
    BinaryOperationNode *bin0 = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 2, 7);
    list_append(exprs, bin0);

    LiteralNode *t = ast_bool_literal_create(true, 3, 6);
    UnaryOperationNode *un1 = ast_unary_create(OP_NOT, (ASTNode *)t, 3, 5);
    list_append(exprs, un1);

    LiteralNode *s1 = ast_string_literal_create("hello", 4, 5);
    LiteralNode *s2 = ast_string_literal_create(" world", 4, 14);
    BinaryOperationNode *bin2 = ast_binary_create(OP_CONCAT, (ASTNode *)s1, (ASTNode *)s2, 4, 12);
    list_append(exprs, bin2);

    LiteralNode *n = ast_number_literal_create(5.0, 5, 6);
    UnaryOperationNode *un3 = ast_unary_create(OP_SUB, (ASTNode *)n, 5, 5);
    list_append(exprs, un3);

    LiteralNode *c = ast_number_literal_create(3.0, 6, 5);
    LiteralNode *d = ast_number_literal_create(4.0, 6, 9);
    BinaryOperationNode *bin4 = ast_binary_create(OP_MULT, (ASTNode *)c, (ASTNode *)d, 6, 7);
    list_append(exprs, bin4);

    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_EXPRESSION_BLOCK);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_EXPRESSION_BLOCK);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_EQUAL(list_count(node->expressions), 5);
    printf("expressions count = %zu (esperado: 5)\n", list_count(node->expressions));

    free(bin0->left);
    free(bin0->right);
    free(bin0);

    free(un1->operand);
    free(un1);

    free(((LiteralNode *)bin2->left)->value.string_value);
    free(bin2->left);
    free(((LiteralNode *)bin2->right)->value.string_value);
    free(bin2->right);
    free(bin2);

    free(un3->operand);
    free(un3);

    free(bin4->left);
    free(bin4->right);
    free(bin4);

    list_destroy(exprs);
    free(node);
    global_tables_destroy();
}

void test_create_block_null_expressions(void)
{
    printf("\n\n------------ Inicio test: test_create_block_null_expressions ------------\n");
    global_tables_init();

    printf("Caso: lista expressions NULL -> debe retornar NULL\n");
    ExpressionBlockNode *node = ast_block_create(NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ExpressionBlockNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ExpressionBlockNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_block_empty", test_create_block_empty);
    CU_add_test(suite, "test_create_block_single", test_create_block_single);
    CU_add_test(suite, "test_create_block_multiple", test_create_block_multiple);
    CU_add_test(suite, "test_create_block_null_expressions", test_create_block_null_expressions);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}