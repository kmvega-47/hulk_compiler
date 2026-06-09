#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_function_call_no_args(void)
{
    printf("\n\n------------ Inicio test: test_print_function_call_no_args ------------\n");
    global_tables_init();

    printf("Caso: print() (linea 1, columna 5)\n");
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    FunctionCallNode *node = ast_function_call_create("print", args, 1, 5);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_function_call_single_arg(void)
{
    printf("\n\n------------ Inicio test: test_print_function_call_single_arg ------------\n");
    global_tables_init();

    printf("Caso: print(42) (linea 2, columna 1)\n");
    ASTNode *arg = (ASTNode *)ast_number_literal_create(42.0, 2, 7);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg);
    FunctionCallNode *node = ast_function_call_create("print", args, 2, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_function_call_multiple_args(void)
{
    printf("\n\n------------ Inicio test: test_print_function_call_multiple_args ------------\n");
    global_tables_init();

    printf("Caso: log(2, 64) (linea 3, columna 1)\n");
    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(2.0, 3, 5);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(64.0, 3, 8);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);
    FunctionCallNode *node = ast_function_call_create("log", args, 3, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_function_call_long_name(void)
{
    printf("\n\n------------ Inicio test: test_print_function_call_long_name ------------\n");
    global_tables_init();

    const char *long_name = "very_long_function_name_that_should_be_stored";
    printf("Caso: %s() (linea 4, columna 3)\n", long_name);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    FunctionCallNode *node = ast_function_call_create(long_name, args, 4, 3);

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
    printf("\n=== Iniciando tests de FunctionCallNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionCallNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_function_call_no_args", test_print_function_call_no_args);
    CU_add_test(suite, "test_print_function_call_single_arg", test_print_function_call_single_arg);
    CU_add_test(suite, "test_print_function_call_multiple_args", test_print_function_call_multiple_args);
    CU_add_test(suite, "test_print_function_call_long_name", test_print_function_call_long_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}