#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_unary_not(void)
{
    printf("\n\n------------ Inicio test: test_print_unary_not ------------\n");
    global_tables_init();

    printf("Caso: !true (linea 1, columna 1)\n");
    LiteralNode *operand = ast_bool_literal_create(true, 1, 2);
    UnaryOperationNode *node = ast_unary_create(OP_NOT, (ASTNode *)operand, 1, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_unary_negative(void)
{
    printf("\n\n------------ Inicio test: test_print_unary_negative ------------\n");
    global_tables_init();

    printf("Caso: -42 (linea 2, columna 3)\n");
    LiteralNode *operand = ast_number_literal_create(42.0, 2, 4);
    UnaryOperationNode *node = ast_unary_create(OP_SUB, (ASTNode *)operand, 2, 3);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_unary_nested(void)
{
    printf("\n\n------------ Inicio test: test_print_unary_nested ------------\n");
    global_tables_init();

    printf("Caso: ---47 (linea 3, columna 1)\n");
    LiteralNode *inner = ast_number_literal_create(47.0, 3, 4);
    UnaryOperationNode *neg1 = ast_unary_create(OP_SUB, (ASTNode *)inner, 3, 3);
    UnaryOperationNode *neg2 = ast_unary_create(OP_SUB, (ASTNode *)neg1, 3, 2);
    UnaryOperationNode *neg3 = ast_unary_create(OP_SUB, (ASTNode *)neg2, 3, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)neg3, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)neg3, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de UnaryOperationNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("UnaryOperationNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_unary_not", test_print_unary_not);
    CU_add_test(suite, "test_print_unary_negative", test_print_unary_negative);
    CU_add_test(suite, "test_print_unary_nested", test_print_unary_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}