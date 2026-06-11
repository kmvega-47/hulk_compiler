#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_let_with_annotation(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_with_annotation ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let x: Number = y in x\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 20);
    list_append(bindings, symbol_binding_create("x", "Number", y1, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 30);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, x_ref, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para y: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_without_annotation(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_without_annotation ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let x = y in x (sin anotacion, y queda Object)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 15);
    list_append(bindings, symbol_binding_create("x", NULL, y1, 1, 5));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, x_ref, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para y: %s (esperado: Object)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_at_body(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_at_body ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let x = 1 in -y\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 15);
    list_append(bindings, symbol_binding_create("x", NULL, one, 1, 5));

    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 1, 22);
    ASTNode *neg = (ASTNode *)ast_unary_create(OP_SUB, y_ref, 1, 21);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, neg, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para y: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_nested(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let x: Number = y in (let z: Number = x in z)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings1 = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 22);
    list_append(bindings1, symbol_binding_create("x", "Number", y1, 1, 5));

    List *bindings2 = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 42);
    list_append(bindings2, symbol_binding_create("z", "Number", x_ref, 1, 30));
    ASTNode *z_ref = (ASTNode *)ast_variable_reference_create("z", 1, 52);
    ASTNode *inner_let = (ASTNode *)ast_let_create(bindings2, z_ref, 1, 30);

    ASTNode *body = (ASTNode *)ast_let_create(bindings1, inner_let, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para y: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_single_param_multiple_bindings_correct(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_single_param_multiple_bindings_correct ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let a: Number = y, b: Number = y in a\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 25);
    ASTNode *y2 = (ASTNode *)ast_variable_reference_create("y", 1, 42);
    list_append(bindings, symbol_binding_create("a", "Number", y1, 1, 5));
    list_append(bindings, symbol_binding_create("b", "Number", y2, 1, 22));

    ASTNode *a_ref = (ASTNode *)ast_variable_reference_create("a", 1, 50);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, a_ref, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para y: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_single_param_multiple_bindings_incompatible(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_single_param_multiple_bindings_incompatible ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let a: Number = y, b: String = y in a (conflicto)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 25);
    ASTNode *y2 = (ASTNode *)ast_variable_reference_create("y", 1, 42);
    list_append(bindings, symbol_binding_create("a", "Number", y1, 1, 5));
    list_append(bindings, symbol_binding_create("b", "String", y2, 1, 22));

    ASTNode *a_ref = (ASTNode *)ast_variable_reference_create("a", 1, 50);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, a_ref, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu (esperado: >= 1)\n", dm_error_count(dm_global));
    CU_ASSERT(dm_error_count(dm_global) >= 1);

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_binding_and_body_correct(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_binding_and_body_correct ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let x: Number = y in -y\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 22);
    list_append(bindings, symbol_binding_create("x", "Number", y1, 1, 5));

    ASTNode *y2 = (ASTNode *)ast_variable_reference_create("y", 1, 32);
    ASTNode *neg = (ASTNode *)ast_unary_create(OP_SUB, y2, 1, 31);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, neg, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para y: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_binding_and_body_incompatible(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_binding_and_body_incompatible ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(y) => let x: Number = y in !y (conflicto)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 5));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 22);
    list_append(bindings, symbol_binding_create("x", "Number", y1, 1, 5));

    ASTNode *y2 = (ASTNode *)ast_variable_reference_create("y", 1, 32);
    ASTNode *not_y = (ASTNode *)ast_unary_create(OP_NOT, y2, 1, 31);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, not_y, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu (esperado: >= 1)\n", dm_error_count(dm_global));
    CU_ASSERT(dm_error_count(dm_global) >= 1);

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_let_multiple_params_at_bindings(void)
{
    printf("\n\n------------ Inicio test: test_collector_let_multiple_params_at_bindings ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(a, b) => let x: Number = a, y: String = b in 1\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("a", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("b", NULL, NULL, 1, 8));

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *a_ref = (ASTNode *)ast_variable_reference_create("a", 1, 32);
    ASTNode *b_ref = (ASTNode *)ast_variable_reference_create("b", 1, 55);
    list_append(bindings, symbol_binding_create("x", "Number", a_ref, 1, 5));
    list_append(bindings, symbol_binding_create("y", "String", b_ref, 1, 22));

    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 65);
    ASTNode *body = (ASTNode *)ast_let_create(bindings, one, 1, 1);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 2);
    TypeDescriptor *a_type = (TypeDescriptor *)list_get(param_types, 0);
    TypeDescriptor *b_type = (TypeDescriptor *)list_get(param_types, 1);

    printf("Tipo inferido para a: %s (esperado: Number)\n", a_type ? a_type->name : "NULL");
    printf("Tipo inferido para b: %s (esperado: String)\n", b_type ? b_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(a_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(b_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de ConstraintCollector (let) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Let Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_let_with_annotation", test_collector_let_with_annotation);
    CU_add_test(suite, "test_collector_let_without_annotation", test_collector_let_without_annotation);
    CU_add_test(suite, "test_collector_let_at_body", test_collector_let_at_body);
    CU_add_test(suite, "test_collector_let_nested", test_collector_let_nested);
    CU_add_test(suite, "test_collector_let_single_param_multiple_bindings_correct", test_collector_let_single_param_multiple_bindings_correct);
    CU_add_test(suite, "test_collector_let_single_param_multiple_bindings_incompatible", test_collector_let_single_param_multiple_bindings_incompatible);
    CU_add_test(suite, "test_collector_let_binding_and_body_correct", test_collector_let_binding_and_body_correct);
    CU_add_test(suite, "test_collector_let_binding_and_body_incompatible", test_collector_let_binding_and_body_incompatible);
    CU_add_test(suite, "test_collector_let_multiple_params_at_bindings", test_collector_let_multiple_params_at_bindings);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}