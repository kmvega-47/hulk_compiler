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

void test_check_conditional_if_without_else_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_if_without_else_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) 42\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 1, 16);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, NULL, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_if_without_else_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_if_without_else_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) (1 + true)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 2, 5);
    LiteralNode *num = ast_number_literal_create(1.0, 2, 16);
    LiteralNode *bad = ast_bool_literal_create(true, 2, 19);
    BinaryOperationNode *then_branch = ast_binary_create(OP_ADD, (ASTNode *)num, (ASTNode *)bad, 2, 18);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, NULL, 2, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_if_with_else_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_if_with_else_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) 42 else 0\n");

    LiteralNode *cond = ast_bool_literal_create(true, 3, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 3, 16);
    LiteralNode *else_branch = ast_number_literal_create(0.0, 3, 24);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 3, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo rama else: %s\n", else_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_if_with_else_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_if_with_else_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) 42 else (false + 1)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 4, 5);
    LiteralNode *then_branch = ast_number_literal_create(42.0, 4, 16);
    LiteralNode *bad_left = ast_bool_literal_create(false, 4, 26);
    LiteralNode *bad_right = ast_number_literal_create(1.0, 4, 34);
    BinaryOperationNode *else_branch = ast_binary_create(OP_ADD, (ASTNode *)bad_left, (ASTNode *)bad_right, 4, 32);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 4, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo rama else: %s\n", else_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_if_else_if_else_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_if_else_if_else_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) 1 else if (false) 2 else 3\n");

    LiteralNode *cond1 = ast_bool_literal_create(true, 5, 5);
    LiteralNode *then1 = ast_number_literal_create(1.0, 5, 16);
    LiteralNode *cond2 = ast_bool_literal_create(false, 5, 28);
    LiteralNode *then2 = ast_number_literal_create(2.0, 5, 39);
    LiteralNode *else_b = ast_number_literal_create(3.0, 5, 46);
    ConditionalNode *inner = ast_conditional_create((ASTNode *)cond2, (ASTNode *)then2, (ASTNode *)else_b, 5, 23);
    ConditionalNode *outer = ast_conditional_create((ASTNode *)cond1, (ASTNode *)then1, (ASTNode *)inner, 5, 1);

    size_t errors = run_inference_and_check((ASTNode *)outer);

    printf("Tipo nodo externo: %s\n", outer->base.return_type->name);
    printf("Tipo nodo interno: %s\n", inner->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_if_else_if_else_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_if_else_if_else_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) 1 else if (false) (\"x\" - 2) else 3\n");

    LiteralNode *cond1 = ast_bool_literal_create(true, 6, 5);
    LiteralNode *then1 = ast_number_literal_create(1.0, 6, 16);
    LiteralNode *cond2 = ast_bool_literal_create(false, 6, 28);
    LiteralNode *bad_left = ast_string_literal_create("x", 6, 40);
    LiteralNode *bad_right = ast_number_literal_create(2.0, 6, 45);
    BinaryOperationNode *then2 = ast_binary_create(OP_SUB, (ASTNode *)bad_left, (ASTNode *)bad_right, 6, 42);
    LiteralNode *else_b = ast_number_literal_create(3.0, 6, 52);
    ConditionalNode *inner = ast_conditional_create((ASTNode *)cond2, (ASTNode *)then2, (ASTNode *)else_b, 6, 23);
    ConditionalNode *outer = ast_conditional_create((ASTNode *)cond1, (ASTNode *)then1, (ASTNode *)inner, 6, 1);

    size_t errors = run_inference_and_check((ASTNode *)outer);

    printf("Tipo nodo externo: %s\n", outer->base.return_type->name);
    printf("Tipo nodo interno: %s\n", inner->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_condition_not_boolean(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_condition_not_boolean ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (42) 1 (condicion no booleana)\n");

    LiteralNode *cond = ast_number_literal_create(42.0, 8, 5);
    LiteralNode *then_branch = ast_number_literal_create(1.0, 8, 10);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, NULL, 8, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_multiple_errors_one_branch(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_multiple_errors_one_branch ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) (1 + true) else 0\n");

    LiteralNode *cond = ast_bool_literal_create(true, 9, 5);
    LiteralNode *num = ast_number_literal_create(1.0, 9, 16);
    LiteralNode *bad = ast_bool_literal_create(true, 9, 19);
    BinaryOperationNode *then_branch = ast_binary_create(OP_ADD, (ASTNode *)num, (ASTNode *)bad, 9, 18);
    LiteralNode *else_branch = ast_number_literal_create(0.0, 9, 26);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 9, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo rama else: %s\n", else_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_multiple_errors_both_branches(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_multiple_errors_both_branches ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (true) (1 + true) else (\"x\" - 2)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 10, 5);
    LiteralNode *t_num = ast_number_literal_create(1.0, 10, 16);
    LiteralNode *t_bad = ast_bool_literal_create(true, 10, 19);
    BinaryOperationNode *then_branch = ast_binary_create(OP_ADD, (ASTNode *)t_num, (ASTNode *)t_bad, 10, 18);
    LiteralNode *e_str = ast_string_literal_create("x", 10, 29);
    LiteralNode *e_num = ast_number_literal_create(2.0, 10, 34);
    BinaryOperationNode *else_branch = ast_binary_create(OP_SUB, (ASTNode *)e_str, (ASTNode *)e_num, 10, 31);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 10, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo rama else: %s\n", else_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 2);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_conditional_multiple_errors_mixed(void)
{
    printf("\n\n------------ Inicio test: test_check_conditional_multiple_errors_mixed ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: if (42) (1 + true) else (\"x\" - 2)\n");

    LiteralNode *cond = ast_number_literal_create(42.0, 11, 5);
    LiteralNode *t_num = ast_number_literal_create(1.0, 11, 13);
    LiteralNode *t_bad = ast_bool_literal_create(true, 11, 16);
    BinaryOperationNode *then_branch = ast_binary_create(OP_ADD, (ASTNode *)t_num, (ASTNode *)t_bad, 11, 15);
    LiteralNode *e_str = ast_string_literal_create("x", 11, 26);
    LiteralNode *e_num = ast_number_literal_create(2.0, 11, 31);
    BinaryOperationNode *else_branch = ast_binary_create(OP_SUB, (ASTNode *)e_str, (ASTNode *)e_num, 11, 28);
    ConditionalNode *node = ast_conditional_create((ASTNode *)cond, (ASTNode *)then_branch, (ASTNode *)else_branch, 11, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo rama then: %s\n", then_branch->base.return_type->name);
    printf("Tipo rama else: %s\n", else_branch->base.return_type->name);
    printf("Tipo del nodo: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 3);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ConditionalNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConditionalNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_conditional_if_without_else_correct", test_check_conditional_if_without_else_correct);
    CU_add_test(suite, "test_check_conditional_if_without_else_incorrect", test_check_conditional_if_without_else_incorrect);
    CU_add_test(suite, "test_check_conditional_if_with_else_correct", test_check_conditional_if_with_else_correct);
    CU_add_test(suite, "test_check_conditional_if_with_else_incorrect", test_check_conditional_if_with_else_incorrect);
    CU_add_test(suite, "test_check_conditional_if_else_if_else_correct", test_check_conditional_if_else_if_else_correct);
    CU_add_test(suite, "test_check_conditional_if_else_if_else_incorrect", test_check_conditional_if_else_if_else_incorrect);
    CU_add_test(suite, "test_check_conditional_condition_not_boolean", test_check_conditional_condition_not_boolean);
    CU_add_test(suite, "test_check_conditional_multiple_errors_one_branch", test_check_conditional_multiple_errors_one_branch);
    CU_add_test(suite, "test_check_conditional_multiple_errors_both_branches", test_check_conditional_multiple_errors_both_branches);
    CU_add_test(suite, "test_check_conditional_multiple_errors_mixed", test_check_conditional_multiple_errors_mixed);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}