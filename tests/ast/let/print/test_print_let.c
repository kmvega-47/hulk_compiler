#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_let_single_binding_annotated(void)
{
    printf("\n\n------------ Inicio test: test_print_let_single_binding_annotated ------------\n");
    global_tables_init();

    printf("Caso: let x: Number = 42 in x (linea 1, columna 1)\n");
    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(bindings, symbol_binding_create("x", "Number", (ASTNode *)ast_number_literal_create(42.0, 1, 17), 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 22);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_let_single_binding_unnanotated(void)
{
    printf("\n\n------------ Inicio test: test_print_let_single_binding_unnanotated ------------\n");
    global_tables_init();

    printf("Caso: let y = true in y (linea 2, columna 1)\n");
    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(bindings, symbol_binding_create("y", NULL, (ASTNode *)ast_bool_literal_create(true, 2, 9), 2, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 2, 17);
    LetInNode *node = ast_let_create(bindings, body, 2, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_let_multiple_bindings_mixed_annotations(void)
{
    printf("\n\n------------ Inicio test: test_print_let_multiple_bindings_mixed_annotations ------------\n");
    global_tables_init();

    printf("Caso: let a: Number = 1, b = true, c: String = \"hello\" in a (linea 3, columna 1)\n");
    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(bindings, symbol_binding_create("a", "Number", (ASTNode *)ast_number_literal_create(1.0, 3, 20), 3, 5));
    list_append(bindings, symbol_binding_create("b", NULL, (ASTNode *)ast_bool_literal_create(true, 3, 28), 3, 12));
    list_append(bindings, symbol_binding_create("c", "String", (ASTNode *)ast_string_literal_create("hello", 3, 42), 3, 19));
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 3, 52);
    LetInNode *node = ast_let_create(bindings, body, 3, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de LetInNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LetInNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_let_single_binding_annotated", test_print_let_single_binding_annotated);
    CU_add_test(suite, "test_print_let_single_binding_unnanotated", test_print_let_single_binding_unnanotated);
    CU_add_test(suite, "test_print_let_multiple_bindings_mixed_annotations", test_print_let_multiple_bindings_mixed_annotations);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}