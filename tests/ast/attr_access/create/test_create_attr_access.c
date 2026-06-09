#include <CUnit/Basic.h>
#include "ast.h"

void test_create_attribute_access_normal(void)
{
    printf("\n\n------------ Inicio test: test_create_attribute_access_normal ------------\n");
    global_tables_init();

    printf("Caso: self.name (linea 5, columna 6)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 5, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, "name", 5, 6);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_ATTRIBUTE_ACCESS);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_ATTRIBUTE_ACCESS);

    CU_ASSERT_EQUAL(node->base.line, 5);
    printf("line = %d (esperado: 5)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 6);
    printf("column = %d (esperado: 6)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_PTR_EQUAL(node->target, target);
    printf("target = %p (esperado: %p)\n", (void *)node->target, (void *)target);

    CU_ASSERT_STRING_EQUAL(node->attribute_name, "name");
    printf("attribute_name = '%s' (esperado: 'name')\n", node->attribute_name);

    free(((VariableReferenceNode *)target)->name);
    free(target);
    free(node->attribute_name);
    free(node);
    global_tables_destroy();
}

void test_create_attribute_access_long_name(void)
{
    printf("\n\n------------ Inicio test: test_create_attribute_access_long_name ------------\n");
    global_tables_init();

    const char *long_name = "very_long_attribute_name_for_testing";
    printf("Caso: self.%s (linea 2, columna 6)\n", long_name);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 2, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, long_name, 2, 6);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_ATTRIBUTE_ACCESS);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_ATTRIBUTE_ACCESS);

    CU_ASSERT_STRING_EQUAL(node->attribute_name, long_name);
    printf("attribute_name = '%s' (esperado: '%s')\n", node->attribute_name, long_name);
    printf("attribute_name length = %zu\n", strlen(node->attribute_name));

    free(((VariableReferenceNode *)target)->name);
    free(target);
    free(node->attribute_name);
    free(node);
    global_tables_destroy();
}

void test_create_attribute_access_null_target(void)
{
    printf("\n\n------------ Inicio test: test_create_attribute_access_null_target ------------\n");
    global_tables_init();

    printf("Caso: target=NULL -> debe retornar NULL\n");

    AttributeAccessNode *node = ast_attribute_access_create(NULL, "x", 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

void test_create_attribute_access_null_name(void)
{
    printf("\n\n------------ Inicio test: test_create_attribute_access_null_name ------------\n");
    global_tables_init();

    printf("Caso: attribute_name=NULL -> debe retornar NULL\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 1, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(((VariableReferenceNode *)target)->name);
    free(target);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de AttributeAccessNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AttributeAccessNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_attribute_access_normal", test_create_attribute_access_normal);
    CU_add_test(suite, "test_create_attribute_access_long_name", test_create_attribute_access_long_name);
    CU_add_test(suite, "test_create_attribute_access_null_target", test_create_attribute_access_null_target);
    CU_add_test(suite, "test_create_attribute_access_null_name", test_create_attribute_access_null_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}