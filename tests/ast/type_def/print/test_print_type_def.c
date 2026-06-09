#include <CUnit/Basic.h>
#include "print_visitor.h"
#include "free_visitor.h"

static void run_print_and_free(ASTNode *node)
{
    PrintVisitor *pv = print_visitor_create();
    ast_accept(node, (Visitor *)pv);
    print_visitor_destroy(pv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept(node, (Visitor *)fv);
    free_visitor_destroy(fv);
}

void test_print_type_definition_simple(void)
{
    printf("\n\n------------ Inicio test: test_print_type_definition_simple ------------\n");
    global_tables_init();
    printf("Caso: type Simple {} (linea 1, columna 1)\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Simple", ip, NULL, fa, at, mt, 1, 1);

    run_print_and_free((ASTNode *)node);
    global_tables_destroy();
}

void test_print_type_definition_init_params(void)
{
    printf("\n\n------------ Inicio test: test_print_type_definition_init_params ------------\n");
    global_tables_init();
    printf("Caso: type Point(x, y) {} (linea 2, columna 1)\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(ip, symbol_binding_create("x", NULL, NULL, 2, 12));
    list_append(ip, symbol_binding_create("y", NULL, NULL, 2, 15));
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Point", ip, NULL, fa, at, mt, 2, 1);

    run_print_and_free((ASTNode *)node);
    global_tables_destroy();
}

void test_print_type_definition_with_father(void)
{
    printf("\n\n------------ Inicio test: test_print_type_definition_with_father ------------\n");
    global_tables_init();
    printf("Caso: type Child inherits Parent {} (linea 3, columna 1)\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Child", ip, "Parent", fa, at, mt, 3, 1);

    run_print_and_free((ASTNode *)node);
    global_tables_destroy();
}

void test_print_type_definition_father_args(void)
{
    printf("\n\n------------ Inicio test: test_print_type_definition_father_args ------------\n");
    global_tables_init();
    printf("Caso: type Derived inherits Base(1, true) {} (linea 4, columna 1)\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    list_append(fa, ast_number_literal_create(1.0, 4, 30));
    list_append(fa, ast_bool_literal_create(true, 4, 33));
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Derived", ip, "Base", fa, at, mt, 4, 1);

    run_print_and_free((ASTNode *)node);
    global_tables_destroy();
}

void test_print_type_definition_attributes(void)
{
    printf("\n\n------------ Inicio test: test_print_type_definition_attributes ------------\n");
    global_tables_init();
    printf("Caso: type Person { name = \"John\"; } (linea 5, columna 1)\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(at, symbol_binding_create("name", NULL, (ASTNode *)ast_string_literal_create("John", 5, 14), 5, 5));
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Person", ip, NULL, fa, at, mt, 5, 1);

    run_print_and_free((ASTNode *)node);
    global_tables_destroy();
}

void test_print_type_definition_methods(void)
{
    printf("\n\n------------ Inicio test: test_print_type_definition_methods ------------\n");
    global_tables_init();
    printf("Caso: type A { f() => 42; } (linea 6, columna 1)\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);

    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 6, 12);
    FunctionDefinitionNode *m = ast_function_definition_create("f", mp, NULL, body, 6, 5);
    list_append(mt, m);

    TypeDefinitionNode *node = ast_type_definition_create("A", ip, NULL, fa, at, mt, 6, 1);

    run_print_and_free((ASTNode *)node);
    global_tables_destroy();
}

void test_print_type_definition_complete(void)
{
    printf("\n\n------------ Inicio test: test_print_type_definition_complete ------------\n");
    global_tables_init();
    printf("Caso: type Point(x: Number, y: Number) inherits Base(1) { x = x; y = y; getX() => x; } (linea 7, columna 1)\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(ip, symbol_binding_create("x", "Number", NULL, 7, 12));
    list_append(ip, symbol_binding_create("y", "Number", NULL, 7, 24));

    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    list_append(fa, ast_number_literal_create(1.0, 7, 46));

    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(at, symbol_binding_create("x", NULL, (ASTNode *)ast_variable_reference_create("x", 7, 50), 7, 5));
    list_append(at, symbol_binding_create("y", NULL, (ASTNode *)ast_variable_reference_create("y", 7, 56), 7, 11));

    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_variable_reference_create("x", 7, 70);
    FunctionDefinitionNode *m = ast_function_definition_create("getX", mp, NULL, body, 7, 62);
    list_append(mt, m);

    TypeDefinitionNode *node = ast_type_definition_create("Point", ip, "Base", fa, at, mt, 7, 1);

    run_print_and_free((ASTNode *)node);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeDefinitionNode (print) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeDefinitionNode Print Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_print_type_definition_simple", test_print_type_definition_simple);
    CU_add_test(suite, "test_print_type_definition_init_params", test_print_type_definition_init_params);
    CU_add_test(suite, "test_print_type_definition_with_father", test_print_type_definition_with_father);
    CU_add_test(suite, "test_print_type_definition_father_args", test_print_type_definition_father_args);
    CU_add_test(suite, "test_print_type_definition_attributes", test_print_type_definition_attributes);
    CU_add_test(suite, "test_print_type_definition_methods", test_print_type_definition_methods);
    CU_add_test(suite, "test_print_type_definition_complete", test_print_type_definition_complete);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}