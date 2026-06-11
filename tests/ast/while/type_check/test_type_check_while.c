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

void test_check_while_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_while_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: while (true) 42\n");

    LiteralNode *cond = ast_bool_literal_create(true, 1, 7);
    LiteralNode *body = ast_number_literal_create(42.0, 1, 14);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo cuerpo: %s\n", body->base.return_type->name);
    printf("Tipo del while: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_while_block_body(void)
{
    printf("\n\n------------ Inicio test: test_check_while_block_body ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: while (true) { 1 + 2; 3 * 4 }\n");

    LiteralNode *a = ast_number_literal_create(1.0, 2, 9);
    LiteralNode *b = ast_number_literal_create(2.0, 2, 13);
    BinaryOperationNode *cond = ast_binary_create(OP_LESS, (ASTNode *)a, (ASTNode *)b, 2, 11);

    LiteralNode *e0_l = ast_number_literal_create(1.0, 2, 18);
    LiteralNode *e0_r = ast_number_literal_create(2.0, 2, 22);
    BinaryOperationNode *e0 = ast_binary_create(OP_ADD, (ASTNode *)e0_l, (ASTNode *)e0_r, 2, 20);

    LiteralNode *e1_l = ast_number_literal_create(3.0, 2, 25);
    LiteralNode *e1_r = ast_number_literal_create(4.0, 2, 29);
    BinaryOperationNode *e1 = ast_binary_create(OP_MULT, (ASTNode *)e1_l, (ASTNode *)e1_r, 2, 27);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, e0);
    list_append(body_exprs, e1);
    ExpressionBlockNode *body_block = ast_block_create(body_exprs, 2, 15);

    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body_block, 2, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo bloque cuerpo: %s\n", body_block->base.return_type->name);
    printf("Tipo del while: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_while_nested(void)
{
    printf("\n\n------------ Inicio test: test_check_while_nested ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: while (true) { while (false) 1 }\n");

    LiteralNode *outer_cond = ast_bool_literal_create(true, 3, 7);
    LiteralNode *inner_cond = ast_bool_literal_create(false, 3, 23);
    LiteralNode *inner_body = ast_number_literal_create(1.0, 3, 31);
    WhileLoopNode *inner_while = ast_while_create((ASTNode *)inner_cond, (ASTNode *)inner_body, 3, 17);

    List *outer_body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(outer_body_exprs, inner_while);
    ExpressionBlockNode *outer_block = ast_block_create(outer_body_exprs, 3, 14);

    WhileLoopNode *outer_while = ast_while_create((ASTNode *)outer_cond, (ASTNode *)outer_block, 3, 1);

    size_t errors = run_inference_and_check((ASTNode *)outer_while);

    printf("While interno - condicion: %s, tipo: %s\n", inner_cond->base.return_type->name, inner_while->base.return_type->name);
    printf("While externo - condicion: %s, tipo: %s\n", outer_cond->base.return_type->name, outer_while->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_while, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_while_condition_not_bool(void)
{
    printf("\n\n------------ Inicio test: test_check_while_condition_not_bool ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: while (42) 1\n");

    LiteralNode *cond = ast_number_literal_create(42.0, 4, 7);
    LiteralNode *body = ast_number_literal_create(1.0, 4, 12);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body, 4, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo cuerpo: %s\n", body->base.return_type->name);
    printf("Tipo del while: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_while_error_at_body(void)
{
    printf("\n\n------------ Inicio test: test_check_while_error_at_body ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: while (true) (1 + true)\n");

    LiteralNode *cond = ast_bool_literal_create(true, 5, 7);
    LiteralNode *bad_l = ast_number_literal_create(1.0, 5, 15);
    LiteralNode *bad_r = ast_bool_literal_create(true, 5, 18);
    BinaryOperationNode *body = ast_binary_create(OP_ADD, (ASTNode *)bad_l, (ASTNode *)bad_r, 5, 17);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body, 5, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo cuerpo: %s\n", body->base.return_type->name);
    printf("Tipo del while: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_while_multiple_errors_at_body(void)
{
    printf("\n\n------------ Inicio test: test_check_while_multiple_errors_at_body ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: while (true) { 1 + true; \"x\" - 2 }\n");

    LiteralNode *cond = ast_bool_literal_create(true, 6, 7);

    LiteralNode *e0_l = ast_number_literal_create(1.0, 6, 18);
    LiteralNode *e0_r = ast_bool_literal_create(true, 6, 21);
    BinaryOperationNode *e0 = ast_binary_create(OP_ADD, (ASTNode *)e0_l, (ASTNode *)e0_r, 6, 20);

    LiteralNode *e1_l = ast_string_literal_create("x", 6, 26);
    LiteralNode *e1_r = ast_number_literal_create(2.0, 6, 31);
    BinaryOperationNode *e1 = ast_binary_create(OP_SUB, (ASTNode *)e1_l, (ASTNode *)e1_r, 6, 28);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, e0);
    list_append(body_exprs, e1);
    ExpressionBlockNode *body_block = ast_block_create(body_exprs, 6, 15);

    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body_block, 6, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo bloque cuerpo: %s\n", body_block->base.return_type->name);
    printf("Tipo del while: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 2);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_while_mixed_errors(void)
{
    printf("\n\n------------ Inicio test: test_check_while_mixed_errors ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: while (42) { 1 + true; \"x\" - 2 }\n");

    LiteralNode *cond = ast_number_literal_create(42.0, 7, 7);

    LiteralNode *e0_l = ast_number_literal_create(1.0, 7, 17);
    LiteralNode *e0_r = ast_bool_literal_create(true, 7, 20);
    BinaryOperationNode *e0 = ast_binary_create(OP_ADD, (ASTNode *)e0_l, (ASTNode *)e0_r, 7, 19);

    LiteralNode *e1_l = ast_string_literal_create("x", 7, 25);
    LiteralNode *e1_r = ast_number_literal_create(2.0, 7, 30);
    BinaryOperationNode *e1 = ast_binary_create(OP_SUB, (ASTNode *)e1_l, (ASTNode *)e1_r, 7, 27);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, e0);
    list_append(body_exprs, e1);
    ExpressionBlockNode *body_block = ast_block_create(body_exprs, 7, 14);

    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body_block, 7, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo condicion: %s\n", cond->base.return_type->name);
    printf("Tipo bloque cuerpo: %s\n", body_block->base.return_type->name);
    printf("Tipo del while: %s\n", node->base.return_type->name);

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
    printf("\n=== Iniciando tests de WhileLoopNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("WhileLoopNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_while_correct", test_check_while_correct);
    CU_add_test(suite, "test_check_while_block_body", test_check_while_block_body);
    CU_add_test(suite, "test_check_while_nested", test_check_while_nested);
    CU_add_test(suite, "test_check_while_condition_not_bool", test_check_while_condition_not_bool);
    CU_add_test(suite, "test_check_while_error_at_body", test_check_while_error_at_body);
    CU_add_test(suite, "test_check_while_multiple_errors_at_body", test_check_while_multiple_errors_at_body);
    CU_add_test(suite, "test_check_while_mixed_errors", test_check_while_mixed_errors);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}