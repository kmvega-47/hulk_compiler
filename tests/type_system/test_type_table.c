#include <CUnit/Basic.h>
#include "type_table.h"

void test_type_table_create(void)
{
    printf("\n\n------------ Inicio test: test_type_table_create ------------\n");
    printf("Creando tabla de tipos\n");

    TypeTable *table = type_table_create();
    CU_ASSERT_PTR_NOT_NULL(table);
    printf("table = %p\n", (void *)table);

    CU_ASSERT_PTR_NOT_NULL(table->builtin_types);
    printf("builtin_types = %p\n", (void *)table->builtin_types);

    CU_ASSERT_PTR_NOT_NULL(table->user_types);
    printf("user_types = %p\n", (void *)table->user_types);

    CU_ASSERT_EQUAL(list_count(table->user_types), 0);
    printf("user_types count = %zu (esperado: 0)\n", list_count(table->user_types));

    type_table_destroy(table);
}

void test_type_table_builtins(void)
{
    printf("\n\n------------ Inicio test: test_type_table_builtins ------------\n");
    printf("Verificando tipos built-in\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *obj = type_table_lookup_by_tag(table, HULK_OBJECT);
    CU_ASSERT_PTR_NOT_NULL(obj);
    CU_ASSERT_EQUAL(obj->tag, HULK_OBJECT);
    CU_ASSERT_STRING_EQUAL(obj->name, "Object");
    CU_ASSERT_PTR_NULL(obj->parent);
    printf("Object: tag=%d, name='%s', parent=%p (esperado: NULL)\n",
           obj->tag, obj->name, (void *)obj->parent);

    TypeDescriptor *num = type_table_lookup_by_tag(table, HULK_NUMBER);
    CU_ASSERT_PTR_NOT_NULL(num);
    CU_ASSERT_EQUAL(num->tag, HULK_NUMBER);
    CU_ASSERT_STRING_EQUAL(num->name, "Number");
    CU_ASSERT_PTR_EQUAL(num->parent, obj);
    printf("Number: tag=%d, name='%s', parent='%s' (esperado: 'Object')\n",
           num->tag, num->name, num->parent ? num->parent->name : "NULL");

    TypeDescriptor *str = type_table_lookup_by_tag(table, HULK_STRING);
    CU_ASSERT_PTR_NOT_NULL(str);
    CU_ASSERT_EQUAL(str->tag, HULK_STRING);
    CU_ASSERT_STRING_EQUAL(str->name, "String");
    CU_ASSERT_PTR_EQUAL(str->parent, obj);
    printf("String: tag=%d, name='%s', parent='%s' (esperado: 'Object')\n",
           str->tag, str->name, str->parent ? str->parent->name : "NULL");

    TypeDescriptor *bl = type_table_lookup_by_tag(table, HULK_BOOL);
    CU_ASSERT_PTR_NOT_NULL(bl);
    CU_ASSERT_EQUAL(bl->tag, HULK_BOOL);
    CU_ASSERT_STRING_EQUAL(bl->name, "Bool");
    CU_ASSERT_PTR_EQUAL(bl->parent, obj);
    printf("Bool: tag=%d, name='%s', parent='%s' (esperado: 'Object')\n",
           bl->tag, bl->name, bl->parent ? bl->parent->name : "NULL");

    TypeDescriptor *vd = type_table_lookup_by_tag(table, HULK_VOID);
    CU_ASSERT_PTR_NOT_NULL(vd);
    CU_ASSERT_EQUAL(vd->tag, HULK_VOID);
    CU_ASSERT_STRING_EQUAL(vd->name, "Void");
    CU_ASSERT_PTR_EQUAL(vd->parent, obj);
    printf("Void: tag=%d, name='%s', parent='%s' (esperado: 'Object')\n",
           vd->tag, vd->name, vd->parent ? vd->parent->name : "NULL");

    type_table_destroy(table);
}

void test_type_table_lookup_by_tag_invalid(void)
{
    printf("\n\n------------ Inicio test: test_type_table_lookup_by_tag_invalid ------------\n");
    printf("Buscando por tag fuera de rango\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *found = type_table_lookup_by_tag(table, (HulkType)99);
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_tag(99) = %p (esperado: NULL)\n", (void *)found);

    found = type_table_lookup_by_tag(table, (HulkType)-1);
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_tag(-1) = %p (esperado: NULL)\n", (void *)found);

    type_table_destroy(table);
}

void test_type_table_lookup_by_tag_null_table(void)
{
    printf("\n\n------------ Inicio test: test_type_table_lookup_by_tag_null_table ------------\n");
    printf("Buscando por tag con tabla NULL\n");

    TypeDescriptor *found = type_table_lookup_by_tag(NULL, HULK_NUMBER);
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_tag(NULL, NUMBER) = %p (esperado: NULL)\n", (void *)found);
}

void test_type_table_lookup_by_name_builtin(void)
{
    printf("\n\n------------ Inicio test: test_type_table_lookup_by_name_builtin ------------\n");
    printf("Buscando tipos built-in por nombre\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *num = type_table_lookup_by_name(table, "Number");
    CU_ASSERT_PTR_NOT_NULL(num);
    CU_ASSERT_EQUAL(num->tag, HULK_NUMBER);
    CU_ASSERT_STRING_EQUAL(num->name, "Number");
    printf("lookup_by_name('Number'): tag=%d, name='%s'\n", num->tag, num->name);

    TypeDescriptor *obj = type_table_lookup_by_name(table, "Object");
    CU_ASSERT_PTR_NOT_NULL(obj);
    CU_ASSERT_EQUAL(obj->tag, HULK_OBJECT);
    printf("lookup_by_name('Object'): tag=%d, name='%s'\n", obj->tag, obj->name);

    TypeDescriptor *str = type_table_lookup_by_name(table, "String");
    CU_ASSERT_PTR_NOT_NULL(str);
    CU_ASSERT_EQUAL(str->tag, HULK_STRING);
    printf("lookup_by_name('String'): tag=%d, name='%s'\n", str->tag, str->name);

    TypeDescriptor *bl = type_table_lookup_by_name(table, "Bool");
    CU_ASSERT_PTR_NOT_NULL(bl);
    CU_ASSERT_EQUAL(bl->tag, HULK_BOOL);
    printf("lookup_by_name('Bool'): tag=%d, name='%s'\n", bl->tag, bl->name);

    TypeDescriptor *vd = type_table_lookup_by_name(table, "Void");
    CU_ASSERT_PTR_NOT_NULL(vd);
    CU_ASSERT_EQUAL(vd->tag, HULK_VOID);
    printf("lookup_by_name('Void'): tag=%d, name='%s'\n", vd->tag, vd->name);

    type_table_destroy(table);
}

void test_type_table_lookup_by_name_not_found(void)
{
    printf("\n\n------------ Inicio test: test_type_table_lookup_by_name_not_found ------------\n");
    printf("Buscando tipo inexistente por nombre\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *found = type_table_lookup_by_name(table, "NoExiste");
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_name('NoExiste') = %p (esperado: NULL)\n", (void *)found);

    type_table_destroy(table);
}

void test_type_table_lookup_by_name_null(void)
{
    printf("\n\n------------ Inicio test: test_type_table_lookup_by_name_null ------------\n");
    printf("Buscando por nombre con parametros NULL\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *found = type_table_lookup_by_name(NULL, "Number");
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_name(NULL, 'Number') = %p (esperado: NULL)\n", (void *)found);

    found = type_table_lookup_by_name(table, NULL);
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_name(table, NULL) = %p (esperado: NULL)\n", (void *)found);

    found = type_table_lookup_by_name(NULL, NULL);
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_name(NULL, NULL) = %p (esperado: NULL)\n", (void *)found);

    type_table_destroy(table);
}

void test_type_table_insert(void)
{
    printf("\n\n------------ Inicio test: test_type_table_insert ------------\n");
    printf("Insertando tipo definido por el usuario\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *obj = type_table_lookup_by_tag(table, HULK_OBJECT);
    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *user = user_type_create("Point", obj, params);

    CU_ASSERT_EQUAL(list_count(table->user_types), 0);
    printf("user_types count antes de insertar = %zu (esperado: 0)\n", list_count(table->user_types));

    bool inserted = type_table_insert(table, user);
    CU_ASSERT_TRUE(inserted);
    printf("insert('Point') = %s (esperado: true)\n", inserted ? "true" : "false");

    CU_ASSERT_EQUAL(list_count(table->user_types), 1);
    printf("user_types count despues de insertar = %zu (esperado: 1)\n", list_count(table->user_types));

    TypeDescriptor *found = type_table_lookup_by_name(table, "Point");
    CU_ASSERT_PTR_EQUAL(found, (TypeDescriptor *)user);
    printf("lookup_by_name('Point') = %p (esperado: %p)\n", (void *)found, (void *)user);

    type_table_destroy(table);
}

void test_type_table_insert_duplicate(void)
{
    printf("\n\n------------ Inicio test: test_type_table_insert_duplicate ------------\n");
    printf("Insertando tipo duplicado\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *obj = type_table_lookup_by_tag(table, HULK_OBJECT);
    List *params1 = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *user1 = user_type_create("MyType", obj, params1);

    List *params2 = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *user2 = user_type_create("MyType", obj, params2);

    bool first = type_table_insert(table, user1);
    CU_ASSERT_TRUE(first);
    printf("primer insert('MyType') = %s (esperado: true)\n", first ? "true" : "false");

    bool second = type_table_insert(table, user2);
    CU_ASSERT_FALSE(second);
    printf("segundo insert('MyType') = %s (esperado: false)\n", second ? "true" : "false");

    CU_ASSERT_EQUAL(list_count(table->user_types), 1);
    printf("user_types count = %zu (esperado: 1)\n", list_count(table->user_types));

    type_table_destroy(table);
    user_type_destroy(user2);
}

void test_type_table_insert_null(void)
{
    printf("\n\n------------ Inicio test: test_type_table_insert_null ------------\n");
    printf("Insertando NULL\n");

    TypeTable *table = type_table_create();

    bool inserted = type_table_insert(NULL, NULL);
    CU_ASSERT_FALSE(inserted);
    printf("insert(NULL, NULL) = %s (esperado: false)\n", inserted ? "true" : "false");

    inserted = type_table_insert(table, NULL);
    CU_ASSERT_FALSE(inserted);
    printf("insert(table, NULL) = %s (esperado: false)\n", inserted ? "true" : "false");

    CU_ASSERT_EQUAL(list_count(table->user_types), 0);
    printf("user_types count = %zu (esperado: 0)\n", list_count(table->user_types));

    type_table_destroy(table);
}

void test_type_table_lookup_by_name_user_type(void)
{
    printf("\n\n------------ Inicio test: test_type_table_lookup_by_name_user_type ------------\n");
    printf("Buscando tipo de usuario por nombre\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *obj = type_table_lookup_by_tag(table, HULK_OBJECT);
    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *user = user_type_create("MyType", obj, params);
    type_table_insert(table, user);

    TypeDescriptor *found = type_table_lookup_by_name(table, "MyType");
    CU_ASSERT_PTR_NOT_NULL(found);
    CU_ASSERT_EQUAL(found->tag, HULK_USER_DEFINED);
    CU_ASSERT_STRING_EQUAL(found->name, "MyType");
    printf("lookup_by_name('MyType'): tag=%d, name='%s'\n", found->tag, found->name);

    type_table_destroy(table);
}

void test_type_table_destroy_null(void)
{
    printf("\n\n------------ Inicio test: test_type_table_destroy_null ------------\n");
    printf("Destruyendo NULL\n");

    type_table_destroy(NULL);
    CU_PASS("type_table_destroy(NULL) no crasheo");
}

void test_type_table_case_sensitive(void)
{
    printf("\n\n------------ Inicio test: test_type_table_case_sensitive ------------\n");
    printf("Verificando que la busqueda es case-sensitive\n");

    TypeTable *table = type_table_create();

    TypeDescriptor *found = type_table_lookup_by_name(table, "number");
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_name('number') = %p (esperado: NULL)\n", (void *)found);

    found = type_table_lookup_by_name(table, "object");
    CU_ASSERT_PTR_NULL(found);
    printf("lookup_by_name('object') = %p (esperado: NULL)\n", (void *)found);

    found = type_table_lookup_by_name(table, "Number");
    CU_ASSERT_PTR_NOT_NULL(found);
    printf("lookup_by_name('Number') = %p (esperado: not NULL)\n", (void *)found);

    found = type_table_lookup_by_name(table, "Object");
    CU_ASSERT_PTR_NOT_NULL(found);
    printf("lookup_by_name('Object') = %p (esperado: not NULL)\n", (void *)found);

    type_table_destroy(table);
}

void test_type_table_multiple_user_types(void)
{
    printf("\n\n------------ Inicio test: test_type_table_multiple_user_types ------------\n");
    printf("Insertando multiples tipos de usuario\n");

    TypeTable *table = type_table_create();
    TypeDescriptor *obj = type_table_lookup_by_tag(table, HULK_OBJECT);

    List *params1 = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type_a = user_type_create("A", obj, params1);

    List *params2 = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type_b = user_type_create("B", obj, params2);

    List *params3 = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *type_c = user_type_create("C", obj, params3);

    type_table_insert(table, type_a);
    type_table_insert(table, type_b);
    type_table_insert(table, type_c);

    CU_ASSERT_EQUAL(list_count(table->user_types), 3);
    printf("user_types count = %zu (esperado: 3)\n", list_count(table->user_types));

    CU_ASSERT_PTR_EQUAL(type_table_lookup_by_name(table, "A"), (TypeDescriptor *)type_a);
    printf("lookup('A') = %p (esperado: %p)\n",
           (void *)type_table_lookup_by_name(table, "A"), (void *)type_a);

    CU_ASSERT_PTR_EQUAL(type_table_lookup_by_name(table, "B"), (TypeDescriptor *)type_b);
    printf("lookup('B') = %p (esperado: %p)\n",
           (void *)type_table_lookup_by_name(table, "B"), (void *)type_b);

    CU_ASSERT_PTR_EQUAL(type_table_lookup_by_name(table, "C"), (TypeDescriptor *)type_c);
    printf("lookup('C') = %p (esperado: %p)\n",
           (void *)type_table_lookup_by_name(table, "C"), (void *)type_c);

    type_table_destroy(table);
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeTable ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeTable Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_type_table_create", test_type_table_create);
    CU_add_test(suite, "test_type_table_builtins", test_type_table_builtins);
    CU_add_test(suite, "test_type_table_lookup_by_tag_invalid", test_type_table_lookup_by_tag_invalid);
    CU_add_test(suite, "test_type_table_lookup_by_tag_null_table", test_type_table_lookup_by_tag_null_table);
    CU_add_test(suite, "test_type_table_lookup_by_name_builtin", test_type_table_lookup_by_name_builtin);
    CU_add_test(suite, "test_type_table_lookup_by_name_not_found", test_type_table_lookup_by_name_not_found);
    CU_add_test(suite, "test_type_table_lookup_by_name_null", test_type_table_lookup_by_name_null);
    CU_add_test(suite, "test_type_table_insert", test_type_table_insert);
    CU_add_test(suite, "test_type_table_insert_duplicate", test_type_table_insert_duplicate);
    CU_add_test(suite, "test_type_table_insert_null", test_type_table_insert_null);
    CU_add_test(suite, "test_type_table_lookup_by_name_user_type", test_type_table_lookup_by_name_user_type);
    CU_add_test(suite, "test_type_table_destroy_null", test_type_table_destroy_null);
    CU_add_test(suite, "test_type_table_case_sensitive", test_type_table_case_sensitive);
    CU_add_test(suite, "test_type_table_multiple_user_types", test_type_table_multiple_user_types);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}