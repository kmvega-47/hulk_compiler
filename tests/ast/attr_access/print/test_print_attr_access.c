#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_attribute_access_normal(void)
{
    printf("\n\n------------ Inicio test: test_print_attribute_access_normal ------------\n");
    global_tables_init();

    printf("Caso: self.name (linea 1, columna 6)\n");
    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 1, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, "name", 1, 6);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_attribute_access_long_name(void)
{
    printf("\n\n------------ Inicio test: test_print_attribute_access_long_name ------------\n");
    global_tables_init();

    const char *long_name = "very_long_attribute_name_for_testing";
    printf("Caso: self.%s (linea 2, columna 6)\n", long_name);
    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 2, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, long_name, 2, 6);

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
    printf("\n=== Iniciando tests de AttributeAccessNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AttributeAccessNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_attribute_access_normal", test_print_attribute_access_normal);
    CU_add_test(suite, "test_print_attribute_access_long_name", test_print_attribute_access_long_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}