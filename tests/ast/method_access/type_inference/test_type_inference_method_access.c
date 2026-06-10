#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

static UserTypeDescriptor *create_type_with_method(const char *type_name, const char *method_name, List *param_types, TypeDescriptor *return_type, TypeDescriptor *parent)
{
    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *ut = user_type_create(type_name, parent, empty);

    char *full_name = function_table_build_method_name(type_name, method_name);
    function_table_insert(global_function_table, full_name, return_type, param_types);
    free(full_name);

    user_type_add_method(ut, method_name);
    type_table_insert(global_type_table, ut);
    return ut;
}

void test_inference_method_access_no_args(void)
{
    printf("\n\n------------ Inicio test: test_inference_method_access_no_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: obj.getX() (linea 1, columna 5)\n");

    TypeDescriptor *obj_type = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    TypeDescriptor *num_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    List *empty_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *point = create_type_with_method("Point", "getX", empty_params, num_type, obj_type);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "obj", (TypeDescriptor *)point);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 1, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, "getX", args, 1, 5);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, num_type);
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

void test_inference_method_access_with_args(void)
{
    printf("\n\n------------ Inicio test: test_inference_method_access_with_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: obj.add(1, 2) (linea 2, columna 5)\n");

    TypeDescriptor *obj_type = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    TypeDescriptor *num_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    List *param_types = list_create(2, (void *[]){num_type, num_type}, NULL, NULL, NULL);
    UserTypeDescriptor *point = create_type_with_method("Point", "add", param_types, num_type, obj_type);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "obj", (TypeDescriptor *)point);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 2, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(1.0, 2, 10));
    list_append(args, ast_number_literal_create(2.0, 2, 13));
    MethodAccessNode *node = ast_method_access_create(target, "add", args, 2, 5);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, num_type);
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

void test_inference_method_access_inherited(void)
{
    printf("\n\n------------ Inicio test: test_inference_method_access_inherited ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: obj.getX() donde obj es Point3D que hereda de Point (linea 3, columna 5)\n");

    TypeDescriptor *obj_type = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    TypeDescriptor *num_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    
    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *point = create_type_with_method("Point", "getX", empty, num_type, obj_type);

    List *empty2 = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *point3d = user_type_create("Point3D", (TypeDescriptor *)point, empty2);
    type_table_insert(global_type_table, point3d);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "obj", (TypeDescriptor *)point3d);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 3, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, "getX", args, 3, 5);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, num_type);
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

void test_inference_method_access_undefined_method(void)
{
    printf("\n\n------------ Inicio test: test_inference_method_access_undefined_method ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: obj.noExiste() (linea 4, columna 5)\n");

    TypeDescriptor *obj_type = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    TypeDescriptor *num_type = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *point = create_type_with_method("Point", "getX", empty, num_type, obj_type);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "obj", (TypeDescriptor *)point);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("obj", 4, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, "noExiste", args, 4, 5);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
    CU_ASSERT_PTR_NULL(node->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1)\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_method_access_builtin_target(void)
{
    printf("\n\n------------ Inicio test: test_inference_method_access_builtin_target ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: 42.toString() (linea 5, columna 5)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    ASTNode *target = (ASTNode *)ast_number_literal_create(42.0, 5, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *node = ast_method_access_create(target, "toString", args, 5, 5);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
    CU_ASSERT_PTR_NULL(node->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1)\n", dm_error_count(dm_global));

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
    printf("\n=== Iniciando tests de MethodAccessNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("MethodAccessNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_method_access_no_args", test_inference_method_access_no_args);
    CU_add_test(suite, "test_inference_method_access_with_args", test_inference_method_access_with_args);
    CU_add_test(suite, "test_inference_method_access_inherited", test_inference_method_access_inherited);
    CU_add_test(suite, "test_inference_method_access_undefined_method", test_inference_method_access_undefined_method);
    CU_add_test(suite, "test_inference_method_access_builtin_target", test_inference_method_access_builtin_target);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}