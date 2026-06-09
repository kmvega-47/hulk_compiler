#include <CUnit/Basic.h>
#include "type_descriptor.h"

static TypeDescriptor *obj;
static TypeDescriptor *num;
static TypeDescriptor *str;
static TypeDescriptor *bl;
static TypeDescriptor *vd;

static void setup_builtin_types(void)
{
    printf("Setting up builtin types...\n");

    obj = type_create(HULK_OBJECT, "Object", NULL);
    num = type_create(HULK_NUMBER, "Number", obj);
    str = type_create(HULK_STRING, "String", obj);
    bl  = type_create(HULK_BOOL,   "Bool",   obj);
    vd  = type_create(HULK_VOID,   "Void",   obj);
}

static void teardown_builtin_types(void)
{
    printf("Tearing down builtin types...\n");
    type_destroy(obj);
    type_destroy(num);
    type_destroy(str);
    type_destroy(bl);
    type_destroy(vd);
}

void test_type_create(void)
{
    printf("\n\n------------ Inicio test: test_type_create ------------\n");
    printf("Verificando creación de tipo sin padre\n");

    TypeDescriptor *t = type_create(HULK_NUMBER, "Number", NULL);
    CU_ASSERT_PTR_NOT_NULL(t);
    CU_ASSERT_EQUAL(t->tag, HULK_NUMBER);
    CU_ASSERT_STRING_EQUAL(t->name, "NUMBER");
    CU_ASSERT_PTR_NULL(t->parent);

    type_destroy(t);
}

void test_type_create_with_parent(void)
{
    printf("\n\n------------ Inicio test: test_type_create_with_parent ------------\n");
    printf("Verificando creación de tipo con padre\n");

    TypeDescriptor *parent = type_create(HULK_OBJECT, "Object", NULL);
    TypeDescriptor *child  = type_create(HULK_NUMBER, "Number", parent);

    CU_ASSERT_PTR_NOT_NULL(child);
    CU_ASSERT_EQUAL(child->tag, HULK_NUMBER);
    CU_ASSERT_STRING_EQUAL(child->name, "NUMBER");
    CU_ASSERT_PTR_EQUAL(child->parent, parent);

    type_destroy(child);
    type_destroy(parent);
}

void test_type_create_user_defined(void)
{
    printf("\n\n------------ Inicio test: test_type_create_user_defined ------------\n");
    printf("Verificando creación de tipo definido por el usuario\n");

    TypeDescriptor *base = type_create(HULK_OBJECT, "Object", NULL);
    TypeDescriptor *user = type_create(HULK_USER_DEFINED, "MyType", base);

    CU_ASSERT_PTR_NOT_NULL(user);
    CU_ASSERT_EQUAL(user->tag, HULK_USER_DEFINED);
    CU_ASSERT_STRING_EQUAL(user->name, "MYTYPE");
    CU_ASSERT_PTR_EQUAL(user->parent, base);

    type_destroy(user);
    type_destroy(base);
}

void test_type_create_null_name(void)
{
    printf("\n\n------------ Inicio test: test_type_create_null_name ------------\n");
    printf("Verificando creación de tipo con name=NULL\n");

    TypeDescriptor *t = type_create(HULK_NUMBER, NULL, NULL);
    CU_ASSERT_PTR_NOT_NULL(t);
    CU_ASSERT_EQUAL(t->tag, HULK_NUMBER);
    CU_ASSERT_PTR_NULL(t->name);
    CU_ASSERT_PTR_NULL(t->parent);

    type_destroy(t);
}

void test_type_conforms_to_same_type(void)
{
    printf("\n\n------------ Inicio test: test_type_conforms_to_same_type ------------\n");
    setup_builtin_types();

    printf("Verificando conformidad de un tipo consigo mismo\n");

    CU_ASSERT_TRUE(type_conforms_to(num, num));
    CU_ASSERT_TRUE(type_conforms_to(str, str));
    CU_ASSERT_TRUE(type_conforms_to(bl, bl));

    teardown_builtin_types();
}

void test_type_conforms_to_parent(void)
{
    printf("\n\n------------ Inicio test: test_type_conforms_to_parent ------------\n");
    setup_builtin_types();

    printf("Verificando conformidad de hijos a Object\n");

    CU_ASSERT_TRUE(type_conforms_to(num, obj));
    CU_ASSERT_TRUE(type_conforms_to(str, obj));
    CU_ASSERT_TRUE(type_conforms_to(bl, obj));

    teardown_builtin_types();
}

void test_type_conforms_to_not_parent(void)
{
    printf("\n\n------------ Inicio test: test_type_conforms_to_not_parent ------------\n");
    setup_builtin_types();

    printf("Verificando no conformidad entre tipos no relacionados\n");

    CU_ASSERT_FALSE(type_conforms_to(num, str));
    CU_ASSERT_FALSE(type_conforms_to(str, num));
    CU_ASSERT_FALSE(type_conforms_to(bl, num));

    teardown_builtin_types();
}

void test_type_conforms_to_null(void)
{
    printf("\n\n------------ Inicio test: test_type_conforms_to_null ------------\n");
    setup_builtin_types();

    printf("Verificando manejo de NULL en conforms_to\n");

    CU_ASSERT_FALSE(type_conforms_to(NULL, num));
    CU_ASSERT_FALSE(type_conforms_to(num, NULL));
    CU_ASSERT_FALSE(type_conforms_to(NULL, NULL));

    teardown_builtin_types();
}

void test_type_conforms_to_grandparent(void)
{
    printf("\n\n------------ Inicio test: test_type_conforms_to_grandparent ------------\n");

    TypeDescriptor *grandparent = type_create(HULK_OBJECT, "Object", NULL);
    TypeDescriptor *parent      = type_create(HULK_NUMBER, "Number", grandparent);
    TypeDescriptor *child       = type_create(HULK_USER_DEFINED, "MyType", parent);

    printf("Verificando herencia multinivel\n");

    CU_ASSERT_TRUE(type_conforms_to(child, grandparent));
    CU_ASSERT_TRUE(type_conforms_to(child, parent));
    CU_ASSERT_TRUE(type_conforms_to(parent, grandparent));

    type_destroy(child);
    type_destroy(parent);
    type_destroy(grandparent);
}

void test_type_lca_same_type(void)
{
    printf("\n\n------------ Inicio test: test_type_lca_same_type ------------\n");
    setup_builtin_types();

    printf("Verificando LCA de un tipo consigo mismo\n");

    TypeDescriptor *result = type_lca(num, num);
    CU_ASSERT_PTR_EQUAL(result, num);

    teardown_builtin_types();
}

void test_type_lca_siblings(void)
{
    printf("\n\n------------ Inicio test: test_type_lca_siblings ------------\n");
    setup_builtin_types();

    printf("Verificando LCA de tipos hermanos\n");

    TypeDescriptor *result = type_lca(num, str);
    CU_ASSERT_PTR_EQUAL(result, obj);

    teardown_builtin_types();
}

void test_type_lca_parent_child(void)
{
    printf("\n\n------------ Inicio test: test_type_lca_parent_child ------------\n");
    setup_builtin_types();

    printf("Verificando LCA de padre e hijo\n");

    TypeDescriptor *result = type_lca(num, obj);
    CU_ASSERT_PTR_EQUAL(result, obj);

    teardown_builtin_types();
}

void test_type_lca_different_depths(void)
{
    printf("\n\n------------ Inicio test: test_type_lca_different_depths ------------\n");
    setup_builtin_types();

    printf("Verificando LCA con diferentes profundidades\n");

    TypeDescriptor *child = type_create(HULK_USER_DEFINED, "Child", num);
    TypeDescriptor *result = type_lca(child, str);
    CU_ASSERT_PTR_EQUAL(result, obj);

    type_destroy(child);
    teardown_builtin_types();
}

void test_type_lca_null(void)
{
    printf("\n\n------------ Inicio test: test_type_lca_null ------------\n");
    setup_builtin_types();

    printf("Verificando LCA con NULL\n");

    CU_ASSERT_PTR_NULL(type_lca(NULL, num));
    CU_ASSERT_PTR_NULL(type_lca(num, NULL));
    CU_ASSERT_PTR_NULL(type_lca(NULL, NULL));

    teardown_builtin_types();
}

void test_type_cmp_same(void)
{
    printf("\n\n------------ Inicio test: test_type_cmp_same ------------\n");
    setup_builtin_types();

    printf("Verificando comparación de tipos idénticos\n");

    CU_ASSERT_EQUAL(type_cmp(num, num), 0);
    CU_ASSERT_EQUAL(type_cmp(obj, obj), 0);

    teardown_builtin_types();
}

void test_type_cmp_different(void)
{
    printf("\n\n------------ Inicio test: test_type_cmp_different ------------\n");
    setup_builtin_types();

    printf("Verificando comparación de tipos diferentes\n");

    CU_ASSERT_NOT_EQUAL(type_cmp(num, str), 0);
    CU_ASSERT_NOT_EQUAL(type_cmp(obj, num), 0);

    teardown_builtin_types();
}

void test_type_cmp_null(void)
{
    printf("\n\n------------ Inicio test: test_type_cmp_null ------------\n");
    setup_builtin_types();

    printf("Verificando comparación cuando uno o ambos son NULL\n");

    CU_ASSERT_EQUAL(type_cmp(NULL, NULL), 0);
    CU_ASSERT_NOT_EQUAL(type_cmp(num, NULL), 0);
    CU_ASSERT_NOT_EQUAL(type_cmp(NULL, num), 0);

    teardown_builtin_types();
}

void test_type_destroy_null(void)
{
    printf("\n\n------------ Inicio test: test_type_destroy_null ------------\n");
    printf("Verificando que type_destroy(NULL) no crashea\n");

    type_destroy(NULL);
    CU_PASS("type_destroy(NULL) executed without errors");
}

void test_type_destroy_valid(void)
{
    printf("\n\n------------ Inicio test: test_type_destroy_valid ------------\n");
    printf("Verificando que type_destroy con un tipo válido no crashea\n");

    TypeDescriptor *t = type_create(HULK_NUMBER, "Number", NULL);
    CU_ASSERT_PTR_NOT_NULL(t);
    type_destroy(t);

    CU_PASS("type_destroy con tipo válido ejecutado sin errores");
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeDescriptor ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeDescriptor Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_type_create", test_type_create);
    CU_add_test(suite, "test_type_create_with_parent", test_type_create_with_parent);
    CU_add_test(suite, "test_type_create_user_defined", test_type_create_user_defined);
    CU_add_test(suite, "test_type_create_null_name", test_type_create_null_name);

    CU_add_test(suite, "test_type_conforms_to_same_type", test_type_conforms_to_same_type);
    CU_add_test(suite, "test_type_conforms_to_parent", test_type_conforms_to_parent);
    CU_add_test(suite, "test_type_conforms_to_not_parent", test_type_conforms_to_not_parent);
    CU_add_test(suite, "test_type_conforms_to_null", test_type_conforms_to_null);
    CU_add_test(suite, "test_type_conforms_to_grandparent", test_type_conforms_to_grandparent);

    CU_add_test(suite, "test_type_lca_same_type", test_type_lca_same_type);
    CU_add_test(suite, "test_type_lca_siblings", test_type_lca_siblings);
    CU_add_test(suite, "test_type_lca_parent_child", test_type_lca_parent_child);
    CU_add_test(suite, "test_type_lca_different_depths", test_type_lca_different_depths);
    CU_add_test(suite, "test_type_lca_null", test_type_lca_null);

    CU_add_test(suite, "test_type_cmp_same", test_type_cmp_same);
    CU_add_test(suite, "test_type_cmp_different", test_type_cmp_different);
    CU_add_test(suite, "test_type_cmp_null", test_type_cmp_null);

    CU_add_test(suite, "test_type_destroy_null", test_type_destroy_null);
    CU_add_test(suite, "test_type_destroy_valid", test_type_destroy_valid);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}