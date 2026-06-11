#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_conditional_as_condition(void)
{
    printf("\n\n------------ Inicio test: test_collector_conditional_as_condition ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function f(x) => if(x) 1;\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 13));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 25);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 30);
    ASTNode *body = (ASTNode *)ast_conditional_create(x_ref, one, NULL, 1, 20);
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
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para x: %s (esperado: Bool)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
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

void test_collector_conditional_branches(void)
{
    printf("\n\n------------ Inicio test: test_collector_conditional_branches ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function f(x) => if(true) -x else x + 1;\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 13));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 35);
    ASTNode *neg = (ASTNode *)ast_unary_create(OP_SUB, x1, 1, 34);
    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 43);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 47);
    ASTNode *add = (ASTNode *)ast_binary_create(OP_ADD, x2, one, 1, 45);
    ASTNode *cond_true = (ASTNode *)ast_bool_literal_create(true, 1, 25);
    ASTNode *body = (ASTNode *)ast_conditional_create(cond_true, neg, add, 1, 20);

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
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para x: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_conditional_infer_among_branches(void)
{
    printf("\n\n------------ Inicio test: test_collector_conditional_infer_among_branches ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function f(x) => if(true) x else 0;\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 13));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 35);
    ASTNode *zero = (ASTNode *)ast_number_literal_create(0.0, 1, 43);
    ASTNode *cond_true = (ASTNode *)ast_bool_literal_create(true, 1, 25);
    ASTNode *body = (ASTNode *)ast_conditional_create(cond_true, x_ref, zero, 1, 20);

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
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para x: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_conditional_else_if(void)
{
    printf("\n\n------------ Inicio test: test_collector_conditional_else_if ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: function f(x) => if(x) 1 elif(true) !x else 0;\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 13));

    ASTNode *x_cond = (ASTNode *)ast_variable_reference_create("x", 1, 25);
    ASTNode *x_ref  = (ASTNode *)ast_variable_reference_create("x", 1, 42);
    ASTNode *not_x = (ASTNode *)ast_unary_create(OP_NOT, x_ref, 1, 41);
    ASTNode *cond_true = (ASTNode *)ast_bool_literal_create(true, 1, 36);
    ASTNode *elif_branch = (ASTNode *)ast_conditional_create(cond_true, not_x,
        (ASTNode *)ast_number_literal_create(0.0, 1, 50), 1, 30);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 28);
    ASTNode *body = (ASTNode *)ast_conditional_create(x_cond, one, elif_branch, 1, 20);

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
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para x: %s (esperado: Bool)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
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

void test_collector_conditional_type_init_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_conditional_type_init_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type Point(x) { x = if(true) -x else 0; }\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(init_params, symbol_binding_create("x", NULL, NULL, 1, 12));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 40);
    ASTNode *neg = (ASTNode *)ast_unary_create(OP_SUB, x1, 1, 39);
    ASTNode *zero = (ASTNode *)ast_number_literal_create(0.0, 1, 48);
    ASTNode *cond_true = (ASTNode *)ast_bool_literal_create(true, 1, 30);
    ASTNode *x_init = (ASTNode *)ast_conditional_create(cond_true, neg, zero, 1, 28);

    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(attributes, symbol_binding_create("x", NULL, x_init, 1, 23));

    TypeDefinitionNode *type_def = ast_type_definition_create("Point", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attributes,
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    UserTypeDescriptor *utype = (UserTypeDescriptor *)type_table_lookup_by_name(global_type_table, "Point");
    List *stored = user_type_get_param_types(utype);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(stored, 0);

    printf("Tipo inferido para x: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_conditional_method_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_conditional_method_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type A { f(x) => if(true) !x else false; }\n");

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(m_params, symbol_binding_create("x", NULL, NULL, 1, 11));

    ASTNode *cond_true = (ASTNode *)ast_bool_literal_create(true, 1, 38);
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 46);
    ASTNode *not_x = (ASTNode *)ast_unary_create(OP_NOT, x_ref, 1, 45);
    ASTNode *false_val = (ASTNode *)ast_bool_literal_create(false, 1, 54);
    ASTNode *m_body = (ASTNode *)ast_conditional_create(cond_true, not_x, false_val, 1, 34);

    FunctionDefinitionNode *method = ast_function_definition_create("f", m_params, NULL, m_body, 1, 8);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    char *full_name = function_table_build_method_name("A", "f");
    List *param_types = function_table_get_params_types(global_function_table, full_name);
    free(full_name);

    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para x (metodo): %s (esperado: Bool)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
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
    printf("\n=== Iniciando tests de ConstraintCollector (conditional) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Conditional Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_conditional_as_condition", test_collector_conditional_as_condition);
    CU_add_test(suite, "test_collector_conditional_branches", test_collector_conditional_branches);
    CU_add_test(suite, "test_collector_conditional_infer_among_branches", test_collector_conditional_infer_among_branches);
    CU_add_test(suite, "test_collector_conditional_else_if", test_collector_conditional_else_if);
    CU_add_test(suite, "test_collector_conditional_type_init_param", test_collector_conditional_type_init_param);
    CU_add_test(suite, "test_collector_conditional_method_param", test_collector_conditional_method_param);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}