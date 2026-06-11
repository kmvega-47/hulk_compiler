#include <CUnit/Basic.h>
#include "type_check_visitor.h"
#include "type_inference_visitor.h"
#include "free_visitor.h"

static size_t run_inference_and_check(ASTNode *node)
{
    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept(node, (Visitor *)iv);
    type_inference_visitor_destroy(iv);

    if (!dm_has_errors(dm_global))
    {
        TypeCheckVisitor *cv = type_check_visitor_create();
        ast_accept(node, (Visitor *)cv);
        type_check_visitor_destroy(cv);
    }

    return dm_error_count(dm_global);
}

/* ── Comparación ── */

void test_check_comparison_number_number(void)
{
    printf("\n\n------------ Inicio test: test_check_comparison_number_number ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: 1 == 2\n");

    LiteralNode *left  = ast_number_literal_create(1.0, 1, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 1, 6);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 1, 4);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_comparison_string_bool(void)
{
    printf("\n\n------------ Inicio test: test_check_comparison_string_bool ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: \"a\" == true\n");

    LiteralNode *left  = ast_string_literal_create("a", 3, 1);
    LiteralNode *right = ast_bool_literal_create(true, 3, 6);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 3, 4);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_comparison_bool_number(void)
{
    printf("\n\n------------ Inicio test: test_check_comparison_bool_number ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: true == 1\n");

    LiteralNode *left  = ast_bool_literal_create(true, 4, 1);
    LiteralNode *right = ast_number_literal_create(1.0, 4, 9);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 4, 7);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_comparison_string_number(void)
{
    printf("\n\n------------ Inicio test: test_check_comparison_string_number ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: \"a\" == 1\n");

    LiteralNode *left  = ast_string_literal_create("a", 5, 1);
    LiteralNode *right = ast_number_literal_create(1.0, 5, 6);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 5, 4);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_comparison_bool_bool(void)
{
    printf("\n\n------------ Inicio test: test_check_comparison_bool_bool ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: true == false\n");

    LiteralNode *left  = ast_bool_literal_create(true, 6, 1);
    LiteralNode *right = ast_bool_literal_create(false, 6, 9);
    BinaryOperationNode *node = ast_binary_create(OP_EQUAL, (ASTNode *)left, (ASTNode *)right, 6, 7);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_comparison_all_operators(void)
{
    printf("\n\n------------ Inicio test: test_check_comparison_all_operators ------------\n");
    global_tables_init();

    printf("Caso: probar todos los operadores de comparacion con tipos incorrectos\n");

    HulkOperator ops[] = {OP_EQUAL, OP_NOT_EQUAL, OP_LESS, OP_LESS_EQUAL, OP_GREATER, OP_GREATER_EQUAL};
    const char *op_names[] = {"==", "!=", "<", "<=", ">", ">="};

    for (int i = 0; i < 6; i++)
    {
        dm_global = dm_create();

        printf("Probando %s con Number y String\n", op_names[i]);

        LiteralNode *num = ast_number_literal_create(1.0, 7, 1);
        LiteralNode *str = ast_string_literal_create("a", 7, 3);
        BinaryOperationNode *node = ast_binary_create(ops[i], (ASTNode *)num, (ASTNode *)str, 7, 2);

        size_t errors = run_inference_and_check((ASTNode *)node);

        dm_print_errors(dm_global);
        CU_ASSERT_EQUAL(errors, 1);

        FreeVisitor *fv = free_visitor_create();
        ast_accept((ASTNode *)node, (Visitor *)fv);
        free_visitor_destroy(fv);

        dm_destroy(dm_global);
    }

    global_tables_destroy();
}

/* ── Concatenación ── */

void test_check_concat_string_string(void)
{
    printf("\n\n------------ Inicio test: test_check_concat_string_string ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: \"Hello\" @ \"World\"\n");

    LiteralNode *left  = ast_string_literal_create("Hello", 8, 1);
    LiteralNode *right = ast_string_literal_create("World", 8, 11);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 8, 9);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_concat_string_number(void)
{
    printf("\n\n------------ Inicio test: test_check_concat_string_number ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: \"x\" @ 1\n");

    LiteralNode *left  = ast_string_literal_create("x", 9, 1);
    LiteralNode *right = ast_number_literal_create(1.0, 9, 6);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 9, 4);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_concat_bool_string(void)
{
    printf("\n\n------------ Inicio test: test_check_concat_bool_string ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: true @ \"x\"\n");

    LiteralNode *left  = ast_bool_literal_create(true, 10, 1);
    LiteralNode *right = ast_string_literal_create("x", 10, 8);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 10, 6);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_concat_string_bool(void)
{
    printf("\n\n------------ Inicio test: test_check_concat_string_bool ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: \"x\" @ true\n");

    LiteralNode *left  = ast_string_literal_create("x", 11, 1);
    LiteralNode *right = ast_bool_literal_create(true, 11, 6);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 11, 4);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_concat_bool_number(void)
{
    printf("\n\n------------ Inicio test: test_check_concat_bool_number ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: true @ 1 (ambos incorrectos)\n");

    LiteralNode *left  = ast_bool_literal_create(true, 12, 1);
    LiteralNode *right = ast_number_literal_create(1.0, 12, 8);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT, (ASTNode *)left, (ASTNode *)right, 12, 6);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_concat_ws_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_concat_ws_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: \"Hello\" @@ \"World\"\n");

    LiteralNode *left  = ast_string_literal_create("Hello", 13, 1);
    LiteralNode *right = ast_string_literal_create("World", 13, 12);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT_WS, (ASTNode *)left, (ASTNode *)right, 13, 10);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_concat_ws_number_right(void)
{
    printf("\n\n------------ Inicio test: test_check_concat_ws_number_right ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: \"Value:\" @@ 42\n");

    LiteralNode *left  = ast_string_literal_create("Value:", 14, 1);
    LiteralNode *right = ast_number_literal_create(42.0, 14, 12);
    BinaryOperationNode *node = ast_binary_create(OP_CONCAT_WS, (ASTNode *)left, (ASTNode *)right, 14, 10);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

/* ── Aritmético y lógico ── */

void test_check_arithmetic_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_arithmetic_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: 1 + 2\n");

    LiteralNode *left  = ast_number_literal_create(1.0, 15, 1);
    LiteralNode *right = ast_number_literal_create(2.0, 15, 5);
    BinaryOperationNode *node = ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 15, 3);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_arithmetic_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_arithmetic_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: 1 + true\n");

    LiteralNode *left  = ast_number_literal_create(1.0, 16, 1);
    LiteralNode *right = ast_bool_literal_create(true, 16, 5);
    BinaryOperationNode *node = ast_binary_create(OP_ADD, (ASTNode *)left, (ASTNode *)right, 16, 3);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}


void test_check_arithmetic_all_operators(void)
{
    printf("\n\n------------ Inicio test: test_check_arithmetic_all_operators ------------\n");
    global_tables_init();

    printf("Caso: probar todos los operadores aritmeticos con tipos correctos\n");

    HulkOperator ops[] = {OP_ADD, OP_SUB, OP_MULT, OP_DIV, OP_MOD, OP_EXP};
    const char *op_names[] = {"+", "-", "*", "/", "%", "^"};

    for (int i = 0; i < 6; i++)
    {
        dm_global = dm_create();

        printf("Probando %s con Number y Number\n", op_names[i]);

        LiteralNode *num1 = ast_number_literal_create(5.0, 17, 1);
        LiteralNode *num2 = ast_number_literal_create(3.0, 17, 3);
        BinaryOperationNode *node = ast_binary_create(ops[i], (ASTNode *)num1, (ASTNode *)num2, 17, 2);

        size_t errors = run_inference_and_check((ASTNode *)node);

        dm_print_errors(dm_global);
        CU_ASSERT_EQUAL(errors, 0);

        FreeVisitor *fv = free_visitor_create();
        ast_accept((ASTNode *)node, (Visitor *)fv);
        free_visitor_destroy(fv);

        dm_destroy(dm_global);
    }

    global_tables_destroy();
}

void test_check_logical_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_logical_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: true & false\n");

    LiteralNode *left  = ast_bool_literal_create(true, 18, 1);
    LiteralNode *right = ast_bool_literal_create(false, 18, 9);
    BinaryOperationNode *node = ast_binary_create(OP_AND, (ASTNode *)left, (ASTNode *)right, 18, 7);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_logical_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_logical_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: 1 & true\n");

    LiteralNode *left  = ast_number_literal_create(1.0, 19, 1);
    LiteralNode *right = ast_bool_literal_create(true, 19, 5);
    BinaryOperationNode *node = ast_binary_create(OP_AND, (ASTNode *)left, (ASTNode *)right, 19, 3);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo left: %s\n", left->base.return_type->name);
    printf("Tipo right: %s\n", right->base.return_type->name);
    printf("Tipo nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}


void test_check_logical_all_operators(void)
{
    printf("\n\n------------ Inicio test: test_check_logical_all_operators ------------\n");
    global_tables_init();

    printf("Caso: probar & y | con tipos correctos\n");

    HulkOperator ops[] = {OP_AND, OP_OR};
    const char *op_names[] = {"&", "|"};

    for (int i = 0; i < 2; i++)
    {
        dm_global = dm_create();

        printf("Probando %s con Bool y Bool\n", op_names[i]);

        LiteralNode *b1 = ast_bool_literal_create(true, 20, 1);
        LiteralNode *b2 = ast_bool_literal_create(false, 20, 9);
        BinaryOperationNode *node = ast_binary_create(ops[i], (ASTNode *)b1, (ASTNode *)b2, 20, 7);

        size_t errors = run_inference_and_check((ASTNode *)node);

        dm_print_errors(dm_global);
        CU_ASSERT_EQUAL(errors, 0);

        FreeVisitor *fv = free_visitor_create();
        ast_accept((ASTNode *)node, (Visitor *)fv);
        free_visitor_destroy(fv);

        dm_destroy(dm_global);
    }

    global_tables_destroy();
}

/* ── Anidados ── */

void test_check_nested_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_nested_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: (1 + 2) * (3 + 4)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 21, 1);
    LiteralNode *b = ast_number_literal_create(2.0, 21, 3);
    LiteralNode *c = ast_number_literal_create(3.0, 21, 9);
    LiteralNode *d = ast_number_literal_create(4.0, 21, 11);

    BinaryOperationNode *add1 = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 21, 2);
    BinaryOperationNode *add2 = ast_binary_create(OP_ADD, (ASTNode *)c, (ASTNode *)d, 21, 10);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)add1, (ASTNode *)add2, 21, 6);

    size_t errors = run_inference_and_check((ASTNode *)mult);

    printf("Tipo add1: %s\n", add1->base.return_type->name);
    printf("Tipo add2: %s\n", add2->base.return_type->name);
    printf("Tipo mult: %s\n", mult->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_nested_incorrect_single_error(void)
{
    printf("\n\n------------ Inicio test: test_check_nested_incorrect_single_error ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: (1 + true) * 2 (error solo en el +)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 22, 1);
    LiteralNode *b = ast_bool_literal_create(true, 22, 3);
    BinaryOperationNode *add = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 22, 2);

    LiteralNode *c = ast_number_literal_create(2.0, 22, 8);
    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)add, (ASTNode *)c, 22, 6);

    size_t errors = run_inference_and_check((ASTNode *)mult);

    printf("Tipo add: %s\n", add->base.return_type->name);
    printf("Tipo mult: %s\n", mult->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_nested_incorrect_multiple_errors(void)
{
    printf("\n\n------------ Inicio test: test_check_nested_incorrect_multiple_errors ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: (1 + \"x\") * (true / 3) (errores independientes)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 23, 1);
    LiteralNode *b = ast_string_literal_create("x", 23, 3);
    BinaryOperationNode *add = ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 23, 2);

    LiteralNode *c = ast_bool_literal_create(true, 23, 9);
    LiteralNode *d = ast_number_literal_create(3.0, 23, 11);
    BinaryOperationNode *div_node = ast_binary_create(OP_DIV, (ASTNode *)c, (ASTNode *)d, 23, 10);

    BinaryOperationNode *mult = ast_binary_create(OP_MULT, (ASTNode *)add, (ASTNode *)div_node, 23, 6);

    size_t errors = run_inference_and_check((ASTNode *)mult);

    printf("Tipo add: %s\n", add->base.return_type->name);
    printf("Tipo div_node: %s\n", div_node->base.return_type->name);
    printf("Tipo mult: %s\n", mult->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 2);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)mult, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de BinaryOperationNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BinaryOperationNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_comparison_number_number", test_check_comparison_number_number);
    CU_add_test(suite, "test_check_comparison_string_bool", test_check_comparison_string_bool);
    CU_add_test(suite, "test_check_comparison_bool_number", test_check_comparison_bool_number);
    CU_add_test(suite, "test_check_comparison_string_number", test_check_comparison_string_number);
    CU_add_test(suite, "test_check_comparison_bool_bool", test_check_comparison_bool_bool);
    CU_add_test(suite, "test_check_comparison_all_operators", test_check_comparison_all_operators);
    CU_add_test(suite, "test_check_concat_string_string", test_check_concat_string_string);
    CU_add_test(suite, "test_check_concat_string_number", test_check_concat_string_number);
    CU_add_test(suite, "test_check_concat_bool_string", test_check_concat_bool_string);
    CU_add_test(suite, "test_check_concat_string_bool", test_check_concat_string_bool);
    CU_add_test(suite, "test_check_concat_bool_number", test_check_concat_bool_number);
    CU_add_test(suite, "test_check_concat_ws_correct", test_check_concat_ws_correct);
    CU_add_test(suite, "test_check_concat_ws_number_right", test_check_concat_ws_number_right);
    CU_add_test(suite, "test_check_arithmetic_correct", test_check_arithmetic_correct);
    CU_add_test(suite, "test_check_arithmetic_incorrect", test_check_arithmetic_incorrect);
    CU_add_test(suite, "test_check_arithmetic_all_operators", test_check_arithmetic_all_operators);
    CU_add_test(suite, "test_check_logical_correct", test_check_logical_correct);
    CU_add_test(suite, "test_check_logical_incorrect", test_check_logical_incorrect);
    CU_add_test(suite, "test_check_logical_all_operators", test_check_logical_all_operators);
    CU_add_test(suite, "test_check_nested_correct", test_check_nested_correct);
    CU_add_test(suite, "test_check_nested_incorrect_single_error", test_check_nested_incorrect_single_error);
    CU_add_test(suite, "test_check_nested_incorrect_multiple_errors", test_check_nested_incorrect_multiple_errors);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}