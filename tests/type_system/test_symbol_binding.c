#include <CUnit/Basic.h>
#include "symbol_binding.h"

void test_symbol_binding_create_basic(void)
{
    printf("\n\n------------ Inicio test: test_symbol_binding_create_basic ------------\n");
    printf("Creando symbol binding basico\n");

    SymbolBinding *sb = symbol_binding_create("x", NULL, NULL, 10, 5);

    CU_ASSERT_PTR_NOT_NULL(sb);
    printf("sb = %p\n", (void *)sb);

    CU_ASSERT_STRING_EQUAL(sb->name, "x");
    printf("name = '%s' (esperado: 'x')\n", sb->name);

    CU_ASSERT_PTR_NULL(sb->type_name);
    printf("type_name = %p (esperado: NULL)\n", (void *)sb->type_name);

    CU_ASSERT_PTR_NULL(sb->initializer);
    printf("initializer = %p (esperado: NULL)\n", (void *)sb->initializer);

    CU_ASSERT_PTR_NULL(sb->return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)sb->return_type);

    CU_ASSERT_EQUAL(sb->line, 10);
    printf("line = %d (esperado: 10)\n", sb->line);

    CU_ASSERT_EQUAL(sb->column, 5);
    printf("column = %d (esperado: 5)\n", sb->column);

    symbol_binding_destroy(sb);
}

void test_symbol_binding_create_with_type(void)
{
    printf("\n\n------------ Inicio test: test_symbol_binding_create_with_type ------------\n");
    printf("Creando symbol binding con anotacion de tipo\n");

    SymbolBinding *sb = symbol_binding_create("param", "Number", NULL, 3, 12);

    CU_ASSERT_PTR_NOT_NULL(sb);
    CU_ASSERT_STRING_EQUAL(sb->name, "param");
    printf("name = '%s' (esperado: 'param')\n", sb->name);

    CU_ASSERT_STRING_EQUAL(sb->type_name, "Number");
    printf("type_name = '%s' (esperado: 'Number')\n", sb->type_name);

    CU_ASSERT_PTR_NULL(sb->initializer);
    printf("initializer = %p (esperado: NULL)\n", (void *)sb->initializer);

    CU_ASSERT_PTR_NULL(sb->return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)sb->return_type);

    CU_ASSERT_EQUAL(sb->line, 3);
    printf("line = %d (esperado: 3)\n", sb->line);

    CU_ASSERT_EQUAL(sb->column, 12);
    printf("column = %d (esperado: 12)\n", sb->column);

    symbol_binding_destroy(sb);
}

void test_symbol_binding_create_null_name(void)
{
    printf("\n\n------------ Inicio test: test_symbol_binding_create_null_name ------------\n");
    printf("Creando symbol binding con name=NULL\n");

    SymbolBinding *sb = symbol_binding_create(NULL, "Number", NULL, 1, 1);
    CU_ASSERT_PTR_NULL(sb);
    printf("sb = %p (esperado: NULL)\n", (void *)sb);
}

void test_symbol_binding_create_with_initializer(void)
{
    printf("\n\n------------ Inicio test: test_symbol_binding_create_with_initializer ------------\n");
    printf("Creando symbol binding con initializer (puntero simulado)\n");

    ASTNode *fake_node = (ASTNode *)0xDEADBEEF;
    SymbolBinding *sb = symbol_binding_create("y", "String", fake_node, 7, 3);

    CU_ASSERT_PTR_NOT_NULL(sb);
    CU_ASSERT_PTR_EQUAL(sb->initializer, fake_node);
    printf("initializer = %p (esperado: %p)\n", (void *)sb->initializer, (void *)fake_node);

    symbol_binding_destroy(sb);
}

void test_symbol_binding_destroy_null(void)
{
    printf("\n\n------------ Inicio test: test_symbol_binding_destroy_null ------------\n");
    printf("Destruyendo NULL\n");

    symbol_binding_destroy(NULL);
    CU_PASS("symbol_binding_destroy(NULL) no crasheo");
}

void test_get_unannotated_params_all_annotated(void)
{
    printf("\n\n------------ Inicio test: test_get_unannotated_params_all_annotated ------------\n");
    printf("Todos los parametros tienen anotacion\n");

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    SymbolBinding *sb1 = symbol_binding_create("a", "Number", NULL, 1, 1);
    SymbolBinding *sb2 = symbol_binding_create("b", "String", NULL, 1, 5);
    list_append(bindings, sb1);
    list_append(bindings, sb2);

    List *unannotated = get_unannotated_params(bindings);
    CU_ASSERT_PTR_NOT_NULL(unannotated);
    CU_ASSERT_EQUAL(list_count(unannotated), 0);
    printf("unannotated count = %zu (esperado: 0)\n", list_count(unannotated));

    list_destroy(unannotated);
    list_destroy(bindings);
    symbol_binding_destroy(sb1);
    symbol_binding_destroy(sb2);
}

void test_get_unannotated_params_some(void)
{
    printf("\n\n------------ Inicio test: test_get_unannotated_params_some ------------\n");
    printf("Algunos parametros sin anotacion\n");

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    SymbolBinding *sb1 = symbol_binding_create("a", "Number", NULL, 1, 1);
    SymbolBinding *sb2 = symbol_binding_create("b", NULL, NULL, 1, 5);
    SymbolBinding *sb3 = symbol_binding_create("c", NULL, NULL, 1, 10);
    SymbolBinding *sb4 = symbol_binding_create("d", "Bool", NULL, 1, 15);
    list_append(bindings, sb1);
    list_append(bindings, sb2);
    list_append(bindings, sb3);
    list_append(bindings, sb4);

    List *unannotated = get_unannotated_params(bindings);
    CU_ASSERT_PTR_NOT_NULL(unannotated);
    CU_ASSERT_EQUAL(list_count(unannotated), 2);
    printf("unannotated count = %zu (esperado: 2)\n", list_count(unannotated));

    CU_ASSERT_STRING_EQUAL((char *)list_get(unannotated, 0), "b");
    printf("unannotated[0] = '%s' (esperado: 'b')\n", (char *)list_get(unannotated, 0));

    CU_ASSERT_STRING_EQUAL((char *)list_get(unannotated, 1), "c");
    printf("unannotated[1] = '%s' (esperado: 'c')\n", (char *)list_get(unannotated, 1));

    list_destroy(unannotated);
    list_destroy(bindings);
    symbol_binding_destroy(sb1);
    symbol_binding_destroy(sb2);
    symbol_binding_destroy(sb3);
    symbol_binding_destroy(sb4);
}

void test_get_unannotated_params_none(void)
{
    printf("\n\n------------ Inicio test: test_get_unannotated_params_none ------------\n");
    printf("Ningun parametro tiene anotacion\n");

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    SymbolBinding *sb1 = symbol_binding_create("x", NULL, NULL, 1, 1);
    SymbolBinding *sb2 = symbol_binding_create("y", NULL, NULL, 1, 5);
    list_append(bindings, sb1);
    list_append(bindings, sb2);

    List *unannotated = get_unannotated_params(bindings);
    CU_ASSERT_EQUAL(list_count(unannotated), 2);
    printf("unannotated count = %zu (esperado: 2)\n", list_count(unannotated));

    CU_ASSERT_STRING_EQUAL((char *)list_get(unannotated, 0), "x");
    printf("unannotated[0] = '%s' (esperado: 'x')\n", (char *)list_get(unannotated, 0));

    CU_ASSERT_STRING_EQUAL((char *)list_get(unannotated, 1), "y");
    printf("unannotated[1] = '%s' (esperado: 'y')\n", (char *)list_get(unannotated, 1));

    list_destroy(unannotated);
    list_destroy(bindings);
    symbol_binding_destroy(sb1);
    symbol_binding_destroy(sb2);
}

void test_get_unannotated_params_empty(void)
{
    printf("\n\n------------ Inicio test: test_get_unannotated_params_empty ------------\n");
    printf("Lista de bindings vacia\n");

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    List *unannotated = get_unannotated_params(bindings);

    CU_ASSERT_PTR_NOT_NULL(unannotated);
    CU_ASSERT_EQUAL(list_count(unannotated), 0);
    printf("unannotated count = %zu (esperado: 0)\n", list_count(unannotated));

    list_destroy(unannotated);
    list_destroy(bindings);
}

void test_build_param_types_all_annotated(void)
{
    printf("\n\n------------ Inicio test: test_build_param_types_all_annotated ------------\n");
    printf("Construyendo param types con todos anotados\n");

    global_tables_init();
    diagnostic_manager_t *dm = dm_create();

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    SymbolBinding *sb1 = symbol_binding_create("a", "Number", NULL, 1, 1);
    SymbolBinding *sb2 = symbol_binding_create("b", "String", NULL, 1, 5);
    list_append(bindings, sb1);
    list_append(bindings, sb2);

    List *types = build_param_types(bindings, dm);
    CU_ASSERT_PTR_NOT_NULL(types);
    CU_ASSERT_EQUAL(list_count(types), 2);
    printf("types count = %zu (esperado: 2)\n", list_count(types));

    TypeDescriptor *t1 = (TypeDescriptor *)list_get(types, 0);
    CU_ASSERT_PTR_NOT_NULL(t1);
    CU_ASSERT_STRING_EQUAL(t1->name, "Number");
    printf("types[0] = '%s' (esperado: 'Number')\n", t1->name);

    TypeDescriptor *t2 = (TypeDescriptor *)list_get(types, 1);
    CU_ASSERT_PTR_NOT_NULL(t2);
    CU_ASSERT_STRING_EQUAL(t2->name, "String");
    printf("types[1] = '%s' (esperado: 'String')\n", t2->name);

    CU_ASSERT_FALSE(dm_has_errors(dm));
    printf("errores = %s (esperado: false)\n", dm_has_errors(dm) ? "true" : "false");

    list_destroy(types);
    list_destroy(bindings);
    symbol_binding_destroy(sb1);
    symbol_binding_destroy(sb2);
    dm_destroy(dm);
    global_tables_destroy();
}

void test_build_param_types_some_unannotated(void)
{
    printf("\n\n------------ Inicio test: test_build_param_types_some_unannotated ------------\n");
    printf("Construyendo param types con algunos sin anotacion\n");

    global_tables_init();
    diagnostic_manager_t *dm = dm_create();

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    SymbolBinding *sb1 = symbol_binding_create("a", "Number", NULL, 1, 1);
    SymbolBinding *sb2 = symbol_binding_create("b", NULL, NULL, 1, 5);
    SymbolBinding *sb3 = symbol_binding_create("c", "Bool", NULL, 1, 10);
    list_append(bindings, sb1);
    list_append(bindings, sb2);
    list_append(bindings, sb3);

    List *types = build_param_types(bindings, dm);
    CU_ASSERT_EQUAL(list_count(types), 3);
    printf("types count = %zu (esperado: 3)\n", list_count(types));

    TypeDescriptor *t1 = (TypeDescriptor *)list_get(types, 0);
    CU_ASSERT_STRING_EQUAL(t1->name, "Number");
    printf("types[0] = '%s' (esperado: 'Number')\n", t1->name);

    TypeDescriptor *t2 = (TypeDescriptor *)list_get(types, 1);
    CU_ASSERT_PTR_NULL(t2);
    printf("types[1] = %p (esperado: NULL)\n", (void *)t2);

    TypeDescriptor *t3 = (TypeDescriptor *)list_get(types, 2);
    CU_ASSERT_STRING_EQUAL(t3->name, "Bool");
    printf("types[2] = '%s' (esperado: 'Bool')\n", t3->name);

    CU_ASSERT_FALSE(dm_has_errors(dm));
    printf("errores = %s (esperado: false)\n", dm_has_errors(dm) ? "true" : "false");

    list_destroy(types);
    list_destroy(bindings);
    symbol_binding_destroy(sb1);
    symbol_binding_destroy(sb2);
    symbol_binding_destroy(sb3);
    dm_destroy(dm);
    global_tables_destroy();
}

void test_build_param_types_undefined(void)
{
    printf("\n\n------------ Inicio test: test_build_param_types_undefined ------------\n");
    printf("Construyendo param types con tipo indefinido\n");

    global_tables_init();
    diagnostic_manager_t *dm = dm_create();

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    SymbolBinding *sb1 = symbol_binding_create("x", "NoExiste", NULL, 5, 3);
    list_append(bindings, sb1);

    List *types = build_param_types(bindings, dm);
    CU_ASSERT_EQUAL(list_count(types), 1);
    printf("types count = %zu (esperado: 1)\n", list_count(types));

    TypeDescriptor *t1 = (TypeDescriptor *)list_get(types, 0);
    CU_ASSERT_PTR_NULL(t1);
    printf("types[0] = %p (esperado: NULL)\n", (void *)t1);

    CU_ASSERT_TRUE(dm_has_errors(dm));
    printf("errores = %s (esperado: true)\n", dm_has_errors(dm) ? "true" : "false");
    CU_ASSERT_EQUAL(dm_error_count(dm), 1);
    printf("error count = %zu (esperado: 1)\n", dm_error_count(dm));

    list_destroy(types);
    list_destroy(bindings);
    symbol_binding_destroy(sb1);
    dm_destroy(dm);
    global_tables_destroy();
}

void test_build_param_types_empty(void)
{
    printf("\n\n------------ Inicio test: test_build_param_types_empty ------------\n");
    printf("Construyendo param types con lista vacia\n");

    global_tables_init();
    diagnostic_manager_t *dm = dm_create();

    List *bindings = list_create(0, NULL, NULL, NULL, NULL);
    List *types = build_param_types(bindings, dm);

    CU_ASSERT_PTR_NOT_NULL(types);
    CU_ASSERT_EQUAL(list_count(types), 0);
    printf("types count = %zu (esperado: 0)\n", list_count(types));

    CU_ASSERT_FALSE(dm_has_errors(dm));
    printf("errores = %s (esperado: false)\n", dm_has_errors(dm) ? "true" : "false");

    list_destroy(types);
    list_destroy(bindings);
    dm_destroy(dm);
    global_tables_destroy();
}

void test_symbol_binding_return_type_set(void)
{
    printf("\n\n------------ Inicio test: test_symbol_binding_return_type_set ------------\n");
    printf("Asignando return_type manualmente\n");

    SymbolBinding *sb = symbol_binding_create("z", "Number", NULL, 2, 8);

    CU_ASSERT_PTR_NULL(sb->return_type);
    printf("return_type antes = %p (esperado: NULL)\n", (void *)sb->return_type);

    TypeDescriptor *num = type_create(HULK_NUMBER, "Number", NULL);
    sb->return_type = num;

    CU_ASSERT_PTR_EQUAL(sb->return_type, num);
    CU_ASSERT_STRING_EQUAL(sb->return_type->name, "Number");
    printf("return_type despues = '%s' (esperado: 'Number')\n", sb->return_type->name);

    symbol_binding_destroy(sb);
    type_destroy(num);
}

int main(void)
{
    printf("\n=== Iniciando tests de SymbolBinding ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("SymbolBinding Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_symbol_binding_create_basic", test_symbol_binding_create_basic);
    CU_add_test(suite, "test_symbol_binding_create_with_type", test_symbol_binding_create_with_type);
    CU_add_test(suite, "test_symbol_binding_create_null_name", test_symbol_binding_create_null_name);
    CU_add_test(suite, "test_symbol_binding_create_with_initializer", test_symbol_binding_create_with_initializer);
    CU_add_test(suite, "test_symbol_binding_destroy_null", test_symbol_binding_destroy_null);

    CU_add_test(suite, "test_get_unannotated_params_all_annotated", test_get_unannotated_params_all_annotated);
    CU_add_test(suite, "test_get_unannotated_params_some", test_get_unannotated_params_some);
    CU_add_test(suite, "test_get_unannotated_params_none", test_get_unannotated_params_none);
    CU_add_test(suite, "test_get_unannotated_params_empty", test_get_unannotated_params_empty);

    CU_add_test(suite, "test_build_param_types_all_annotated", test_build_param_types_all_annotated);
    CU_add_test(suite, "test_build_param_types_some_unannotated", test_build_param_types_some_unannotated);
    CU_add_test(suite, "test_build_param_types_undefined", test_build_param_types_undefined);
    CU_add_test(suite, "test_build_param_types_empty", test_build_param_types_empty);

    CU_add_test(suite, "test_symbol_binding_return_type_set", test_symbol_binding_return_type_set);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}