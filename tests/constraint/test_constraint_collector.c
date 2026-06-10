#include <CUnit/Basic.h>
#include "constraint_collector_visitor.h"

void test_collector_create_with_params(void)
{
    printf("\n\n------------ Inicio test: test_collector_create_with_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: crear collector con ['x', 'y']\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));
    list_append(unannotated, strdup("y"));

    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    CU_ASSERT_PTR_NOT_NULL(collector);
    CU_ASSERT_PTR_NOT_NULL(collector->constraints);
    CU_ASSERT_EQUAL(list_count(collector->constraints), 2);

    TypeConstraint *cx = constraint_collector_get(collector, "x");
    CU_ASSERT_PTR_NOT_NULL(cx);
    CU_ASSERT_EQUAL(cx->kind, CONSTRAINT_CONFORMS);
    CU_ASSERT_EQUAL(list_count(cx->allowed_types), 1);
    CU_ASSERT_PTR_EQUAL(list_get(cx->allowed_types, 0), type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    CU_ASSERT_EQUAL(list_count(cx->lines), 0);
    printf("x: CONFORMS Object, lines=0\n");

    TypeConstraint *cy = constraint_collector_get(collector, "y");
    CU_ASSERT_PTR_NOT_NULL(cy);
    CU_ASSERT_EQUAL(cy->kind, CONSTRAINT_CONFORMS);
    printf("y: CONFORMS Object, lines=0\n");

    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_create_null_params(void)
{
    printf("\n\n------------ Inicio test: test_collector_create_null_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: crear collector con NULL\n");

    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, NULL);

    CU_ASSERT_PTR_NOT_NULL(collector);
    CU_ASSERT_PTR_NOT_NULL(collector->constraints);
    CU_ASSERT_EQUAL(list_count(collector->constraints), 0);
    printf("Collector creado con 0 restricciones\n");

    constraint_collector_destroy(collector);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_create_empty_params(void)
{
    printf("\n\n------------ Inicio test: test_collector_create_empty_params ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: crear collector con lista vacia\n");

    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, empty);

    CU_ASSERT_PTR_NOT_NULL(collector);
    CU_ASSERT_PTR_NOT_NULL(collector->constraints);
    CU_ASSERT_EQUAL(list_count(collector->constraints), 0);
    printf("Collector creado con 0 restricciones\n");

    constraint_collector_destroy(collector);
    list_destroy(empty);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_get_existing(void)
{
    printf("\n\n------------ Inicio test: test_collector_get_existing ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: obtener restriccion existente\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));
    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    TypeConstraint *c = constraint_collector_get(collector, "x");
    CU_ASSERT_PTR_NOT_NULL(c);
    CU_ASSERT_STRING_EQUAL(c->param_name, "x");
    printf("Encontrada: %s\n", c->param_name);

    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_get_nonexistent(void)
{
    printf("\n\n------------ Inicio test: test_collector_get_nonexistent ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: obtener restriccion no existente\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));
    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    TypeConstraint *c = constraint_collector_get(collector, "y");
    CU_ASSERT_PTR_NULL(c);
    printf("No encontrada: NULL\n");

    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_get_null(void)
{
    printf("\n\n------------ Inicio test: test_collector_get_null ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: get con NULL\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));
    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    CU_ASSERT_PTR_NULL(constraint_collector_get(NULL, "x"));
    CU_ASSERT_PTR_NULL(constraint_collector_get(collector, NULL));
    CU_ASSERT_PTR_NULL(constraint_collector_get(NULL, NULL));
    printf("get con NULL = NULL\n");

    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_add_existing_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_add_existing_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: añadir restriccion a parametro existente\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));
    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };
    TypeConstraint *c = constraint_create("x", CONSTRAINT_CONFORMS, types, 1, 10);
    constraint_collector_add(collector, c);

    TypeConstraint *result = constraint_collector_get(collector, "x");
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_EQUAL(result->kind, CONSTRAINT_CONFORMS);
    CU_ASSERT_EQUAL(list_count(result->allowed_types), 1);
    CU_ASSERT_PTR_EQUAL(list_get(result->allowed_types, 0), num);
    CU_ASSERT_EQUAL(list_count(result->lines), 1);
    CU_ASSERT_EQUAL((int)(intptr_t)list_get(result->lines, 0), 10);
    printf("x: CONFORMS Number, line=10\n");

    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_add_nonexistent_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_add_nonexistent_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: añadir restriccion a parametro no listado\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));
    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };
    TypeConstraint *c = constraint_create("y", CONSTRAINT_CONFORMS, types, 1, 10);
    constraint_collector_add(collector, c);

    TypeConstraint *result = constraint_collector_get(collector, "x");
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_EQUAL(result->kind, CONSTRAINT_CONFORMS);
    CU_ASSERT_EQUAL(list_count(result->allowed_types), 1);
    CU_ASSERT_PTR_EQUAL(list_get(result->allowed_types, 0), type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    CU_ASSERT_EQUAL(list_count(result->lines), 0);
    printf("x no modificada\n");

    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_add_null(void)
{
    printf("\n\n------------ Inicio test: test_collector_add_null ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: add con NULL\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));
    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    constraint_collector_add(NULL, NULL);
    constraint_collector_add(collector, NULL);
    CU_PASS("add con NULL no causa crash");

    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_resolve_single(void)
{
    printf("\n\n------------ Inicio test: test_collector_resolve_single ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: resolver con tipo unico\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));

    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };
    TypeConstraint *c = constraint_create("x", CONSTRAINT_EQUAL, types, 1, 1);
    constraint_collector_add(collector, c);

    List *resolved = constraint_collector_resolve(collector);

    CU_ASSERT_PTR_NOT_NULL(resolved);
    CU_ASSERT_EQUAL(list_count(resolved), 1);
    CU_ASSERT_PTR_EQUAL(list_get(resolved, 0), num);
    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Tipo resuelto: Number, Errores: false\n");

    list_destroy(resolved);
    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_resolve_conflict(void)
{
    printf("\n\n------------ Inicio test: test_collector_resolve_conflict ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: resolver con conflicto\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));

    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types1[] = { num };
    TypeDescriptor *types2[] = { str };
    TypeConstraint *c1 = constraint_create("x", CONSTRAINT_EQUAL, types1, 1, 1);
    TypeConstraint *c2 = constraint_create("x", CONSTRAINT_EQUAL, types2, 1, 2);
    constraint_collector_add(collector, c1);
    constraint_collector_add(collector, c2);

    List *resolved = constraint_collector_resolve(collector);

    CU_ASSERT_PTR_NOT_NULL(resolved);
    CU_ASSERT_EQUAL(list_count(resolved), 1);
    CU_ASSERT_PTR_NULL(list_get(resolved, 0));
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Tipo resuelto: NULL, Errores: 1\n");

    list_destroy(resolved);
    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_resolve_multiple(void)
{
    printf("\n\n------------ Inicio test: test_collector_resolve_multiple ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: resolver con multiples tipos\n");

    List *unannotated = list_create(0, NULL, free, NULL, NULL);
    list_append(unannotated, strdup("x"));

    ConstraintCollectorVisitor *collector = constraint_collector_create(NULL, unannotated);

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types[] = { num, str };
    TypeConstraint *c = constraint_create("x", CONSTRAINT_CONFORMS, types, 2, 1);
    constraint_collector_add(collector, c);

    List *resolved = constraint_collector_resolve(collector);

    CU_ASSERT_PTR_NOT_NULL(resolved);
    CU_ASSERT_EQUAL(list_count(resolved), 1);
    CU_ASSERT_PTR_NULL(list_get(resolved, 0));
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Tipo resuelto: NULL, Errores: 1\n");

    list_destroy(resolved);
    constraint_collector_destroy(collector);
    list_destroy(unannotated);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_destroy_null(void)
{
    printf("\n\n------------ Inicio test: test_collector_destroy_null ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: destroy con NULL\n");

    constraint_collector_destroy(NULL);
    CU_PASS("constraint_collector_destroy(NULL) no crashea");

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_collector_resolve_null(void)
{
    printf("\n\n------------ Inicio test: test_collector_resolve_null ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: resolve con NULL\n");

    List *resolved = constraint_collector_resolve(NULL);
    CU_ASSERT_PTR_NULL(resolved);
    printf("Resultado: NULL\n");

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de ConstraintCollector (management) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector Management Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_create_with_params", test_collector_create_with_params);
    CU_add_test(suite, "test_collector_create_null_params", test_collector_create_null_params);
    CU_add_test(suite, "test_collector_create_empty_params", test_collector_create_empty_params);
    CU_add_test(suite, "test_collector_get_existing", test_collector_get_existing);
    CU_add_test(suite, "test_collector_get_nonexistent", test_collector_get_nonexistent);
    CU_add_test(suite, "test_collector_get_null", test_collector_get_null);
    CU_add_test(suite, "test_collector_add_existing_param", test_collector_add_existing_param);
    CU_add_test(suite, "test_collector_add_nonexistent_param", test_collector_add_nonexistent_param);
    CU_add_test(suite, "test_collector_add_null", test_collector_add_null);
    CU_add_test(suite, "test_collector_resolve_single", test_collector_resolve_single);
    CU_add_test(suite, "test_collector_resolve_conflict", test_collector_resolve_conflict);
    CU_add_test(suite, "test_collector_resolve_multiple", test_collector_resolve_multiple);
    CU_add_test(suite, "test_collector_destroy_null", test_collector_destroy_null);
    CU_add_test(suite, "test_collector_resolve_null", test_collector_resolve_null);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}