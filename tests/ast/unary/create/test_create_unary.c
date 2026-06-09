#include <CUnit/Basic.h>
#include "ast.h"

void test_create_unary_not(void)
{
    printf("\n\n------------ Inicio test: test_create_unary_not ------------\n");
    global_tables_init();

    printf("Caso: !true, linea 1, col 1\n");
    LiteralNode *operand = ast_bool_literal_create(true, 1, 2);
    UnaryOperationNode *node = ast_unary_create(OP_NOT, (ASTNode *)operand, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_UNARY_OPERATION);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_UNARY_OPERATION);

    CU_ASSERT_EQUAL(node->operator, OP_NOT);
    printf("operator = %d (esperado: %d)\n", node->operator, OP_NOT);

    CU_ASSERT_PTR_EQUAL(node->operand, (ASTNode *)operand);
    printf("operand = %p (esperado: %p)\n", (void *)node->operand, (void *)operand);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    free(operand);
    free(node);
    global_tables_destroy();
}

void test_create_unary_negative(void)
{
    printf("\n\n------------ Inicio test: test_create_unary_negative ------------\n");
    global_tables_init();

    printf("Caso: -42, linea 2, col 1\n");
    LiteralNode *operand = ast_number_literal_create(42.0, 2, 2);
    UnaryOperationNode *node = ast_unary_create(OP_SUB, (ASTNode *)operand, 2, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_UNARY_OPERATION);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_UNARY_OPERATION);

    CU_ASSERT_EQUAL(node->operator, OP_SUB);
    printf("operator = %d (esperado: %d)\n", node->operator, OP_SUB);

    CU_ASSERT_PTR_EQUAL(node->operand, (ASTNode *)operand);
    printf("operand = %p (esperado: %p)\n", (void *)node->operand, (void *)operand);

    CU_ASSERT_EQUAL(node->base.line, 2);
    printf("line = %d (esperado: 2)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    free(operand);
    free(node);
    global_tables_destroy();
}

void test_create_unary_nested(void)
{
    printf("\n\n------------ Inicio test: test_create_unary_nested ------------\n");
    global_tables_init();

    printf("Caso: ---47, linea 3\n");
    LiteralNode *inner = ast_number_literal_create(47.0, 3, 4);
    UnaryOperationNode *neg1 = ast_unary_create(OP_SUB, (ASTNode *)inner, 3, 3);
    UnaryOperationNode *neg2 = ast_unary_create(OP_SUB, (ASTNode *)neg1, 3, 2);
    UnaryOperationNode *neg3 = ast_unary_create(OP_SUB, (ASTNode *)neg2, 3, 1);

    CU_ASSERT_PTR_NOT_NULL(neg3);
    CU_ASSERT_EQUAL(neg3->base.node_type, NODE_UNARY_OPERATION);
    printf("neg3 node_type = %d (esperado: %d)\n", neg3->base.node_type, NODE_UNARY_OPERATION);

    CU_ASSERT_EQUAL(neg3->operator, OP_SUB);
    printf("neg3 operator = %d (esperado: %d)\n", neg3->operator, OP_SUB);

    CU_ASSERT_PTR_EQUAL(neg3->operand, (ASTNode *)neg2);
    printf("neg3 operand = %p (esperado: %p)\n", (void *)neg3->operand, (void *)neg2);

    CU_ASSERT_EQUAL(neg3->base.line, 3);
    printf("neg3 line = %d (esperado: 3)\n", neg3->base.line);

    CU_ASSERT_EQUAL(neg3->base.column, 1);
    printf("neg3 column = %d (esperado: 1)\n", neg3->base.column);

    free(inner);
    free(neg1);
    free(neg2);
    free(neg3);
    global_tables_destroy();
}

void test_create_unary_null_operand(void)
{
    printf("\n\n------------ Inicio test: test_create_unary_null_operand ------------\n");
    global_tables_init();

    printf("Caso: operando NULL (debe fallar)\n");
    UnaryOperationNode *node = ast_unary_create(OP_NOT, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de UnaryOperationNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("UnaryOperationNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_unary_not", test_create_unary_not);
    CU_add_test(suite, "test_create_unary_negative", test_create_unary_negative);
    CU_add_test(suite, "test_create_unary_nested", test_create_unary_nested);
    CU_add_test(suite, "test_create_unary_null_operand", test_create_unary_null_operand);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}