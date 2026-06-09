#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_reassignment_complex_value(void)
{
    printf("\n\n------------ Inicio test: test_print_reassignment_complex_value ------------\n");
    global_tables_init();

    printf("Caso: x := (2 + 7) * 5 (linea 10, columna 1)\n");
    VariableReferenceNode *target = ast_variable_reference_create("x", 10, 1);
    ASTNode *two   = (ASTNode *)ast_number_literal_create(2.0, 10, 5);
    ASTNode *seven = (ASTNode *)ast_number_literal_create(7.0, 10, 9);
    ASTNode *five  = (ASTNode *)ast_number_literal_create(5.0, 10, 15);
    ASTNode *add   = (ASTNode *)ast_binary_create(OP_ADD, two, seven, 10, 7);
    ASTNode *mul   = (ASTNode *)ast_binary_create(OP_MULT, add, five, 10, 12);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, mul, 10, 3);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_reassignment_long_name(void)
{
    printf("\n\n------------ Inicio test: test_print_reassignment_long_name ------------\n");
    global_tables_init();

    const char *long_name = "very_very_long_variable_name_for_reassignment";
    printf("Caso: %s := 42 (linea 5, columna 1)\n", long_name);
    VariableReferenceNode *target = ast_variable_reference_create(long_name, 5, 1);
    ASTNode *val = (ASTNode *)ast_number_literal_create(42.0, 5, strlen(long_name) + 5);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, val, 5, 3);

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
    printf("\n=== Iniciando tests de ReassignmentNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ReassignmentNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_reassignment_complex_value", test_print_reassignment_complex_value);
    CU_add_test(suite, "test_print_reassignment_long_name", test_print_reassignment_long_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}