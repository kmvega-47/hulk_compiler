#include <CUnit/Basic.h>
#include "ast.h"

void test_create_type_definition_simple(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_simple ------------\n");
    global_tables_init();

    printf("Caso: type Simple {} (linea 1, columna 1)\n");

    List *init_params      = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes       = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods          = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Simple", init_params, NULL,
                                                          father_init_args, attributes, methods, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_TYPE_DEFINITION);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_TYPE_DEFINITION);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_STRING_EQUAL(node->name, "Simple");
    printf("name = '%s' (esperado: 'Simple')\n", node->name);

    CU_ASSERT_STRING_EQUAL(node->father_name, "Object");
    printf("father_name = '%s' (esperado: 'Object')\n", node->father_name);

    CU_ASSERT_EQUAL(list_count(node->init_params), 0);
    printf("init_params count = %zu (esperado: 0)\n", list_count(node->init_params));

    CU_ASSERT_EQUAL(list_count(node->father_init_args), 0);
    printf("father_init_args count = %zu (esperado: 0)\n", list_count(node->father_init_args));

    CU_ASSERT_EQUAL(list_count(node->attributes), 0);
    printf("attributes count = %zu (esperado: 0)\n", list_count(node->attributes));

    CU_ASSERT_EQUAL(list_count(node->methods), 0);
    printf("methods count = %zu (esperado: 0)\n", list_count(node->methods));

    list_destroy(node->init_params);
    list_destroy(node->father_init_args);
    list_destroy(node->attributes);
    list_destroy(node->methods);
    free(node->father_name);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_type_definition_init_params(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_init_params ------------\n");
    global_tables_init();

    printf("Caso: type Point(x, y) {} (linea 1, columna 1)\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(init_params, symbol_binding_create("x", NULL, NULL, 1, 12));
    list_append(init_params, symbol_binding_create("y", NULL, NULL, 1, 15));

    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes       = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods          = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Point", init_params, NULL,
                                                          father_init_args, attributes, methods, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "Point");
    printf("name = '%s' (esperado: 'Point')\n", node->name);

    CU_ASSERT_EQUAL(list_count(node->init_params), 2);
    printf("init_params count = %zu (esperado: 2)\n", list_count(node->init_params));

    SymbolBinding *p0 = (SymbolBinding *)list_get(node->init_params, 0);
    CU_ASSERT_STRING_EQUAL(p0->name, "x");
    printf("init_params[0].name = '%s' (esperado: 'x')\n", p0->name);

    SymbolBinding *p1 = (SymbolBinding *)list_get(node->init_params, 1);
    CU_ASSERT_STRING_EQUAL(p1->name, "y");
    printf("init_params[1].name = '%s' (esperado: 'y')\n", p1->name);

    list_destroy(node->init_params);
    list_destroy(node->father_init_args);
    list_destroy(node->attributes);
    list_destroy(node->methods);
    free(node->father_name);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_type_definition_with_father(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_with_father ------------\n");
    global_tables_init();

    printf("Caso: type Child inherits Parent {} (linea 1, columna 1)\n");

    List *init_params      = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes       = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods          = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Child", init_params, "Parent",
                                                          father_init_args, attributes, methods, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "Child");
    printf("name = '%s' (esperado: 'Child')\n", node->name);

    CU_ASSERT_STRING_EQUAL(node->father_name, "Parent");
    printf("father_name = '%s' (esperado: 'Parent')\n", node->father_name);

    list_destroy(node->init_params);
    list_destroy(node->father_init_args);
    list_destroy(node->attributes);
    list_destroy(node->methods);
    free(node->father_name);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_type_definition_father_args(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_father_args ------------\n");
    global_tables_init();

    printf("Caso: type Derived inherits Base(1, true) {} (linea 1, columna 1)\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(father_init_args, (void *)ast_number_literal_create(1.0, 1, 28));
    list_append(father_init_args, (void *)ast_bool_literal_create(true, 1, 31));

    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods    = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Derived", init_params, "Base",
                                                          father_init_args, attributes, methods, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "Derived");
    printf("name = '%s' (esperado: 'Derived')\n", node->name);

    CU_ASSERT_STRING_EQUAL(node->father_name, "Base");
    printf("father_name = '%s' (esperado: 'Base')\n", node->father_name);

    CU_ASSERT_EQUAL(list_count(node->father_init_args), 2);
    printf("father_init_args count = %zu (esperado: 2)\n", list_count(node->father_init_args));

    for (size_t i = 0; i < list_count(node->father_init_args); i++)
        free(list_get(node->father_init_args, i));
    list_destroy(node->father_init_args);
    list_destroy(node->init_params);
    list_destroy(node->attributes);
    list_destroy(node->methods);
    free(node->father_name);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_type_definition_attributes(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_attributes ------------\n");
    global_tables_init();

    printf("Caso: type Person { name = \"John\"; } (lineas 1-2)\n");

    List *init_params      = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes       = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);

    ASTNode *str_lit = (ASTNode *)ast_string_literal_create("John", 2, 12);
    list_append(attributes, symbol_binding_create("name", NULL, str_lit, 2, 5));

    List *methods = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Person", init_params, NULL,
                                                          father_init_args, attributes, methods, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "Person");
    printf("name = '%s' (esperado: 'Person')\n", node->name);

    CU_ASSERT_EQUAL(list_count(node->attributes), 1);
    printf("attributes count = %zu (esperado: 1)\n", list_count(node->attributes));

    SymbolBinding *attr = (SymbolBinding *)list_get(node->attributes, 0);
    CU_ASSERT_STRING_EQUAL(attr->name, "name");
    printf("attribute.name = '%s' (esperado: 'name')\n", attr->name);

    list_destroy(node->init_params);
    list_destroy(node->father_init_args);
    list_destroy(node->attributes);
    free(((LiteralNode *)str_lit)->value.string_value);
    free(str_lit);
    list_destroy(node->methods);
    free(node->father_name);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_type_definition_methods(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_methods ------------\n");
    global_tables_init();

    printf("Caso: type A { f() => 42; } (lineas 1-2)\n");

    List *init_params      = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes       = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods          = list_create(0, NULL, NULL, NULL, NULL);

    List *method_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 2, 12);
    FunctionDefinitionNode *method = ast_function_definition_create("f", method_params, NULL, body, 2, 5);
    list_append(methods, method);

    TypeDefinitionNode *node = ast_type_definition_create("A", init_params, NULL,
                                                          father_init_args, attributes, methods, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "A");
    printf("name = '%s' (esperado: 'A')\n", node->name);

    CU_ASSERT_EQUAL(list_count(node->methods), 1);
    printf("methods count = %zu (esperado: 1)\n", list_count(node->methods));

    for (size_t i = 0; i < list_count(node->methods); i++)
    {
        FunctionDefinitionNode *fm = (FunctionDefinitionNode *)list_get(node->methods, i);
        free(fm->body);
        free(fm->name);
        if (fm->params) list_destroy(fm->params);
        free(fm->return_type_annotation);
        free(fm);
    }
    list_destroy(node->methods);
    list_destroy(node->init_params);
    list_destroy(node->father_init_args);
    list_destroy(node->attributes);
    free(node->father_name);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_type_definition_complete(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_complete ------------\n");
    global_tables_init();

    printf("Caso: type Point(x: Number, y: Number) inherits Base(1) { x = x; y = y; getX() => x; } (lineas 1-3)\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(init_params, symbol_binding_create("x", "Number", NULL, 1, 12));
    list_append(init_params, symbol_binding_create("y", "Number", NULL, 1, 24));

    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *base_arg = (ASTNode *)ast_number_literal_create(1.0, 1, 46);
    list_append(father_init_args, base_arg);

    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *attr_x = (ASTNode *)ast_variable_reference_create("x", 2, 9);
    ASTNode *attr_y = (ASTNode *)ast_variable_reference_create("y", 2, 15);
    list_append(attributes, symbol_binding_create("x", NULL, attr_x, 2, 5));
    list_append(attributes, symbol_binding_create("y", NULL, attr_y, 2, 11));

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *method_body = (ASTNode *)ast_variable_reference_create("x", 3, 15);
    FunctionDefinitionNode *method = ast_function_definition_create("getX", m_params, NULL, method_body, 3, 5);
    list_append(methods, method);

    TypeDefinitionNode *node = ast_type_definition_create("Point", init_params, "Base",
                                                          father_init_args, attributes, methods, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "Point");
    printf("name = '%s' (esperado: 'Point')\n", node->name);

    CU_ASSERT_STRING_EQUAL(node->father_name, "Base");
    printf("father_name = '%s' (esperado: 'Base')\n", node->father_name);

    CU_ASSERT_EQUAL(list_count(node->init_params), 2);
    printf("init_params count = %zu (esperado: 2)\n", list_count(node->init_params));

    CU_ASSERT_EQUAL(list_count(node->father_init_args), 1);
    printf("father_init_args count = %zu (esperado: 1)\n", list_count(node->father_init_args));

    CU_ASSERT_EQUAL(list_count(node->attributes), 2);
    printf("attributes count = %zu (esperado: 2)\n", list_count(node->attributes));

    CU_ASSERT_EQUAL(list_count(node->methods), 1);
    printf("methods count = %zu (esperado: 1)\n", list_count(node->methods));

    free(base_arg);
    list_destroy(node->father_init_args);

    list_destroy(node->attributes);
    free(((VariableReferenceNode *)attr_x)->name);
    free(attr_x);
    free(((VariableReferenceNode *)attr_y)->name);
    free(attr_y);

    for (size_t i = 0; i < list_count(node->methods); i++)
    {
        FunctionDefinitionNode *fm = (FunctionDefinitionNode *)list_get(node->methods, i);
        free(((VariableReferenceNode *)fm->body)->name);
        free(fm->body);
        free(fm->name);
        if (fm->params) list_destroy(fm->params);
        free(fm->return_type_annotation);
        free(fm);
    }
    list_destroy(node->methods);
    list_destroy(node->init_params);
    free(node->father_name);
    free(node->name);
    free(node);
    global_tables_destroy();
}

void test_create_type_definition_null_name(void)
{
    printf("\n\n------------ Inicio test: test_create_type_definition_null_name ------------\n");
    global_tables_init();

    printf("Caso: name=NULL -> debe retornar NULL\n");

    List *init_params      = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes       = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods          = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create(NULL, init_params, NULL,
                                                          father_init_args, attributes, methods, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(init_params);
    list_destroy(father_init_args);
    list_destroy(attributes);
    list_destroy(methods);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeDefinitionNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeDefinitionNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_type_definition_simple", test_create_type_definition_simple);
    CU_add_test(suite, "test_create_type_definition_init_params", test_create_type_definition_init_params);
    CU_add_test(suite, "test_create_type_definition_with_father", test_create_type_definition_with_father);
    CU_add_test(suite, "test_create_type_definition_father_args", test_create_type_definition_father_args);
    CU_add_test(suite, "test_create_type_definition_attributes", test_create_type_definition_attributes);
    CU_add_test(suite, "test_create_type_definition_methods", test_create_type_definition_methods);
    CU_add_test(suite, "test_create_type_definition_complete", test_create_type_definition_complete);
    CU_add_test(suite, "test_create_type_definition_null_name", test_create_type_definition_null_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}