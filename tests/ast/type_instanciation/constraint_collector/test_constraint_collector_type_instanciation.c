#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_type_instanciation_single_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_type_instanciation_single_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => new Point(x)  -->  x Number\n");

    List *param_types = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    UserTypeDescriptor *point_type = user_type_create("Point",
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT), param_types);
    type_table_insert(global_type_table, point_type);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 20);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, x_ref);
    ASTNode *body = (ASTNode *)ast_type_instanciation_create("Point", args, 1, 15);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types_f = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types_f);
    CU_ASSERT_EQUAL(list_count(param_types_f), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types_f, 0);

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

void test_collector_type_instanciation_multiple_params(void)
{
    printf("\n\n------------ Inicio test: test_collector_type_instanciation_multiple_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x, y) => new Point(x, y)  -->  x Number, y Number\n");

    List *pt_params = list_create(2, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER), type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    UserTypeDescriptor *point_type = user_type_create("Point",
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT), pt_params);
    type_table_insert(global_type_table, point_type);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));
    list_append(params, symbol_binding_create("y", NULL, NULL, 1, 8));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    ASTNode *y_ref = (ASTNode *)ast_variable_reference_create("y", 1, 25);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, x_ref);
    list_append(args, y_ref);
    ASTNode *body = (ASTNode *)ast_type_instanciation_create("Point", args, 1, 15);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types_f = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types_f);
    CU_ASSERT_EQUAL(list_count(param_types_f), 2);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types_f, 0);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types_f, 1);

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

void test_collector_type_instanciation_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_type_instanciation_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("f(x) => { new Point(x); new StringBox(x); }  -->  x conflicto\n");

    List *pt_params = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    UserTypeDescriptor *point_type = user_type_create("Point",
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT), pt_params);
    type_table_insert(global_type_table, point_type);

    List *sb_params = list_create(1, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_STRING) }, NULL, NULL, NULL);
    UserTypeDescriptor *sb_type = user_type_create("StringBox",
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT), sb_params);
    type_table_insert(global_type_table, sb_type);

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", NULL, NULL, 1, 5));

    ASTNode *x1 = (ASTNode *)ast_variable_reference_create("x", 1, 22);
    List *args1 = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args1, x1);
    ASTNode *new_point = (ASTNode *)ast_type_instanciation_create("Point", args1, 1, 15);

    ASTNode *x2 = (ASTNode *)ast_variable_reference_create("x", 1, 32);
    List *args2 = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args2, x2);
    ASTNode *new_sb = (ASTNode *)ast_type_instanciation_create("StringBox", args2, 1, 30);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, new_point);
    list_append(body_exprs, new_sb);
    ASTNode *body = (ASTNode *)ast_block_create(body_exprs, 1, 18);

    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 2, 1);
    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    List *param_types_f = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types_f);
    CU_ASSERT_EQUAL(list_count(param_types_f), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types_f, 0);

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

void test_collector_type_instanciation_no_use(void)
{
    printf("\n\n------------ Inicio test: test_collector_type_instanciation_no_use ------------\n");
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

    List *param_types_f = function_table_get_params_types(global_function_table, "f");
    CU_ASSERT_PTR_NOT_NULL(param_types_f);
    CU_ASSERT_EQUAL(list_count(param_types_f), 1);
    TypeDescriptor *x_type = (TypeDescriptor *)list_get(param_types_f, 0);

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
    printf("\n=== Iniciando tests de ConstraintCollector (type_instanciation) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector TypeInstanciation Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_type_instanciation_single_param", test_collector_type_instanciation_single_param);
    CU_add_test(suite, "test_collector_type_instanciation_multiple_params", test_collector_type_instanciation_multiple_params);
    CU_add_test(suite, "test_collector_type_instanciation_conflict", test_collector_type_instanciation_conflict);
    CU_add_test(suite, "test_collector_type_instanciation_no_use", test_collector_type_instanciation_no_use);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}