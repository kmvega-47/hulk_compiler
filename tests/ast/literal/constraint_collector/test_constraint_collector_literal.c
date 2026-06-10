#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_literal_function_body(void)
{
    printf("\n\n------------ Inicio test: test_collector_literal_function_body ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Funcion foo(x) => 42 (parametro sin anotar, solo literal)\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 10));
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 1, 20);
    FunctionDefinitionNode *foo = ast_function_definition_create("foo", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, foo);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types = function_table_get_params_types(global_function_table, "foo");
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Object)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));

    bool found = false;
    TypeDescriptor *ret = function_table_get_return_type(global_function_table, "foo", &found);
    CU_ASSERT_TRUE(found);
    printf("Tipo de retorno de foo: %s (esperado: Number)\n", ret->name);
    CU_ASSERT_PTR_EQUAL(ret, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

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

void test_collector_literal_method_body(void)
{
    printf("\n\n------------ Inicio test: test_collector_literal_method_body ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Metodo A::m(x) => true (parametro sin anotar, solo literal)\n");

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    List *params_m = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params_m, symbol_binding_create("x", NULL, NULL, 1, 16));
    ASTNode *body_m = (ASTNode *)ast_bool_literal_create(true, 1, 30);
    FunctionDefinitionNode *m = ast_function_definition_create("m", params_m, NULL, body_m, 1, 5);
    list_append(methods, m);

    TypeDefinitionNode *typeA = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, typeA);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    UserTypeDescriptor *ut = (UserTypeDescriptor *)type_table_lookup_by_name(global_type_table, "A");
    CU_ASSERT_PTR_NOT_NULL(ut);

    char *full_name = function_table_build_method_name("A", "m");
    List *param_types = function_table_get_params_types(global_function_table, full_name);
    free(full_name);

    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types, 0);

    printf("Parametro 'x' resuelto a: %s (esperado: Object)\n", x_type ? x_type->name : "NULL");
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

void test_collector_literal_type_init_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_literal_type_init_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Tipo B(x) { attr = 42; } (init_param sin anotar, inicializadores literales)\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(init_params, symbol_binding_create("x", NULL, NULL, 1, 8));

    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init_attr = (ASTNode *)ast_number_literal_create(42.0, 1, 20);
    list_append(attributes, symbol_binding_create("attr", NULL, init_attr, 1, 14));

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *typeB = ast_type_definition_create("B", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attributes, methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, typeB);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    UserTypeDescriptor *ut = (UserTypeDescriptor *)type_table_lookup_by_name(global_type_table, "B");
    CU_ASSERT_PTR_NOT_NULL(ut);
    List *stored_params = user_type_get_param_types(ut);
    CU_ASSERT_PTR_NOT_NULL(stored_params);
    CU_ASSERT_EQUAL(list_count(stored_params), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(stored_params, 0);

    printf("Init param 'x' resuelto a: %s (esperado: Object)\n", x_type ? x_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(x_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));

    TypeDescriptor *attr_type = user_type_lookup_attribute(ut, "attr");
    printf("Atributo 'attr' tipo: %s (esperado: Number)\n", attr_type->name);
    CU_ASSERT_PTR_EQUAL(attr_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

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
    printf("\n=== Iniciando tests de ConstraintCollector (literal) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Literal Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_literal_function_body", test_collector_literal_function_body);
    CU_add_test(suite, "test_collector_literal_method_body", test_collector_literal_method_body);
    CU_add_test(suite, "test_collector_literal_type_init_param", test_collector_literal_type_init_param);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}