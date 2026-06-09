#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_program_minimal(void)
{
    printf("\n\n------------ Inicio test: test_free_program_minimal ------------\n");
    global_tables_init();

    printf("Caso: 42\n");
    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);
    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *node = ast_program_create(root, funcs, types, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_program_with_functions(void)
{
    printf("\n\n------------ Inicio test: test_free_program_with_functions ------------\n");
    global_tables_init();

    printf("Caso: function foo() => 1; function bar() => 2; 42\n");
    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *params1 = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body1 = (ASTNode *)ast_number_literal_create(1.0, 2, 10);
    list_append(funcs, ast_function_definition_create("foo", params1, NULL, body1, 2, 1));

    List *params2 = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body2 = (ASTNode *)ast_number_literal_create(2.0, 3, 10);
    list_append(funcs, ast_function_definition_create("bar", params2, NULL, body2, 3, 1));

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *node = ast_program_create(root, funcs, types, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_program_with_types(void)
{
    printf("\n\n------------ Inicio test: test_free_program_with_types ------------\n");
    global_tables_init();

    printf("Caso: type Point { x = 1; } 42\n");
    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *types = list_create(0, NULL, NULL, NULL, NULL);

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(at, symbol_binding_create("x", NULL, (ASTNode *)ast_number_literal_create(1.0, 2, 9), 2, 5));
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *type = ast_type_definition_create("Point", ip, NULL, fa, at, mt, 3, 1);
    list_append(types, type);

    ProgramNode *node = ast_program_create(root, funcs, types, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_program_complete(void)
{
    printf("\n\n------------ Inicio test: test_free_program_complete ------------\n");
    global_tables_init();

    printf("Caso: type Point { x = 1; } function foo() => 42; new Point(1)\n");
    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 2, 10);
    list_append(funcs, ast_function_definition_create("foo", params, NULL, body, 2, 1));

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(at, symbol_binding_create("x", NULL, (ASTNode *)ast_number_literal_create(1.0, 4, 9), 4, 5));
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *type = ast_type_definition_create("Point", ip, NULL, fa, at, mt, 3, 1);
    list_append(types, type);

    ProgramNode *node = ast_program_create(root, funcs, types, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ProgramNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ProgramNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_program_minimal", test_free_program_minimal);
    CU_add_test(suite, "test_free_program_with_functions", test_free_program_with_functions);
    CU_add_test(suite, "test_free_program_with_types", test_free_program_with_types);
    CU_add_test(suite, "test_free_program_complete", test_free_program_complete);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}