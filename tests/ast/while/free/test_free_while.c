#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_while_single_expression_body(void)
{
    printf("\n\n------------ Inicio test: test_free_while_single_expression_body ------------\n");
    global_tables_init();

    printf("Caso: while (true) 42\n");
    LiteralNode *cond = ast_bool_literal_create(true, 1, 8);
    LiteralNode *body = ast_number_literal_create(42.0, 1, 14);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_while_multiple_expression_body(void)
{
    printf("\n\n------------ Inicio test: test_free_while_multiple_expression_body ------------\n");
    global_tables_init();

    printf("Caso: while (x<10) { 1+2; 3*4; 5-6 }\n");
    LiteralNode *a = ast_number_literal_create(1.0, 1, 14);
    LiteralNode *b = ast_number_literal_create(10.0, 1, 18);
    BinaryOperationNode *cond = ast_binary_create(OP_LESS, (ASTNode *)a, (ASTNode *)b, 1, 16);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, ast_binary_create(OP_ADD,
                 (ASTNode *)ast_number_literal_create(1.0, 2, 5),
                 (ASTNode *)ast_number_literal_create(2.0, 2, 9), 2, 7));
    list_append(body_exprs, ast_binary_create(OP_MULT,
                 (ASTNode *)ast_number_literal_create(3.0, 3, 5),
                 (ASTNode *)ast_number_literal_create(4.0, 3, 9), 3, 7));
    list_append(body_exprs, ast_binary_create(OP_SUB,
                 (ASTNode *)ast_number_literal_create(5.0, 4, 5),
                 (ASTNode *)ast_number_literal_create(6.0, 4, 9), 4, 7));
    ExpressionBlockNode *body_block = ast_block_create(body_exprs, 1, 21);

    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body_block, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_while_complex_condition(void)
{
    printf("\n\n------------ Inicio test: test_free_while_complex_condition ------------\n");
    global_tables_init();

    printf("Caso: while (!(a>b) & (c<=d)) 42\n");
    LiteralNode *a = ast_number_literal_create(1.0, 1, 15);
    LiteralNode *b = ast_number_literal_create(2.0, 1, 19);
    BinaryOperationNode *gt = ast_binary_create(OP_GREATER, (ASTNode *)a, (ASTNode *)b, 1, 17);
    UnaryOperationNode *not_op = ast_unary_create(OP_NOT, (ASTNode *)gt, 1, 14);

    LiteralNode *c = ast_number_literal_create(3.0, 1, 26);
    LiteralNode *d = ast_number_literal_create(4.0, 1, 30);
    BinaryOperationNode *le = ast_binary_create(OP_LESS_EQUAL, (ASTNode *)c, (ASTNode *)d, 1, 28);

    BinaryOperationNode *and_op = ast_binary_create(OP_AND, (ASTNode *)not_op, (ASTNode *)le, 1, 23);

    LiteralNode *body = ast_number_literal_create(42.0, 1, 35);
    WhileLoopNode *node = ast_while_create((ASTNode *)and_op, (ASTNode *)body, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de WhileLoopNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("WhileLoopNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_while_single_expression_body", test_free_while_single_expression_body);
    CU_add_test(suite, "test_free_while_multiple_expression_body", test_free_while_multiple_expression_body);
    CU_add_test(suite, "test_free_while_complex_condition", test_free_while_complex_condition);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}