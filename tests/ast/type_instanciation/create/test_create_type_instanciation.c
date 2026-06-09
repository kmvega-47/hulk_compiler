#include <CUnit/Basic.h>
#include "ast.h"

void test_create_type_instanciation_no_args(void)
{
    printf("\n\n------------ Inicio test: test_create_type_instanciation_no_args ------------\n");
    global_tables_init();

    printf("Caso: new Point() (linea 3, columna 5)\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create("Point", args, 3, 5);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_TYPE_INSTANCIATION);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_TYPE_INSTANCIATION);

    CU_ASSERT_EQUAL(node->base.line, 3);
    printf("line = %d (esperado: 3)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 5);
    printf("column = %d (esperado: 5)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_STRING_EQUAL(node->type_name, "Point");
    printf("type_name = '%s' (esperado: 'Point')\n", node->type_name);

    CU_ASSERT_PTR_NOT_NULL(node->args);
    CU_ASSERT_EQUAL(list_count(node->args), 0);
    printf("args count = %zu (esperado: 0)\n", list_count(node->args));

    list_destroy(node->args);
    free(node->type_name);
    free(node);
    global_tables_destroy();
}

void test_create_type_instanciation_multiple_args(void)
{
    printf("\n\n------------ Inicio test: test_create_type_instanciation_multiple_args ------------\n");
    global_tables_init();

    printf("Caso: new Point(3, 4) (linea 4, columna 1)\n");

    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(3.0, 4, 12);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(4.0, 4, 15);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);

    TypeInstanciationNode *node = ast_type_instanciation_create("Point", args, 4, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->type_name, "Point");
    printf("type_name = '%s' (esperado: 'Point')\n", node->type_name);

    CU_ASSERT_EQUAL(list_count(node->args), 2);
    printf("args count = %zu (esperado: 2)\n", list_count(node->args));

    free(arg1);
    free(arg2);
    list_destroy(node->args);
    free(node->type_name);
    free(node);
    global_tables_destroy();
}

void test_create_type_instanciation_long_name(void)
{
    printf("\n\n------------ Inicio test: test_create_type_instanciation_long_name ------------\n");
    global_tables_init();

    const char *long_name = "VeryLongTypeName_WithUnderscore_1234567890";
    printf("Caso: new %s() (linea 2, columna 8)\n", long_name);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create(long_name, args, 2, 8);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->type_name, long_name);
    printf("type_name = '%s' (esperado: '%s')\n", node->type_name, long_name);
    printf("type_name length = %zu\n", strlen(node->type_name));

    list_destroy(node->args);
    free(node->type_name);
    free(node);
    global_tables_destroy();
}

void test_create_type_instanciation_null_name(void)
{
    printf("\n\n------------ Inicio test: test_create_type_instanciation_null_name ------------\n");
    global_tables_init();

    printf("Caso: type_name=NULL -> debe retornar NULL\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create(NULL, args, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(args);
    global_tables_destroy();
}

void test_create_type_instanciation_null_args(void)
{
    printf("\n\n------------ Inicio test: test_create_type_instanciation_null_args ------------\n");
    global_tables_init();

    printf("Caso: args=NULL -> debe retornar NULL\n");

    TypeInstanciationNode *node = ast_type_instanciation_create("T", NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeInstanciationNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeInstanciationNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_type_instanciation_no_args", test_create_type_instanciation_no_args);
    CU_add_test(suite, "test_create_type_instanciation_multiple_args", test_create_type_instanciation_multiple_args);
    CU_add_test(suite, "test_create_type_instanciation_long_name", test_create_type_instanciation_long_name);
    CU_add_test(suite, "test_create_type_instanciation_null_name", test_create_type_instanciation_null_name);
    CU_add_test(suite, "test_create_type_instanciation_null_args", test_create_type_instanciation_null_args);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}