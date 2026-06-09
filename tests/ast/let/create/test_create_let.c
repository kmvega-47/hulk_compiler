#include <CUnit/Basic.h>
#include "ast.h"

void test_create_let_single_binding(void)
{
    printf("\n\n------------ Inicio test: test_create_let_single_binding ------------\n");
    global_tables_init();

    printf("Caso: let x = 42 in 99 (linea 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 1, 9);
    SymbolBinding *b = symbol_binding_create("x", NULL, init, 1, 5);
    list_append(bindings, b);

    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 15);

    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_LET_IN);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_LET_IN);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_EQUAL(node->bindings, bindings);
    printf("bindings = %p (esperado: %p)\n", (void *)node->bindings, (void *)bindings);

    CU_ASSERT_PTR_EQUAL(node->body, body);
    printf("body = %p (esperado: %p)\n", (void *)node->body, (void *)body);

    CU_ASSERT_EQUAL(list_count(node->bindings), 1);
    printf("bindings count = %zu (esperado: 1)\n", list_count(node->bindings));

    SymbolBinding *b0 = (SymbolBinding *)list_get(bindings, 0);
    CU_ASSERT_STRING_EQUAL(b0->name, "x");
    printf("binding[0].name = '%s' (esperado: 'x')\n", b0->name);

    CU_ASSERT_PTR_EQUAL(b0->initializer, init);
    printf("binding[0].initializer = %p (esperado: %p)\n", (void *)b0->initializer, (void *)init);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    free(init);
    list_destroy(bindings);
    free(body);
    free(node);
    global_tables_destroy();
}

void test_create_let_multiple_bindings(void)
{
    printf("\n\n------------ Inicio test: test_create_let_multiple_bindings ------------\n");
    global_tables_init();

    printf("Caso: let a = 1, b = true, c = \"hello\" in 42 (lineas 1-3)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);

    ASTNode *init_a = (ASTNode *)ast_number_literal_create(1.0, 1, 9);
    list_append(bindings, symbol_binding_create("a", NULL, init_a, 1, 5));

    ASTNode *init_b = (ASTNode *)ast_bool_literal_create(true, 2, 9);
    list_append(bindings, symbol_binding_create("b", NULL, init_b, 2, 5));

    ASTNode *init_c = (ASTNode *)ast_string_literal_create("hello", 3, 9);
    list_append(bindings, symbol_binding_create("c", NULL, init_c, 3, 5));

    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 4, 5);

    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(list_count(node->bindings), 3);
    printf("bindings count = %zu (esperado: 3)\n", list_count(node->bindings));

    SymbolBinding *b0 = (SymbolBinding *)list_get(bindings, 0);
    CU_ASSERT_STRING_EQUAL(b0->name, "a");
    printf("binding[0].name = '%s' (esperado: 'a')\n", b0->name);

    SymbolBinding *b1 = (SymbolBinding *)list_get(bindings, 1);
    CU_ASSERT_STRING_EQUAL(b1->name, "b");
    printf("binding[1].name = '%s' (esperado: 'b')\n", b1->name);

    SymbolBinding *b2 = (SymbolBinding *)list_get(bindings, 2);
    CU_ASSERT_STRING_EQUAL(b2->name, "c");
    printf("binding[2].name = '%s' (esperado: 'c')\n", b2->name);

    free(init_a);
    free(init_b);
    free(((LiteralNode *)init_c)->value.string_value);
    free(init_c);
    list_destroy(bindings);
    free(body);
    free(node);
    global_tables_destroy();
}

void test_create_let_nested(void)
{
    printf("\n\n------------ Inicio test: test_create_let_nested ------------\n");
    global_tables_init();

    printf("Caso: let x = 1 in (let y = 2 in 3 + 4) (linea 1)\n");

    List *inner_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *inner_init = (ASTNode *)ast_number_literal_create(2.0, 1, 24);
    list_append(inner_bindings, symbol_binding_create("y", NULL, inner_init, 1, 20));

    LiteralNode *a = ast_number_literal_create(3.0, 1, 29);
    LiteralNode *b = ast_number_literal_create(4.0, 1, 33);
    ASTNode *inner_body = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)a, (ASTNode *)b, 1, 31);

    LetInNode *inner_let = ast_let_create(inner_bindings, inner_body, 1, 17);

    List *outer_bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *outer_init = (ASTNode *)ast_number_literal_create(1.0, 1, 9);
    list_append(outer_bindings, symbol_binding_create("x", NULL, outer_init, 1, 5));

    LetInNode *outer_let = ast_let_create(outer_bindings, (ASTNode *)inner_let, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(outer_let);
    CU_ASSERT_EQUAL(outer_let->base.node_type, NODE_LET_IN);
    printf("node_type = %d (esperado: %d)\n", outer_let->base.node_type, NODE_LET_IN);

    CU_ASSERT_PTR_EQUAL(outer_let->body, (ASTNode *)inner_let);
    printf("body = %p (esperado: %p)\n", (void *)outer_let->body, (void *)inner_let);

    free(outer_init);
    list_destroy(outer_bindings);
    free(inner_init);
    list_destroy(inner_bindings);
    free(a); free(b);
    free(inner_body);
    free(inner_let);
    free(outer_let);
    global_tables_destroy();
}

void test_create_let_with_type_annotation(void)
{
    printf("\n\n------------ Inicio test: test_create_let_with_type_annotation ------------\n");
    global_tables_init();

    printf("Caso: let x: Number = 42 in 99 (linea 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 1, 17);
    SymbolBinding *b = symbol_binding_create("x", "Number", init, 1, 5);
    list_append(bindings, b);

    ASTNode *body = (ASTNode *)ast_number_literal_create(99.0, 1, 22);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    SymbolBinding *b0 = (SymbolBinding *)list_get(bindings, 0);
    CU_ASSERT_STRING_EQUAL(b0->name, "x");
    printf("name = '%s' (esperado: 'x')\n", b0->name);

    CU_ASSERT_PTR_NOT_NULL(b0->type_name);
    CU_ASSERT_STRING_EQUAL(b0->type_name, "Number");
    printf("type_name = '%s' (esperado: 'Number')\n", b0->type_name);

    free(init);
    list_destroy(bindings);
    free(body);
    free(node);
    global_tables_destroy();
}

void test_create_let_empty_bindings(void)
{
    printf("\n\n------------ Inicio test: test_create_let_empty_bindings ------------\n");
    global_tables_init();

    printf("Caso: bindings vacia -> debe retornar NULL\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 1, 1);
    LetInNode *node = ast_let_create(bindings, body, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(bindings);
    free(body);
    global_tables_destroy();
}

void test_create_let_null_bindings(void)
{
    printf("\n\n------------ Inicio test: test_create_let_null_bindings ------------\n");
    global_tables_init();

    printf("Caso: bindings=NULL -> debe retornar NULL\n");

    ASTNode *body = (ASTNode *)ast_number_literal_create(1.0, 1, 1);
    LetInNode *node = ast_let_create(NULL, body, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(body);
    global_tables_destroy();
}

void test_create_let_null_body(void)
{
    printf("\n\n------------ Inicio test: test_create_let_null_body ------------\n");
    global_tables_init();

    printf("Caso: body=NULL -> debe retornar NULL\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(42.0, 1, 9);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));

    LetInNode *node = ast_let_create(bindings, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(init);
    list_destroy(bindings);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de LetInNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LetInNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_let_single_binding", test_create_let_single_binding);
    CU_add_test(suite, "test_create_let_multiple_bindings", test_create_let_multiple_bindings);
    CU_add_test(suite, "test_create_let_nested", test_create_let_nested);
    CU_add_test(suite, "test_create_let_with_type_annotation", test_create_let_with_type_annotation);
    CU_add_test(suite, "test_create_let_empty_bindings", test_create_let_empty_bindings);
    CU_add_test(suite, "test_create_let_null_bindings", test_create_let_null_bindings);
    CU_add_test(suite, "test_create_let_null_body", test_create_let_null_body);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}