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

void test_check_let_single_binding_annotated(void)
{
    printf("\n\n------------ Inicio test: test_check_let_single_binding_annotated ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x: Number = 42 in 99\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 1, 17);
    list_append(bindings, symbol_binding_create("x", "Number", init, 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 22);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo inicializador: %s\n", init->return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_single_binding_unannotated(void)
{
    printf("\n\n------------ Inicio test: test_check_let_single_binding_unannotated ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x = 42 in 99\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 1, 9);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 14);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo inicializador: %s\n", init->return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_multiple_bindings_mixed(void)
{
    printf("\n\n------------ Inicio test: test_check_let_multiple_bindings_mixed ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let a: Number = 1, b = true, c: String = \"hello\" in 42\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init_a = (ASTNode *)ast_number_literal_create(1.0, 1, 20);
    ASTNode *init_b = (ASTNode *)ast_bool_literal_create(true, 1, 32);
    ASTNode *init_c = (ASTNode *)ast_string_literal_create("hello", 1, 50);
    list_append(bindings, symbol_binding_create("a", "Number", init_a, 1, 5));
    list_append(bindings, symbol_binding_create("b", NULL, init_b, 1, 23));
    list_append(bindings, symbol_binding_create("c", "String", init_c, 1, 41));
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 60);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo a: %s, b: %s, c: %s\n", init_a->return_type->name, init_b->return_type->name, init_c->return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_nested(void)
{
    printf("\n\n------------ Inicio test: test_check_let_nested ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x = 1 in (let y = true in 99)\n");

    List *inner_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *inner_init = (ASTNode *)ast_bool_literal_create(true, 1, 28);
    list_append(inner_bindings, symbol_binding_create("y", NULL, inner_init, 1, 20));
    ASTNode *inner_body = (ASTNode *)ast_number_literal_create(99.0, 1, 35);
    LetInNode *inner_let = ast_let_create(inner_bindings, inner_body, 1, 17);

    List *outer_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *outer_init = (ASTNode *)ast_number_literal_create(1.0, 1, 9);
    list_append(outer_bindings, symbol_binding_create("x", NULL, outer_init, 1, 5));
    LetInNode *outer_let = ast_let_create(outer_bindings, (ASTNode *)inner_let, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)outer_let);

    printf("Tipo let interno: %s\n", inner_let->base.return_type->name);
    printf("Tipo let externo: %s\n", outer_let->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_let, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_single_incompatible_type(void)
{
    printf("\n\n------------ Inicio test: test_check_let_single_incompatible_type ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x: Number = true in 42\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_bool_literal_create(true, 1, 18);
    list_append(bindings, symbol_binding_create("x", "Number", init, 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 26);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo inicializador: %s\n", init->return_type->name);
    printf("Tipo anotado: Number\n");
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_multiple_incompatible_types(void)
{
    printf("\n\n------------ Inicio test: test_check_let_multiple_incompatible_types ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let a: Number = true, b: String = 42 in 99\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init_a = (ASTNode *)ast_bool_literal_create(true, 1, 20);
    ASTNode *init_b = (ASTNode *)ast_number_literal_create(42.0, 1, 38);
    list_append(bindings, symbol_binding_create("a", "Number", init_a, 1, 5));
    list_append(bindings, symbol_binding_create("b", "String", init_b, 1, 23));
    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 45);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo a: %s (anotado Number)\n", init_a->return_type->name);
    printf("Tipo b: %s (anotado String)\n", init_b->return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 2);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_error_at_initializer(void)
{
    printf("\n\n------------ Inicio test: test_check_let_error_at_initializer ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x = (true + 1) in 42\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    LiteralNode *bad_l = ast_bool_literal_create(true, 1, 11);
    LiteralNode *bad_r = ast_number_literal_create(1.0, 1, 17);
    ASTNode *init = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)bad_l, (ASTNode *)bad_r, 1, 15);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 22);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo inicializador: %s\n", init->return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_error_at_body(void)
{
    printf("\n\n------------ Inicio test: test_check_let_error_at_body ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x = 1 in (true + 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(1.0, 1, 9);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));
    LiteralNode *bad_l = ast_bool_literal_create(true, 1, 14);
    LiteralNode *bad_r = ast_number_literal_create(1.0, 1, 20);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)bad_l, (ASTNode *)bad_r, 1, 18);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo inicializador: %s\n", init->return_type->name);
    printf("Tipo del cuerpo: %s\n", body->return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_let_mixed_errors(void)
{
    printf("\n\n------------ Inicio test: test_check_let_mixed_errors ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let a: Number = (true + 1), b: String = 42 in (\"x\" - 2)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);

    LiteralNode *ia_l = ast_bool_literal_create(true, 1, 22);
    LiteralNode *ia_r = ast_number_literal_create(1.0, 1, 28);
    ASTNode *init_a = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)ia_l, (ASTNode *)ia_r, 1, 26);
    list_append(bindings, symbol_binding_create("a", "Number", init_a, 1, 5));

    ASTNode *init_b = (ASTNode *)ast_number_literal_create(42.0, 1, 46);
    list_append(bindings, symbol_binding_create("b", "String", init_b, 1, 31));

    LiteralNode *bl = ast_string_literal_create("x", 1, 56);
    LiteralNode *br = ast_number_literal_create(2.0, 1, 61);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_SUB, (ASTNode *)bl, (ASTNode *)br, 1, 58);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)node);

    printf("Tipo init_a: %s (anotado Number)\n", init_a->return_type->name);
    printf("Tipo init_b: %s (anotado String)\n", init_b->return_type->name);
    printf("Tipo del cuerpo: %s\n", body->return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

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
    printf("\n=== Iniciando tests de LetInNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LetInNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_let_single_binding_annotated", test_check_let_single_binding_annotated);
    CU_add_test(suite, "test_check_let_single_binding_unannotated", test_check_let_single_binding_unannotated);
    CU_add_test(suite, "test_check_let_multiple_bindings_mixed", test_check_let_multiple_bindings_mixed);
    CU_add_test(suite, "test_check_let_nested", test_check_let_nested);
    CU_add_test(suite, "test_check_let_single_incompatible_type", test_check_let_single_incompatible_type);
    CU_add_test(suite, "test_check_let_multiple_incompatible_types", test_check_let_multiple_incompatible_types);
    CU_add_test(suite, "test_check_let_error_at_initializer", test_check_let_error_at_initializer);
    CU_add_test(suite, "test_check_let_error_at_body", test_check_let_error_at_body);
    CU_add_test(suite, "test_check_let_mixed_errors", test_check_let_mixed_errors);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}