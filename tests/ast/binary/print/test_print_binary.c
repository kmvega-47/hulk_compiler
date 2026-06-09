#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_binary_arithmetic(void)
{
    printf("\n\n------------ Inicio test: test_print_binary_arithmetic ------------\n");
    global_tables_init();

    printf("Caso: 1 + 2 (linea 1, columna 3)\n");
    LiteralNode *left = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 5);
    BinaryOperationNode *node = ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 1, 3);
    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 10 - 4 (linea 2, columna 4)\n");
    left  = ast_number_literal_create(10.0, 2, 1);
    right = ast_number_literal_create(4.0, 2, 6);
    node = ast_binary_create(OP_SUB, (ASTNode *)left, (ASTNode *)right, 2, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 6 * 7 (linea 3, columna 4)\n");
    left  = ast_number_literal_create(6.0, 3, 1);
    right = ast_number_literal_create(7.0, 3, 5);
    node = ast_binary_create(OP_MULT, (ASTNode *)left, (ASTNode *)right, 3, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 8 / 2 (linea 4, columna 4)\n");
    left  = ast_number_literal_create(8.0, 4, 1);
    right = ast_number_literal_create(2.0, 4, 5);
    node = ast_binary_create(OP_DIV, (ASTNode *)left, (ASTNode *)right, 4, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 10 %% 3 (linea 5, columna 5)\n");
    left  = ast_number_literal_create(10.0, 5, 1);
    right = ast_number_literal_create(3.0, 5, 8);
    node = ast_binary_create(OP_MOD, (ASTNode *)left, (ASTNode *)right, 5, 5);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 2 ^ 3 (linea 6, columna 4)\n");
    left  = ast_number_literal_create(2.0, 6, 1);
    right = ast_number_literal_create(3.0, 6, 5);
    node = ast_binary_create(OP_EXP, (ASTNode *)left, (ASTNode *)right, 6, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_binary_logical(void)
{
    printf("\n\n------------ Inicio test: test_print_binary_logical ------------\n");
    global_tables_init();

    printf("Caso: true & false (linea 1, columna 6)\n");
    LiteralNode *left = ast_bool_literal_create(true, 1, 1);
    LiteralNode *right = ast_bool_literal_create(false, 1, 8);
    BinaryOperationNode *node = ast_binary_create(OP_AND, (ASTNode *)left, (ASTNode *)right, 1, 6);
    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: true | false (linea 2, columna 6)\n");
    left  = ast_bool_literal_create(true, 2, 1);
    right = ast_bool_literal_create(false, 2, 8);
    node = ast_binary_create(OP_OR, (ASTNode *)left, (ASTNode *)right, 2, 6);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_binary_comparison(void)
{
    printf("\n\n------------ Inicio test: test_print_binary_comparison ------------\n");
    global_tables_init();

    printf("Caso: 1 == 2 (linea 1, columna 4)\n");
    LiteralNode *left = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 6);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 1, 4);
    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 1 != 2 (linea 2, columna 4)\n");
    left  = ast_number_literal_create(1.0, 2, 1);
    right = ast_number_literal_create(2.0, 2, 6);
    node = ast_binary_create(OP_NOT_EQUAL, (ASTNode *)left, (ASTNode *)right, 2, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 1 < 2 (linea 3, columna 4)\n");
    left  = ast_number_literal_create(1.0, 3, 1);
    right = ast_number_literal_create(2.0, 3, 5);
    node = ast_binary_create(OP_LESS, (ASTNode *)left, (ASTNode *)right, 3, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 1 > 2 (linea 4, columna 4)\n");
    left  = ast_number_literal_create(1.0, 4, 1);
    right = ast_number_literal_create(2.0, 4, 5);
    node = ast_binary_create(OP_GREATER, (ASTNode *)left, (ASTNode *)right, 4, 4);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 1 <= 2 (linea 5, columna 5)\n");
    left  = ast_number_literal_create(1.0, 5, 1);
    right = ast_number_literal_create(2.0, 5, 6);
    node = ast_binary_create(OP_LESS_EQUAL, (ASTNode *)left, (ASTNode *)right, 5, 5);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: 1 >= 2 (linea 6, columna 5)\n");
    left  = ast_number_literal_create(1.0, 6, 1);
    right = ast_number_literal_create(2.0, 6, 6);
    node = ast_binary_create(OP_GREATER_EQUAL, (ASTNode *)left, (ASTNode *)right, 6, 5);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_binary_string(void)
{
    printf("\n\n------------ Inicio test: test_print_binary_string ------------\n");
    global_tables_init();

    printf("Caso: \"Hello\" @ \"World\" (linea 1, columna 9)\n");
    LiteralNode *left = ast_string_literal_create("Hello", 1, 1);
    LiteralNode *right = ast_string_literal_create("World", 1, 10);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 1, 9);
    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    printf("Caso: \"Hello\" @@ \"World\" (linea 2, columna 10)\n");
    left  = ast_string_literal_create("Hello", 2, 1);
    right = ast_string_literal_create("World", 2, 11);
    node = ast_binary_create(OP_CONCAT_WS, (ASTNode *)left, (ASTNode *)right, 2, 10);
    pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);
    fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_binary_nested(void)
{
    printf("\n\n------------ Inicio test: test_print_binary_nested ------------\n");
    global_tables_init();

    printf("Caso: (1 + 2) * (3 + 4) (linea 5, columna 1)\n");
    LiteralNode *a = ast_number_literal_create(1.0, 5, 1);
    LiteralNode *b = ast_number_literal_create(2.0, 5, 3);
    LiteralNode *c = ast_number_literal_create(3.0, 5, 7);
    LiteralNode *d = ast_number_literal_create(4.0, 5, 9);
    BinaryOperationNode *add1 = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 5, 2);
    BinaryOperationNode *add2 = ast_binary_create(OP_ADD, (ASTNode *)c, (ASTNode *)d, 5, 8);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)add1, (ASTNode *)add2, 5, 5);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de BinaryOperationNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BinaryOperationNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_binary_arithmetic", test_print_binary_arithmetic);
    CU_add_test(suite, "test_print_binary_logical", test_print_binary_logical);
    CU_add_test(suite, "test_print_binary_comparison", test_print_binary_comparison);
    CU_add_test(suite, "test_print_binary_string", test_print_binary_string);
    CU_add_test(suite, "test_print_binary_nested", test_print_binary_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}