#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_method_access_no_args(void)
{
    printf("\n\n------------ Inicio test: test_free_method_access_no_args ------------\n");
    global_tables_init();

    printf("Caso: obj.foo() (linea 1, columna 5)\n");
    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 1, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, "foo", args, 1, 5);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_method_access_with_args(void)
{
    printf("\n\n------------ Inicio test: test_free_method_access_with_args ------------\n");
    global_tables_init();

    printf("Caso: obj.calc(1, x+2) (linea 2, columna 5)\n");
    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 2, 1);
    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(1.0, 2, 10);
    ASTNode *a = (ASTNode *)ast_number_literal_create(1.0, 2, 14);
    ASTNode *b = (ASTNode *)ast_number_literal_create(2.0, 2, 18);
    ASTNode *arg2 = (ASTNode *)ast_binary_create(OP_ADD, a, b, 2, 16);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);
    MethodAccessNode *node = ast_method_access_create(target, "calc", args, 2, 5);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_method_access_long_name(void)
{
    printf("\n\n------------ Inicio test: test_free_method_access_long_name ------------\n");
    global_tables_init();

    const char *long_name = "very_long_method_name_for_testing_purposes";
    printf("Caso: obj.%s() (linea 3, columna 5)\n", long_name);
    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 3, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, long_name, args, 3, 5);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de MethodAccessNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("MethodAccessNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_method_access_no_args", test_free_method_access_no_args);
    CU_add_test(suite, "test_free_method_access_with_args", test_free_method_access_with_args);
    CU_add_test(suite, "test_free_method_access_long_name", test_free_method_access_long_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}