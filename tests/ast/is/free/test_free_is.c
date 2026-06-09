#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_is(void)
{
    printf("\n\n------------ Inicio test: test_free_is ------------\n");
    global_tables_init();

    printf("Caso: x is Point\n");
    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 3, 1);
    IsNode *node = ast_is_create(target, "Point", 3, 3);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_is_complex_target(void)
{
    printf("\n\n------------ Inicio test: test_free_is_complex_target ------------\n");
    global_tables_init();

    printf("Caso: (a + b) is Number\n");
    LiteralNode *a = ast_number_literal_create(1.0, 5, 2);
    LiteralNode *b = ast_number_literal_create(2.0, 5, 6);
    ASTNode *target = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 5, 4);
    IsNode *node = ast_is_create(target, "Number", 5, 9);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de IsNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("IsNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_is", test_free_is);
    CU_add_test(suite, "test_free_is_complex_target", test_free_is_complex_target);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}