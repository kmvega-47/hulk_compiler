#include <CUnit/Basic.h>
#include "ast.h"

void test_create_base_call_no_args(void)
{
    printf("\n\n------------ Inicio test: test_create_base_call_no_args ------------\n");
    global_tables_init();

    printf("Caso: base() (linea 5, columna 9)\n");
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *node = ast_base_call_create(args, 5, 9);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_BASE_CALL);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_BASE_CALL);

    CU_ASSERT_EQUAL(node->base.line, 5);
    printf("line = %d (esperado: 5)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 9);
    printf("column = %d (esperado: 9)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_PTR_NULL(node->method_name);
    printf("method_name = %p (esperado: NULL)\n", (void *)node->method_name);

    CU_ASSERT_PTR_NULL(node->type_name);
    printf("type_name = %p (esperado: NULL)\n", (void *)node->type_name);

    CU_ASSERT_PTR_NOT_NULL(node->args);
    CU_ASSERT_EQUAL(list_count(node->args), 0);
    printf("args count = %zu (esperado: 0)\n", list_count(node->args));

    list_destroy(node->args);
    free(node);
    global_tables_destroy();
}

void test_create_base_call_with_args(void)
{
    printf("\n\n------------ Inicio test: test_create_base_call_with_args ------------\n");
    global_tables_init();

    printf("Caso: base(1, x + 2) (linea 10, columna 5)\n");
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(1.0, 10, 10);
    list_append(args, arg1);

    LiteralNode *a = ast_number_literal_create(1.0, 10, 14);
    LiteralNode *b = ast_number_literal_create(2.0, 10, 18);
    ASTNode *arg2 = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 10, 16);
    list_append(args, arg2);

    BaseCallNode *node = ast_base_call_create(args, 10, 5);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_PTR_NULL(node->method_name);
    printf("method_name = %p (esperado: NULL)\n", (void *)node->method_name);

    CU_ASSERT_PTR_NULL(node->type_name);
    printf("type_name = %p (esperado: NULL)\n", (void *)node->type_name);

    CU_ASSERT_EQUAL(list_count(node->args), 2);
    printf("args count = %zu (esperado: 2)\n", list_count(node->args));

    free(arg1);
    free(a); free(b); free(arg2);
    list_destroy(node->args);
    free(node);
    global_tables_destroy();
}

void test_create_base_call_null_args(void)
{
    printf("\n\n------------ Inicio test: test_create_base_call_null_args ------------\n");
    global_tables_init();

    printf("Caso: args=NULL -> debe retornar NULL\n");
    BaseCallNode *node = ast_base_call_create(NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de BaseCallNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BaseCallNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_base_call_no_args", test_create_base_call_no_args);
    CU_add_test(suite, "test_create_base_call_with_args", test_create_base_call_with_args);
    CU_add_test(suite, "test_create_base_call_null_args", test_create_base_call_null_args);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}