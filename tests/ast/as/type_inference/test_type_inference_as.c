#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_as_simple(void)
{
    printf("\n\n------------ Inicio test: test_inference_as_simple ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x as Point (linea 1, columna 3)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    UserTypeDescriptor *point = user_type_create("Point", obj, list_create(0, NULL, NULL, NULL, NULL));
    type_table_insert(global_type_table, point);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 1, 1);
    AsNode *node = ast_as_create(target, "Point", 1, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", (TypeDescriptor *)point);
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Point)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, (TypeDescriptor *)point);

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

void test_inference_as_complex_target(void)
{
    printf("\n\n------------ Inicio test: test_inference_as_complex_target ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: (a + b) as Number (linea 2, columna 7)\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    ASTNode *target = (ASTNode *)ast_binary_create(OP_ADD,
        (ASTNode *)ast_number_literal_create(1.0, 2, 2),
        (ASTNode *)ast_number_literal_create(2.0, 2, 6), 2, 4);
    AsNode *node = ast_as_create(target, "Number", 2, 9);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, num);
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

void test_inference_as_undefined_type(void)
{
    printf("\n\n------------ Inicio test: test_inference_as_undefined_type ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x as NoExiste (linea 3, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 3, 1);
    AsNode *node = ast_as_create(target, "NoExiste", 3, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
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

void test_inference_as_builtin_type(void)
{
    printf("\n\n------------ Inicio test: test_inference_as_builtin_type ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x as String (linea 4, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 4, 1);
    AsNode *node = ast_as_create(target, "String", 4, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: String)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));

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

void test_inference_as_downcast_self(void)
{
    printf("\n\n------------ Inicio test: test_inference_as_downcast_self ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self as Point (self es Object, downcast a Point)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    UserTypeDescriptor *point = user_type_create("Point", obj, list_create(0, NULL, NULL, NULL, NULL));
    type_table_insert(global_type_table, point);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_self_instance(iv->current_scope, obj);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 5, 1);
    AsNode *node = ast_as_create(target, "Point", 5, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Point)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, (TypeDescriptor *)point);

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

void test_inference_as_void_type(void)
{
    printf("\n\n------------ Inicio test: test_inference_as_void_type ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: x as Void (linea 6, columna 3)\n");

    ASTNode *target = (ASTNode *)ast_variable_reference_create("x", 6, 1);
    AsNode *node = ast_as_create(target, "Void", 6, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "x", type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Void)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));

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
    printf("\n=== Iniciando tests de AsNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AsNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_as_simple", test_inference_as_simple);
    CU_add_test(suite, "test_inference_as_complex_target", test_inference_as_complex_target);
    CU_add_test(suite, "test_inference_as_undefined_type", test_inference_as_undefined_type);
    CU_add_test(suite, "test_inference_as_builtin_type", test_inference_as_builtin_type);
    CU_add_test(suite, "test_inference_as_downcast_self", test_inference_as_downcast_self);
    CU_add_test(suite, "test_inference_as_void_type", test_inference_as_void_type);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}