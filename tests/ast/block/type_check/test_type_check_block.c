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

void test_check_block_empty(void)
{
    printf("\n\n------------ Inicio test: test_check_block_empty ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: { }\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo del bloque: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_block_single(void)
{
    printf("\n\n------------ Inicio test: test_check_block_single ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: { 42 }\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_number_literal_create(42.0, 1, 3));
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    ASTNode *first = (ASTNode *)list_get(node->expressions, 0);
    printf("Tipo de la expresion: %s\n", first->return_type->name);
    printf("Tipo del bloque: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_block_multiple(void)
{
    printf("\n\n------------ Inicio test: test_check_block_multiple ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: { 1; true; \"hello\" }\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_number_literal_create(1.0, 1, 3));
    list_append(exprs, ast_bool_literal_create(true, 1, 6));
    list_append(exprs, ast_string_literal_create("hello", 1, 12));
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    for (size_t i = 0; i < list_count(node->expressions); i++)
    {
        ASTNode *expr = (ASTNode *)list_get(node->expressions, i);
        printf("  expr[%zu]: %s\n", i, expr->return_type->name);
    }
    printf("Tipo del bloque: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_block_nested(void)
{
    printf("\n\n------------ Inicio test: test_check_block_nested ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: { { 1; true; \"hello\" } }\n");

    List *inner_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(inner_exprs, ast_number_literal_create(1.0, 1, 5));
    list_append(inner_exprs, ast_bool_literal_create(true, 1, 8));
    list_append(inner_exprs, ast_string_literal_create("hello", 1, 14));
    ExpressionBlockNode *inner = ast_block_create(inner_exprs, 1, 3);

    List *outer_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(outer_exprs, inner);
    ExpressionBlockNode *outer = ast_block_create(outer_exprs, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)outer);

    printf("Tipo bloque interno: %s\n", inner->base.return_type->name);
    printf("Tipo bloque externo: %s\n", outer->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_block_single_error(void)
{
    printf("\n\n------------ Inicio test: test_check_block_single_error ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: { 1; true + 5; \"hello\" }\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, ast_number_literal_create(1.0, 1, 3));

    LiteralNode *bad_left = ast_bool_literal_create(true, 1, 8);
    LiteralNode *bad_right = ast_number_literal_create(5.0, 1, 16);
    list_append(exprs, ast_binary_create(OP_ADD, (ASTNode *)bad_left, (ASTNode *)bad_right, 1, 12));

    list_append(exprs, ast_string_literal_create("hello", 1, 19));
    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo del bloque: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_block_multiple_errors(void)
{
    printf("\n\n------------ Inicio test: test_check_block_multiple_errors ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: { true + 1; \"x\" - 2; 3 / false }\n");

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);

    LiteralNode *e1_left = ast_bool_literal_create(true, 1, 3);
    LiteralNode *e1_right = ast_number_literal_create(1.0, 1, 10);
    list_append(exprs, ast_binary_create(OP_ADD, (ASTNode *)e1_left, (ASTNode *)e1_right, 1, 8));

    LiteralNode *e2_left = ast_string_literal_create("x", 1, 14);
    LiteralNode *e2_right = ast_number_literal_create(2.0, 1, 19);
    list_append(exprs, ast_binary_create(OP_SUB, (ASTNode *)e2_left, (ASTNode *)e2_right, 1, 16));

    LiteralNode *e3_left = ast_number_literal_create(3.0, 1, 23);
    LiteralNode *e3_right = ast_bool_literal_create(false, 1, 28);
    list_append(exprs, ast_binary_create(OP_DIV, (ASTNode *)e3_left, (ASTNode *)e3_right, 1, 25));

    ExpressionBlockNode *node = ast_block_create(exprs, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo del bloque: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 3);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_block_nested_error(void)
{
    printf("\n\n------------ Inicio test: test_check_block_nested_error ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: { { true + 1 } }\n");

    LiteralNode *bad_left = ast_bool_literal_create(true, 1, 5);
    LiteralNode *bad_right = ast_number_literal_create(1.0, 1, 12);
    List *inner_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(inner_exprs, ast_binary_create(OP_ADD, (ASTNode *)bad_left, (ASTNode *)bad_right, 1, 10));
    ExpressionBlockNode *inner = ast_block_create(inner_exprs, 1, 3);

    List *outer_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(outer_exprs, inner);
    ExpressionBlockNode *outer = ast_block_create(outer_exprs, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)outer);

    printf("Tipo bloque interno: %s\n", inner->base.return_type->name);
    printf("Tipo bloque externo: %s\n", outer->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ExpressionBlockNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ExpressionBlockNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_block_empty", test_check_block_empty);
    CU_add_test(suite, "test_check_block_single", test_check_block_single);
    CU_add_test(suite, "test_check_block_multiple", test_check_block_multiple);
    CU_add_test(suite, "test_check_block_nested", test_check_block_nested);
    CU_add_test(suite, "test_check_block_single_error", test_check_block_single_error);
    CU_add_test(suite, "test_check_block_multiple_errors", test_check_block_multiple_errors);
    CU_add_test(suite, "test_check_block_nested_error", test_check_block_nested_error);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}