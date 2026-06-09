#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_literal_number(void)
{
    printf("\n\n------------ Inicio test: test_free_literal_number ------------\n");
    global_tables_init();

    printf("Caso: 42.0 (linea 1, columna 1)\n");
    LiteralNode *node = ast_number_literal_create(42.0, 1, 1);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 3.1416 (linea 2, columna 5)\n");
    node = ast_number_literal_create(3.1416, 2, 5);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_literal_bool(void)
{
    printf("\n\n------------ Inicio test: test_free_literal_bool ------------\n");
    global_tables_init();

    printf("Caso: true (linea 1, columna 3)\n");
    LiteralNode *node = ast_bool_literal_create(true, 1, 3);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: false (linea 4, columna 1)\n");
    node = ast_bool_literal_create(false, 4, 1);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_literal_string(void)
{
    printf("\n\n------------ Inicio test: test_free_literal_string ------------\n");
    global_tables_init();

    printf("Caso: \"Hola mundo\" (linea 2, columna 7)\n");
    LiteralNode *node = ast_string_literal_create("Hola mundo", 2, 7);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: \"\" (linea 5, columna 3)\n");
    node = ast_string_literal_create("", 5, 3);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: texto largo (linea 10, columna 1)\n");
    const char *long_text = "Lorem ipsum dolor sit amet...";
    node = ast_string_literal_create(long_text, 10, 1);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de LiteralNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LiteralNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_literal_number", test_free_literal_number);
    CU_add_test(suite, "test_free_literal_bool", test_free_literal_bool);
    CU_add_test(suite, "test_free_literal_string", test_free_literal_string);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}