#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_literal_number(void)
{
    printf("\n\n------------ Inicio test: test_print_literal_number ------------\n");
    global_tables_init();

    printf("Caso: 42.0 (linea 10, columna 5)\n");
    LiteralNode *node = ast_number_literal_create(42.0, 10, 5);
    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 3.1416 (linea 15, columna 2)\n");
    node = ast_number_literal_create(3.1416, 15, 2);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_literal_bool(void)
{
    printf("\n\n------------ Inicio test: test_print_literal_bool ------------\n");
    global_tables_init();

    printf("Caso: true (linea 5, columna 1)\n");
    LiteralNode *node = ast_bool_literal_create(true, 5, 1);
    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: false (linea 8, columna 3)\n");
    node = ast_bool_literal_create(false, 8, 3);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_literal_string(void)
{
    printf("\n\n------------ Inicio test: test_print_literal_string ------------\n");
    global_tables_init();

    printf("Caso: \"Hola mundo\" (linea 3, columna 1)\n");
    LiteralNode *node = ast_string_literal_create("Hola mundo", 3, 1);
    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: \"\" (linea 1, columna 4)\n");
    node = ast_string_literal_create("", 1, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: texto largo (linea 6, columna 2)\n");
    const char *long_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
                            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi "
                            "ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit "
                            "in voluptate velit esse cillum dolore eu fugiat nulla pariatur.";
    node = ast_string_literal_create(long_text, 6, 2);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de LiteralNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LiteralNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_literal_number", test_print_literal_number);
    CU_add_test(suite, "test_print_literal_bool", test_print_literal_bool);
    CU_add_test(suite, "test_print_literal_string", test_print_literal_string);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}