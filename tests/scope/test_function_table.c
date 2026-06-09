#include <CUnit/Basic.h>
#include "function_table.h"

static TypeDescriptor *obj_type;
static TypeDescriptor *num_type;
static TypeDescriptor *str_type;
static TypeDescriptor *void_type;

static void setup_types(void)
{
    obj_type  = type_create(HULK_OBJECT, "Object", NULL);
    num_type  = type_create(HULK_NUMBER, "Number", obj_type);
    str_type  = type_create(HULK_STRING, "String", obj_type);
    void_type = type_create(HULK_VOID,   "Void",   obj_type);
}

static void teardown_types(void)
{
    type_destroy(void_type);
    type_destroy(str_type);
    type_destroy(num_type);
    type_destroy(obj_type);
}

void test_function_table_create(void)
{
    printf("\n\n------------ Inicio test: test_function_table_create ------------\n");
    printf("Creando tabla de funciones vacia\n");

    FunctionTable *table = function_table_create();
    CU_ASSERT_PTR_NOT_NULL(table);
    printf("Tabla creada\n");

    CU_ASSERT_FALSE(function_table_exists(table, "anything"));
    printf("exists('anything') = false (esperado: false)\n");

    function_table_destroy(table);
}

void test_function_table_destroy_null(void)
{
    printf("\n\n------------ Inicio test: test_function_table_destroy_null ------------\n");
    printf("Llamando a function_table_destroy(NULL)\n");

    function_table_destroy(NULL);
    CU_PASS("No crash on function_table_destroy(NULL)");
}

void test_function_table_insert(void)
{
    printf("\n\n------------ Inicio test: test_function_table_insert ------------\n");
    printf("Insertando funcion 'print' con retorno Void y sin parametros\n");

    FunctionTable *table = function_table_create();
    List *params = list_create(0, NULL, NULL, NULL, NULL);
    bool inserted = function_table_insert(table, "print", void_type, params);
    CU_ASSERT_TRUE(inserted);
    printf("Insercion de 'print': %s (esperado: true)\n", inserted ? "true" : "false");

    CU_ASSERT_TRUE(function_table_exists(table, "print"));
    printf("exists('print') = true (esperado: true)\n");

    TypeDescriptor *ret = function_table_get_return_type(table, "print");
    CU_ASSERT_PTR_EQUAL(ret, void_type);
    printf("Tipo de retorno de 'print': %s (esperado: %s)\n",
           ret ? ret->name : "NULL", void_type->name);

    List *got_params = function_table_get_params_types(table, "print");
    CU_ASSERT_PTR_NOT_NULL(got_params);
    CU_ASSERT_EQUAL(list_count(got_params), 0);
    printf("Parametros count = %zu (esperado: 0)\n", list_count(got_params));

    function_table_destroy(table);
}

void test_function_table_insert_with_params(void)
{
    printf("\n\n------------ Inicio test: test_function_table_insert_with_params ------------\n");
    printf("Insertando 'max' con retorno Number, parametros [Number, String]\n");

    FunctionTable *table = function_table_create();
    List *params = list_create(2, (void *[]){num_type, str_type}, NULL, NULL, NULL);
    bool inserted = function_table_insert(table, "max", num_type, params);
    CU_ASSERT_TRUE(inserted);

    CU_ASSERT_TRUE(function_table_exists(table, "max"));
    TypeDescriptor *ret = function_table_get_return_type(table, "max");
    CU_ASSERT_PTR_EQUAL(ret, num_type);
    printf("Retorno de 'max': %s (esperado: %s)\n", ret->name, num_type->name);

    List *got_params = function_table_get_params_types(table, "max");
    CU_ASSERT_EQUAL(list_count(got_params), 2);
    CU_ASSERT_PTR_EQUAL(list_get(got_params, 0), num_type);
    CU_ASSERT_PTR_EQUAL(list_get(got_params, 1), str_type);
    printf("Parametros: [%s, %s]\n",
           ((TypeDescriptor *)list_get(got_params, 0))->name,
           ((TypeDescriptor *)list_get(got_params, 1))->name);

    function_table_destroy(table);
}

void test_function_table_insert_duplicate(void)
{
    printf("\n\n------------ Inicio test: test_function_table_insert_duplicate ------------\n");
    printf("Intentando insertar funcion duplicada\n");

    FunctionTable *table = function_table_create();
    List *params1 = list_create(0, NULL, NULL, NULL, NULL);
    function_table_insert(table, "f", void_type, params1);

    List *params2 = list_create(1, (void *[]){num_type}, NULL, NULL, NULL);
    bool second = function_table_insert(table, "f", num_type, params2);
    CU_ASSERT_FALSE(second);
    printf("Segunda insercion de 'f': %s (esperado: false)\n", second ? "true" : "false");

    CU_ASSERT_PTR_EQUAL(function_table_get_return_type(table, "f"), void_type);
    CU_ASSERT_EQUAL(list_count(function_table_get_params_types(table, "f")), 0);
    printf("La primera insercion permanece intacta\n");

    function_table_destroy(table);
}

void test_function_table_insert_null_params(void)
{
    printf("\n\n------------ Inicio test: test_function_table_insert_null_params ------------\n");
    printf("Insertando funcion con params=NULL (debe fallar)\n");

    FunctionTable *table = function_table_create();
    bool inserted = function_table_insert(table, "g", void_type, NULL);
    CU_ASSERT_FALSE(inserted);
    printf("Insercion con params=NULL retorno %s (esperado: false)\n",
           inserted ? "true" : "false");

    CU_ASSERT_FALSE(function_table_exists(table, "g"));
    printf("exists('g') = false (esperado: false)\n");

    function_table_destroy(table);
}

void test_function_table_exists(void)
{
    printf("\n\n------------ Inicio test: test_function_table_exists ------------\n");
    printf("Verificando existencia de funciones\n");

    FunctionTable *table = function_table_create();
    CU_ASSERT_FALSE(function_table_exists(table, "h"));
    printf("Antes de insertar, exists('h') = false (esperado: false)\n");

    List *params = list_create(0, NULL, NULL, NULL, NULL);
    function_table_insert(table, "h", void_type, params);
    CU_ASSERT_TRUE(function_table_exists(table, "h"));
    printf("Despues de insertar, exists('h') = true (esperado: true)\n");

    function_table_destroy(table);
}

void test_function_table_get_nonexistent(void)
{
    printf("\n\n------------ Inicio test: test_function_table_get_nonexistent ------------\n");
    printf("Obteniendo retorno de funcion inexistente\n");

    FunctionTable *table = function_table_create();
    TypeDescriptor *ret = function_table_get_return_type(table, "no_such");
    CU_ASSERT_PTR_NULL(ret);
    printf("Retorno: %s (esperado: NULL)\n", ret ? ret->name : "NULL");

    List *params = function_table_get_params_types(table, "no_such");
    CU_ASSERT_PTR_NULL(params);
    printf("Parametros: %s (esperado: NULL)\n", params ? "not null" : "NULL");

    function_table_destroy(table);
}

void test_function_table_update_existing(void)
{
    printf("\n\n------------ Inicio test: test_function_table_update_existing ------------\n");
    printf("Actualizando funcion existente\n");

    FunctionTable *table = function_table_create();
    List *initial_params = list_create(1, (void *[]){num_type}, NULL, NULL, NULL);
    function_table_insert(table, "update_me", void_type, initial_params);

    List *new_params = list_create(2, (void *[]){num_type, str_type}, NULL, NULL, NULL);
    bool updated = function_table_update(table, "update_me", num_type, new_params);
    CU_ASSERT_TRUE(updated);
    printf("Actualizacion: %s (esperado: true)\n", updated ? "true" : "false");

    CU_ASSERT_PTR_EQUAL(function_table_get_return_type(table, "update_me"), num_type);
    List *params = function_table_get_params_types(table, "update_me");
    CU_ASSERT_EQUAL(list_count(params), 2);
    CU_ASSERT_PTR_EQUAL(list_get(params, 0), num_type);
    CU_ASSERT_PTR_EQUAL(list_get(params, 1), str_type);
    printf("Nuevo retorno: %s, params: [%s, %s]\n",
           num_type->name, num_type->name, str_type->name);

    function_table_destroy(table);
}

void test_function_table_update_nonexistent(void)
{
    printf("\n\n------------ Inicio test: test_function_table_update_nonexistent ------------\n");
    printf("Actualizando funcion inexistente\n");

    FunctionTable *table = function_table_create();
    List *new_params = list_create(0, NULL, NULL, NULL, NULL);
    bool updated = function_table_update(table, "ghost", num_type, new_params);
    CU_ASSERT_FALSE(updated);
    printf("Actualizacion: %s (esperado: false)\n", updated ? "true" : "false");

    function_table_destroy(table);
}

void test_function_table_update_partial(void)
{
    printf("\n\n------------ Inicio test: test_function_table_update_partial ------------\n");
    printf("Actualizacion parcial (solo retorno o solo params)\n");

    FunctionTable *table = function_table_create();
    List *params = list_create(1, (void *[]){num_type}, NULL, NULL, NULL);
    function_table_insert(table, "partial", void_type, params);

    bool u1 = function_table_update(table, "partial", num_type, NULL);
    CU_ASSERT_TRUE(u1);
    CU_ASSERT_PTR_EQUAL(function_table_get_return_type(table, "partial"), num_type);
    List *p1 = function_table_get_params_types(table, "partial");
    CU_ASSERT_EQUAL(list_count(p1), 1);
    CU_ASSERT_PTR_EQUAL(list_get(p1, 0), num_type);
    printf("Tras actualizar retorno: ret = %s, params count = %zu\n",
           num_type->name, list_count(p1));

    List *new_params = list_create(0, NULL, NULL, NULL, NULL);
    bool u2 = function_table_update(table, "partial", NULL, new_params);
    CU_ASSERT_TRUE(u2);
    CU_ASSERT_PTR_EQUAL(function_table_get_return_type(table, "partial"), num_type);
    List *p2 = function_table_get_params_types(table, "partial");
    CU_ASSERT_EQUAL(list_count(p2), 0);
    printf("Tras actualizar params: ret = %s, params count = %zu\n",
           num_type->name, list_count(p2));

    function_table_destroy(table);
}

void test_function_table_build_method_name(void)
{
    printf("\n\n------------ Inicio test: test_function_table_build_method_name ------------\n");
    printf("Construyendo nombres de metodos\n");

    char *name1 = function_table_build_method_name("MyType", "run");
    CU_ASSERT_PTR_NOT_NULL(name1);
    CU_ASSERT_STRING_EQUAL(name1, "_MyType_run");
    printf("build_method_name('MyType', 'run') = '%s' (esperado: '_MyType_run')\n", name1);
    free(name1);

    char *name2 = function_table_build_method_name("Point", "getX");
    CU_ASSERT_PTR_NOT_NULL(name2);
    CU_ASSERT_STRING_EQUAL(name2, "_Point_getX");
    printf("build_method_name('Point', 'getX') = '%s' (esperado: '_Point_getX')\n", name2);
    free(name2);
}

void test_function_table_build_method_name_null(void)
{
    printf("\n\n------------ Inicio test: test_function_table_build_method_name_null ------------\n");
    printf("Construyendo nombres de metodos con NULL\n");

    char *name1 = function_table_build_method_name(NULL, "run");
    CU_ASSERT_PTR_NULL(name1);
    printf("build_method_name(NULL, 'run') = %p (esperado: NULL)\n", (void *)name1);

    char *name2 = function_table_build_method_name("Type", NULL);
    CU_ASSERT_PTR_NULL(name2);
    printf("build_method_name('Type', NULL) = %p (esperado: NULL)\n", (void *)name2);

    char *name3 = function_table_build_method_name(NULL, NULL);
    CU_ASSERT_PTR_NULL(name3);
    printf("build_method_name(NULL, NULL) = %p (esperado: NULL)\n", (void *)name3);
}

int main(void)
{
    printf("\n=== Iniciando tests de FunctionTable ===\n");

    setup_types();

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionTable Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        teardown_types();
        return CU_get_error();
    }

    CU_add_test(suite, "test_function_table_create", test_function_table_create);
    CU_add_test(suite, "test_function_table_destroy_null", test_function_table_destroy_null);
    CU_add_test(suite, "test_function_table_insert", test_function_table_insert);
    CU_add_test(suite, "test_function_table_insert_with_params", test_function_table_insert_with_params);
    CU_add_test(suite, "test_function_table_insert_duplicate", test_function_table_insert_duplicate);
    CU_add_test(suite, "test_function_table_insert_null_params", test_function_table_insert_null_params);
    CU_add_test(suite, "test_function_table_exists", test_function_table_exists);
    CU_add_test(suite, "test_function_table_get_nonexistent", test_function_table_get_nonexistent);
    CU_add_test(suite, "test_function_table_update_existing", test_function_table_update_existing);
    CU_add_test(suite, "test_function_table_update_nonexistent", test_function_table_update_nonexistent);
    CU_add_test(suite, "test_function_table_update_partial", test_function_table_update_partial);
    CU_add_test(suite, "test_function_table_build_method_name", test_function_table_build_method_name);
    CU_add_test(suite, "test_function_table_build_method_name_null", test_function_table_build_method_name_null);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    teardown_types();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}