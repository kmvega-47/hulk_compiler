#include <CUnit/Basic.h>
#include "ast.h"

void test_create_function_definition_no_params(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_no_params ------------\n");
    global_tables_init();

    printf("Caso: function foo() => 42 (linea 1)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 20);
    FunctionDefinitionNode *node = ast_function_definition_create("foo", params, NULL, body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_FUNCTION_DEFINITION);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_FUNCTION_DEFINITION);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_STRING_EQUAL(node->name, "foo");
    printf("name = '%s' (esperado: 'foo')\n", node->name);

    CU_ASSERT_PTR_NOT_NULL(node->params);
    CU_ASSERT_EQUAL(list_count(node->params), 0);
    printf("params count = %zu (esperado: 0)\n", list_count(node->params));

    CU_ASSERT_PTR_NULL(node->return_type_annotation);
    printf("return_type_annotation = %p (esperado: NULL)\n", (void *)node->return_type_annotation);

    CU_ASSERT_PTR_EQUAL(node->body, body);
    printf("body = %p (esperado: %p)\n", (void *)node->body, (void *)body);

    list_destroy(node->params);
    free(node->name);
    free(node);
    free(body);
    global_tables_destroy();
}

void test_create_function_definition_all_annotated(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_all_annotated ------------\n");
    global_tables_init();

    printf("Caso: function add(x: Number, y: Number) => 0 (linea 1)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 1, 14));
    list_append(params, symbol_binding_create("y", "Number", NULL, 1, 26));

    ASTNode *body = (ASTNode *)ast_number_literal_create(0.0, 1, 38);
    FunctionDefinitionNode *node = ast_function_definition_create("add", params, NULL, body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "add");
    printf("name = '%s' (esperado: 'add')\n", node->name);

    CU_ASSERT_EQUAL(list_count(node->params), 2);
    printf("params count = %zu (esperado: 2)\n", list_count(node->params));

    SymbolBinding *p0 = (SymbolBinding *)list_get(node->params, 0);
    CU_ASSERT_STRING_EQUAL(p0->name, "x");
    CU_ASSERT_STRING_EQUAL(p0->type_name, "Number");
    printf("param[0]: name='%s', type_name='%s'\n", p0->name, p0->type_name);

    SymbolBinding *p1 = (SymbolBinding *)list_get(node->params, 1);
    CU_ASSERT_STRING_EQUAL(p1->name, "y");
    CU_ASSERT_STRING_EQUAL(p1->type_name, "Number");
    printf("param[1]: name='%s', type_name='%s'\n", p1->name, p1->type_name);

    list_destroy(node->params);
    free(node->name);
    free(node);
    free(body);
    global_tables_destroy();
}

void test_create_function_definition_none_annotated(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_none_annotated ------------\n");
    global_tables_init();

    printf("Caso: function multiply(a, b) => 1 (linea 1)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("a", NULL, NULL, 1, 19));
    list_append(params, symbol_binding_create("b", NULL, NULL, 1, 22));

    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 1, 27);
    FunctionDefinitionNode *node = ast_function_definition_create("multiply", params, NULL, body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "multiply");
    printf("name = '%s' (esperado: 'multiply')\n", node->name);

    CU_ASSERT_EQUAL(list_count(node->params), 2);
    printf("params count = %zu (esperado: 2)\n", list_count(node->params));

    SymbolBinding *p0 = (SymbolBinding *)list_get(node->params, 0);
    CU_ASSERT_STRING_EQUAL(p0->name, "a");
    CU_ASSERT_PTR_NULL(p0->type_name);
    printf("param[0]: name='%s', type_name=%p (esperado: NULL)\n", p0->name, (void *)p0->type_name);

    SymbolBinding *p1 = (SymbolBinding *)list_get(node->params, 1);
    CU_ASSERT_STRING_EQUAL(p1->name, "b");
    CU_ASSERT_PTR_NULL(p1->type_name);
    printf("param[1]: name='%s', type_name=%p (esperado: NULL)\n", p1->name, (void *)p1->type_name);

    list_destroy(node->params);
    free(node->name);
    free(node);
    free(body);
    global_tables_destroy();
}

void test_create_function_definition_mixed_params(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_mixed_params ------------\n");
    global_tables_init();

    printf("Caso: function f(a: Number, b, c: String) => 1 (linea 1)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("a", "Number", NULL, 1, 13));
    list_append(params, symbol_binding_create("b", NULL, NULL, 1, 24));
    list_append(params, symbol_binding_create("c", "String", NULL, 1, 27));

    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 1, 42);
    FunctionDefinitionNode *node = ast_function_definition_create("f", params, NULL, body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(list_count(node->params), 3);
    printf("params count = %zu (esperado: 3)\n", list_count(node->params));

    SymbolBinding *p0 = (SymbolBinding *)list_get(node->params, 0);
    CU_ASSERT_STRING_EQUAL(p0->name, "a");
    CU_ASSERT_STRING_EQUAL(p0->type_name, "Number");
    printf("param[0]: name='%s', type_name='%s'\n", p0->name, p0->type_name);

    SymbolBinding *p1 = (SymbolBinding *)list_get(node->params, 1);
    CU_ASSERT_STRING_EQUAL(p1->name, "b");
    CU_ASSERT_PTR_NULL(p1->type_name);
    printf("param[1]: name='%s', type_name=%p (esperado: NULL)\n", p1->name, (void *)p1->type_name);

    SymbolBinding *p2 = (SymbolBinding *)list_get(node->params, 2);
    CU_ASSERT_STRING_EQUAL(p2->name, "c");
    CU_ASSERT_STRING_EQUAL(p2->type_name, "String");
    printf("param[2]: name='%s', type_name='%s'\n", p2->name, p2->type_name);

    list_destroy(node->params);
    free(node->name);
    free(node);
    free(body);
    global_tables_destroy();
}

void test_create_function_definition_with_return_annotation(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_with_return_annotation ------------\n");
    global_tables_init();

    printf("Caso: function greet(name: String): String => \"Hello\" (linea 1)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("name", "String", NULL, 1, 17));

    ASTNode *body = (ASTNode *)ast_string_literal_create("Hello", 1, 42);
    FunctionDefinitionNode *node = ast_function_definition_create("greet", params, "String", body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->name, "greet");
    printf("name = '%s' (esperado: 'greet')\n", node->name);

    CU_ASSERT_EQUAL(list_count(node->params), 1);
    printf("params count = %zu (esperado: 1)\n", list_count(node->params));

    CU_ASSERT_PTR_NOT_NULL(node->return_type_annotation);
    CU_ASSERT_STRING_EQUAL(node->return_type_annotation, "String");
    printf("return_type_annotation = '%s' (esperado: 'String')\n", node->return_type_annotation);

    CU_ASSERT_PTR_EQUAL(node->body, body);
    printf("body = %p (esperado: %p)\n", (void *)node->body, (void *)body);

    list_destroy(node->params);
    free(node->name);
    free(node->return_type_annotation);
    free(node);
    free(((LiteralNode *)body)->value.string_value);
    free(body);
    global_tables_destroy();
}

void test_create_function_definition_null_name(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_null_name ------------\n");
    global_tables_init();

    printf("Caso: name=NULL -> debe retornar NULL\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 1, 1);
    FunctionDefinitionNode *node = ast_function_definition_create(NULL, params, NULL, body, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(params);
    free(body);
    global_tables_destroy();
}

void test_create_function_definition_null_params(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_null_params ------------\n");
    global_tables_init();

    printf("Caso: params=NULL -> debe retornar NULL\n");

    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 1, 1);
    FunctionDefinitionNode *node = ast_function_definition_create("f", NULL, NULL, body, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(body);
    global_tables_destroy();
}

void test_create_function_definition_null_body(void)
{
    printf("\n\n------------ Inicio test: test_create_function_definition_null_body ------------\n");
    global_tables_init();

    printf("Caso: body=NULL -> debe retornar NULL\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    FunctionDefinitionNode *node = ast_function_definition_create("f", params, NULL, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(params);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de FunctionDefinitionNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionDefinitionNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_function_definition_no_params", test_create_function_definition_no_params);
    CU_add_test(suite, "test_create_function_definition_all_annotated", test_create_function_definition_all_annotated);
    CU_add_test(suite, "test_create_function_definition_none_annotated", test_create_function_definition_none_annotated);
    CU_add_test(suite, "test_create_function_definition_mixed_params", test_create_function_definition_mixed_params);
    CU_add_test(suite, "test_create_function_definition_with_return_annotation", test_create_function_definition_with_return_annotation);
    CU_add_test(suite, "test_create_function_definition_null_name", test_create_function_definition_null_name);
    CU_add_test(suite, "test_create_function_definition_null_params", test_create_function_definition_null_params);
    CU_add_test(suite, "test_create_function_definition_null_body", test_create_function_definition_null_body);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}