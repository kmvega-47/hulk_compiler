#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_reassignment_simple(void)
{
    printf("\n\n------------ Inicio test: test_collector_reassignment_simple ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { x := 5; } (x sin anotar)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 20);
    ASTNode *five = (ASTNode *)ast_number_literal_create(5.0, 1, 24);
    ASTNode *body = (ASTNode *)ast_reassignment_create(x_ref, five, 1, 22);

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

void test_collector_reassignment_from_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_reassignment_from_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x, y) => { y := 5; x := y; } (x sin anotar, y anotado Number)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("y", "Number", NULL, 1, 8));

    ASTNode *y1 = (ASTNode *)ast_variable_reference_create("y", 1, 22);
    ASTNode *five = (ASTNode *)ast_number_literal_create(5.0, 1, 27);
    ASTNode *assign_y = (ASTNode *)ast_reassignment_create(y1, five, 1, 24);

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 32);
    ASTNode *y2 = (ASTNode *)ast_variable_reference_create("y", 1, 37);
    ASTNode *assign_x = (ASTNode *)ast_reassignment_create(x_ref, y2, 1, 34);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, assign_y);
    list_append(body_exprs, assign_x);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

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
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 1);

    printf("Tipo inferido para x: %s (esperado: Number)\n", x_type ? x_type->name : "NULL");
    printf("Tipo inferido para y: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_reassignment_to_attribute(void)
{
    printf("\n\n------------ Inicio test: test_collector_reassignment_to_attribute ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { x: Number = 0; setX(v) => self.x := v; }\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init0 = (ASTNode *)ast_number_literal_create(0.0, 1, 20);
    list_append(attrs, symbol_binding_create("x", "Number", init0, 1, 5));

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(m_params, symbol_binding_create("v", NULL, NULL, 1, 25));

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 1, 35);
    ASTNode *self_x = (ASTNode *)ast_attribute_access_create(self_ref, "x", 1, 40);
    ASTNode *v_ref = (ASTNode *)ast_variable_reference_create("v", 1, 45);
    ASTNode *m_body = (ASTNode *)ast_reassignment_create(self_x, v_ref, 1, 42);

    FunctionDefinitionNode *method = ast_function_definition_create("setX", m_params, NULL, m_body, 1, 15);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attrs, methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    char *full_name = function_table_build_method_name("A", "setX");
    List *param_types = function_table_get_params_types(global_function_table, full_name);
    free(full_name);

    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *v_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para v: %s (esperado: Number)\n", v_type ? v_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(v_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_reassignment_from_attribute(void)
{
    printf("\n\n------------ Inicio test: test_collector_reassignment_from_attribute ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { x: Number = 0; getX(flag) => flag := self.x; }\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init0 = (ASTNode *)ast_number_literal_create(0.0, 1, 20);
    list_append(attrs, symbol_binding_create("x", "Number", init0, 1, 5));

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(m_params, symbol_binding_create("flag", NULL, NULL, 1, 25));

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 1, 40);
    ASTNode *self_x = (ASTNode *)ast_attribute_access_create(self_ref, "x", 1, 45);
    ASTNode *flag_ref = (ASTNode *)ast_variable_reference_create("flag", 1, 50);
    ASTNode *m_body = (ASTNode *)ast_reassignment_create(flag_ref, self_x, 1, 47);

    FunctionDefinitionNode *method = ast_function_definition_create("getX", m_params, NULL, m_body, 1, 15);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attrs, methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    char *full_name = function_table_build_method_name("A", "getX");
    List *param_types = function_table_get_params_types(global_function_table, full_name);
    free(full_name);

    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *flag_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para flag: %s (esperado: Number)\n", flag_type ? flag_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(flag_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_reassignment_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_reassignment_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { x := 5; x := \"hello\"; } (conflicto)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *five = (ASTNode *)ast_number_literal_create(5.0, 1, 27);
    ASTNode *assign1 = (ASTNode *)ast_reassignment_create(x1, five, 1, 24);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 32);
    ASTNode *hello = (ASTNode *)ast_string_literal_create("hello", 1, 37);
    ASTNode *assign2 = (ASTNode *)ast_reassignment_create(x2, hello, 1, 34);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, assign1);
    list_append(body_exprs, assign2);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

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

    printf("Tipo inferido para x: %s (esperado: NULL - conflicto)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_NULL(x_type);
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

void test_collector_reassignment_type_init_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_reassignment_type_init_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type Point(a) { x = a := 5; }\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(init_params, symbol_binding_create("a", NULL, NULL, 1, 12));

    ASTNode *a_ref = (ASTNode *)ast_variable_reference_create("a", 1, 30);
    ASTNode *five = (ASTNode *)ast_number_literal_create(5.0, 1, 35);
    ASTNode *a_assign = (ASTNode *)ast_reassignment_create(a_ref, five, 1, 32);

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(attrs, symbol_binding_create("x", NULL, a_assign, 1, 23));

    TypeDefinitionNode *type_def = ast_type_definition_create("Point", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attrs,
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    UserTypeDescriptor *utype = (UserTypeDescriptor *)type_table_lookup_by_name(global_type_table, "Point");
    List *stored = user_type_get_param_types(utype);
    CU_ASSERT_PTR_NOT_NULL(stored);
    CU_ASSERT_EQUAL(list_count(stored), 1);
    TypeDescriptor *a_type = (TypeDescriptor *)list_get(stored, 0);

    printf("Tipo inferido para a: %s (esperado: Number)\n", a_type ? a_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(a_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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
    printf("\n=== Iniciando tests de ConstraintCollector (reassignment) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Reassignment Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_reassignment_simple", test_collector_reassignment_simple);
    CU_add_test(suite, "test_collector_reassignment_from_param", test_collector_reassignment_from_param);
    CU_add_test(suite, "test_collector_reassignment_to_attribute", test_collector_reassignment_to_attribute);
    CU_add_test(suite, "test_collector_reassignment_from_attribute", test_collector_reassignment_from_attribute);
    CU_add_test(suite, "test_collector_reassignment_conflict", test_collector_reassignment_conflict);
    CU_add_test(suite, "test_collector_reassignment_type_init_param", test_collector_reassignment_type_init_param);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}