#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_binary_arithmetic(void)
{
    printf("\n\n------------ Inicio test: test_free_binary_arithmetic ------------\n");
    global_tables_init();

    printf("Caso: 1 + 2\n");
    LiteralNode *left = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 5);
    BinaryOperationNode *node = ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 1, 3);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 10 - 4\n");
    left = ast_number_literal_create(10.0, 2, 1);
    right = ast_number_literal_create(4.0, 2, 5);
    node = ast_binary_create(OP_SUB, (ASTNode *)left, (ASTNode *)right, 2, 3);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 6 * 7\n");
    left = ast_number_literal_create(6.0, 3, 1);
    right = ast_number_literal_create(7.0, 3, 5);
    node = ast_binary_create(OP_MULT, (ASTNode *)left, (ASTNode *)right, 3, 3);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 8 / 2\n");
    left = ast_number_literal_create(8.0, 4, 1);
    right = ast_number_literal_create(2.0, 4, 5);
    node = ast_binary_create(OP_DIV, (ASTNode *)left, (ASTNode *)right, 4, 3);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 10 %% 3\n");
    left = ast_number_literal_create(10.0, 5, 1);
    right = ast_number_literal_create(3.0, 5, 6);
    node = ast_binary_create(OP_MOD, (ASTNode *)left, (ASTNode *)right, 5, 4);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 2 ^ 3\n");
    left = ast_number_literal_create(2.0, 6, 1);
    right = ast_number_literal_create(3.0, 6, 5);
    node = ast_binary_create(OP_EXP, (ASTNode *)left, (ASTNode *)right, 6, 3);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_binary_logical(void)
{
    printf("\n\n------------ Inicio test: test_free_binary_logical ------------\n");
    global_tables_init();

    printf("Caso: true & false\n");
    LiteralNode *left = ast_bool_literal_create(true, 1, 1);
    LiteralNode *right = ast_bool_literal_create(false, 1, 8);
    BinaryOperationNode *node = ast_binary_create(OP_AND, (ASTNode *)left, (ASTNode *)right, 1, 6);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: true | false\n");
    left = ast_bool_literal_create(true, 2, 1);
    right = ast_bool_literal_create(false, 2, 9);
    node = ast_binary_create(OP_OR, (ASTNode *)left, (ASTNode *)right, 2, 7);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_binary_comparison(void)
{
    printf("\n\n------------ Inicio test: test_free_binary_comparison ------------\n");
    global_tables_init();

    printf("Caso: 1 == 2\n");
    LiteralNode *left = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 6);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 1, 3);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 1 < 2\n");
    left = ast_number_literal_create(1.0, 2, 1);
    right = ast_number_literal_create(2.0, 2, 5);
    node = ast_binary_create(OP_LESS, (ASTNode *)left, (ASTNode *)right, 2, 3);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_binary_string(void)
{
    printf("\n\n------------ Inicio test: test_free_binary_string ------------\n");
    global_tables_init();

    printf("Caso: \"Hello\" @ \"World\"\n");
    LiteralNode *left = ast_string_literal_create("Hello", 1, 1);
    LiteralNode *right = ast_string_literal_create("World", 1, 10);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 1, 8);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: \"Hello\" @@ \"World\"\n");
    left = ast_string_literal_create("Hello", 2, 1);
    right = ast_string_literal_create("World", 2, 11);
    node = ast_binary_create(OP_CONCAT_WS, (ASTNode *)left, (ASTNode *)right, 2, 9);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_binary_nested(void)
{
    printf("\n\n------------ Inicio test: test_free_binary_nested ------------\n");
    global_tables_init();

    printf("Caso: (1+2)*(3+4)\n");
    LiteralNode *a = ast_number_literal_create(1.0, 1, 2);
    LiteralNode *b = ast_number_literal_create(2.0, 1, 6);
    LiteralNode *c = ast_number_literal_create(3.0, 1, 11);
    LiteralNode *d = ast_number_literal_create(4.0, 1, 15);
    BinaryOperationNode *add1 = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 1, 4);
    BinaryOperationNode *add2 = ast_binary_create(OP_ADD, (ASTNode *)c, (ASTNode *)d, 1, 13);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)add1, (ASTNode *)add2, 1, 8);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de BinaryOperationNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BinaryOperationNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_binary_arithmetic", test_free_binary_arithmetic);
    CU_add_test(suite, "test_free_binary_logical", test_free_binary_logical);
    CU_add_test(suite, "test_free_binary_comparison", test_free_binary_comparison);
    CU_add_test(suite, "test_free_binary_string", test_free_binary_string);
    CU_add_test(suite, "test_free_binary_nested", test_free_binary_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}