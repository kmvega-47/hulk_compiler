#include <CUnit/Basic.h>
#include "ast.h"

void test_create_reassignment_variable(void)
{
    printf("\n\n------------ Inicio test: test_create_reassignment_variable ------------\n");
    global_tables_init();

    printf("Caso: x := 42 (linea 5, columna 3)\n");

    VariableReferenceNode *target = ast_variable_reference_create("x", 5, 1);
    LiteralNode *value = ast_number_literal_create(42.0, 5, 6);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, (ASTNode *)value, 5, 3);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_REASSIGNMENT);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_REASSIGNMENT);

    CU_ASSERT_EQUAL(node->base.line, 5);
    printf("line = %d (esperado: 5)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 3);
    printf("column = %d (esperado: 3)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_PTR_EQUAL(node->target, (ASTNode *)target);
    printf("target = %p (esperado: %p)\n", (void *)node->target, (void *)target);

    CU_ASSERT_PTR_EQUAL(node->value, (ASTNode *)value);
    printf("value = %p (esperado: %p)\n", (void *)node->value, (void *)value);

    free(target->name);
    free(target);
    free(value);
    free(node);
    global_tables_destroy();
}

void test_create_reassignment_bool_value(void)
{
    printf("\n\n------------ Inicio test: test_create_reassignment_bool_value ------------\n");
    global_tables_init();

    printf("Caso: flag := true (linea 1, columna 6)\n");

    VariableReferenceNode *target = ast_variable_reference_create("flag", 1, 1);
    LiteralNode *value = ast_bool_literal_create(true, 1, 9);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, (ASTNode *)value, 1, 6);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_REASSIGNMENT);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_REASSIGNMENT);

    CU_ASSERT_PTR_EQUAL(node->target, (ASTNode *)target);
    printf("target = %p (esperado: %p)\n", (void *)node->target, (void *)target);

    CU_ASSERT_PTR_EQUAL(node->value, (ASTNode *)value);
    printf("value = %p (esperado: %p)\n", (void *)node->value, (void *)value);

    CU_ASSERT_EQUAL(((LiteralNode *)node->value)->value.bool_value, true);
    printf("value.bool_value = %s (esperado: true)\n",
           ((LiteralNode *)node->value)->value.bool_value ? "true" : "false");

    free(target->name);
    free(target);
    free(value);
    free(node);
    global_tables_destroy();
}

void test_create_reassignment_complex_value(void)
{
    printf("\n\n------------ Inicio test: test_create_reassignment_complex_value ------------\n");
    global_tables_init();

    printf("Caso: result := 2 + 3 (linea 3, columna 8)\n");

    VariableReferenceNode *target = ast_variable_reference_create("result", 3, 1);
    LiteralNode *left = ast_number_literal_create(2.0, 3, 11);
    LiteralNode *right = ast_number_literal_create(3.0, 3, 15);
    BinaryOperationNode *value = ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 3, 13);

    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, (ASTNode *)value, 3, 8);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_REASSIGNMENT);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_REASSIGNMENT);

    CU_ASSERT_EQUAL(node->base.line, 3);
    printf("line = %d (esperado: 3)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 8);
    printf("column = %d (esperado: 8)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->target, (ASTNode *)target);
    printf("target = %p (esperado: %p)\n", (void *)node->target, (void *)target);

    CU_ASSERT_PTR_EQUAL(node->value, (ASTNode *)value);
    printf("value = %p (esperado: %p)\n", (void *)node->value, (void *)value);

    free(target->name);
    free(target);
    free(left);
    free(right);
    free(value);
    free(node);
    global_tables_destroy();
}

void test_create_reassignment_null_target(void)
{
    printf("\n\n------------ Inicio test: test_create_reassignment_null_target ------------\n");
    global_tables_init();

    printf("Caso: target=NULL -> debe retornar NULL\n");

    LiteralNode *value = ast_number_literal_create(1.0, 1, 1);
    ReassignmentNode *node = ast_reassignment_create(NULL, (ASTNode *)value, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(value);
    global_tables_destroy();
}

void test_create_reassignment_null_value(void)
{
    printf("\n\n------------ Inicio test: test_create_reassignment_null_value ------------\n");
    global_tables_init();

    printf("Caso: value=NULL -> debe retornar NULL\n");

    VariableReferenceNode *target = ast_variable_reference_create("x", 1, 1);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(target->name);
    free(target);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ReassignmentNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ReassignmentNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_reassignment_variable", test_create_reassignment_variable);
    CU_add_test(suite, "test_create_reassignment_bool_value", test_create_reassignment_bool_value);
    CU_add_test(suite, "test_create_reassignment_complex_value", test_create_reassignment_complex_value);
    CU_add_test(suite, "test_create_reassignment_null_target", test_create_reassignment_null_target);
    CU_add_test(suite, "test_create_reassignment_null_value", test_create_reassignment_null_value);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}