#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

static void register_type(const char *name, TypeDescriptor *parent, List *param_types)
{
    UserTypeDescriptor *ut = user_type_create(name, parent, param_types);
    type_table_insert(global_type_table, ut);
}

void test_inference_type_definition_simple(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_definition_simple ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type Simple {} (linea 1, columna 1)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    register_type("Simple", obj, empty);

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *node = ast_type_definition_create("Simple", init_params, NULL,
                                                          father_init_args, attributes, methods, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    printf("Tipo nodo: Void\n");

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

void test_inference_type_definition_init_params(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_definition_init_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type Point(x: Number, y: Number) {} (linea 2, columna 1)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    List *param_types = list_create(2, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER), type_table_lookup_by_tag(global_type_table, HULK_NUMBER) }, NULL, NULL, NULL);
    register_type("Point", obj, param_types);

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(init_params, symbol_binding_create("x", "Number", NULL, 2, 12));
    list_append(init_params, symbol_binding_create("y", "Number", NULL, 2, 24));

    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Point", init_params, NULL, father_init_args, attributes, methods, 2, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    printf("Tipo nodo: Void\n");

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    UserTypeDescriptor *ut = (UserTypeDescriptor *)type_table_lookup_by_name(global_type_table, "Point");
    CU_ASSERT_PTR_NOT_NULL(ut);

    List *stored_params = user_type_get_param_types(ut);
    CU_ASSERT_EQUAL(list_count(stored_params), 2);
    CU_ASSERT_PTR_EQUAL(list_get(stored_params, 0), type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(list_get(stored_params, 1), type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Parametros: Number, Number\n");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_type_definition_inheritance(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_definition_inheritance ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type Child inherits Parent {} (linea 3, columna 1)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    register_type("Parent", obj, list_create(0, NULL, NULL, NULL, NULL));
    register_type("Child", type_table_lookup_by_name(global_type_table, "Parent"), list_create(0, NULL, NULL, NULL, NULL));

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Child", init_params, "Parent", father_init_args, attributes, methods, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    printf("Tipo nodo: Void\n");

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

void test_inference_type_definition_father_init_args(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_definition_father_init_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type Derived inherits Base(1, true) {} (linea 4, columna 1)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    List *base_params = list_create(2, (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER), type_table_lookup_by_tag(global_type_table, HULK_BOOL) }, NULL, NULL, NULL);
    register_type("Base", obj, base_params);

    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    register_type("Derived", type_table_lookup_by_name(global_type_table, "Base"), empty);

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(father_init_args, ast_number_literal_create(1.0, 4, 30));
    list_append(father_init_args, ast_bool_literal_create(true, 4, 33));

    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *methods = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Derived", init_params, "Base", father_init_args, attributes, methods, 4, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    printf("Tipo nodo: Void\n");

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    CU_ASSERT_PTR_EQUAL(((ASTNode *)list_get(father_init_args, 0))->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(((ASTNode *)list_get(father_init_args, 1))->return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));
    printf("Father args: Number, Bool\n");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_type_definition_attributes(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_definition_attributes ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type Person { name = \"John\"; age = 30; } (linea 5, columna 1)\n");

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    register_type("Person", obj, empty);

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *father_init_args = list_create(0, NULL, NULL, NULL, NULL);
    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);

    list_append(attributes, symbol_binding_create("name", NULL, (ASTNode *)ast_string_literal_create("John", 5, 14), 5, 5));
    list_append(attributes, symbol_binding_create("age", NULL, (ASTNode *)ast_number_literal_create(30.0, 5, 28), 5, 19));

    List *methods = list_create(0, NULL, NULL, NULL, NULL);

    TypeDefinitionNode *node = ast_type_definition_create("Person", init_params, NULL, father_init_args, attributes, methods, 5, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    printf("Tipo nodo: Void\n");

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    UserTypeDescriptor *ut = (UserTypeDescriptor *)type_table_lookup_by_name(global_type_table, "Person");
    CU_ASSERT_PTR_NOT_NULL(ut);

    TypeDescriptor *name_type = user_type_lookup_attribute(ut, "name");
    CU_ASSERT_PTR_EQUAL(name_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));

    TypeDescriptor *age_type = user_type_lookup_attribute(ut, "age");
    CU_ASSERT_PTR_EQUAL(age_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Atributos: name=String, age=Number\n");

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
    printf("\n=== Iniciando tests de TypeDefinitionNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeDefinitionNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_type_definition_simple", test_inference_type_definition_simple);
    CU_add_test(suite, "test_inference_type_definition_init_params", test_inference_type_definition_init_params);
    CU_add_test(suite, "test_inference_type_definition_inheritance", test_inference_type_definition_inheritance);
    CU_add_test(suite, "test_inference_type_definition_father_init_args", test_inference_type_definition_father_init_args);
    CU_add_test(suite, "test_inference_type_definition_attributes", test_inference_type_definition_attributes);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}