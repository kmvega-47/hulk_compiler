#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

static void register_function(const char *name, List *param_types, TypeDescriptor *return_type)
{
    function_table_insert(global_function_table, name, return_type, param_types);
}

void test_inference_function_definition_no_params(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_definition_no_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function foo() => 42 (linea 1, columna 1)\n");

    List *empty_params = list_create(0, NULL, NULL, NULL, NULL);
    register_function("foo", empty_params, NULL);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 20);
    FunctionDefinitionNode *node = ast_function_definition_create("foo", params, NULL, body, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del nodo funcion: %s (esperado: Void)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));

    bool found = false;
    TypeDescriptor *ret = function_table_get_return_type(global_function_table, "foo", &found);
    CU_ASSERT_TRUE(found);
    printf("Tipo de retorno en tabla: %s (esperado: Number)\n", ret->name);
    CU_ASSERT_PTR_EQUAL(ret, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_function_definition_annotated_params(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_definition_annotated_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function add(x: Number, y: Number) => x + y (linea 2, columna 1)\n");

    List *param_types = list_create(2, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER), type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    register_function("add", param_types, NULL);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 2, 14));
    list_append(params, symbol_binding_create("y", "Number", NULL, 2, 26));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 2, 30);
    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 2, 32);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_ADD, x_ref, y_ref, 2, 31);
    FunctionDefinitionNode *node = ast_function_definition_create("add", params, NULL, body, 2, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo de x_ref: %s (Number)\n", x_ref->return_type->name);
    printf("Tipo de y_ref: %s (Number)\n", y_ref->return_type->name);
    CU_ASSERT_PTR_EQUAL(x_ref->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(y_ref->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    printf("Tipo del nodo funcion: %s (Void)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));

    bool found = false;
    TypeDescriptor *ret = function_table_get_return_type(global_function_table, "add", &found);
    CU_ASSERT_TRUE(found);
    printf("Tipo de retorno en tabla: %s (esperado: Number)\n", ret->name);
    CU_ASSERT_PTR_EQUAL(ret, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_function_definition_return_annotation(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_definition_return_annotation ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function square(x: Number): Number => x * x (linea 3, columna 1)\n");

    List *param_types = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    TypeDescriptor *ret_annotated = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    register_function("square", param_types, ret_annotated);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 3, 17));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 3, 32);
    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 3, 36);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_MULT, x1, x2, 3, 34);
    FunctionDefinitionNode *node = ast_function_definition_create("square", params, "Number", body, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));

    bool found = false;
    TypeDescriptor *ret = function_table_get_return_type(global_function_table, "square", &found);
    CU_ASSERT_TRUE(found);
    CU_ASSERT_PTR_EQUAL(ret, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Tipo de retorno: %s (esperado: Number)\n", ret->name);

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_function_definition_parameter_shadowing(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_definition_parameter_shadowing ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function shadow(x: Number) => { let x = \"hello\" in x; x } (linea 4, columna 1)\n");

    List *param_types = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    register_function("shadow", param_types, NULL);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 4, 17));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(bindings, symbol_binding_create("x", NULL, (ASTNode *)ast_string_literal_create("hello", 4, 42), 4, 38));
    ASTNode *inner_x = (ASTNode *)ast_variable_reference_create("x", 4, 52);
    ASTNode *let_in = (ASTNode *)ast_let_create(bindings, inner_x, 4, 30);

    ASTNode *outer_x = (ASTNode *)ast_variable_reference_create("x", 4, 55);

    List *exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(exprs, let_in);
    list_append(exprs, outer_x);
    ASTNode *body = (ASTNode *)ast_block_create(exprs, 4, 28);

    FunctionDefinitionNode *node = ast_function_definition_create("shadow", params, NULL, body, 4, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(inner_x->return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_PTR_EQUAL(outer_x->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(body->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    printf("inner_x: String, outer_x: Number, body: Number\n");

    bool found = false;
    TypeDescriptor *ret = function_table_get_return_type(global_function_table, "shadow", &found);
    CU_ASSERT_TRUE(found);
    CU_ASSERT_PTR_EQUAL(ret, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Tipo de retorno: Number\n");

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de FunctionDefinitionNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionDefinitionNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_function_definition_no_params", test_inference_function_definition_no_params);
    CU_add_test(suite, "test_inference_function_definition_annotated_params", test_inference_function_definition_annotated_params);
    CU_add_test(suite, "test_inference_function_definition_return_annotation", test_inference_function_definition_return_annotation);
    CU_add_test(suite, "test_inference_function_definition_parameter_shadowing", test_inference_function_definition_parameter_shadowing);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}