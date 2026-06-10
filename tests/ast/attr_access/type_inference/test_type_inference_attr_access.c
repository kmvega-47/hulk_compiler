#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

static UserTypeDescriptor *register_dummy_type(const char *type_name, const char *attr_name, TypeDescriptor *attr_type)
{
    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    UserTypeDescriptor *ut = user_type_create(type_name, obj, list_create(0, NULL, NULL, NULL, NULL));
    user_type_add_attribute(ut, attr_name, attr_type);
    type_table_insert(global_type_table, ut);
    return ut;
}

void test_inference_attribute_access_number(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_number ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self.x donde x es Number (linea 1, columna 6)\n");

    UserTypeDescriptor *foo = register_dummy_type("Foo", "x", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    Scope *old_scope = iv->current_scope;
    iv->current_scope = scope_create(old_scope);
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)foo);

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 1, 1);
    AttributeAccessNode *node = ast_attribute_access_create(self_ref, "x", 1, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo de self.x: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    scope_destroy(iv->current_scope);
    iv->current_scope = old_scope;
    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_attribute_access_string(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_string ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self.name donde name es String (linea 2, columna 6)\n");

    UserTypeDescriptor *foo = register_dummy_type("Foo", "name", type_table_lookup_by_tag(global_type_table, HULK_STRING));

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    Scope *old_scope = iv->current_scope;
    iv->current_scope = scope_create(old_scope);
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)foo);

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 2, 1);
    AttributeAccessNode *node = ast_attribute_access_create(self_ref, "name", 2, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo de self.name: %s (esperado: String)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    scope_destroy(iv->current_scope);
    iv->current_scope = old_scope;
    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_attribute_access_bool(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_bool ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self.flag donde flag es Bool (linea 3, columna 6)\n");

    UserTypeDescriptor *foo = register_dummy_type("Foo", "flag", type_table_lookup_by_tag(global_type_table, HULK_BOOL));

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    Scope *old_scope = iv->current_scope;
    iv->current_scope = scope_create(old_scope);
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)foo);

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 3, 1);
    AttributeAccessNode *node = ast_attribute_access_create(self_ref, "flag", 3, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo de self.flag: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    scope_destroy(iv->current_scope);
    iv->current_scope = old_scope;
    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_attribute_access_object(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_object ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self.objAttr donde objAttr es Object (linea 4, columna 6)\n");

    UserTypeDescriptor *foo = register_dummy_type("Foo", "objAttr", type_table_lookup_by_tag(global_type_table, HULK_OBJECT));

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    Scope *old_scope = iv->current_scope;
    iv->current_scope = scope_create(old_scope);
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)foo);

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 4, 1);
    AttributeAccessNode *node = ast_attribute_access_create(self_ref, "objAttr", 4, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo de self.objAttr: %s (esperado: Object)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    scope_destroy(iv->current_scope);
    iv->current_scope = old_scope;
    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_attribute_access_non_variable_target(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_non_variable_target ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: (1).x -- error (target literal, linea 5, columna 6)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    ASTNode *target = (ASTNode *)ast_number_literal_create(1.0, 5, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, "x", 5, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del nodo: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
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

void test_inference_attribute_access_not_self(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_not_self ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: a.x con a variable normal -- error (linea 6, columna 6)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "a", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    ASTNode *target = (ASTNode *)ast_variable_reference_create("a", 6, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, "x", 6, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del nodo: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
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

void test_inference_attribute_access_self_not_in_scope(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_self_not_in_scope ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self.x sin self en el scope -- error (linea 7, columna 6)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 7, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, "x", 7, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del nodo: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
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

void test_inference_attribute_access_unknown_attribute(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_unknown_attribute ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self.noExiste -- error (linea 8, columna 6)\n");

    UserTypeDescriptor *foo = register_dummy_type("Foo", "x", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    Scope *old_scope = iv->current_scope;
    iv->current_scope = scope_create(old_scope);
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)foo);

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 8, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, "noExiste", 8, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del nodo: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
    CU_ASSERT_PTR_NULL(node->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1)\n", dm_error_count(dm_global));

    scope_destroy(iv->current_scope);
    iv->current_scope = old_scope;
    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_attribute_access_self_not_instance(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_self_not_instance ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self.x con self como variable normal (no instancia) -- error (linea 9, columna 6)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    scope_add_variable(iv->current_scope, "self", type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    ASTNode *target = (ASTNode *)ast_variable_reference_create("self", 9, 1);
    AttributeAccessNode *node = ast_attribute_access_create(target, "x", 9, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del nodo: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
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

void test_inference_attribute_access_long_name(void)
{
    printf("\n\n------------ Inicio test: test_inference_attribute_access_long_name ------------\n");
    dm_global = dm_create();
    global_tables_init();

    const char *long_name = "very_long_attribute_name_that_exceeds_usual_length";
    printf("Caso: self.%s (linea 10, columna 6)\n", long_name);

    UserTypeDescriptor *foo = register_dummy_type("Foo", long_name, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    Scope *old_scope = iv->current_scope;
    iv->current_scope = scope_create(old_scope);
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)foo);

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 10, 1);
    AttributeAccessNode *node = ast_attribute_access_create(self_ref, long_name, 10, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del atributo largo: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    scope_destroy(iv->current_scope);
    iv->current_scope = old_scope;
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
    printf("\n=== Iniciando tests de AttributeAccessNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AttributeAccessNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_attribute_access_number", test_inference_attribute_access_number);
    CU_add_test(suite, "test_inference_attribute_access_string", test_inference_attribute_access_string);
    CU_add_test(suite, "test_inference_attribute_access_bool", test_inference_attribute_access_bool);
    CU_add_test(suite, "test_inference_attribute_access_object", test_inference_attribute_access_object);
    CU_add_test(suite, "test_inference_attribute_access_non_variable_target", test_inference_attribute_access_non_variable_target);
    CU_add_test(suite, "test_inference_attribute_access_not_self", test_inference_attribute_access_not_self);
    CU_add_test(suite, "test_inference_attribute_access_self_not_in_scope", test_inference_attribute_access_self_not_in_scope);
    CU_add_test(suite, "test_inference_attribute_access_unknown_attribute", test_inference_attribute_access_unknown_attribute);
    CU_add_test(suite, "test_inference_attribute_access_self_not_instance", test_inference_attribute_access_self_not_instance);
    CU_add_test(suite, "test_inference_attribute_access_long_name", test_inference_attribute_access_long_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}