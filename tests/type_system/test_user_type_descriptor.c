#include <CUnit/Basic.h>
#include "user_type_descriptor.h"

void test_user_type_create_basic(void)
{
    printf("\n\n------------ Inicio test: test_user_type_create_basic ------------\n");
    printf("Creando tipo usuario basico\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("MyType", NULL, params);

    CU_ASSERT_PTR_NOT_NULL(type);
    printf("type = %p\n", (void *)type);

    CU_ASSERT_EQUAL(type->base.tag, HULK_USER_DEFINED);
    printf("base.tag = %d (esperado: %d)\n", type->base.tag, HULK_USER_DEFINED);

    CU_ASSERT_STRING_EQUAL(type->base.name, "MyType");
    printf("base.name = '%s' (esperado: 'MyType')\n", type->base.name);

    CU_ASSERT_PTR_NULL(type->base.parent);
    printf("base.parent = %p (esperado: NULL)\n", (void *)type->base.parent);

    CU_ASSERT_PTR_EQUAL(type->param_types, params);
    printf("param_types = %p (esperado: %p)\n", (void *)type->param_types, (void *)params);

    CU_ASSERT_PTR_NOT_NULL(type->attribute_names);
    printf("attribute_names = %p\n", (void *)type->attribute_names);
    CU_ASSERT_EQUAL(list_count(type->attribute_names), 0);
    printf("attribute_names count = %zu (esperado: 0)\n", list_count(type->attribute_names));

    CU_ASSERT_PTR_NOT_NULL(type->attribute_types);
    printf("attribute_types = %p\n", (void *)type->attribute_types);
    CU_ASSERT_EQUAL(list_count(type->attribute_types), 0);
    printf("attribute_types count = %zu (esperado: 0)\n", list_count(type->attribute_types));

    CU_ASSERT_PTR_NOT_NULL(type->method_names);
    printf("method_names = %p\n", (void *)type->method_names);
    CU_ASSERT_EQUAL(list_count(type->method_names), 0);
    printf("method_names count = %zu (esperado: 0)\n", list_count(type->method_names));

    CU_ASSERT_PTR_NULL(type->struct_type);
    printf("struct_type = %p (esperado: NULL)\n", (void *)type->struct_type);

    CU_ASSERT_PTR_NULL(type->constructor);
    printf("constructor = %p (esperado: NULL)\n", (void *)type->constructor);

    CU_ASSERT_PTR_NULL(type->constructor_type);
    printf("constructor_type = %p (esperado: NULL)\n", (void *)type->constructor_type);

    CU_ASSERT_EQUAL(type->offset, 0);
    printf("offset = %d (esperado: 0)\n", type->offset);

    user_type_destroy(type);
}

void test_user_type_create_with_parent_user_defined(void)
{
    printf("\n\n------------ Inicio test: test_user_type_create_with_parent_user_defined ------------\n");
    printf("Creando tipo usuario con padre user-defined\n");

    List *parent_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *parent = user_type_create("Parent", NULL, parent_params);

    TypeDescriptor *attr1 = type_create(HULK_NUMBER, "Number", NULL);
    TypeDescriptor *attr2 = type_create(HULK_STRING, "String", NULL);
    user_type_add_attribute(parent, "x", attr1);
    user_type_add_attribute(parent, "y", attr2);

    printf("Parent offset = %d\n", parent->offset);
    printf("Parent attribute count = %zu\n", list_count(parent->attribute_names));

    List *child_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *child = user_type_create("Child", (TypeDescriptor *)parent, child_params);

    CU_ASSERT_PTR_EQUAL(child->base.parent, (TypeDescriptor *)parent);
    printf("child.base.parent = %p (esperado: %p)\n", (void *)child->base.parent, (void *)parent);

    CU_ASSERT_EQUAL(child->offset, parent->offset + (int)list_count(parent->attribute_names));
    printf("child.offset = %d (esperado: %d)\n", child->offset, parent->offset + (int)list_count(parent->attribute_names));

    user_type_destroy(child);
    user_type_destroy(parent);
    type_destroy(attr1);
    type_destroy(attr2);
}

void test_user_type_create_with_parent_builtin(void)
{
    printf("\n\n------------ Inicio test: test_user_type_create_with_parent_builtin ------------\n");
    printf("Creando tipo usuario con padre built-in\n");

    TypeDescriptor *parent = type_create(HULK_OBJECT, "Object", NULL);
    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Derived", parent, params);

    CU_ASSERT_PTR_EQUAL(type->base.parent, parent);
    printf("base.parent = %p (esperado: %p)\n", (void *)type->base.parent, (void *)parent);

    CU_ASSERT_EQUAL(type->offset, 0);
    printf("offset = %d (esperado: 0)\n", type->offset);

    user_type_destroy(type);
    type_destroy(parent);
}

void test_user_type_create_null_name(void)
{
    printf("\n\n------------ Inicio test: test_user_type_create_null_name ------------\n");
    printf("Creando tipo usuario con name=NULL\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create(NULL, NULL, params);

    CU_ASSERT_PTR_NOT_NULL(type);
    CU_ASSERT_PTR_NULL(type->base.name);
    printf("base.name = %p (esperado: NULL)\n", (void *)type->base.name);

    user_type_destroy(type);
}

void test_type_to_user_defined_valid(void)
{
    printf("\n\n------------ Inicio test: test_type_to_user_defined_valid ------------\n");
    printf("Convirtiendo TypeDescriptor valido a UserTypeDescriptor\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);
    TypeDescriptor *base = (TypeDescriptor *)type;

    UserTypeDescriptor *result = type_to_user_defined(base);
    CU_ASSERT_PTR_EQUAL(result, type);
    printf("result = %p (esperado: %p)\n", (void *)result, (void *)type);

    user_type_destroy(type);
}

void test_type_to_user_defined_invalid_tag(void)
{
    printf("\n\n------------ Inicio test: test_type_to_user_defined_invalid_tag ------------\n");
    printf("Convirtiendo TypeDescriptor con tag incorrecto\n");

    TypeDescriptor *builtin = type_create(HULK_NUMBER, "Number", NULL);
    UserTypeDescriptor *result = type_to_user_defined(builtin);
    CU_ASSERT_PTR_NULL(result);
    printf("result = %p (esperado: NULL)\n", (void *)result);

    type_destroy(builtin);
}

void test_type_to_user_defined_null(void)
{
    printf("\n\n------------ Inicio test: test_type_to_user_defined_null ------------\n");
    printf("Convirtiendo NULL\n");

    UserTypeDescriptor *result = type_to_user_defined(NULL);
    CU_ASSERT_PTR_NULL(result);
    printf("result = %p (esperado: NULL)\n", (void *)result);
}

void test_user_type_get_param_types(void)
{
    printf("\n\n------------ Inicio test: test_user_type_get_param_types ------------\n");
    printf("Obteniendo param types\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    List *result = user_type_get_param_types(type);
    CU_ASSERT_PTR_EQUAL(result, params);
    printf("result = %p (esperado: %p)\n", (void *)result, (void *)params);

    user_type_destroy(type);
}

void test_user_type_get_param_types_null(void)
{
    printf("\n\n------------ Inicio test: test_user_type_get_param_types_null ------------\n");
    printf("Obteniendo param types de NULL\n");

    List *result = user_type_get_param_types(NULL);
    CU_ASSERT_PTR_NULL(result);
    printf("result = %p (esperado: NULL)\n", (void *)result);
}

void test_user_type_update_param_type(void)
{
    printf("\n\n------------ Inicio test: test_user_type_update_param_type ------------\n");
    printf("Actualizando tipo de parametro\n");

    TypeDescriptor *old_type = type_create(HULK_NUMBER, "Number", NULL);
    TypeDescriptor *new_type = type_create(HULK_STRING, "String", NULL);

    List *params = list_create(1, (void *[]){old_type}, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    TypeDescriptor *before = (TypeDescriptor *)list_get(type->param_types, 0);
    printf("antes de update: param[0] = %p\n", (void *)before);

    user_type_update_param_type(type, 0, new_type);

    TypeDescriptor *after = (TypeDescriptor *)list_get(type->param_types, 0);
    CU_ASSERT_PTR_EQUAL(after, new_type);
    printf("despues de update: param[0] = %p (esperado: %p)\n", (void *)after, (void *)new_type);

    user_type_destroy(type);
    type_destroy(old_type);
    type_destroy(new_type);
}

void test_user_type_update_param_type_null_type(void)
{
    printf("\n\n------------ Inicio test: test_user_type_update_param_type_null_type ------------\n");
    printf("Actualizando parametro con type=NULL (no crashea)\n");

    user_type_update_param_type(NULL, 0, NULL);
    CU_PASS("user_type_update_param_type(NULL, 0, NULL) no crasheo");
}

void test_user_type_add_and_lookup_attribute(void)
{
    printf("\n\n------------ Inicio test: test_user_type_add_and_lookup_attribute ------------\n");
    printf("Agregando y buscando atributos\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    TypeDescriptor *attr1 = type_create(HULK_NUMBER, "Number", NULL);
    TypeDescriptor *attr2 = type_create(HULK_STRING, "String", NULL);

    user_type_add_attribute(type, "count", attr1);
    user_type_add_attribute(type, "name", attr2);

    printf("attribute_names count = %zu (esperado: 2)\n", list_count(type->attribute_names));
    printf("attribute_types count = %zu (esperado: 2)\n", list_count(type->attribute_types));
    CU_ASSERT_EQUAL(list_count(type->attribute_names), 2);
    CU_ASSERT_EQUAL(list_count(type->attribute_types), 2);

    TypeDescriptor *found1 = user_type_lookup_attribute(type, "count");
    CU_ASSERT_PTR_EQUAL(found1, attr1);
    printf("lookup 'count' = %p (esperado: %p)\n", (void *)found1, (void *)attr1);

    TypeDescriptor *found2 = user_type_lookup_attribute(type, "name");
    CU_ASSERT_PTR_EQUAL(found2, attr2);
    printf("lookup 'name' = %p (esperado: %p)\n", (void *)found2, (void *)attr2);

    user_type_destroy(type);
    type_destroy(attr1);
    type_destroy(attr2);
}

void test_user_type_lookup_attribute_not_found(void)
{
    printf("\n\n------------ Inicio test: test_user_type_lookup_attribute_not_found ------------\n");
    printf("Buscando atributo que no existe\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    TypeDescriptor *attr = type_create(HULK_NUMBER, "Number", NULL);
    user_type_add_attribute(type, "x", attr);

    TypeDescriptor *found = user_type_lookup_attribute(type, "no_existe");
    CU_ASSERT_PTR_NULL(found);
    printf("lookup 'no_existe' = %p (esperado: NULL)\n", (void *)found);

    user_type_destroy(type);
    type_destroy(attr);
}

void test_user_type_lookup_attribute_no_ancestor_search(void)
{
    printf("\n\n------------ Inicio test: test_user_type_lookup_attribute_no_ancestor_search ------------\n");
    printf("Verificando que lookup NO busca en ancestros (atributos privados)\n");

    List *parent_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *parent = user_type_create("Parent", NULL, parent_params);
    TypeDescriptor *parent_attr = type_create(HULK_NUMBER, "Number", NULL);
    user_type_add_attribute(parent, "secret", parent_attr);

    List *child_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *child = user_type_create("Child", (TypeDescriptor *)parent, child_params);

    TypeDescriptor *found = user_type_lookup_attribute(child, "secret");
    CU_ASSERT_PTR_NULL(found);
    printf("lookup 'secret' en child = %p (esperado: NULL)\n", (void *)found);

    TypeDescriptor *found_parent = user_type_lookup_attribute(parent, "secret");
    CU_ASSERT_PTR_EQUAL(found_parent, parent_attr);
    printf("lookup 'secret' en parent = %p (esperado: %p)\n", (void *)found_parent, (void *)parent_attr);

    user_type_destroy(child);
    user_type_destroy(parent);
    type_destroy(parent_attr);
}

void test_user_type_lookup_attribute_null(void)
{
    printf("\n\n------------ Inicio test: test_user_type_lookup_attribute_null ------------\n");
    printf("Buscando atributo con parametros NULL\n");

    CU_ASSERT_PTR_NULL(user_type_lookup_attribute(NULL, "x"));
    printf("lookup(NULL, 'x') = NULL\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    CU_ASSERT_PTR_NULL(user_type_lookup_attribute(type, NULL));
    printf("lookup(type, NULL) = NULL\n");

    CU_ASSERT_PTR_NULL(user_type_lookup_attribute(NULL, NULL));
    printf("lookup(NULL, NULL) = NULL\n");

    user_type_destroy(type);
}

void test_user_type_add_attribute_null(void)
{
    printf("\n\n------------ Inicio test: test_user_type_add_attribute_null ------------\n");
    printf("Agregando atributo con parametros NULL (no crashea)\n");

    user_type_add_attribute(NULL, "x", NULL);
    CU_PASS("user_type_add_attribute(NULL, 'x', NULL) no crasheo");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);
    TypeDescriptor *attr = type_create(HULK_NUMBER, "Number", NULL);

    user_type_add_attribute(type, NULL, attr);
    CU_PASS("user_type_add_attribute(type, NULL, attr) no crasheo");

    user_type_add_attribute(type, "x", NULL);
    CU_PASS("user_type_add_attribute(type, 'x', NULL) no crasheo");

    CU_ASSERT_EQUAL(list_count(type->attribute_names), 0);
    printf("attribute_names count = %zu (esperado: 0)\n", list_count(type->attribute_names));

    user_type_destroy(type);
    type_destroy(attr);
}

void test_user_type_add_and_has_method(void)
{
    printf("\n\n------------ Inicio test: test_user_type_add_and_has_method ------------\n");
    printf("Agregando y verificando metodos\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    user_type_add_method(type, "run");
    user_type_add_method(type, "stop");

    printf("method_names count = %zu (esperado: 2)\n", list_count(type->method_names));
    CU_ASSERT_EQUAL(list_count(type->method_names), 2);

    CU_ASSERT_TRUE(user_type_has_method(type, "run"));
    printf("has_method 'run' = true (esperado: true)\n");

    CU_ASSERT_TRUE(user_type_has_method(type, "stop"));
    printf("has_method 'stop' = true (esperado: true)\n");

    CU_ASSERT_FALSE(user_type_has_method(type, "no_existe"));
    printf("has_method 'no_existe' = false (esperado: false)\n");

    user_type_destroy(type);
}

void test_user_type_has_method_ancestor(void)
{
    printf("\n\n------------ Inicio test: test_user_type_has_method_ancestor ------------\n");
    printf("Verificando busqueda de metodos en ancestros\n");

    List *parent_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *parent = user_type_create("Parent", NULL, parent_params);
    user_type_add_method(parent, "foo");

    List *child_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *child = user_type_create("Child", (TypeDescriptor *)parent, child_params);
    user_type_add_method(child, "bar");

    CU_ASSERT_TRUE(user_type_has_method(child, "bar"));
    printf("child has_method 'bar' = true (esperado: true)\n");

    CU_ASSERT_TRUE(user_type_has_method(child, "foo"));
    printf("child has_method 'foo' (heredado) = true (esperado: true)\n");

    CU_ASSERT_FALSE(user_type_has_method(child, "baz"));
    printf("child has_method 'baz' = false (esperado: false)\n");

    CU_ASSERT_TRUE(user_type_has_method(parent, "foo"));
    printf("parent has_method 'foo' = true (esperado: true)\n");

    CU_ASSERT_FALSE(user_type_has_method(parent, "bar"));
    printf("parent has_method 'bar' = false (esperado: false)\n");

    user_type_destroy(child);
    user_type_destroy(parent);
}

void test_user_type_has_method_builtin_parent(void)
{
    printf("\n\n------------ Inicio test: test_user_type_has_method_builtin_parent ------------\n");
    printf("Verificando metodos con padre built-in\n");

    TypeDescriptor *parent = type_create(HULK_OBJECT, "Object", NULL);
    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Derived", parent, params);

    user_type_add_method(type, "my_method");

    CU_ASSERT_TRUE(user_type_has_method(type, "my_method"));
    printf("has_method 'my_method' = true (esperado: true)\n");

    CU_ASSERT_FALSE(user_type_has_method(type, "other"));
    printf("has_method 'other' = false (esperado: false)\n");

    user_type_destroy(type);
    type_destroy(parent);
}

void test_user_type_has_method_null(void)
{
    printf("\n\n------------ Inicio test: test_user_type_has_method_null ------------\n");
    printf("Verificando has_method con parametros NULL\n");

    CU_ASSERT_FALSE(user_type_has_method(NULL, "foo"));
    printf("has_method(NULL, 'foo') = false (esperado: false)\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    CU_ASSERT_FALSE(user_type_has_method(type, NULL));
    printf("has_method(type, NULL) = false (esperado: false)\n");

    CU_ASSERT_FALSE(user_type_has_method(NULL, NULL));
    printf("has_method(NULL, NULL) = false (esperado: false)\n");

    user_type_destroy(type);
}

void test_user_type_add_method_null(void)
{
    printf("\n\n------------ Inicio test: test_user_type_add_method_null ------------\n");
    printf("Agregando metodo con parametros NULL (no crashea)\n");

    user_type_add_method(NULL, "foo");
    CU_PASS("user_type_add_method(NULL, 'foo') no crasheo");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("Test", NULL, params);

    user_type_add_method(type, NULL);
    CU_PASS("user_type_add_method(type, NULL) no crasheo");

    CU_ASSERT_EQUAL(list_count(type->method_names), 0);
    printf("method_names count = %zu (esperado: 0)\n", list_count(type->method_names));

    user_type_destroy(type);
}

void test_user_type_destroy_null(void)
{
    printf("\n\n------------ Inicio test: test_user_type_destroy_null ------------\n");
    printf("Destruyendo NULL (no crashea)\n");

    user_type_destroy(NULL);
    CU_PASS("user_type_destroy(NULL) no crasheo");
}

void test_user_type_destroy_with_data(void)
{
    printf("\n\n------------ Inicio test: test_user_type_destroy_with_data ------------\n");
    printf("Destruyendo tipo con atributos y metodos\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type = user_type_create("FullType", NULL, params);

    TypeDescriptor *attr1 = type_create(HULK_NUMBER, "Number", NULL);
    TypeDescriptor *attr2 = type_create(HULK_STRING, "String", NULL);
    user_type_add_attribute(type, "a", attr1);
    user_type_add_attribute(type, "b", attr2);
    user_type_add_method(type, "m1");
    user_type_add_method(type, "m2");

    printf("attribute_names count = %zu\n", list_count(type->attribute_names));
    printf("attribute_types count = %zu\n", list_count(type->attribute_types));
    printf("method_names count = %zu\n", list_count(type->method_names));

    user_type_destroy(type);
    type_destroy(attr1);
    type_destroy(attr2);

    CU_PASS("user_type_destroy con datos no crasheo");
}

int main(void)
{
    printf("\n=== Iniciando tests de UserTypeDescriptor ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("UserTypeDescriptor Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_user_type_create_basic", test_user_type_create_basic);
    CU_add_test(suite, "test_user_type_create_with_parent_user_defined", test_user_type_create_with_parent_user_defined);
    CU_add_test(suite, "test_user_type_create_with_parent_builtin", test_user_type_create_with_parent_builtin);
    CU_add_test(suite, "test_user_type_create_null_name", test_user_type_create_null_name);

    CU_add_test(suite, "test_type_to_user_defined_valid", test_type_to_user_defined_valid);
    CU_add_test(suite, "test_type_to_user_defined_invalid_tag", test_type_to_user_defined_invalid_tag);
    CU_add_test(suite, "test_type_to_user_defined_null", test_type_to_user_defined_null);

    CU_add_test(suite, "test_user_type_get_param_types", test_user_type_get_param_types);
    CU_add_test(suite, "test_user_type_get_param_types_null", test_user_type_get_param_types_null);

    CU_add_test(suite, "test_user_type_update_param_type", test_user_type_update_param_type);
    CU_add_test(suite, "test_user_type_update_param_type_null_type", test_user_type_update_param_type_null_type);

    CU_add_test(suite, "test_user_type_add_and_lookup_attribute", test_user_type_add_and_lookup_attribute);
    CU_add_test(suite, "test_user_type_lookup_attribute_not_found", test_user_type_lookup_attribute_not_found);
    CU_add_test(suite, "test_user_type_lookup_attribute_no_ancestor_search", test_user_type_lookup_attribute_no_ancestor_search);
    CU_add_test(suite, "test_user_type_lookup_attribute_null", test_user_type_lookup_attribute_null);
    CU_add_test(suite, "test_user_type_add_attribute_null", test_user_type_add_attribute_null);

    CU_add_test(suite, "test_user_type_add_and_has_method", test_user_type_add_and_has_method);
    CU_add_test(suite, "test_user_type_has_method_ancestor", test_user_type_has_method_ancestor);
    CU_add_test(suite, "test_user_type_has_method_builtin_parent", test_user_type_has_method_builtin_parent);
    CU_add_test(suite, "test_user_type_has_method_null", test_user_type_has_method_null);
    CU_add_test(suite, "test_user_type_add_method_null", test_user_type_add_method_null);

    CU_add_test(suite, "test_user_type_destroy_null", test_user_type_destroy_null);
    CU_add_test(suite, "test_user_type_destroy_with_data", test_user_type_destroy_with_data);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}