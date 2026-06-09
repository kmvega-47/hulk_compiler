#include <CUnit/Basic.h>
#include "ast.h"

void test_create_binary_arithmetic(void)
{
    printf("\n\n------------ Inicio test: test_create_binary_arithmetic ------------\n");
    global_tables_init();

    printf("Caso: 1 + 2 (linea 1, col 3)\n");
    LiteralNode *left = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 5);
    BinaryOperationNode *add = ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 1, 3);

    CU_ASSERT_PTR_NOT_NULL(add);
    CU_ASSERT_EQUAL(add->base.node_type, NODE_BINARY_OPERATION);
    printf("node_type = %d (esperado: %d)\n", add->base.node_type, NODE_BINARY_OPERATION);

    CU_ASSERT_EQUAL(add->operator, OP_ADD);
    printf("operator = %d (esperado: %d)\n", add->operator, OP_ADD);

    CU_ASSERT_PTR_EQUAL(add->left, (ASTNode *)left);
    printf("left = %p (esperado: %p)\n", (void *)add->left, (void *)left);

    CU_ASSERT_PTR_EQUAL(add->right, (ASTNode *)right);
    printf("right = %p (esperado: %p)\n", (void *)add->right, (void *)right);

    CU_ASSERT_EQUAL(add->base.line, 1);
    printf("line = %d (esperado: 1)\n", add->base.line);

    CU_ASSERT_EQUAL(add->base.column, 3);
    printf("column = %d (esperado: 3)\n", add->base.column);

    CU_ASSERT_PTR_NULL(add->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)add->base.return_type);

    free(left);
    free(right);
    free(add);

    printf("Caso: 10 - 4 (linea 2, col 3)\n");
    left  = ast_number_literal_create(10.0, 2, 1);
    right = ast_number_literal_create(4.0,  2, 5);
    BinaryOperationNode *sub = ast_binary_create(OP_SUB, (ASTNode *)left, (ASTNode *)right, 2, 3);
    CU_ASSERT_PTR_NOT_NULL(sub);
    CU_ASSERT_EQUAL(sub->operator, OP_SUB);
    printf("operator = %d (esperado: %d)\n", sub->operator, OP_SUB);
    free(left); free(right); free(sub);

    printf("Caso: 6 * 7 (linea 3, col 3)\n");
    left  = ast_number_literal_create(6.0, 3, 1);
    right = ast_number_literal_create(7.0, 3, 5);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)left, (ASTNode *)right, 3, 3);
    CU_ASSERT_PTR_NOT_NULL(mult);
    CU_ASSERT_EQUAL(mult->operator, OP_MULT);
    printf("operator = %d (esperado: %d)\n", mult->operator, OP_MULT);
    free(left); free(right); free(mult);

    printf("Caso: 8 / 2 (linea 4, col 3)\n");
    left  = ast_number_literal_create(8.0, 4, 1);
    right = ast_number_literal_create(2.0, 4, 5);
    BinaryOperationNode *div_node = ast_binary_create(OP_DIV, (ASTNode *)left, (ASTNode *)right, 4, 3);
    CU_ASSERT_PTR_NOT_NULL(div_node);
    CU_ASSERT_EQUAL(div_node->operator, OP_DIV);
    printf("operator = %d (esperado: %d)\n", div_node->operator, OP_DIV);
    free(left); free(right); free(div_node);

    printf("Caso: 10 %% 3 (linea 5, col 3)\n");
    left  = ast_number_literal_create(10.0, 5, 1);
    right = ast_number_literal_create(3.0,  5, 6);
    BinaryOperationNode *mod = ast_binary_create(OP_MOD, (ASTNode *)left, (ASTNode *)right, 5, 4);
    CU_ASSERT_PTR_NOT_NULL(mod);
    CU_ASSERT_EQUAL(mod->operator, OP_MOD);
    printf("operator = %d (esperado: %d)\n", mod->operator, OP_MOD);
    free(left); free(right); free(mod);

    printf("Caso: 2 ^ 3 (linea 6, col 3)\n");
    left  = ast_number_literal_create(2.0, 6, 1);
    right = ast_number_literal_create(3.0, 6, 5);
    BinaryOperationNode *exp_node = ast_binary_create(OP_EXP, (ASTNode *)left, (ASTNode *)right, 6, 3);
    CU_ASSERT_PTR_NOT_NULL(exp_node);
    CU_ASSERT_EQUAL(exp_node->operator, OP_EXP);
    printf("operator = %d (esperado: %d)\n", exp_node->operator, OP_EXP);
    free(left); free(right); free(exp_node);

    global_tables_destroy();
}

void test_create_binary_logical(void)
{
    printf("\n\n------------ Inicio test: test_create_binary_logical ------------\n");
    global_tables_init();

    printf("Caso: true & false (linea 1, col 6)\n");
    LiteralNode *left = ast_bool_literal_create(true,  1, 1);
    LiteralNode *right = ast_bool_literal_create(false, 1, 8);
    BinaryOperationNode *and_node = ast_binary_create(OP_AND, (ASTNode *)left, (ASTNode *)right, 1, 6);

    CU_ASSERT_PTR_NOT_NULL(and_node);
    CU_ASSERT_EQUAL(and_node->operator, OP_AND);
    printf("operator = %d (esperado: %d)\n", and_node->operator, OP_AND);
    CU_ASSERT_EQUAL(and_node->base.line, 1);
    printf("line = %d (esperado: 1)\n", and_node->base.line);
    CU_ASSERT_EQUAL(and_node->base.column, 6);
    printf("column = %d (esperado: 6)\n", and_node->base.column);

    free(left); free(right); free(and_node);

    printf("Caso: true | false (linea 2, col 6)\n");
    left  = ast_bool_literal_create(true,  2, 1);
    right = ast_bool_literal_create(false, 2, 9);
    BinaryOperationNode *or_node = ast_binary_create(OP_OR, (ASTNode *)left, (ASTNode *)right, 2, 7);
    CU_ASSERT_PTR_NOT_NULL(or_node);
    CU_ASSERT_EQUAL(or_node->operator, OP_OR);
    printf("operator = %d (esperado: %d)\n", or_node->operator, OP_OR);
    free(left); free(right); free(or_node);

    global_tables_destroy();
}

void test_create_binary_comparison(void)
{
    printf("\n\n------------ Inicio test: test_create_binary_comparison ------------\n");
    global_tables_init();

    printf("Caso: 1 == 2 (linea 1, col 3)\n");
    LiteralNode *left  = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 6);
    BinaryOperationNode *eq = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 1, 3);
    CU_ASSERT_PTR_NOT_NULL(eq);
    CU_ASSERT_EQUAL(eq->operator, OP_EQUAL);
    printf("operator = %d (esperado: %d)\n", eq->operator, OP_EQUAL);
    free(left); free(right); free(eq);

    printf("Caso: 1 != 2 (linea 2, col 3)\n");
    left  = ast_number_literal_create(1.0, 2, 1);
    right = ast_number_literal_create(2.0, 2, 6);
    BinaryOperationNode *neq = ast_binary_create(OP_NOT_EQUAL, (ASTNode *)left, (ASTNode *)right, 2, 3);
    CU_ASSERT_PTR_NOT_NULL(neq);
    CU_ASSERT_EQUAL(neq->operator, OP_NOT_EQUAL);
    printf("operator = %d (esperado: %d)\n", neq->operator, OP_NOT_EQUAL);
    free(left); free(right); free(neq);

    printf("Caso: 1 < 2 (linea 3, col 3)\n");
    left  = ast_number_literal_create(1.0, 3, 1);
    right = ast_number_literal_create(2.0, 3, 5);
    BinaryOperationNode *lt = ast_binary_create(OP_LESS, (ASTNode *)left, (ASTNode *)right, 3, 3);
    CU_ASSERT_PTR_NOT_NULL(lt);
    CU_ASSERT_EQUAL(lt->operator, OP_LESS);
    printf("operator = %d (esperado: %d)\n", lt->operator, OP_LESS);
    free(left); free(right); free(lt);

    printf("Caso: 1 > 2 (linea 4, col 3)\n");
    left  = ast_number_literal_create(1.0, 4, 1);
    right = ast_number_literal_create(2.0, 4, 5);
    BinaryOperationNode *gt = ast_binary_create(OP_GREATER, (ASTNode *)left, (ASTNode *)right, 4, 3);
    CU_ASSERT_PTR_NOT_NULL(gt);
    CU_ASSERT_EQUAL(gt->operator, OP_GREATER);
    printf("operator = %d (esperado: %d)\n", gt->operator, OP_GREATER);
    free(left); free(right); free(gt);

    printf("Caso: 1 <= 2 (linea 5, col 3)\n");
    left  = ast_number_literal_create(1.0, 5, 1);
    right = ast_number_literal_create(2.0, 5, 6);
    BinaryOperationNode *le = ast_binary_create(OP_LESS_EQUAL, (ASTNode *)left, (ASTNode *)right, 5, 3);
    CU_ASSERT_PTR_NOT_NULL(le);
    CU_ASSERT_EQUAL(le->operator, OP_LESS_EQUAL);
    printf("operator = %d (esperado: %d)\n", le->operator, OP_LESS_EQUAL);
    free(left); free(right); free(le);

    printf("Caso: 1 >= 2 (linea 6, col 3)\n");
    left  = ast_number_literal_create(1.0, 6, 1);
    right = ast_number_literal_create(2.0, 6, 6);
    BinaryOperationNode *ge = ast_binary_create(OP_GREATER_EQUAL, (ASTNode *)left, (ASTNode *)right, 6, 3);
    CU_ASSERT_PTR_NOT_NULL(ge);
    CU_ASSERT_EQUAL(ge->operator, OP_GREATER_EQUAL);
    printf("operator = %d (esperado: %d)\n", ge->operator, OP_GREATER_EQUAL);
    free(left); free(right); free(ge);

    global_tables_destroy();
}

void test_create_binary_string(void)
{
    printf("\n\n------------ Inicio test: test_create_binary_string ------------\n");
    global_tables_init();

    printf("Caso: \"Hello\" @ \"World\" (linea 1, col 8)\n");
    LiteralNode *left  = ast_string_literal_create("Hello", 1, 1);
    LiteralNode *right = ast_string_literal_create("World", 1, 10);
    BinaryOperationNode *concat = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 1, 8);

    CU_ASSERT_PTR_NOT_NULL(concat);
    CU_ASSERT_EQUAL(concat->operator, OP_CONCAT);
    printf("operator = %d (esperado: %d)\n", concat->operator, OP_CONCAT);

    free(left->value.string_value); free(left);
    free(right->value.string_value); free(right);
    free(concat);

    printf("Caso: \"Hello\" @@ \"World\" (linea 2, col 8)\n");
    left  = ast_string_literal_create("Hello", 2, 1);
    right = ast_string_literal_create("World", 2, 11);
    BinaryOperationNode *concat_ws = ast_binary_create(OP_CONCAT_WS, (ASTNode *)left, (ASTNode *)right, 2, 9);
    CU_ASSERT_PTR_NOT_NULL(concat_ws);
    CU_ASSERT_EQUAL(concat_ws->operator, OP_CONCAT_WS);
    printf("operator = %d (esperado: %d)\n", concat_ws->operator, OP_CONCAT_WS);

    free(left->value.string_value); free(left);
    free(right->value.string_value); free(right);
    free(concat_ws);

    global_tables_destroy();
}

void test_create_binary_nested(void)
{
    printf("\n\n------------ Inicio test: test_create_binary_nested ------------\n");
    global_tables_init();

    printf("Caso: (1 + 2) * (3 + 4) (linea 1)\n");
    LiteralNode *a = ast_number_literal_create(1.0, 1, 2);
    LiteralNode *b = ast_number_literal_create(2.0, 1, 6);
    LiteralNode *c = ast_number_literal_create(3.0, 1, 11);
    LiteralNode *d = ast_number_literal_create(4.0, 1, 15);

    BinaryOperationNode *add1 = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 1, 4);
    BinaryOperationNode *add2 = ast_binary_create(OP_ADD, (ASTNode *)c, (ASTNode *)d, 1, 13);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)add1, (ASTNode *)add2, 1, 8);

    CU_ASSERT_PTR_NOT_NULL(mult);
    CU_ASSERT_EQUAL(mult->base.node_type, NODE_BINARY_OPERATION);
    printf("node_type = %d (esperado: %d)\n", mult->base.node_type, NODE_BINARY_OPERATION);

    CU_ASSERT_EQUAL(mult->operator, OP_MULT);
    printf("operator = %d (esperado: %d)\n", mult->operator, OP_MULT);

    CU_ASSERT_PTR_EQUAL(mult->left, (ASTNode *)add1);
    printf("left = %p (esperado: %p)\n", (void *)mult->left, (void *)add1);

    CU_ASSERT_PTR_EQUAL(mult->right, (ASTNode *)add2);
    printf("right = %p (esperado: %p)\n", (void *)mult->right, (void *)add2);

    free(a); free(b); free(c); free(d);
    free(add1); free(add2);
    free(mult);

    global_tables_destroy();
}

void test_create_binary_null_left(void)
{
    printf("\n\n------------ Inicio test: test_create_binary_null_left ------------\n");
    global_tables_init();

    printf("Caso: left=NULL -> debe retornar NULL\n");
    LiteralNode *right = ast_number_literal_create(2.0, 1, 5);
    BinaryOperationNode *node = ast_binary_create(OP_ADD, NULL, (ASTNode *)right, 1, 3);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(right);
    global_tables_destroy();
}

void test_create_binary_null_right(void)
{
    printf("\n\n------------ Inicio test: test_create_binary_null_right ------------\n");
    global_tables_init();

    printf("Caso: right=NULL -> debe retornar NULL\n");
    LiteralNode *left = ast_number_literal_create(1.0, 1, 1);
    BinaryOperationNode *node = ast_binary_create(OP_ADD, (ASTNode *)left, NULL, 1, 3);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(left);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de BinaryOperationNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BinaryOperationNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_binary_arithmetic", test_create_binary_arithmetic);
    CU_add_test(suite, "test_create_binary_logical", test_create_binary_logical);
    CU_add_test(suite, "test_create_binary_comparison", test_create_binary_comparison);
    CU_add_test(suite, "test_create_binary_string", test_create_binary_string);
    CU_add_test(suite, "test_create_binary_nested", test_create_binary_nested);
    CU_add_test(suite, "test_create_binary_null_left", test_create_binary_null_left);
    CU_add_test(suite, "test_create_binary_null_right", test_create_binary_null_right);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}