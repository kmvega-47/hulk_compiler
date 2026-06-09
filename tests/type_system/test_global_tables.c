#include <CUnit/Basic.h>
#include "global_tables.h"

void test_global_tables_init(void)
{
    printf("\n\n------------ Inicio test: test_global_tables_init ------------\n");
    printf("Inicializando tablas globales\n");

    CU_ASSERT_PTR_NULL(global_type_table);
    CU_ASSERT_PTR_NULL(global_function_table);
    printf("Antes de init: global_type_table = %p, global_function_table = %p (esperado: NULL, NULL)\n",
           (void *)global_type_table, (void *)global_function_table);

    global_tables_init();

    CU_ASSERT_PTR_NOT_NULL(global_type_table);
    CU_ASSERT_PTR_NOT_NULL(global_function_table);
    printf("Despues de init: global_type_table = %p, global_function_table = %p (esperado: not NULL)\n",
           (void *)global_type_table, (void *)global_function_table);

    global_tables_destroy();

    CU_ASSERT_PTR_NULL(global_type_table);
    CU_ASSERT_PTR_NULL(global_function_table);
    printf("Despues de destroy: global_type_table = %p, global_function_table = %p (esperado: NULL, NULL)\n",
           (void *)global_type_table, (void *)global_function_table);
}

void test_global_tables_builtin_types(void)
{
    printf("\n\n------------ Inicio test: test_global_tables_builtin_types ------------\n");
    printf("Verificando tipos built-in en tabla global\n");

    global_tables_init();

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    CU_ASSERT_PTR_NOT_NULL(obj);
    CU_ASSERT_STRING_EQUAL(obj->name, "Object");
    printf("Object: name='%s'\n", obj->name);

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    CU_ASSERT_PTR_NOT_NULL(num);
    CU_ASSERT_STRING_EQUAL(num->name, "Number");
    printf("Number: name='%s'\n", num->name);

    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    CU_ASSERT_PTR_NOT_NULL(str);
    CU_ASSERT_STRING_EQUAL(str->name, "String");
    printf("String: name='%s'\n", str->name);

    TypeDescriptor *bl = type_table_lookup_by_tag(global_type_table, HULK_BOOL);
    CU_ASSERT_PTR_NOT_NULL(bl);
    CU_ASSERT_STRING_EQUAL(bl->name, "Bool");
    printf("Bool: name='%s'\n", bl->name);

    TypeDescriptor *vd = type_table_lookup_by_tag(global_type_table, HULK_VOID);
    CU_ASSERT_PTR_NOT_NULL(vd);
    CU_ASSERT_STRING_EQUAL(vd->name, "Void");
    printf("Void: name='%s'\n", vd->name);

    global_tables_destroy();
}

void test_global_tables_builtin_functions(void)
{
    printf("\n\n------------ Inicio test: test_global_tables_builtin_functions ------------\n");
    printf("Verificando funciones built-in en tabla global\n");

    global_tables_init();

    CU_ASSERT_TRUE(function_table_exists(global_function_table, "print"));
    printf("exists('print') = true (esperado: true)\n");

    TypeDescriptor *print_ret = function_table_get_return_type(global_function_table, "print");
    CU_ASSERT_PTR_NOT_NULL(print_ret);
    CU_ASSERT_EQUAL(print_ret->tag, HULK_VOID);
    printf("print return: tag=%d, name='%s' (esperado: Void)\n", print_ret->tag, print_ret->name);

    List *print_params = function_table_get_params_types(global_function_table, "print");
    CU_ASSERT_PTR_NOT_NULL(print_params);
    CU_ASSERT_EQUAL(list_count(print_params), 1);
    TypeDescriptor *print_param = (TypeDescriptor *)list_get(print_params, 0);
    CU_ASSERT_EQUAL(print_param->tag, HULK_OBJECT);
    printf("print params count = %zu, param[0] = '%s' (esperado: 1, 'Object')\n",
           list_count(print_params), print_param->name);

    CU_ASSERT_TRUE(function_table_exists(global_function_table, "sqrt"));
    printf("exists('sqrt') = true (esperado: true)\n");

    TypeDescriptor *sqrt_ret = function_table_get_return_type(global_function_table, "sqrt");
    CU_ASSERT_EQUAL(sqrt_ret->tag, HULK_NUMBER);
    printf("sqrt return: '%s' (esperado: 'Number')\n", sqrt_ret->name);

    CU_ASSERT_TRUE(function_table_exists(global_function_table, "sin"));
    printf("exists('sin') = true (esperado: true)\n");

    CU_ASSERT_TRUE(function_table_exists(global_function_table, "cos"));
    printf("exists('cos') = true (esperado: true)\n");

    CU_ASSERT_TRUE(function_table_exists(global_function_table, "exp"));
    printf("exists('exp') = true (esperado: true)\n");

    CU_ASSERT_TRUE(function_table_exists(global_function_table, "log"));
    printf("exists('log') = true (esperado: true)\n");

    List *log_params = function_table_get_params_types(global_function_table, "log");
    CU_ASSERT_EQUAL(list_count(log_params), 2);
    printf("log params count = %zu (esperado: 2)\n", list_count(log_params));

    CU_ASSERT_TRUE(function_table_exists(global_function_table, "rand"));
    printf("exists('rand') = true (esperado: true)\n");

    List *rand_params = function_table_get_params_types(global_function_table, "rand");
    CU_ASSERT_EQUAL(list_count(rand_params), 0);
    printf("rand params count = %zu (esperado: 0)\n", list_count(rand_params));

    global_tables_destroy();
}

void test_get_type_from_annotation(void)
{
    printf("\n\n------------ Inicio test: test_get_type_from_annotation ------------\n");
    printf("Obteniendo tipo desde anotacion\n");

    global_tables_init();

    TypeDescriptor *num = get_type_from_annotation("Number");
    CU_ASSERT_PTR_NOT_NULL(num);
    CU_ASSERT_STRING_EQUAL(num->name, "Number");
    printf("get_type_from_annotation('Number'): name='%s'\n", num->name);

    TypeDescriptor *obj = get_type_from_annotation("Object");
    CU_ASSERT_PTR_NOT_NULL(obj);
    CU_ASSERT_STRING_EQUAL(obj->name, "Object");
    printf("get_type_from_annotation('Object'): name='%s'\n", obj->name);

    TypeDescriptor *str = get_type_from_annotation("String");
    CU_ASSERT_PTR_NOT_NULL(str);
    CU_ASSERT_STRING_EQUAL(str->name, "String");
    printf("get_type_from_annotation('String'): name='%s'\n", str->name);

    TypeDescriptor *bl = get_type_from_annotation("Bool");
    CU_ASSERT_PTR_NOT_NULL(bl);
    CU_ASSERT_STRING_EQUAL(bl->name, "Bool");
    printf("get_type_from_annotation('Bool'): name='%s'\n", bl->name);

    TypeDescriptor *vd = get_type_from_annotation("Void");
    CU_ASSERT_PTR_NOT_NULL(vd);
    CU_ASSERT_STRING_EQUAL(vd->name, "Void");
    printf("get_type_from_annotation('Void'): name='%s'\n", vd->name);

    global_tables_destroy();
}

void test_get_type_from_annotation_null(void)
{
    printf("\n\n------------ Inicio test: test_get_type_from_annotation_null ------------\n");
    printf("Obteniendo tipo con anotacion NULL\n");

    global_tables_init();

    TypeDescriptor *found = get_type_from_annotation(NULL);
    CU_ASSERT_PTR_NULL(found);
    printf("get_type_from_annotation(NULL) = %p (esperado: NULL)\n", (void *)found);

    global_tables_destroy();
}

void test_get_type_from_annotation_not_found(void)
{
    printf("\n\n------------ Inicio test: test_get_type_from_annotation_not_found ------------\n");
    printf("Obteniendo tipo inexistente\n");

    global_tables_init();

    TypeDescriptor *found = get_type_from_annotation("NoExiste");
    CU_ASSERT_PTR_NULL(found);
    printf("get_type_from_annotation('NoExiste') = %p (esperado: NULL)\n", (void *)found);

    global_tables_destroy();
}

void test_get_type_from_annotation_user_type(void)
{
    printf("\n\n------------ Inicio test: test_get_type_from_annotation_user_type ------------\n");
    printf("Obteniendo tipo de usuario desde anotacion\n");

    global_tables_init();

    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    List *params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *user = user_type_create("MyType", obj, params);
    type_table_insert(global_type_table, user);

    TypeDescriptor *found = get_type_from_annotation("MyType");
    CU_ASSERT_PTR_NOT_NULL(found);
    CU_ASSERT_STRING_EQUAL(found->name, "MyType");
    printf("get_type_from_annotation('MyType'): name='%s'\n", found->name);

    global_tables_destroy();
}

void test_global_tables_destroy_safe(void)
{
    printf("\n\n------------ Inicio test: test_global_tables_destroy_safe ------------\n");
    printf("Verificando que destroy es seguro con tablas NULL\n");

    global_type_table = NULL;
    global_function_table = NULL;

    global_tables_destroy();
    CU_PASS("global_tables_destroy() con tablas NULL no crasheo");

    CU_ASSERT_PTR_NULL(global_type_table);
    CU_ASSERT_PTR_NULL(global_function_table);
    printf("Tablas siguen siendo NULL\n");
}

int main(void)
{
    printf("\n=== Iniciando tests de GlobalTables ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("GlobalTables Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_global_tables_init", test_global_tables_init);
    CU_add_test(suite, "test_global_tables_builtin_types", test_global_tables_builtin_types);
    CU_add_test(suite, "test_global_tables_builtin_functions", test_global_tables_builtin_functions);
    CU_add_test(suite, "test_get_type_from_annotation", test_get_type_from_annotation);
    CU_add_test(suite, "test_get_type_from_annotation_null", test_get_type_from_annotation_null);
    CU_add_test(suite, "test_get_type_from_annotation_not_found", test_get_type_from_annotation_not_found);
    CU_add_test(suite, "test_get_type_from_annotation_user_type", test_get_type_from_annotation_user_type);
    CU_add_test(suite, "test_global_tables_destroy_safe", test_global_tables_destroy_safe);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}