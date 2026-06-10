#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_while_single_expression_body(void)
{
    printf("\n\n------------ Inicio test: test_inference_while_single_expression_body ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: while (true) 42 (linea 1, columna 1)\n");

    LiteralNode *cond  = ast_bool_literal_create(true, 1, 8);
    LiteralNode *body  = ast_number_literal_create(42.0, 1, 14);
    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo condicion: %s (esperado: Bool)\n", cond->base.return_type->name);
    printf("Tipo cuerpo: %s (esperado: Number)\n", body->base.return_type->name);
    printf("Tipo del while: %s (esperado: Number)\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(cond->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(body->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_while_multiple_expression_body(void)
{
    printf("\n\n------------ Inicio test: test_inference_while_multiple_expression_body ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: while (x < 10) { 1 + 2; 3 * 4; 5 - 6 } (linea 2, columna 1)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 2, 14);
    LiteralNode *b = ast_number_literal_create(10.0, 2, 18);
    BinaryOperationNode *cond = ast_binary_create(OP_LESS, (ASTNode *)a, (ASTNode *)b, 2, 16);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, ast_binary_create(OP_ADD,
        (ASTNode *)ast_number_literal_create(1.0, 3, 5),
        (ASTNode *)ast_number_literal_create(2.0, 3, 9), 3, 7));
    list_append(body_exprs, ast_binary_create(OP_MULT,
        (ASTNode *)ast_number_literal_create(3.0, 4, 5),
        (ASTNode *)ast_number_literal_create(4.0, 4, 9), 4, 7));
    list_append(body_exprs, ast_binary_create(OP_SUB,
        (ASTNode *)ast_number_literal_create(5.0, 5, 5),
        (ASTNode *)ast_number_literal_create(6.0, 5, 9), 5, 7));
    ExpressionBlockNode *body_block = ast_block_create(body_exprs, 2, 21);

    WhileLoopNode *node = ast_while_create((ASTNode *)cond, (ASTNode *)body_block, 2, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo condicion: %s (esperado: Bool)\n", cond->base.return_type->name);
    for (size_t i = 0; i < list_count(body_block->expressions); i++)
    {
        ASTNode *expr = (ASTNode *)list_get(body_block->expressions, i);
        printf("Tipo expr[%zu] del cuerpo: %s (esperado: Number)\n", i, expr->return_type->name);
        CU_ASSERT_PTR_EQUAL(expr->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    }
    printf("Tipo del bloque-cuerpo: %s (esperado: Number)\n", body_block->base.return_type->name);
    printf("Tipo del while: %s (esperado: Number)\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(cond->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(body_block->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_while_complex_condition(void)
{
    printf("\n\n------------ Inicio test: test_inference_while_complex_condition ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: while (!(a > b) & (c <= d)) 42 (linea 3, columna 1)\n");

    LiteralNode *a = ast_number_literal_create(1.0, 3, 15);
    LiteralNode *b = ast_number_literal_create(2.0, 3, 19);
    BinaryOperationNode *gt = ast_binary_create(OP_GREATER, (ASTNode *)a, (ASTNode *)b, 3, 17);
    UnaryOperationNode *not_op = ast_unary_create(OP_NOT, (ASTNode *)gt, 3, 14);

    LiteralNode *c = ast_number_literal_create(3.0, 3, 26);
    LiteralNode *d = ast_number_literal_create(4.0, 3, 30);
    BinaryOperationNode *le = ast_binary_create(OP_LESS_EQUAL, (ASTNode *)c, (ASTNode *)d, 3, 28);

    BinaryOperationNode *and_op = ast_binary_create(OP_AND, (ASTNode *)not_op, (ASTNode *)le, 3, 23);

    LiteralNode *body = ast_number_literal_create(42.0, 3, 35);
    WhileLoopNode *node = ast_while_create((ASTNode *)and_op, (ASTNode *)body, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipos en la condicion:\n");
    printf("  a > b : %s (esperado: Bool)\n", gt->base.return_type->name);
    printf("  !(a>b): %s (esperado: Bool)\n", not_op->base.return_type->name);
    printf("  c <= d: %s (esperado: Bool)\n", le->base.return_type->name);
    printf("  AND   : %s (esperado: Bool)\n", and_op->base.return_type->name);
    printf("Tipo cuerpo: %s (esperado: Number)\n", body->base.return_type->name);
    printf("Tipo del while: %s (esperado: Number)\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(gt->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(not_op->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(le->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(and_op->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(body->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_while_nested(void)
{
    printf("\n\n------------ Inicio test: test_inference_while_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: while (true) { while (false) 1 } (linea 4, columna 1)\n");

    LiteralNode *outer_cond = ast_bool_literal_create(true, 4, 8);
    LiteralNode *inner_cond = ast_bool_literal_create(false, 4, 24);
    LiteralNode *inner_body = ast_number_literal_create(1.0, 4, 32);
    WhileLoopNode *inner_while = ast_while_create((ASTNode *)inner_cond, (ASTNode *)inner_body, 4, 18);

    List *outer_body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(outer_body_exprs, inner_while);
    ExpressionBlockNode *outer_block = ast_block_create(outer_body_exprs, 4, 15);
    WhileLoopNode *outer_while = ast_while_create((ASTNode *)outer_cond, (ASTNode *)outer_block, 4, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)outer_while, (Visitor *)iv);

    printf("While interno:\n");
    printf("  condicion: %s\n", inner_cond->base.return_type->name);
    printf("  cuerpo: %s\n", inner_body->base.return_type->name);
    printf("  tipo while interno: %s (esperado: Number)\n", inner_while->base.return_type->name);
    printf("While externo:\n");
    printf("  condicion: %s\n", outer_cond->base.return_type->name);
    printf("  tipo bloque-cuerpo: %s (esperado: Number)\n", outer_block->base.return_type->name);
    printf("  tipo while externo: %s (esperado: Number)\n", outer_while->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(inner_while->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(outer_while->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_while, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de WhileLoopNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("WhileLoopNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_while_single_expression_body", test_inference_while_single_expression_body);
    CU_add_test(suite, "test_inference_while_multiple_expression_body", test_inference_while_multiple_expression_body);
    CU_add_test(suite, "test_inference_while_complex_condition", test_inference_while_complex_condition);
    CU_add_test(suite, "test_inference_while_nested", test_inference_while_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}