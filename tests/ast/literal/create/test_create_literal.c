#include <CUnit/Basic.h>
#include "ast.h"

void test_create_literal_number(void)
{
    printf("\n\n------------ Inicio test: test_create_literal_number ------------\n");
    global_tables_init();

    printf("Creando literal Number entero: 47.0, linea 10, col 4\n");
    LiteralNode *int_node = ast_number_literal_create(47.0, 10, 4);
    CU_ASSERT_PTR_NOT_NULL(int_node);

    CU_ASSERT_EQUAL(int_node->base.node_type, NODE_LITERAL);
    printf("node_type = %d (esperado: %d)\n", int_node->base.node_type, NODE_LITERAL);

    CU_ASSERT_PTR_EQUAL(int_node->base.return_type,
                        type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("return_type = '%s' (esperado: 'Number')\n", int_node->base.return_type->name);

    CU_ASSERT_EQUAL(int_node->base.line, 10);
    printf("line = %d (esperado: 10)\n", int_node->base.line);

    CU_ASSERT_EQUAL(int_node->base.column, 4);
    printf("column = %d (esperado: 4)\n", int_node->base.column);

    CU_ASSERT_DOUBLE_EQUAL(int_node->value.number_value, 47.0, 0.001);
    printf("value = %g (esperado: 47)\n", int_node->value.number_value);

    free(int_node);

    printf("Creando literal Number decimal: 3.1416, linea 15, col 2\n");
    LiteralNode *dec_node = ast_number_literal_create(3.1416, 15, 2);
    CU_ASSERT_PTR_NOT_NULL(dec_node);

    CU_ASSERT_EQUAL(dec_node->base.node_type, NODE_LITERAL);
    CU_ASSERT_PTR_EQUAL(dec_node->base.return_type,
                        type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_EQUAL(dec_node->base.line, 15);
    CU_ASSERT_EQUAL(dec_node->base.column, 2);
    CU_ASSERT_DOUBLE_EQUAL(dec_node->value.number_value, 3.1416, 0.0001);
    printf("value = %g (esperado: 3.1416)\n", dec_node->value.number_value);

    free(dec_node);
    global_tables_destroy();
}

void test_create_literal_bool(void)
{
    printf("\n\n------------ Inicio test: test_create_literal_bool ------------\n");
    global_tables_init();

    printf("Creando literal Bool: true, linea 5, col 3\n");
    LiteralNode *true_node = ast_bool_literal_create(true, 5, 3);
    CU_ASSERT_PTR_NOT_NULL(true_node);

    CU_ASSERT_EQUAL(true_node->base.node_type, NODE_LITERAL);
    printf("node_type = %d (esperado: %d)\n", true_node->base.node_type, NODE_LITERAL);

    CU_ASSERT_PTR_EQUAL(true_node->base.return_type,
                        type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    printf("return_type = '%s' (esperado: 'Bool')\n", true_node->base.return_type->name);

    CU_ASSERT_EQUAL(true_node->base.line, 5);
    printf("line = %d (esperado: 5)\n", true_node->base.line);

    CU_ASSERT_EQUAL(true_node->base.column, 3);
    printf("column = %d (esperado: 3)\n", true_node->base.column);

    CU_ASSERT_EQUAL(true_node->value.bool_value, true);
    printf("value = %s (esperado: true)\n", true_node->value.bool_value ? "true" : "false");

    free(true_node);

    printf("Creando literal Bool: false, linea 8, col 7\n");
    LiteralNode *false_node = ast_bool_literal_create(false, 8, 7);
    CU_ASSERT_PTR_NOT_NULL(false_node);

    CU_ASSERT_EQUAL(false_node->base.node_type, NODE_LITERAL);
    CU_ASSERT_PTR_EQUAL(false_node->base.return_type,
                        type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_EQUAL(false_node->base.line, 8);
    CU_ASSERT_EQUAL(false_node->base.column, 7);
    CU_ASSERT_EQUAL(false_node->value.bool_value, false);
    printf("value = %s (esperado: false)\n", false_node->value.bool_value ? "true" : "false");

    free(false_node);
    global_tables_destroy();
}

void test_create_literal_string(void)
{
    printf("\n\n------------ Inicio test: test_create_literal_string ------------\n");
    global_tables_init();

    printf("Creando literal String: \"Hola mundo\", linea 3, col 1\n");
    LiteralNode *str_node = ast_string_literal_create("Hola mundo", 3, 1);
    CU_ASSERT_PTR_NOT_NULL(str_node);

    CU_ASSERT_EQUAL(str_node->base.node_type, NODE_LITERAL);
    printf("node_type = %d (esperado: %d)\n", str_node->base.node_type, NODE_LITERAL);

    CU_ASSERT_PTR_EQUAL(str_node->base.return_type,
                        type_table_lookup_by_tag(global_type_table, HULK_STRING));
    printf("return_type = '%s' (esperado: 'String')\n", str_node->base.return_type->name);

    CU_ASSERT_EQUAL(str_node->base.line, 3);
    printf("line = %d (esperado: 3)\n", str_node->base.line);

    CU_ASSERT_EQUAL(str_node->base.column, 1);
    printf("column = %d (esperado: 1)\n", str_node->base.column);

    CU_ASSERT_STRING_EQUAL(str_node->value.string_value, "Hola mundo");
    printf("value = '%s' (esperado: 'Hola mundo')\n", str_node->value.string_value);

    free(str_node->value.string_value);
    free(str_node);
    global_tables_destroy();
}

void test_create_literal_string_empty(void)
{
    printf("\n\n------------ Inicio test: test_create_literal_string_empty ------------\n");
    global_tables_init();

    printf("Creando literal String vacio: \"\", linea 1, col 1\n");
    LiteralNode *empty_node = ast_string_literal_create("", 1, 1);
    CU_ASSERT_PTR_NOT_NULL(empty_node);

    CU_ASSERT_EQUAL(empty_node->base.node_type, NODE_LITERAL);
    CU_ASSERT_PTR_EQUAL(empty_node->base.return_type,
                        type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_EQUAL(empty_node->base.line, 1);
    CU_ASSERT_EQUAL(empty_node->base.column, 1);
    CU_ASSERT_STRING_EQUAL(empty_node->value.string_value, "");
    printf("value = '%s' (esperado: '')\n", empty_node->value.string_value);

    free(empty_node->value.string_value);
    free(empty_node);
    global_tables_destroy();
}

void test_create_literal_string_long(void)
{
    printf("\n\n------------ Inicio test: test_create_literal_string_long ------------\n");
    global_tables_init();

    printf("Creando literal String largo\n");
    const char *long_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi "
                            "ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit "
                            "in voluptate velit esse cillum dolore eu fugiat nulla pariatur.";

    LiteralNode *long_node = ast_string_literal_create(long_text, 6, 3);
    CU_ASSERT_PTR_NOT_NULL(long_node);

    CU_ASSERT_EQUAL(long_node->base.node_type, NODE_LITERAL);
    CU_ASSERT_PTR_EQUAL(long_node->base.return_type,
                        type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_EQUAL(long_node->base.line, 6);
    CU_ASSERT_EQUAL(long_node->base.column, 3);
    CU_ASSERT_STRING_EQUAL(long_node->value.string_value, long_text);
    printf("value length = %zu (esperado: %zu)\n",
           strlen(long_node->value.string_value), strlen(long_text));

    free(long_node->value.string_value);
    free(long_node);
    global_tables_destroy();
}

void test_create_literal_string_null(void)
{
    printf("\n\n------------ Inicio test: test_create_literal_string_null ------------\n");
    printf("Creando literal String con NULL\n");

    LiteralNode *node = ast_string_literal_create(NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("node = %p (esperado: NULL)\n", (void *)node);
}

int main(void)
{
    printf("\n=== Iniciando tests de LiteralNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LiteralNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_literal_number", test_create_literal_number);
    CU_add_test(suite, "test_create_literal_bool", test_create_literal_bool);
    CU_add_test(suite, "test_create_literal_string", test_create_literal_string);
    CU_add_test(suite, "test_create_literal_string_empty", test_create_literal_string_empty);
    CU_add_test(suite, "test_create_literal_string_long", test_create_literal_string_long);
    CU_add_test(suite, "test_create_literal_string_null", test_create_literal_string_null);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}