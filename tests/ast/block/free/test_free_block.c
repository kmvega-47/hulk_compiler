#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_block_empty(void)
{
    printf("\n\n------------ Inicio test: test_free_block_empty ------------\n");
    global_tables_init();

    printf("Caso: { }\n");
    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_block_single(void)
{
    printf("\n\n------------ Inicio test: test_free_block_single ------------\n");
    global_tables_init();

    printf("Caso: { 42 }\n");
    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_number_literal_create(42.0, 1, 3));
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_block_multiple(void)
{
    printf("\n\n------------ Inicio test: test_free_block_multiple ------------\n");
    global_tables_init();

    printf("Caso: { 1+2; !true; \"hello\" }\n");
    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_binary_create(OP_ADD,
                 (ASTNode *)ast_number_literal_create(1.0, 1, 3),
                 (ASTNode *)ast_number_literal_create(2.0, 1, 5), 1, 4));
    list_append(exprs, ast_unary_create(OP_NOT,
                 (ASTNode *)ast_bool_literal_create(true, 1, 9), 1, 8));
    list_append(exprs, ast_string_literal_create("hello", 1, 13));
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_block_nested(void)
{
    printf("\n\n------------ Inicio test: test_free_block_nested ------------\n");
    global_tables_init();

    printf("Caso: { { 1; true; \"hello\" } }\n");
    List *inner = list_create(0, NULL, NULL, NULL, NULL);
    list_append(inner, ast_number_literal_create(1.0, 1, 5));
    list_append(inner, ast_bool_literal_create(true, 1, 8));
    list_append(inner, ast_string_literal_create("hello", 1, 14));
    ExpressionBlockNode *inner_block = ast_block_create(inner, 1, 3);

    List *outer = list_create(0, NULL, NULL, NULL, NULL);
    list_append(outer, inner_block);
    ExpressionBlockNode *outer_block = ast_block_create(outer, 1, 1);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_block, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ExpressionBlockNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ExpressionBlockNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_block_empty", test_free_block_empty);
    CU_add_test(suite, "test_free_block_single", test_free_block_single);
    CU_add_test(suite, "test_free_block_multiple", test_free_block_multiple);
    CU_add_test(suite, "test_free_block_nested", test_free_block_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}