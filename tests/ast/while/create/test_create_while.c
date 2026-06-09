#include <CUnit/Basic.h>
#include "ast.h"

void test_create_while_single_expression_body(void)
{
    printf("\n\n------------ Inicio test: test_create_while_single_expression_body ------------\n");
    global_tables_init();

    printf("Caso: while (true) 42 (linea 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 8);
    LiteralNode *body = ast_number_literal_create(42.0, 1, 14);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_WHILE_LOOP);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_WHILE_LOOP);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->condition, (ASTNode *)cond);
    printf("condition = %p (esperado: %p)\n", (void *)node->condition, (void *)cond);

    CU_ASSERT_PTR_EQUAL(node->body, (ASTNode *)body);
    printf("body = %p (esperado: %p)\n", (void *)node->body, (void *)body);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    free(cond);
    free(body);
    free(node);
    global_tables_destroy();
}

void test_create_while_multiple_expression_body(void)
{
    printf("\n\n------------ Inicio test: test_create_while_multiple_expression_body ------------\n");
    global_tables_init();

    printf("Caso: while (x < 10) { 1 + 2; 3 * 4; 5 - 6; } (lineas 1-5)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 1, 14);
    LiteralNode *b = ast_number_literal_create(10.0, 1, 18);
    BinaryOperationNode *cond = ast_binary_create(OP_LESS, (ASTNode *)a, (ASTNode *)b, 1, 16);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);

    LiteralNode *c1 = ast_number_literal_create(1.0, 2, 5);
    LiteralNode *c2 = ast_number_literal_create(2.0, 2, 9);
    BinaryOperationNode *add = ast_binary_create(OP_ADD, (ASTNode *)c1, (ASTNode *)c2, 2, 7);
    list_append(body_exprs, add);

    LiteralNode *d1 = ast_number_literal_create(3.0, 3, 5);
    LiteralNode *d2 = ast_number_literal_create(4.0, 3, 9);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)d1, (ASTNode *)d2, 3, 7);
    list_append(body_exprs, mult);

    LiteralNode *e1 = ast_number_literal_create(5.0, 4, 5);
    LiteralNode *e2 = ast_number_literal_create(6.0, 4, 9);
    BinaryOperationNode *sub = ast_binary_create(OP_SUB, (ASTNode *)e1, (ASTNode *)e2, 4, 7);
    list_append(body_exprs, sub);

    ExpressionBlockNode *body_block = ast_block_create(body_exprs, 1, 21);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body_block, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_WHILE_LOOP);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_WHILE_LOOP);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->condition, (ASTNode *)cond);
    printf("condition = %p (esperado: %p)\n", (void *)node->condition, (void *)cond);

    CU_ASSERT_PTR_EQUAL(node->body, (ASTNode *)body_block);
    printf("body = %p (esperado: %p)\n", (void *)node->body, (void *)body_block);

    for (size_t i = 0; i < list_count(body_exprs); i++)
    {
        BinaryOperationNode *expr = (BinaryOperationNode *)list_get(body_exprs, i);
        free(expr->left);
        free(expr->right);
        free(expr);
    }
    list_destroy(body_exprs);
    free(a); free(b); free(cond);
    free(body_block);
    free(node);
    global_tables_destroy();
}

void test_create_while_complex_condition(void)
{
    printf("\n\n------------ Inicio test: test_create_while_complex_condition ------------\n");
    global_tables_init();

    printf("Caso: while (!(a > b) & (c <= d)) 42 (linea 1)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 1, 15);
    LiteralNode *b = ast_number_literal_create(2.0, 1, 19);
    BinaryOperationNode *gt = ast_binary_create(OP_GREATER, (ASTNode *)a, (ASTNode *)b, 1, 17);
    UnaryOperationNode *not_op = ast_unary_create(OP_NOT, (ASTNode *)gt, 1, 14);

    LiteralNode *c = ast_number_literal_create(3.0, 1, 26);
    LiteralNode *d = ast_number_literal_create(4.0, 1, 30);
    BinaryOperationNode *le = ast_binary_create(OP_LESS_EQUAL, (ASTNode *)c, (ASTNode *)d, 1, 28);

    BinaryOperationNode *and_op = ast_binary_create(OP_AND, (ASTNode *)not_op, (ASTNode *)le, 1, 23);

    LiteralNode *body = ast_number_literal_create(42.0, 1, 35);
    WhileLoopNode *node = ast_while_create((ASTNode *)and_op, (ASTNode *)body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_WHILE_LOOP);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_WHILE_LOOP);

    CU_ASSERT_PTR_EQUAL(node->condition, (ASTNode *)and_op);
    printf("condition = %p (esperado: %p)\n", (void *)node->condition, (void *)and_op);

    CU_ASSERT_PTR_EQUAL(node->body, (ASTNode *)body);
    printf("body = %p (esperado: %p)\n", (void *)node->body, (void *)body);

    free(a); free(b); free(gt); free(not_op);
    free(c); free(d); free(le); free(and_op);
    free(body);
    free(node);
    global_tables_destroy();
}

void test_create_while_null_condition(void)
{
    printf("\n\n------------ Inicio test: test_create_while_null_condition ------------\n");
    global_tables_init();

    printf("Caso: condition=NULL -> debe retornar NULL\n");

    LiteralNode *body = ast_number_literal_create(42.0, 1, 1);
    WhileLoopNode *node = ast_while_create(NULL, (ASTNode *)body, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(body);
    global_tables_destroy();
}

void test_create_while_null_body(void)
{
    printf("\n\n------------ Inicio test: test_create_while_null_body ------------\n");
    global_tables_init();

    printf("Caso: body=NULL (while sin cuerpo, linea 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 8);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, NULL, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_WHILE_LOOP);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_WHILE_LOOP);

    CU_ASSERT_PTR_NOT_NULL(node->condition);
    printf("condition = %p (esperado: not NULL)\n", (void *)node->condition);

    CU_ASSERT_PTR_NULL(node->body);
    printf("body = %p (esperado: NULL)\n", (void *)node->body);

    free(cond);
    free(node);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de WhileLoopNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("WhileLoopNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_while_single_expression_body", test_create_while_single_expression_body);
    CU_add_test(suite, "test_create_while_multiple_expression_body", test_create_while_multiple_expression_body);
    CU_add_test(suite, "test_create_while_complex_condition", test_create_while_complex_condition);
    CU_add_test(suite, "test_create_while_null_condition", test_create_while_null_condition);
    CU_add_test(suite, "test_create_while_null_body", test_create_while_null_body);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}