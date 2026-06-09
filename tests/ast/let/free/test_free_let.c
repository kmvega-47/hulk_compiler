#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_let_single_binding(void)
{
    printf("\n\n------------ Inicio test: test_free_let_single_binding ------------\n");
    global_tables_init();

    printf("Caso: let x = 42 in 99\n");
    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 1, 9);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 14);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_let_multiple_bindings(void)
{
    printf("\n\n------------ Inicio test: test_free_let_multiple_bindings ------------\n");
    global_tables_init();

    printf("Caso: let a = 1, b = true, c = \"hello\" in 42\n");
    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(bindings, symbol_binding_create("a", NULL, (ASTNode *)ast_number_literal_create(1.0, 1, 9), 1, 5));
    list_append(bindings, symbol_binding_create("b", NULL, (ASTNode *)ast_bool_literal_create(true, 1, 16), 1, 12));
    list_append(bindings, symbol_binding_create("c", NULL, (ASTNode *)ast_string_literal_create("hello", 1, 28), 1, 19));
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 38);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_let_nested(void)
{
    printf("\n\n------------ Inicio test: test_free_let_nested ------------\n");
    global_tables_init();

    printf("Caso: let x = 1 in (let y = 2 in 99)\n");
    List *inner_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(inner_bindings, symbol_binding_create("y", NULL, (ASTNode *)ast_number_literal_create(2.0, 1, 24), 1, 20));
    ASTNode *inner_body = (ASTNode *)ast_number_literal_create(99.0, 1, 29);
    LetInNode *inner_let = ast_let_create(inner_bindings, inner_body, 1, 17);

    List *outer_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(outer_bindings, symbol_binding_create("x", NULL, (ASTNode *)ast_number_literal_create(1.0, 1, 9), 1, 5));
    LetInNode *outer_let = ast_let_create(outer_bindings, (ASTNode *)inner_let, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)outer_let, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de LetInNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LetInNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_let_single_binding", test_free_let_single_binding);
    CU_add_test(suite, "test_free_let_multiple_bindings", test_free_let_multiple_bindings);
    CU_add_test(suite, "test_free_let_nested", test_free_let_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}