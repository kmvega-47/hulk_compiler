#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_is_simple(void)
{
    printf("\n\n------------ Inicio test: test_inference_is_simple ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x is Point (linea 1, columna 3)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    UserTypeDescriptor *point = user_type_create("Point", obj, list_create(0, NULL, NULL, NULL, NULL));
    type_table_insert(global_type_table, point);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 1, 1);
    IsNode *node = ast_is_create(target, "Point", 1, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", (TypeDescriptor *)point);
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));

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

void test_inference_is_complex_target(void)
{
    printf("\n\n------------ Inicio test: test_inference_is_complex_target ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: (a + b) is Number (linea 2, columna 7)\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    ASTNode *target = (ASTNode *)ast_binary_create(OP_ADD,
        (ASTNode *)ast_number_literal_create(1.0, 2, 2),
        (ASTNode *)ast_number_literal_create(2.0, 2, 6), 2, 4);
    IsNode *node = ast_is_create(target, "Number", 2, 9);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(target->return_type, num);
    printf("Tipo del target: Number\n");

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

void test_inference_is_undefined_type(void)
{
    printf("\n\n------------ Inicio test: test_inference_is_undefined_type ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x is NoExiste (linea 3, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 3, 1);
    IsNode *node = ast_is_create(target, "NoExiste", 3, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));

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

void test_inference_is_builtin_type(void)
{
    printf("\n\n------------ Inicio test: test_inference_is_builtin_type ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x is Number (linea 4, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 4, 1);
    IsNode *node = ast_is_create(target, "Number", 4, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));

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

void test_inference_is_self(void)
{
    printf("\n\n------------ Inicio test: test_inference_is_self ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self is Point (linea 5, columna 6)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    UserTypeDescriptor *point = user_type_create("Point", obj, list_create(0, NULL, NULL, NULL, NULL));
    type_table_insert(global_type_table, point);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)point);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 5, 1);
    IsNode *node = ast_is_create(target, "Point", 5, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_PTR_EQUAL(target->return_type, (TypeDescriptor *)point);
    printf("Tipo del target: Point\n");

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

void test_inference_is_string_type(void)
{
    printf("\n\n------------ Inicio test: test_inference_is_string_type ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x is String (linea 6, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 6, 1);
    IsNode *node = ast_is_create(target, "String", 6, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));

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
    printf("\n=== Iniciando tests de IsNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("IsNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_is_simple", test_inference_is_simple);
    CU_add_test(suite, "test_inference_is_complex_target", test_inference_is_complex_target);
    CU_add_test(suite, "test_inference_is_undefined_type", test_inference_is_undefined_type);
    CU_add_test(suite, "test_inference_is_builtin_type", test_inference_is_builtin_type);
    CU_add_test(suite, "test_inference_is_self", test_inference_is_self);
    CU_add_test(suite, "test_inference_is_string_type", test_inference_is_string_type);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}