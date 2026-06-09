#include <CUnit/Basic.h>
#include "ast.h"

void test_create_program_minimal(void)
{
    printf("\n\n------------ Inicio test: test_create_program_minimal ------------\n");
    global_tables_init();

    printf("Caso: 42 (linea 1, columna 1)\n");

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);
    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *types = list_create(0, NULL, NULL, NULL, NULL);

    ProgramNode *node = ast_program_create(root, funcs, types, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->base.node_type, NODE_PROGRAM);
    printf("node_type = %d (esperado: %d)\n", node->base.node_type, NODE_PROGRAM);

    CU_ASSERT_EQUAL(node->base.line, 1);
    printf("line = %d (esperado: 1)\n", node->base.line);

    CU_ASSERT_EQUAL(node->base.column, 1);
    printf("column = %d (esperado: 1)\n", node->base.column);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node->base.return_type);

    CU_ASSERT_PTR_EQUAL(node->root, root);
    printf("root = %p (esperado: %p)\n", (void *)node->root, (void *)root);

    CU_ASSERT_PTR_NOT_NULL(node->function_definitions);
    CU_ASSERT_EQUAL(list_count(node->function_definitions), 0);
    printf("function_definitions count = %zu (esperado: 0)\n", list_count(node->function_definitions));

    CU_ASSERT_PTR_NOT_NULL(node->type_definitions);
    CU_ASSERT_EQUAL(list_count(node->type_definitions), 0);
    printf("type_definitions count = %zu (esperado: 0)\n", list_count(node->type_definitions));

    free(root);
    list_destroy(node->function_definitions);
    list_destroy(node->type_definitions);
    free(node);
    global_tables_destroy();
}

void test_create_program_with_functions(void)
{
    printf("\n\n------------ Inicio test: test_create_program_with_functions ------------\n");
    global_tables_init();

    printf("Caso: program with 2 functions (linea 1)\n");

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

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(list_count(node->function_definitions), 2);
    printf("function_definitions count = %zu (esperado: 2)\n", list_count(node->function_definitions));

    CU_ASSERT_EQUAL(list_count(node->type_definitions), 0);
    printf("type_definitions count = %zu (esperado: 0)\n", list_count(node->type_definitions));

    for (size_t i = 0; i < list_count(node->function_definitions); i++)
    {
        FunctionDefinitionNode *fn = (FunctionDefinitionNode *)list_get(node->function_definitions, i);
        free(fn->body);
        free(fn->name);
        list_destroy(fn->params);
        free(fn->return_type_annotation);
        free(fn);
    }
    list_destroy(node->function_definitions);
    list_destroy(node->type_definitions);
    free(root);
    free(node);
    global_tables_destroy();
}

void test_create_program_with_types(void)
{
    printf("\n\n------------ Inicio test: test_create_program_with_types ------------\n");
    global_tables_init();

    printf("Caso: program with 1 type (linea 1)\n");

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);
    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *types = list_create(0, NULL, NULL, NULL, NULL);

    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *attr_init = (ASTNode *)ast_number_literal_create(1.0, 2, 9);
    list_append(at, symbol_binding_create("x", NULL, attr_init, 2, 5));
    List *mt = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *type = ast_type_definition_create("Point", ip, NULL, fa, at, mt, 1, 1);
    list_append(types, type);

    ProgramNode *node = ast_program_create(root, funcs, types, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(list_count(node->function_definitions), 0);
    printf("function_definitions count = %zu (esperado: 0)\n", list_count(node->function_definitions));

    CU_ASSERT_EQUAL(list_count(node->type_definitions), 1);
    printf("type_definitions count = %zu (esperado: 1)\n", list_count(node->type_definitions));

    for (size_t i = 0; i < list_count(node->type_definitions); i++)
    {
        TypeDefinitionNode *td = (TypeDefinitionNode *)list_get(node->type_definitions, i);
        list_destroy(td->init_params);
        for (size_t j = 0; j < list_count(td->father_init_args); j++)
            free(list_get(td->father_init_args, j));
        list_destroy(td->father_init_args);
        list_destroy(td->attributes);
        free(attr_init);
        list_destroy(td->methods);
        free(td->father_name);
        free(td->name);
        free(td);
    }
    list_destroy(node->type_definitions);
    list_destroy(node->function_definitions);
    free(root);
    free(node);
    global_tables_destroy();
}

void test_create_program_complete(void)
{
    printf("\n\n------------ Inicio test: test_create_program_complete ------------\n");
    global_tables_init();

    printf("Caso: program with 1 type and 1 function (linea 1)\n");

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 2, 10);
    list_append(funcs, ast_function_definition_create("foo", params, NULL, body, 2, 1));

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    List *ip = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *fa = list_create(0, NULL, NULL, NULL, NULL);
    List *at = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *attr_init = (ASTNode *)ast_number_literal_create(1.0, 4, 9);
    list_append(at, symbol_binding_create("x", NULL, attr_init, 4, 5));
    List *mt = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, ast_type_definition_create("Point", ip, NULL, fa, at, mt, 3, 1));

    ProgramNode *node = ast_program_create(root, funcs, types, 1, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(list_count(node->function_definitions), 1);
    printf("function_definitions count = %zu (esperado: 1)\n", list_count(node->function_definitions));

    CU_ASSERT_EQUAL(list_count(node->type_definitions), 1);
    printf("type_definitions count = %zu (esperado: 1)\n", list_count(node->type_definitions));

    for (size_t i = 0; i < list_count(node->function_definitions); i++)
    {
        FunctionDefinitionNode *f = (FunctionDefinitionNode *)list_get(node->function_definitions, i);
        free(f->body);
        free(f->name);
        list_destroy(f->params);
        free(f->return_type_annotation);
        free(f);
    }
    list_destroy(node->function_definitions);

    for (size_t i = 0; i < list_count(node->type_definitions); i++)
    {
        TypeDefinitionNode *td = (TypeDefinitionNode *)list_get(node->type_definitions, i);
        list_destroy(td->init_params);
        for (size_t j = 0; j < list_count(td->father_init_args); j++)
            free(list_get(td->father_init_args, j));
        list_destroy(td->father_init_args);
        list_destroy(td->attributes);
        free(attr_init);
        list_destroy(td->methods);
        free(td->father_name);
        free(td->name);
        free(td);
    }
    list_destroy(node->type_definitions);

    free(root);
    free(node);
    global_tables_destroy();
}

void test_create_program_null_root(void)
{
    printf("\n\n------------ Inicio test: test_create_program_null_root ------------\n");
    global_tables_init();

    printf("Caso: root=NULL -> debe retornar NULL\n");

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *types = list_create(0, NULL, NULL, NULL, NULL);

    ProgramNode *node = ast_program_create(NULL, funcs, types, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    list_destroy(funcs);
    list_destroy(types);
    global_tables_destroy();
}

void test_create_program_null_funcs(void)
{
    printf("\n\n------------ Inicio test: test_create_program_null_funcs ------------\n");
    global_tables_init();

    printf("Caso: function_definitions=NULL -> debe retornar NULL\n");

    ASTNode *root = (ASTNode *)ast_number_literal_create(1.0, 1, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);

    ProgramNode *node = ast_program_create(root, NULL, types, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(root);
    list_destroy(types);
    global_tables_destroy();
}

void test_create_program_null_types(void)
{
    printf("\n\n------------ Inicio test: test_create_program_null_types ------------\n");
    global_tables_init();

    printf("Caso: type_definitions=NULL -> debe retornar NULL\n");

    ASTNode *root = (ASTNode *)ast_number_literal_create(1.0, 1, 1);
    List *funcs = list_create(0, NULL, NULL, NULL, NULL);

    ProgramNode *node = ast_program_create(root, funcs, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(node);
    printf("Resultado: %s (esperado: NULL)\n", node ? "not null" : "NULL");

    free(root);
    list_destroy(funcs);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ProgramNode (create) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ProgramNode Create Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_create_program_minimal", test_create_program_minimal);
    CU_add_test(suite, "test_create_program_with_functions", test_create_program_with_functions);
    CU_add_test(suite, "test_create_program_with_types", test_create_program_with_types);
    CU_add_test(suite, "test_create_program_complete", test_create_program_complete);
    CU_add_test(suite, "test_create_program_null_root", test_create_program_null_root);
    CU_add_test(suite, "test_create_program_null_funcs", test_create_program_null_funcs);
    CU_add_test(suite, "test_create_program_null_types", test_create_program_null_types);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}