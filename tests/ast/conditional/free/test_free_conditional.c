#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_conditional_if_without_else(void)
{
    printf("\n\n------------ Inicio test: test_free_conditional_if_without_else ------------\n");
    global_tables_init();

    printf("Caso: if (true) 42\n");
    LiteralNode *cond = ast_bool_literal_create(true, 1, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 12);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, NULL, 1, 1);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_conditional_if_with_else(void)
{
    printf("\n\n------------ Inicio test: test_free_conditional_if_with_else ------------\n");
    global_tables_init();

    printf("Caso: if (true) 42 else 0\n");
    LiteralNode *cond = ast_bool_literal_create(true, 1, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 12);
    LiteralNode *else_branch = ast_number_literal_create(0.0, 1, 20);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 1, 1);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_conditional_if_else_if_else(void)
{
    printf("\n\n------------ Inicio test: test_free_conditional_if_else_if_else ------------\n");
    global_tables_init();

    printf("Caso: if (x<0) -1 else if (x>0) 1 else 0\n");
    LiteralNode *a = ast_number_literal_create(1.0, 1, 9);
    LiteralNode *b = ast_number_literal_create(0.0, 1, 13);
    BinaryOperationNode *cond1 = ast_binary_create(OP_LESS, (ASTNode *)a, (ASTNode *)b, 1, 11);
    LiteralNode *then1 = ast_number_literal_create(-1.0, 1, 16);

    LiteralNode *c = ast_number_literal_create(1.0, 1, 30);
    LiteralNode *d = ast_number_literal_create(0.0, 1, 34);
    BinaryOperationNode *cond2 = ast_binary_create(OP_GREATER, (ASTNode *)c, (ASTNode *)d, 1, 32);
    LiteralNode *then2 = ast_number_literal_create(1.0, 1, 37);

    LiteralNode *else_branch = ast_number_literal_create(0.0, 1, 44);

    ConditionalNode *inner_if = ast_conditional_create((ASTNode *)cond2, (ASTNode *)then2, (ASTNode *)else_branch, 1, 24);
    ConditionalNode *outer_if = ast_conditional_create((ASTNode *)cond1, (ASTNode *)then1, (ASTNode *)inner_if, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_if, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_conditional_complex_condition(void)
{
    printf("\n\n------------ Inicio test: test_free_conditional_complex_condition ------------\n");
    global_tables_init();

    printf("Caso: if (!(a>b) & (c<=d)) 42\n");
    LiteralNode *a = ast_number_literal_create(1.0, 1, 9);
    LiteralNode *b = ast_number_literal_create(2.0, 1, 13);
    BinaryOperationNode *gt = ast_binary_create(OP_GREATER, (ASTNode *)a, (ASTNode *)b, 1, 11);
    UnaryOperationNode *not_op = ast_unary_create(OP_NOT, (ASTNode *)gt, 1, 7);

    LiteralNode *c = ast_number_literal_create(3.0, 1, 20);
    LiteralNode *d = ast_number_literal_create(4.0, 1, 24);
    BinaryOperationNode *le = ast_binary_create(OP_LESS_EQUAL, (ASTNode *)c, (ASTNode *)d, 1, 22);

    BinaryOperationNode *and_op = ast_binary_create(OP_AND, (ASTNode *)not_op, (ASTNode *)le, 1, 16);

    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 29);
    ConditionalNode *node = ast_conditional_create((ASTNode *)and_op, (ASTNode *)then_branch, NULL, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ConditionalNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConditionalNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_conditional_if_without_else", test_free_conditional_if_without_else);
    CU_add_test(suite, "test_free_conditional_if_with_else", test_free_conditional_if_with_else);
    CU_add_test(suite, "test_free_conditional_if_else_if_else", test_free_conditional_if_else_if_else);
    CU_add_test(suite, "test_free_conditional_complex_condition", test_free_conditional_complex_condition);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}