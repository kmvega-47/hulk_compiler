#include <CUnit/Basic.h>
#include "ast.h"

void test_create_method_access_no_args(void)
{
    printf("\n\n------------ Inicio test: test_create_method_access_no_args ------------\n");
    global_tables_init();

    printf("Caso: obj.foo() (linea 3, columna 5)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 3, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, "foo", args, 3, 5);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_METHOD_ACCESS);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_METHOD_ACCESS);

    CU_ASSERT_EQUAL(node->base.line, 3);
    printf("line = %d (esperado: 3)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 5);
    printf("column = %d (esperado: 5)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_PTR_EQUAL(node->target, target);
    printf("target = %p (esperado: %p)\n", (void *)node->target, (void *)target);

    CU_ASSERT_STRING_EQUAL(node->method_name, "foo");
    printf("method_name = '%s' (esperado: 'foo')\n", node->method_name);

    CU_ASSERT_PTR_NOT_NULL(node->args);
    CU_ASSERT_EQUAL(list_count(node->args), 0);
    printf("args count = %zu (esperado: 0)\n", list_count(node->args));

    free(((VariableReferenceNode *)target)->name);
    free(target);
    free(node->method_name);
    list_destroy(node->args);
    free(node);
    global_tables_destroy();
}

void test_create_method_access_with_args(void)
{
    printf("\n\n------------ Inicio test: test_create_method_access_with_args ------------\n");
    global_tables_init();

    printf("Caso: self.calc(1, x + 2) (linea 4, columna 5)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 4, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(1.0, 4, 11);
    list_append(args, arg1);

    LiteralNode *a = ast_number_literal_create(1.0, 4, 15);
    LiteralNode *b = ast_number_literal_create(2.0, 4, 19);
    ASTNode *arg2 = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 4, 17);
    list_append(args, arg2);

    MethodAccessNode *node = ast_method_access_create(target, "calc", args, 4, 5);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->method_name, "calc");
    printf("method_name = '%s' (esperado: 'calc')\n", node->method_name);

    CU_ASSERT_EQUAL(list_count(node->args), 2);
    printf("args count = %zu (esperado: 2)\n", list_count(node->args));

    free(((VariableReferenceNode *)target)->name);
    free(target);
    free(arg1);
    free(a); free(b); free(arg2);
    list_destroy(node->args);
    free(node->method_name);
    free(node);
    global_tables_destroy();
}

void test_create_method_access_null_target(void)
{
    printf("\n\n------------ Inicio test: test_create_method_access_null_target ------------\n");
    global_tables_init();

    printf("Caso: target=NULL -> debe retornar NULL\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(NULL, "m", args, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(args);
    global_tables_destroy();
}

void test_create_method_access_null_method_name(void)
{
    printf("\n\n------------ Inicio test: test_create_method_access_null_method_name ------------\n");
    global_tables_init();

    printf("Caso: method_name=NULL -> debe retornar NULL\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 1, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, NULL, args, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(((VariableReferenceNode *)target)->name);
    free(target);
    list_destroy(args);
    global_tables_destroy();
}

void test_create_method_access_null_args(void)
{
    printf("\n\n------------ Inicio test: test_create_method_access_null_args ------------\n");
    global_tables_init();

    printf("Caso: args=NULL -> debe retornar NULL\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 1, 1);
    MethodAccessNode *node = ast_method_access_create(target, "m", NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(((VariableReferenceNode *)target)->name);
    free(target);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de MethodAccessNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("MethodAccessNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_method_access_no_args", test_create_method_access_no_args);
    CU_add_test(suite, "test_create_method_access_with_args", test_create_method_access_with_args);
    CU_add_test(suite, "test_create_method_access_null_target", test_create_method_access_null_target);
    CU_add_test(suite, "test_create_method_access_null_method_name", test_create_method_access_null_method_name);
    CU_add_test(suite, "test_create_method_access_null_args", test_create_method_access_null_args);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}