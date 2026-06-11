#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

static UserTypeDescriptor *create_type_with_method(const char *type_name, const char *method_name, List *param_types, TypeDescriptor *return_type)
{
    UserTypeDescriptor *ut = user_type_create(type_name,
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT),
        list_create(0, NULL, NULL, NULL, NULL));

    char *full_name = function_table_build_method_name(type_name, method_name);
    function_table_insert(global_function_table, full_name, return_type, param_types);
    free(full_name);

    user_type_add_method(ut, method_name);
    type_table_insert(global_type_table, ut);
    return ut;
}

void test_collector_method_access_single_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_method_access_single_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { m(x: Number) => 0; }  f(p) => new A().m(p)  -->  p Number\n");

    List *m_params = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    create_type_with_method("A", "m", m_params, type_table_lookup_by_tag(global_type_table, HULK_VOID));

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("p", NULL, NULL, 1, 5));

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 1, 20);
    ASTNode *p_ref = (ASTNode *)ast_variable_reference_create("p", 1, 35);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, p_ref);
    ASTNode *body = (ASTNode *)ast_method_access_create(instance, "m", args, 1, 30);
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
    TypeDescriptor *p_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para p: %s (esperado: Number)\n", p_type ? p_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(p_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_collector_method_access_multiple_params(void)
{
    printf("\n\n------------ Inicio test: test_collector_method_access_multiple_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { add(x: Number, y: String) => 0; }  f(a, b) => new A().add(a, b)\n");

    List *m_params = list_create(2, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER), type_table_lookup_by_tag(global_type_table, HULK_STRING) }, NULL, NULL, NULL);
    create_type_with_method("A", "add", m_params, type_table_lookup_by_tag(global_type_table, HULK_VOID));

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("a", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("b", NULL, NULL, 1, 8));

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 1, 22);
    ASTNode *a_ref = (ASTNode *)ast_variable_reference_create("a", 1, 40);
    ASTNode *b_ref = (ASTNode *)ast_variable_reference_create("b", 1, 43);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, a_ref);
    list_append(args, b_ref);
    ASTNode *body = (ASTNode *)ast_method_access_create(instance, "add", args, 1, 35);
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

void test_collector_method_access_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_method_access_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { m1(x: Number)=>0; m2(x: String)=>0; }  f(p) => { new A().m1(p); new A().m2(p); }\n");

    UserTypeDescriptor *a_type = user_type_create("A",
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT),
        list_create(0, NULL, NULL, NULL, NULL));

    List *m1_params = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    char *full_m1 = function_table_build_method_name("A", "m1");
    function_table_insert(global_function_table, full_m1, type_table_lookup_by_tag(global_type_table, HULK_VOID), m1_params);
    free(full_m1);
    user_type_add_method(a_type, "m1");

    List *m2_params = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_STRING) }, NULL, NULL, NULL);
    char *full_m2 = function_table_build_method_name("A", "m2");
    function_table_insert(global_function_table, full_m2, type_table_lookup_by_tag(global_type_table, HULK_VOID), m2_params);
    free(full_m2);
    user_type_add_method(a_type, "m2");

    type_table_insert(global_type_table, a_type);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("p", NULL, NULL, 1, 5));

    ASTNode *inst1 = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 1, 22);
    ASTNode *p1 = (ASTNode *)ast_variable_reference_create("p", 1, 40);
    List *args1 = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args1, p1);
    ASTNode *call_m1 = (ASTNode *)ast_method_access_create(inst1, "m1", args1, 1, 35);

    ASTNode *inst2 = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 1, 50);
    ASTNode *p2 = (ASTNode *)ast_variable_reference_create("p", 1, 68);
    List *args2 = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args2, p2);
    ASTNode *call_m2 = (ASTNode *)ast_method_access_create(inst2, "m2", args2, 1, 63);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, call_m1);
    list_append(body_exprs, call_m2);
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
    TypeDescriptor *p_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Tipo inferido para p: %s (esperado: NULL - conflicto)\n", p_type ? p_type->name : "NULL");
    CU_ASSERT_PTR_NULL(p_type);
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

void test_collector_method_access_no_use(void)
{
    printf("\n\n------------ Inicio test: test_collector_method_access_no_use ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => 42  -->  x Object\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 15);
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

    printf("Tipo inferido para x: %s (esperado: Object)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
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
    printf("\n=== Iniciando tests de ConstraintCollector (method_access) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector MethodAccess Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_method_access_single_param", test_collector_method_access_single_param);
    CU_add_test(suite, "test_collector_method_access_multiple_params", test_collector_method_access_multiple_params);
    CU_add_test(suite, "test_collector_method_access_conflict", test_collector_method_access_conflict);
    CU_add_test(suite, "test_collector_method_access_no_use", test_collector_method_access_no_use);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}