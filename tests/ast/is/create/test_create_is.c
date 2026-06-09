#include <CUnit/Basic.h>
#include "ast.h"

void test_create_is(void)
{
    printf("\n\n------------ Inicio test: test_create_is ------------\n");
    global_tables_init();

    printf("Caso: x is Point (linea 3, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 3, 1);
    IsNode *node = ast_is_create(target, "Point", 3, 3);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_IS);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_IS);

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

void test_create_is_complex_target(void)
{
    printf("\n\n------------ Inicio test: test_create_is_complex_target ------------\n");
    global_tables_init();

    printf("Caso: (a + b) is Number (linea 5, columna 7)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 5, 2);
    LiteralNode *b = ast_number_literal_create(2.0, 5, 6);
    ASTNode *target = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 5, 4);

    IsNode *node = ast_is_create(target, "Number", 5, 9);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->type_name, "Number");
    printf("type_name = '%s' (esperado: 'Number')\n", node->type_name);

    free(a); free(b);
    free(target);
    free(node->type_name);
    free(node);
    global_tables_destroy();
}

void test_create_is_null_target(void)
{
    printf("\n\n------------ Inicio test: test_create_is_null_target ------------\n");
    global_tables_init();

    printf("Caso: target=NULL -> debe retornar NULL\n");

    IsNode *node = ast_is_create(NULL, "Point", 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

void test_create_is_null_type_name(void)
{
    printf("\n\n------------ Inicio test: test_create_is_null_type_name ------------\n");
    global_tables_init();

    printf("Caso: type_name=NULL -> debe retornar NULL\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 1, 1);
    IsNode *node = ast_is_create(target, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(((VariableReferenceNode *)target)->name);
    free(target);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de IsNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("IsNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_is", test_create_is);
    CU_add_test(suite, "test_create_is_complex_target", test_create_is_complex_target);
    CU_add_test(suite, "test_create_is_null_target", test_create_is_null_target);
    CU_add_test(suite, "test_create_is_null_type_name", test_create_is_null_type_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}