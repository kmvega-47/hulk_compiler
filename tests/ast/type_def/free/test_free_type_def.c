#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_type_definition_simple(void)
{
    printf("\n\n------------ Inicio test: test_free_type_definition_simple ------------\n");
    global_tables_init();
    printf("Caso: type Simple {}\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Simple", ip, NULL, fa, at, mt, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);
    global_tables_destroy();
}

void test_free_type_definition_init_params(void)
{
    printf("\n\n------------ Inicio test: test_free_type_definition_init_params ------------\n");
    global_tables_init();
    printf("Caso: type Point(x, y) {}\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(ip, symbol_binding_create("x", NULL, NULL, 1, 12));
    list_append(ip, symbol_binding_create("y", NULL, NULL, 1, 15));
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Point", ip, NULL, fa, at, mt, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);
    global_tables_destroy();
}

void test_free_type_definition_with_father(void)
{
    printf("\n\n------------ Inicio test: test_free_type_definition_with_father ------------\n");
    global_tables_init();
    printf("Caso: type Child inherits Parent {}\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Child", ip, "Parent", fa, at, mt, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);
    global_tables_destroy();
}

void test_free_type_definition_father_args(void)
{
    printf("\n\n------------ Inicio test: test_free_type_definition_father_args ------------\n");
    global_tables_init();
    printf("Caso: type Derived inherits Base(1, true) {}\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    list_append(fa, ast_number_literal_create(1.0, 1, 28));
    list_append(fa, ast_bool_literal_create(true, 1, 31));
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Derived", ip, "Base", fa, at, mt, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);
    global_tables_destroy();
}

void test_free_type_definition_attributes(void)
{
    printf("\n\n------------ Inicio test: test_free_type_definition_attributes ------------\n");
    global_tables_init();
    printf("Caso: type Person { name = \"John\"; }\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(at, symbol_binding_create("name", NULL, (ASTNode *)ast_string_literal_create("John", 1, 14), 1, 5));
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Person", ip, NULL, fa, at, mt, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);
    global_tables_destroy();
}

void test_free_type_definition_methods(void)
{
    printf("\n\n------------ Inicio test: test_free_type_definition_methods ------------\n");
    global_tables_init();
    printf("Caso: type A { f() => 42; }\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *mt = list_create(0, NULL, NULL, NULL, NULL);

    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 2, 12);
    FunctionDefinitionNode *m = ast_function_definition_create("f", mp, NULL, body, 2, 5);
    list_append(mt, m);

    TypeDefinitionNode *node = ast_type_definition_create("A", ip, NULL, fa, at, mt, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);
    global_tables_destroy();
}

void test_free_type_definition_complete(void)
{
    printf("\n\n------------ Inicio test: test_free_type_definition_complete ------------\n");
    global_tables_init();
    printf("Caso: type Point(x: Number, y: Number) inherits Base(1) { x = x; y = y; getX() => x; }\n");

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(ip, symbol_binding_create("x", "Number", NULL, 1, 12));
    list_append(ip, symbol_binding_create("y", "Number", NULL, 1, 24));

    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    list_append(fa, ast_number_literal_create(1.0, 1, 46));

    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(at, symbol_binding_create("x", NULL, (ASTNode *)ast_variable_reference_create("x", 2, 9), 2, 5));
    list_append(at, symbol_binding_create("y", NULL, (ASTNode *)ast_variable_reference_create("y", 2, 15), 2, 11));

    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_variable_reference_create("x", 3, 15);
    FunctionDefinitionNode *m = ast_function_definition_create("getX", mp, NULL, body, 3, 5);
    list_append(mt, m);

    TypeDefinitionNode *node = ast_type_definition_create("Point", ip, "Base", fa, at, mt, 1, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeDefinitionNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeDefinitionNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_type_definition_simple", test_free_type_definition_simple);
    CU_add_test(suite, "test_free_type_definition_init_params", test_free_type_definition_init_params);
    CU_add_test(suite, "test_free_type_definition_with_father", test_free_type_definition_with_father);
    CU_add_test(suite, "test_free_type_definition_father_args", test_free_type_definition_father_args);
    CU_add_test(suite, "test_free_type_definition_attributes", test_free_type_definition_attributes);
    CU_add_test(suite, "test_free_type_definition_methods", test_free_type_definition_methods);
    CU_add_test(suite, "test_free_type_definition_complete", test_free_type_definition_complete);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}