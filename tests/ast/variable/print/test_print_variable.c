#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_variable_reference(void)
{
    printf("\n\n------------ Inicio test: test_print_variable_reference ------------\n");
    global_tables_init();

    printf("Caso: x (linea 10, columna 5)\n");
    VariableReferenceNode *node = ast_variable_reference_create("x", 10, 5);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_variable_reference_long_name(void)
{
    printf("\n\n------------ Inicio test: test_print_variable_reference_long_name ------------\n");
    global_tables_init();

    const char *long_name = "a_very_long_variable_name_that_exceeds_usual_length";
    printf("Caso: %s (linea 5, columna 2)\n", long_name);
    VariableReferenceNode *node = ast_variable_reference_create(long_name, 5, 2);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de VariableReferenceNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("VariableReferenceNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_variable_reference", test_print_variable_reference);
    CU_add_test(suite, "test_print_variable_reference_long_name", test_print_variable_reference_long_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}