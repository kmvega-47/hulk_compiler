#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_let_single_binding(void)
{
    printf("\n\n------------ Inicio test: test_inference_let_single_binding ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: let x = 42 in 99 (linea 1, columna 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 1, 9);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 14);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inicializador de x: %s (esperado: Number)\n", init->return_type->name);
    printf("Tipo del let-in: %s (esperado: Number)\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(init->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_inference_let_multiple_bindings(void)
{
    printf("\n\n------------ Inicio test: test_inference_let_multiple_bindings ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: let a = 1, b = true, c = \"hello\" in 0 (linea 2, columna 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init_a = (ASTNode *)ast_number_literal_create(1.0, 2, 9);
    ASTNode *init_b = (ASTNode *)ast_bool_literal_create(true, 2, 16);
    ASTNode *init_c = (ASTNode *)ast_string_literal_create("hello", 2, 28);
    list_append(bindings, symbol_binding_create("a", NULL, init_a, 2, 5));
    list_append(bindings, symbol_binding_create("b", NULL, init_b, 2, 12));
    list_append(bindings, symbol_binding_create("c", NULL, init_c, 2, 19));

    ASTNode *body = (ASTNode *)ast_number_literal_create(0.0, 2, 38);
    LetInNode *node = ast_let_create(bindings, body, 2, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo init a: %s (Number)\n", init_a->return_type->name);
    printf("Tipo init b: %s (Bool)\n", init_b->return_type->name);
    printf("Tipo init c: %s (String)\n", init_c->return_type->name);
    printf("Tipo del let-in: %s (Number)\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(init_a->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(init_b->return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(init_c->return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
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

void test_inference_let_undefined_type_annotation(void)
{
    printf("\n\n------------ Inicio test: test_inference_let_undefined_type_annotation ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: let x: NoExiste = 42 in 0 (linea 3, columna 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 3, 18);
    list_append(bindings, symbol_binding_create("x", "NoExiste", init, 3, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(0.0, 3, 23);
    LetInNode *node = ast_let_create(bindings, body, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo init x: %s (Number)\n", init->return_type->name);
    printf("Tipo del let-in: %s (Number)\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(init->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: 1 (tipo no definido)\n");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_let_nested(void)
{
    printf("\n\n------------ Inicio test: test_inference_let_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: let x = 1 in (let y = true in 0) (linea 4, columna 1)\n");

    List *inner_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *inner_init = (ASTNode *)ast_bool_literal_create(true, 4, 28);
    list_append(inner_bindings, symbol_binding_create("y", NULL, inner_init, 4, 24));
    ASTNode *inner_body = (ASTNode *)ast_number_literal_create(0.0, 4, 35);
    LetInNode *inner_let = ast_let_create(inner_bindings, inner_body, 4, 17);

    List *outer_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *outer_init = (ASTNode *)ast_number_literal_create(1.0, 4, 9);
    list_append(outer_bindings, symbol_binding_create("x", NULL, outer_init, 4, 5));
    LetInNode *outer_let = ast_let_create(outer_bindings, (ASTNode *)inner_let, 4, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)outer_let, (Visitor *)iv);

    printf("Tipo init x: %s (Number)\n", outer_init->return_type->name);
    printf("Tipo init y: %s (Bool)\n", inner_init->return_type->name);
    printf("Tipo let interno: %s (Number)\n", inner_let->base.return_type->name);
    printf("Tipo let externo: %s (Number)\n", outer_let->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(outer_init->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(inner_init->return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(inner_let->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(outer_let->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_let, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_let_shadowing(void)
{
    printf("\n\n------------ Inicio test: test_inference_let_shadowing ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: let x = 5 in let x = 10 in x (linea 5, columna 1)\n");

    List *inner_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *inner_init = (ASTNode *)ast_number_literal_create(10.0, 5, 28);
    list_append(inner_bindings, symbol_binding_create("x", NULL, inner_init, 5, 24));
    ASTNode *inner_body = (ASTNode *)ast_number_literal_create(10.0, 5, 33);
    LetInNode *inner_let = ast_let_create(inner_bindings, inner_body, 5, 17);

    List *outer_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *outer_init = (ASTNode *)ast_number_literal_create(5.0, 5, 9);
    list_append(outer_bindings, symbol_binding_create("x", NULL, outer_init, 5, 5));
    LetInNode *outer_let = ast_let_create(outer_bindings, (ASTNode *)inner_let, 5, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)outer_let, (Visitor *)iv);

    printf("Tipo init x externo: %s (Number)\n", outer_init->return_type->name);
    printf("Tipo init x interno: %s (Number)\n", inner_init->return_type->name);
    printf("Tipo let interno: %s (Number)\n", inner_let->base.return_type->name);
    printf("Tipo let externo: %s (Number)\n", outer_let->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(outer_init->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(inner_init->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(inner_let->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(outer_let->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_let, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de LetInNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LetInNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_let_single_binding", test_inference_let_single_binding);
    CU_add_test(suite, "test_inference_let_multiple_bindings", test_inference_let_multiple_bindings);
    CU_add_test(suite, "test_inference_let_undefined_type_annotation", test_inference_let_undefined_type_annotation);
    CU_add_test(suite, "test_inference_let_nested", test_inference_let_nested);
    CU_add_test(suite, "test_inference_let_shadowing", test_inference_let_shadowing);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}