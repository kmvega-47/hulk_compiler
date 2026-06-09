#include <CUnit/Basic.h>
#include "ast.h"

void test_create_conditional_if_without_else(void)
{
    printf("\n\n------------ Inicio test: test_create_conditional_if_without_else ------------\n");
    global_tables_init();

    printf("Caso: if (true) 42 (linea 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 12);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, NULL, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_CONDITIONAL);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_CONDITIONAL);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->condition, (ASTNode *)cond);
    printf("condition = %p (esperado: %p)\n", (void *)node->condition, (void *)cond);

    CU_ASSERT_PTR_EQUAL(node->then_branch, (ASTNode *)then_branch);
    printf("then_branch = %p (esperado: %p)\n", (void *)node->then_branch, (void *)then_branch);

    CU_ASSERT_PTR_NULL(node->else_branch);
    printf("else_branch = %p (esperado: NULL)\n", (void *)node->else_branch);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    free(cond);
    free(then_branch);
    free(node);
    global_tables_destroy();
}

void test_create_conditional_if_with_else(void)
{
    printf("\n\n------------ Inicio test: test_create_conditional_if_with_else ------------\n");
    global_tables_init();

    printf("Caso: if (true) 42 else 0 (linea 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 12);
    LiteralNode *else_branch = ast_number_literal_create(0.0, 1, 20);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_CONDITIONAL);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_CONDITIONAL);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->condition, (ASTNode *)cond);
    printf("condition = %p (esperado: %p)\n", (void *)node->condition, (void *)cond);

    CU_ASSERT_PTR_EQUAL(node->then_branch, (ASTNode *)then_branch);
    printf("then_branch = %p (esperado: %p)\n", (void *)node->then_branch, (void *)then_branch);

    CU_ASSERT_PTR_EQUAL(node->else_branch, (ASTNode *)else_branch);
    printf("else_branch = %p (esperado: %p)\n", (void *)node->else_branch, (void *)else_branch);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    free(cond);
    free(then_branch);
    free(else_branch);
    free(node);
    global_tables_destroy();
}

void test_create_conditional_if_else_if_else(void)
{
    printf("\n\n------------ Inicio test: test_create_conditional_if_else_if_else ------------\n");
    global_tables_init();

    printf("Caso: if (x < 0) -1 else if (x > 0) 1 else 0 (lineas 1-2)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 1, 9);
    LiteralNode *b = ast_number_literal_create(0.0, 1, 13);
    BinaryOperationNode *cond1 = ast_binary_create(OP_LESS, (ASTNode *)a, (ASTNode *)b, 1, 11);
    LiteralNode *then1 = ast_number_literal_create(-1.0, 1, 16);

    LiteralNode *c = ast_number_literal_create(1.0, 2, 14);
    LiteralNode *d = ast_number_literal_create(0.0, 2, 18);
    BinaryOperationNode *cond2 = ast_binary_create(OP_GREATER, (ASTNode *)c, (ASTNode *)d, 2, 16);
    LiteralNode *then2 = ast_number_literal_create(1.0, 2, 21);

    LiteralNode *else_branch = ast_number_literal_create(0.0, 2, 28);

    ConditionalNode *inner_if = ast_conditional_create((ASTNode *)cond2, (ASTNode *)then2, (ASTNode *)else_branch, 2, 6);
    ConditionalNode *outer_if = ast_conditional_create((ASTNode *)cond1, (ASTNode *)then1, (ASTNode *)inner_if, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(outer_if);
    CU_ASSERT_EQUAL(outer_if->base.node_type, NODE_CONDITIONAL);
    printf("node_type = %d (esperado: %d)\n", outer_if->base.node_type, NODE_CONDITIONAL);

    CU_ASSERT_PTR_EQUAL(outer_if->condition, (ASTNode *)cond1);
    printf("condition = %p (esperado: %p)\n", (void *)outer_if->condition, (void *)cond1);

    CU_ASSERT_PTR_EQUAL(outer_if->then_branch, (ASTNode *)then1);
    printf("then_branch = %p (esperado: %p)\n", (void *)outer_if->then_branch, (void *)then1);

    CU_ASSERT_PTR_EQUAL(outer_if->else_branch, (ASTNode *)inner_if);
    printf("else_branch = %p (esperado: %p)\n", (void *)outer_if->else_branch, (void *)inner_if);

    free(a); free(b); free(cond1); free(then1);
    free(c); free(d); free(cond2); free(then2);
    free(else_branch);
    free(inner_if);
    free(outer_if);
    global_tables_destroy();
}

void test_create_conditional_complex_condition(void)
{
    printf("\n\n------------ Inicio test: test_create_conditional_complex_condition ------------\n");
    global_tables_init();

    printf("Caso: if (!(a > b) & (c <= d)) 42 (linea 1)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 1, 9);
    LiteralNode *b = ast_number_literal_create(2.0, 1, 13);
    BinaryOperationNode *gt = ast_binary_create(OP_GREATER, (ASTNode *)a, (ASTNode *)b, 1, 11);
    UnaryOperationNode *not_op = ast_unary_create(OP_NOT, (ASTNode *)gt, 1, 7);

    LiteralNode *c = ast_number_literal_create(3.0, 1, 20);
    LiteralNode *d = ast_number_literal_create(4.0, 1, 24);
    BinaryOperationNode *le = ast_binary_create(OP_LESS_EQUAL, (ASTNode *)c, (ASTNode *)d, 1, 22);

    BinaryOperationNode *and_op = ast_binary_create(OP_AND, (ASTNode *)not_op, (ASTNode *)le, 1, 16);

    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 29);
    ConditionalNode *node = ast_conditional_create((ASTNode *)and_op, (ASTNode *)then_branch, NULL, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_CONDITIONAL);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_CONDITIONAL);

    CU_ASSERT_PTR_EQUAL(node->condition, (ASTNode *)and_op);
    printf("condition = %p (esperado: %p)\n", (void *)node->condition, (void *)and_op);

    CU_ASSERT_PTR_EQUAL(node->then_branch, (ASTNode *)then_branch);
    printf("then_branch = %p (esperado: %p)\n", (void *)node->then_branch, (void *)then_branch);

    CU_ASSERT_PTR_NULL(node->else_branch);
    printf("else_branch = %p (esperado: NULL)\n", (void *)node->else_branch);

    free(a); free(b); free(gt); free(not_op);
    free(c); free(d); free(le); free(and_op);
    free(then_branch);
    free(node);
    global_tables_destroy();
}

void test_create_conditional_null_condition(void)
{
    printf("\n\n------------ Inicio test: test_create_conditional_null_condition ------------\n");
    global_tables_init();

    printf("Caso: condition=NULL -> debe retornar NULL\n");

    LiteralNode *then_branch = ast_number_literal_create(1.0, 1, 1);
    ConditionalNode *node = ast_conditional_create(NULL, (ASTNode *)then_branch, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(then_branch);
    global_tables_destroy();
}

void test_create_conditional_null_then_branch(void)
{
    printf("\n\n------------ Inicio test: test_create_conditional_null_then_branch ------------\n");
    global_tables_init();

    printf("Caso: then_branch=NULL -> debe retornar NULL\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 5);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, NULL, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(cond);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ConditionalNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConditionalNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_conditional_if_without_else", test_create_conditional_if_without_else);
    CU_add_test(suite, "test_create_conditional_if_with_else", test_create_conditional_if_with_else);
    CU_add_test(suite, "test_create_conditional_if_else_if_else", test_create_conditional_if_else_if_else);
    CU_add_test(suite, "test_create_conditional_complex_condition", test_create_conditional_complex_condition);
    CU_add_test(suite, "test_create_conditional_null_condition", test_create_conditional_null_condition);
    CU_add_test(suite, "test_create_conditional_null_then_branch", test_create_conditional_null_then_branch);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}