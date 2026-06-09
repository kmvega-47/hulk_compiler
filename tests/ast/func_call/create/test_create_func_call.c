#include <CUnit/Basic.h>
#include "ast.h"

void test_create_function_call_no_args(void)
{
    printf("\n\n------------ Inicio test: test_create_function_call_no_args ------------\n");
    global_tables_init();

    printf("Caso: foo() (linea 3, columna 5)\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    FunctionCallNode *node = ast_function_call_create("foo", args, 3, 5);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_FUNCTION_CALL);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_FUNCTION_CALL);

    CU_ASSERT_EQUAL(node->base.line, 3);
    printf("line = %d (esperado: 3)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 5);
    printf("column = %d (esperado: 5)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_STRING_EQUAL(node->name, "foo");
    printf("name = '%s' (esperado: 'foo')\n", node->name);

    CU_ASSERT_PTR_NOT_NULL(node->args);
    CU_ASSERT_EQUAL(list_count(node->args), 0);
    printf("args count = %zu (esperado: 0)\n", list_count(node->args));

    list_destroy(node->args);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_function_call_with_args(void)
{
    printf("\n\n------------ Inicio test: test_create_function_call_with_args ------------\n");
    global_tables_init();

    printf("Caso: add(1, 2 + 3) (linea 1, columna 1)\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *arg0 = (ASTNode *)ast_number_literal_create(1.0, 1, 5);
    list_append(args, arg0);

    LiteralNode *left = ast_number_literal_create(2.0, 1, 10);
    LiteralNode *right = ast_number_literal_create(3.0, 1, 14);
    ASTNode *arg1 = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 1, 12);
    list_append(args, arg1);

    FunctionCallNode *node = ast_function_call_create("add", args, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "add");
    printf("name = '%s' (esperado: 'add')\n", node->name);

    CU_ASSERT_EQUAL(list_count(node->args), 2);
    printf("args count = %zu (esperado: 2)\n", list_count(node->args));

    free(arg0);
    free(left); free(right);
    free(arg1);
    list_destroy(node->args);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_function_call_null_name(void)
{
    printf("\n\n------------ Inicio test: test_create_function_call_null_name ------------\n");
    global_tables_init();

    printf("Caso: name=NULL -> debe retornar NULL\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    FunctionCallNode *node = ast_function_call_create(NULL, args, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(args);
    global_tables_destroy();
}

void test_create_function_call_null_args(void)
{
    printf("\n\n------------ Inicio test: test_create_function_call_null_args ------------\n");
    global_tables_init();

    printf("Caso: args=NULL -> debe retornar NULL\n");

    FunctionCallNode *node = ast_function_call_create("f", NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de FunctionCallNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionCallNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_function_call_no_args", test_create_function_call_no_args);
    CU_add_test(suite, "test_create_function_call_with_args", test_create_function_call_with_args);
    CU_add_test(suite, "test_create_function_call_null_name", test_create_function_call_null_name);
    CU_add_test(suite, "test_create_function_call_null_args", test_create_function_call_null_args);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}