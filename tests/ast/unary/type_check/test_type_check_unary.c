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

void test_check_unary_not_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_unary_not_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: !true\n");

    LiteralNode *operand = ast_bool_literal_create(true, 1, 2);
    UnaryOperationNode *node = ast_unary_create(OP_NOT, (ASTNode *)operand, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo del operando: %s\n", operand->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_unary_not_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_unary_not_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: !42\n");

    LiteralNode *operand = ast_number_literal_create(42.0, 2, 3);
    UnaryOperationNode *node = ast_unary_create(OP_NOT, (ASTNode *)operand, 2, 2);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo del operando: %s\n", operand->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_unary_negative_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_unary_negative_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: -42\n");

    LiteralNode *operand = ast_number_literal_create(42.0, 3, 4);
    UnaryOperationNode *node = ast_unary_create(OP_SUB, (ASTNode *)operand, 3, 3);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo del operando: %s\n", operand->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_unary_negative_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_unary_negative_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: -true\n");

    LiteralNode *operand = ast_bool_literal_create(true, 4, 5);
    UnaryOperationNode *node = ast_unary_create(OP_SUB, (ASTNode *)operand, 4, 4);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo del operando: %s\n", operand->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);
    printf("Operador: %s\n", operator_to_string(node->operator));

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_unary_nested_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_unary_nested_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: ---47\n");

    LiteralNode *inner = ast_number_literal_create(47.0, 5, 6);
    UnaryOperationNode *neg1 = ast_unary_create(OP_SUB, (ASTNode *)inner, 5, 5);
    UnaryOperationNode *neg2 = ast_unary_create(OP_SUB, (ASTNode *)neg1, 5, 4);
    UnaryOperationNode *neg3 = ast_unary_create(OP_SUB, (ASTNode *)neg2, 5, 3);

    size_t errors = run_inference_and_check((ASTNode *)neg3);

    printf("Tipo neg3: %s\n", neg3->base.return_type->name);
    printf("Tipo neg2: %s\n", neg2->base.return_type->name);
    printf("Tipo neg1: %s\n", neg1->base.return_type->name);
    printf("Tipo inner: %s\n", inner->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)neg3, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_unary_nested_incorrect_first(void)
{
    printf("\n\n------------ Inicio test: test_check_unary_nested_incorrect_first ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: !!!5 donde el error esta en el operando mas interno\n");

    LiteralNode *inner = ast_number_literal_create(5.0, 6, 7);
    UnaryOperationNode *not1 = ast_unary_create(OP_NOT, (ASTNode *)inner, 6, 6);
    UnaryOperationNode *not2 = ast_unary_create(OP_NOT, (ASTNode *)not1, 6, 5);
    UnaryOperationNode *not3 = ast_unary_create(OP_NOT, (ASTNode *)not2, 6, 4);

    size_t errors = run_inference_and_check((ASTNode *)not3);

    printf("Tipo not3: %s\n", not3->base.return_type->name);
    printf("Tipo not2: %s\n", not2->base.return_type->name);
    printf("Tipo not1: %s\n", not1->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)not3, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_unary_mixed_operators(void)
{
    printf("\n\n------------ Inicio test: test_check_unary_mixed_operators ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: -(!4) donde !4 falla y - recibe Bool en vez de Number\n");

    LiteralNode *inner = ast_number_literal_create(4.0, 7, 8);
    UnaryOperationNode *not_node = ast_unary_create(OP_NOT, (ASTNode *)inner, 7, 7);
    UnaryOperationNode *neg_node = ast_unary_create(OP_SUB, (ASTNode *)not_node, 7, 6);

    size_t errors = run_inference_and_check((ASTNode *)neg_node);

    printf("Tipo neg_node: %s\n", neg_node->base.return_type->name);
    printf("Tipo not_node: %s\n", not_node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 2);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)neg_node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de UnaryOperationNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("UnaryOperationNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_unary_not_correct", test_check_unary_not_correct);
    CU_add_test(suite, "test_check_unary_not_incorrect", test_check_unary_not_incorrect);
    CU_add_test(suite, "test_check_unary_negative_correct", test_check_unary_negative_correct);
    CU_add_test(suite, "test_check_unary_negative_incorrect", test_check_unary_negative_incorrect);
    CU_add_test(suite, "test_check_unary_nested_correct", test_check_unary_nested_correct);
    CU_add_test(suite, "test_check_unary_nested_incorrect_first", test_check_unary_nested_incorrect_first);
    CU_add_test(suite, "test_check_unary_mixed_operators", test_check_unary_mixed_operators);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}