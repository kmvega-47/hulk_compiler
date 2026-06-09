#include <CUnit/Basic.h>
#include "scope.h"

static TypeDescriptor *obj_type;
static TypeDescriptor *num_type;
static TypeDescriptor *str_type;

static void setup_types(void)
{
    obj_type = type_create(HULK_OBJECT, "Object", NULL);
    num_type = type_create(HULK_NUMBER, "Number", obj_type);
    str_type = type_create(HULK_STRING, "String", obj_type);
}

static void teardown_types(void)
{
    type_destroy(str_type);
    type_destroy(num_type);
    type_destroy(obj_type);
}

void test_scope_create(void)
{
    printf("\n\n------------ Inicio test: test_scope_create ------------\n");
    printf("Creando scope sin padre\n");

    Scope *scope = scope_create(NULL);
    CU_ASSERT_PTR_NOT_NULL(scope);
    CU_ASSERT_PTR_NULL(scope->parent);
    printf("scope = %p, parent = %p (esperado: NULL)\n", (void *)scope, (void *)scope->parent);

    scope_destroy(scope);
}

void test_scope_create_with_parent(void)
{
    printf("\n\n------------ Inicio test: test_scope_create_with_parent ------------\n");
    printf("Creando scope con padre\n");

    Scope *parent = scope_create(NULL);
    Scope *child = scope_create(parent);

    CU_ASSERT_PTR_NOT_NULL(child);
    CU_ASSERT_PTR_EQUAL(child->parent, parent);
    printf("child.parent = %p (esperado: %p)\n", (void *)child->parent, (void *)parent);

    scope_destroy(child);
    scope_destroy(parent);
}

void test_scope_destroy_null(void)
{
    printf("\n\n------------ Inicio test: test_scope_destroy_null ------------\n");
    printf("Destruyendo NULL\n");

    scope_destroy(NULL);
    CU_PASS("scope_destroy(NULL) no crasheo");
}

void test_scope_add_variable(void)
{
    printf("\n\n------------ Inicio test: test_scope_add_variable ------------\n");
    printf("Agregando variable\n");

    Scope *scope = scope_create(NULL);
    bool added = scope_add_variable(scope, "x", num_type);
    CU_ASSERT_TRUE(added);
    printf("add_variable('x') = %s (esperado: true)\n", added ? "true" : "false");

    TypeDescriptor *found = scope_lookup(scope, "x");
    CU_ASSERT_PTR_EQUAL(found, num_type);
    printf("lookup('x') = %s (esperado: %s)\n", found ? found->name : "NULL", num_type->name);

    scope_destroy(scope);
}

void test_scope_add_parameter(void)
{
    printf("\n\n------------ Inicio test: test_scope_add_parameter ------------\n");
    printf("Agregando parametro\n");

    Scope *scope = scope_create(NULL);
    bool added = scope_add_parameter(scope, "p", str_type);
    CU_ASSERT_TRUE(added);
    printf("add_parameter('p') = %s (esperado: true)\n", added ? "true" : "false");

    CU_ASSERT_TRUE(scope_is_parameter(scope, "p"));
    printf("is_parameter('p') = true (esperado: true)\n");

    CU_ASSERT_FALSE(scope_is_constant(scope, "p"));
    printf("is_constant('p') = false (esperado: false)\n");

    scope_destroy(scope);
}

void test_scope_add_constant(void)
{
    printf("\n\n------------ Inicio test: test_scope_add_constant ------------\n");
    printf("Agregando constante\n");

    Scope *scope = scope_create(NULL);
    bool added = scope_add_constant(scope, "PI", num_type);
    CU_ASSERT_TRUE(added);
    printf("add_constant('PI') = %s (esperado: true)\n", added ? "true" : "false");

    CU_ASSERT_TRUE(scope_is_constant(scope, "PI"));
    printf("is_constant('PI') = true (esperado: true)\n");

    CU_ASSERT_FALSE(scope_is_parameter(scope, "PI"));
    printf("is_parameter('PI') = false (esperado: false)\n");

    scope_destroy(scope);
}

void test_scope_add_self_instance(void)
{
    printf("\n\n------------ Inicio test: test_scope_add_self_instance ------------\n");
    printf("Agregando self instance\n");

    Scope *scope = scope_create(NULL);
    bool added = scope_add_self_instance(scope, obj_type);
    CU_ASSERT_TRUE(added);
    printf("add_self_instance() = %s (esperado: true)\n", added ? "true" : "false");

    TypeDescriptor *found = scope_lookup(scope, "self");
    CU_ASSERT_PTR_EQUAL(found, obj_type);
    printf("lookup('self') = %s (esperado: %s)\n", found ? found->name : "NULL", obj_type->name);

    CU_ASSERT_TRUE(scope_is_self_instance(scope, "self"));
    printf("is_self_instance('self') = true (esperado: true)\n");

    scope_destroy(scope);
}

void test_scope_add_duplicate_in_same_scope(void)
{
    printf("\n\n------------ Inicio test: test_scope_add_duplicate_in_same_scope ------------\n");
    printf("Intentando agregar duplicado en el mismo scope\n");

    Scope *scope = scope_create(NULL);
    bool first = scope_add_variable(scope, "x", num_type);
    CU_ASSERT_TRUE(first);
    printf("primer add_variable('x') = %s (esperado: true)\n", first ? "true" : "false");

    bool second = scope_add_variable(scope, "x", str_type);
    CU_ASSERT_FALSE(second);
    printf("segundo add_variable('x') = %s (esperado: false)\n", second ? "true" : "false");

    TypeDescriptor *found = scope_lookup(scope, "x");
    CU_ASSERT_PTR_EQUAL(found, num_type);
    printf("lookup('x') sigue siendo %s (esperado: %s)\n", found ? found->name : "NULL", num_type->name);

    scope_destroy(scope);
}

void test_scope_shadowing(void)
{
    printf("\n\n------------ Inicio test: test_scope_shadowing ------------\n");
    printf("Verificando shadowing entre scopes\n");

    Scope *parent = scope_create(NULL);
    scope_add_variable(parent, "x", num_type);

    Scope *child = scope_create(parent);
    scope_add_variable(child, "x", str_type);

    TypeDescriptor *from_child = scope_lookup(child, "x");
    CU_ASSERT_PTR_EQUAL(from_child, str_type);
    printf("lookup('x') desde child = %s (esperado: %s)\n",
           from_child ? from_child->name : "NULL", str_type->name);

    TypeDescriptor *from_parent = scope_lookup(parent, "x");
    CU_ASSERT_PTR_EQUAL(from_parent, num_type);
    printf("lookup('x') desde parent = %s (esperado: %s)\n",
           from_parent ? from_parent->name : "NULL", num_type->name);

    scope_destroy(child);
    scope_destroy(parent);
}

void test_scope_lookup_ancestor(void)
{
    printf("\n\n------------ Inicio test: test_scope_lookup_ancestor ------------\n");
    printf("Buscando variable en ancestro\n");

    Scope *grandparent = scope_create(NULL);
    scope_add_variable(grandparent, "a", num_type);

    Scope *parent = scope_create(grandparent);
    Scope *child = scope_create(parent);

    TypeDescriptor *found = scope_lookup(child, "a");
    CU_ASSERT_PTR_EQUAL(found, num_type);
    printf("lookup('a') desde child = %s (esperado: %s)\n",
           found ? found->name : "NULL", num_type->name);

    scope_destroy(child);
    scope_destroy(parent);
    scope_destroy(grandparent);
}

void test_scope_lookup_not_found(void)
{
    printf("\n\n------------ Inicio test: test_scope_lookup_not_found ------------\n");
    printf("Buscando variable inexistente\n");

    Scope *scope = scope_create(NULL);
    TypeDescriptor *found = scope_lookup(scope, "no_existe");
    CU_ASSERT_PTR_NULL(found);
    printf("lookup('no_existe') = %s (esperado: NULL)\n", found ? found->name : "NULL");

    scope_destroy(scope);
}

void test_scope_lookup_current(void)
{
    printf("\n\n------------ Inicio test: test_scope_lookup_current ------------\n");
    printf("Verificando lookup solo en scope actual\n");

    Scope *parent = scope_create(NULL);
    scope_add_variable(parent, "x", num_type);

    Scope *child = scope_create(parent);
    scope_add_variable(child, "y", str_type);

    CU_ASSERT_TRUE(scope_lookup_current(child, "y"));
    printf("lookup_current(child, 'y') = true (esperado: true)\n");

    CU_ASSERT_FALSE(scope_lookup_current(child, "x"));
    printf("lookup_current(child, 'x') = false (esperado: false)\n");

    CU_ASSERT_TRUE(scope_lookup_current(parent, "x"));
    printf("lookup_current(parent, 'x') = true (esperado: true)\n");

    scope_destroy(child);
    scope_destroy(parent);
}

void test_scope_update_symbol(void)
{
    printf("\n\n------------ Inicio test: test_scope_update_symbol ------------\n");
    printf("Actualizando tipo de simbolo\n");

    Scope *scope = scope_create(NULL);
    scope_add_variable(scope, "x", num_type);

    TypeDescriptor *before = scope_lookup(scope, "x");
    CU_ASSERT_PTR_EQUAL(before, num_type);
    printf("antes: lookup('x') = %s\n", before->name);

    scope_update_symbol(scope, "x", str_type);

    TypeDescriptor *after = scope_lookup(scope, "x");
    CU_ASSERT_PTR_EQUAL(after, str_type);
    printf("despues: lookup('x') = %s (esperado: %s)\n",
           after ? after->name : "NULL", str_type->name);

    scope_destroy(scope);
}

void test_scope_update_symbol_in_ancestor(void)
{
    printf("\n\n------------ Inicio test: test_scope_update_symbol_in_ancestor ------------\n");
    printf("Actualizando simbolo en ancestro desde scope hijo\n");

    Scope *parent = scope_create(NULL);
    scope_add_variable(parent, "x", num_type);

    Scope *child = scope_create(parent);
    scope_update_symbol(child, "x", str_type);

    TypeDescriptor *from_parent = scope_lookup(parent, "x");
    CU_ASSERT_PTR_EQUAL(from_parent, str_type);
    printf("lookup('x') desde parent = %s (esperado: %s)\n",
           from_parent ? from_parent->name : "NULL", str_type->name);

    scope_destroy(child);
    scope_destroy(parent);
}

void test_scope_is_functions(void)
{
    printf("\n\n------------ Inicio test: test_scope_is_functions ------------\n");
    printf("Verificando flags de simbolos\n");

    Scope *scope = scope_create(NULL);

    scope_add_variable(scope, "var", num_type);
    CU_ASSERT_FALSE(scope_is_constant(scope, "var"));
    CU_ASSERT_FALSE(scope_is_parameter(scope, "var"));
    CU_ASSERT_FALSE(scope_is_self_instance(scope, "var"));
    printf("variable 'var': constant=%s, parameter=%s, self=%s (esperado: false, false, false)\n",
           scope_is_constant(scope, "var") ? "true" : "false",
           scope_is_parameter(scope, "var") ? "true" : "false",
           scope_is_self_instance(scope, "var") ? "true" : "false");

    scope_add_constant(scope, "C", num_type);
    CU_ASSERT_TRUE(scope_is_constant(scope, "C"));
    CU_ASSERT_FALSE(scope_is_parameter(scope, "C"));
    printf("constante 'C': constant=%s, parameter=%s (esperado: true, false)\n",
           scope_is_constant(scope, "C") ? "true" : "false",
           scope_is_parameter(scope, "C") ? "true" : "false");

    scope_add_parameter(scope, "p", str_type);
    CU_ASSERT_TRUE(scope_is_parameter(scope, "p"));
    CU_ASSERT_FALSE(scope_is_constant(scope, "p"));
    printf("parametro 'p': parameter=%s, constant=%s (esperado: true, false)\n",
           scope_is_parameter(scope, "p") ? "true" : "false",
           scope_is_constant(scope, "p") ? "true" : "false");

    scope_destroy(scope);
}

void test_scope_is_functions_not_found(void)
{
    printf("\n\n------------ Inicio test: test_scope_is_functions_not_found ------------\n");
    printf("Verificando flags con simbolo inexistente\n");

    Scope *scope = scope_create(NULL);

    CU_ASSERT_FALSE(scope_is_constant(scope, "x"));
    CU_ASSERT_FALSE(scope_is_parameter(scope, "x"));
    CU_ASSERT_FALSE(scope_is_self_instance(scope, "x"));
    printf("simbolo inexistente: constant=%s, parameter=%s, self=%s (esperado: false, false, false)\n",
           scope_is_constant(scope, "x") ? "true" : "false",
           scope_is_parameter(scope, "x") ? "true" : "false",
           scope_is_self_instance(scope, "x") ? "true" : "false");

    scope_destroy(scope);
}

void test_scope_destroy_until(void)
{
    printf("\n\n------------ Inicio test: test_scope_destroy_until ------------\n");
    printf("Destruyendo cadena de scopes hasta un target\n");

    Scope *s1 = scope_create(NULL);
    Scope *s2 = scope_create(s1);
    Scope *s3 = scope_create(s2);
    Scope *s4 = scope_create(s3);

    scope_add_variable(s1, "a", num_type);
    scope_add_variable(s2, "b", num_type);
    scope_add_variable(s3, "c", num_type);
    scope_add_variable(s4, "d", num_type);

    scope_destroy_until(s4, s2);

    CU_ASSERT_PTR_NOT_NULL(s2);
    printf("s2 sigue vivo\n");

    CU_ASSERT_PTR_NOT_NULL(s1);
    printf("s1 sigue vivo\n");

    CU_ASSERT_PTR_NOT_NULL(scope_lookup(s2, "b"));
    printf("lookup('b') en s2 = %s (esperado: %s)\n", scope_lookup(s2, "b")->name, num_type->name);

    CU_ASSERT_PTR_NOT_NULL(scope_lookup(s1, "a"));
    printf("lookup('a') en s1 = %s (esperado: %s)\n", scope_lookup(s1, "a")->name, num_type->name);

    scope_destroy(s2);
    scope_destroy(s1);
}

void test_scope_destroy_until_null_target(void)
{
    printf("\n\n------------ Inicio test: test_scope_destroy_until_null_target ------------\n");
    printf("Destruyendo toda la cadena (target=NULL)\n");

    Scope *s1 = scope_create(NULL);
    Scope *s2 = scope_create(s1);
    Scope *s3 = scope_create(s2);

    scope_destroy_until(s3, NULL);
    CU_PASS("scope_destroy_until(s3, NULL) no crasheo");
}

int main(void)
{
    printf("\n=== Iniciando tests de Scope ===\n");

    setup_types();

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Scope Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        teardown_types();
        return CU_get_error();
    }

    CU_add_test(suite, "test_scope_create", test_scope_create);
    CU_add_test(suite, "test_scope_create_with_parent", test_scope_create_with_parent);
    CU_add_test(suite, "test_scope_destroy_null", test_scope_destroy_null);
    CU_add_test(suite, "test_scope_add_variable", test_scope_add_variable);
    CU_add_test(suite, "test_scope_add_parameter", test_scope_add_parameter);
    CU_add_test(suite, "test_scope_add_constant", test_scope_add_constant);
    CU_add_test(suite, "test_scope_add_self_instance", test_scope_add_self_instance);
    CU_add_test(suite, "test_scope_add_duplicate_in_same_scope", test_scope_add_duplicate_in_same_scope);
    CU_add_test(suite, "test_scope_shadowing", test_scope_shadowing);
    CU_add_test(suite, "test_scope_lookup_ancestor", test_scope_lookup_ancestor);
    CU_add_test(suite, "test_scope_lookup_not_found", test_scope_lookup_not_found);
    CU_add_test(suite, "test_scope_lookup_current", test_scope_lookup_current);
    CU_add_test(suite, "test_scope_update_symbol", test_scope_update_symbol);
    CU_add_test(suite, "test_scope_update_symbol_in_ancestor", test_scope_update_symbol_in_ancestor);
    CU_add_test(suite, "test_scope_is_functions", test_scope_is_functions);
    CU_add_test(suite, "test_scope_is_functions_not_found", test_scope_is_functions_not_found);
    CU_add_test(suite, "test_scope_destroy_until", test_scope_destroy_until);
    CU_add_test(suite, "test_scope_destroy_until_null_target", test_scope_destroy_until_null_target);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    teardown_types();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}