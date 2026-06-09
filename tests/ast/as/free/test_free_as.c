#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_as(void)
{
    printf("\n\n------------ Inicio test: test_free_as ------------\n");
    global_tables_init();

    printf("Caso: x as Point\n");
    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 3, 1);
    AsNode *node = ast_as_create(target, "Point", 3, 3);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_as_complex_target(void)
{
    printf("\n\n------------ Inicio test: test_free_as_complex_target ------------\n");
    global_tables_init();

    printf("Caso: self.name as String\n");
    ASTNode *inner = (ASTNode *)ast_variable_reference_create("self", 8, 1);
    ASTNode *target = (ASTNode *)ast_attribute_access_create(inner, "name", 8, 6);
    AsNode *node = ast_as_create(target, "String", 8, 11);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de AsNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AsNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_as", test_free_as);
    CU_add_test(suite, "test_free_as_complex_target", test_free_as_complex_target);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}