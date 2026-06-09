#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

void test_print_function_definition_no_params(void)
{
    printf("\n\n------------ Inicio test: test_print_function_definition_no_params ------------\n");
    global_tables_init();

    printf("Caso: function foo() => 42 (linea 1, columna 1)\n");
    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 20);
    FunctionDefinitionNode *node = ast_function_definition_create("foo", params, NULL, body, 1, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_function_definition_all_annotated(void)
{
    printf("\n\n------------ Inicio test: test_print_function_definition_all_annotated ------------\n");
    global_tables_init();

    printf("Caso: function add(x: Number, y: Number) => 0 (linea 2, columna 1)\n");
    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 2, 14));
    list_append(params, symbol_binding_create("y", "Number", NULL, 2, 26));
    ASTNode *body = (ASTNode *)ast_number_literal_create(0.0, 2, 38);
    FunctionDefinitionNode *node = ast_function_definition_create("add", params, NULL, body, 2, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_function_definition_none_annotated(void)
{
    printf("\n\n------------ Inicio test: test_print_function_definition_none_annotated ------------\n");
    global_tables_init();

    printf("Caso: function multiply(a, b) => 1 (linea 3, columna 1)\n");
    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("a", NULL, NULL, 3, 19));
    list_append(params, symbol_binding_create("b", NULL, NULL, 3, 22));
    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 3, 27);
    FunctionDefinitionNode *node = ast_function_definition_create("multiply", params, NULL, body, 3, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_function_definition_mixed_params(void)
{
    printf("\n\n------------ Inicio test: test_print_function_definition_mixed_params ------------\n");
    global_tables_init();

    printf("Caso: function f(a: Number, b, c: String) => 1 (linea 4, columna 1)\n");
    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("a", "Number", NULL, 4, 13));
    list_append(params, symbol_binding_create("b", NULL, NULL, 4, 24));
    list_append(params, symbol_binding_create("c", "String", NULL, 4, 27));
    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 4, 42);
    FunctionDefinitionNode *node = ast_function_definition_create("f", params, NULL, body, 4, 1);

    PrintVisitor *pv = print_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_print_function_definition_with_return_annotation(void)
{
    printf("\n\n------------ Inicio test: test_print_function_definition_with_return_annotation ------------\n");
    global_tables_init();

    printf("Caso: function greet(name: String): String => \"Hello\" (linea 5, columna 1)\n");
    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("name", "String", NULL, 5, 17));
    ASTNode *body = (ASTNode *)ast_string_literal_create("Hello", 5, 42);
    FunctionDefinitionNode *node = ast_function_definition_create("greet", params, "String", body, 5, 1);

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
    printf("\n=== Iniciando tests de FunctionDefinitionNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionDefinitionNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_function_definition_no_params", test_print_function_definition_no_params);
    CU_add_test(suite, "test_print_function_definition_all_annotated", test_print_function_definition_all_annotated);
    CU_add_test(suite, "test_print_function_definition_none_annotated", test_print_function_definition_none_annotated);
    CU_add_test(suite, "test_print_function_definition_mixed_params", test_print_function_definition_mixed_params);
    CU_add_test(suite, "test_print_function_definition_with_return_annotation", test_print_function_definition_with_return_annotation);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}