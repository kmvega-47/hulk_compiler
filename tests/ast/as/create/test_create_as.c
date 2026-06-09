#include <CUnit/Basic.h>
#include "ast.h"

void test_create_as(void)
{
    printf("\n\n------------ Inicio test: test_create_as ------------\n");
    global_tables_init();

    printf("Caso: x as Point (linea 3, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 3, 1);
    AsNode *node = ast_as_create(target, "Point", 3, 3);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_AS);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_AS);

    CU_ASSERT_EQUAL(node->base.line, 3);
    printf("line = %d (esperado: 3)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 3);
    printf("column = %d (esperado: 3)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_PTR_EQUAL(node->target, target);
    printf("target = %p (esperado: %p)\n", (void *)node->target, (void *)target);

    CU_ASSERT_STRING_EQUAL(node->type_name, "Point");
    printf("type_name = '%s' (esperado: 'Point')\n", node->type_name);

    free(((VariableReferenceNode *)target)->name);
    free(target);
    free(node->type_name);
    free(node);
    global_tables_destroy();
}

void test_create_as_complex_target(void)
{
    printf("\n\n------------ Inicio test: test_create_as_complex_target ------------\n");
    global_tables_init();

    printf("Caso: self.name as String (linea 8, columna 11)\n");

    ASTNode *inner = (ASTNode *)ast_variable_reference_create("self", 8, 1);
    ASTNode *target = (ASTNode *)ast_attribute_access_create(inner, "name", 8, 6);

    AsNode *node = ast_as_create(target, "String", 8, 11);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->type_name, "String");
    printf("type_name = '%s' (esperado: 'String')\n", node->type_name);

    free(((AttributeAccessNode *)target)->attribute_name);
    free(((VariableReferenceNode *)inner)->name);
    free(inner);
    free(target);
    free(node->type_name);
    free(node);
    global_tables_destroy();
}

void test_create_as_null_target(void)
{
    printf("\n\n------------ Inicio test: test_create_as_null_target ------------\n");
    global_tables_init();

    printf("Caso: target=NULL -> debe retornar NULL\n");

    AsNode *node = ast_as_create(NULL, "Point", 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

void test_create_as_null_type_name(void)
{
    printf("\n\n------------ Inicio test: test_create_as_null_type_name ------------\n");
    global_tables_init();

    printf("Caso: type_name=NULL -> debe retornar NULL\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 1, 1);
    AsNode *node = ast_as_create(target, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(((VariableReferenceNode *)target)->name);
    free(target);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de AsNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AsNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_as", test_create_as);
    CU_add_test(suite, "test_create_as_complex_target", test_create_as_complex_target);
    CU_add_test(suite, "test_create_as_null_target", test_create_as_null_target);
    CU_add_test(suite, "test_create_as_null_type_name", test_create_as_null_type_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}