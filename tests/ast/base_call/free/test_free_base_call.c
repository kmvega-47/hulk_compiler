#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_base_call_no_args(void)
{
    printf("\n\n------------ Inicio test: test_free_base_call_no_args ------------\n");
    global_tables_init();

    printf("Caso: base()\n");
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *node = ast_base_call_create(args, 5, 9);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_base_call_with_args(void)
{
    printf("\n\n------------ Inicio test: test_free_base_call_with_args ------------\n");
    global_tables_init();

    printf("Caso: base(1, x + 2)\n");
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(1.0, 10, 10);
    list_append(args, arg1);

    LiteralNode *a = ast_number_literal_create(1.0, 10, 14);
    LiteralNode *b = ast_number_literal_create(2.0, 10, 18);
    ASTNode *arg2 = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 10, 16);
    list_append(args, arg2);

    BaseCallNode *node = ast_base_call_create(args, 10, 5);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de BaseCallNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BaseCallNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_base_call_no_args", test_free_base_call_no_args);
    CU_add_test(suite, "test_free_base_call_with_args", test_free_base_call_with_args);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}