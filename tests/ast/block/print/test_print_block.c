#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_block_empty(void)
{
    printf("\n\n------------ Inicio test: test_print_block_empty ------------\n");
    global_tables_init();

    printf("Caso: { } (linea 1, columna 1)\n");
    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_block_single(void)
{
    printf("\n\n------------ Inicio test: test_print_block_single ------------\n");
    global_tables_init();

    printf("Caso: { 42 } (linea 2, columna 1)\n");
    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_number_literal_create(42.0, 2, 3));
    ExpressionBlockNode *node = ast_block_create(exprs, 2, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_block_multiple(void)
{
    printf("\n\n------------ Inicio test: test_print_block_multiple ------------\n");
    global_tables_init();

    printf("Caso: { 1+2; !true; \"hello\" } (linea 3, columna 1)\n");
    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_binary_create(OP_ADD,
                 (ASTNode *)ast_number_literal_create(1.0, 3, 4),
                 (ASTNode *)ast_number_literal_create(2.0, 3, 6), 3, 5));
    list_append(exprs, ast_unary_create(OP_NOT,
                 (ASTNode *)ast_bool_literal_create(true, 3, 10), 3, 9));
    list_append(exprs, ast_string_literal_create("hello", 3, 15));
    ExpressionBlockNode *node = ast_block_create(exprs, 3, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_block_nested(void)
{
    printf("\n\n------------ Inicio test: test_print_block_nested ------------\n");
    global_tables_init();

    printf("Caso: { { 1; true; \"hello\" } } (linea 4, columna 1)\n");
    List *inner = list_create(0, NULL, NULL, NULL, NULL);
    list_append(inner, ast_number_literal_create(1.0, 4, 5));
    list_append(inner, ast_bool_literal_create(true, 4, 8));
    list_append(inner, ast_string_literal_create("hello", 4, 14));
    ExpressionBlockNode *inner_block = ast_block_create(inner, 4, 3);

    List *outer = list_create(0, NULL, NULL, NULL, NULL);
    list_append(outer, inner_block);
    ExpressionBlockNode *outer_block = ast_block_create(outer, 4, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)outer_block, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_block, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ExpressionBlockNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ExpressionBlockNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_block_empty", test_print_block_empty);
    CU_add_test(suite, "test_print_block_single", test_print_block_single);
    CU_add_test(suite, "test_print_block_multiple", test_print_block_multiple);
    CU_add_test(suite, "test_print_block_nested", test_print_block_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}