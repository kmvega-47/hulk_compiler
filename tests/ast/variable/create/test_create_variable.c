#include <CUnit/Basic.h>
#include "ast.h"

void test_create_variable_reference(void)
{
    printf("\n\n------------ Inicio test: test_create_variable_reference ------------\n");
    global_tables_init();

    printf("Caso: x (linea 10, columna 4)\n");

    VariableReferenceNode *node = ast_variable_reference_create("x", 10, 4);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_VARIABLE_REFERENCE);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_VARIABLE_REFERENCE);

    CU_ASSERT_EQUAL(node->base.line, 10);
    printf("line = %d (esperado: 10)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 4);
    printf("column = %d (esperado: 4)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_STRING_EQUAL(node->name, "x");
    printf("name = '%s' (esperado: 'x')\n", node->name);

    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_variable_reference_long_name(void)
{
    printf("\n\n------------ Inicio test: test_create_variable_reference_long_name ------------\n");
    global_tables_init();

    const char *long_name = "a_very_long_variable_name_that_exceeds_usual_length";
    printf("Caso: %s (linea 5, columna 2)\n", long_name);

    VariableReferenceNode *node = ast_variable_reference_create(long_name, 5, 2);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_VARIABLE_REFERENCE);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_VARIABLE_REFERENCE);

    CU_ASSERT_EQUAL(node->base.line, 5);
    printf("line = %d (esperado: 5)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 2);
    printf("column = %d (esperado: 2)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_STRING_EQUAL(node->name, long_name);
    printf("name length = %zu (esperado: %zu)\n", strlen(node->name), strlen(long_name));

    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_variable_reference_null_name(void)
{
    printf("\n\n------------ Inicio test: test_create_variable_reference_null_name ------------\n");
    global_tables_init();

    printf("Caso: name=NULL (debe retornar NULL)\n");

    VariableReferenceNode *node = ast_variable_reference_create(NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de VariableReferenceNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("VariableReferenceNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_variable_reference", test_create_variable_reference);
    CU_add_test(suite, "test_create_variable_reference_long_name", test_create_variable_reference_long_name);
    CU_add_test(suite, "test_create_variable_reference_null_name", test_create_variable_reference_null_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}