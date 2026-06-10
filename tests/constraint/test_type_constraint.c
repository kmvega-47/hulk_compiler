#include <CUnit/Basic.h>
#include "type_constraint.h"

void test_constraint_create_valid(void)
{
    printf("\n\n------------ Inicio test: test_constraint_create_valid ------------\n");
    global_tables_init();

    printf("Caso: constraint_create('x', CONFORMS, [Number, String], line=10)\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types[] = { num, str };

    TypeConstraint *c = constraint_create("x", CONSTRAINT_CONFORMS, types, 2, 10);

    CU_ASSERT_PTR_NOT_NULL(c);
    CU_ASSERT_STRING_EQUAL(c->param_name, "x");
    CU_ASSERT_EQUAL(c->kind, CONSTRAINT_CONFORMS);
    CU_ASSERT_EQUAL(list_count(c->allowed_types), 2);
    CU_ASSERT_PTR_EQUAL(list_get(c->allowed_types, 0), num);
    CU_ASSERT_PTR_EQUAL(list_get(c->allowed_types, 1), str);
    CU_ASSERT_EQUAL(list_count(c->lines), 1);
    CU_ASSERT_EQUAL((int)(intptr_t)list_get(c->lines, 0), 10);

    printf("param_name: %s\n", c->param_name);
    printf("kind: %s\n", constraint_kind_to_string(c->kind));
    printf("allowed_types count: %zu\n", list_count(c->allowed_types));
    for (size_t i = 0; i < list_count(c->allowed_types); i++)
    {
        TypeDescriptor *t = (TypeDescriptor *)list_get(c->allowed_types, i);
        printf("  allowed_types[%zu]: %s\n", i, t->name);
    }
    printf("lines count: %zu\n", list_count(c->lines));

    char *cstr = constraint_to_string(c);
    printf("constraint_to_string: %s\n", cstr);
    free(cstr);

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_create_no_line(void)
{
    printf("\n\n------------ Inicio test: test_constraint_create_no_line ------------\n");
    global_tables_init();

    printf("Caso: constraint_create('y', EQUAL, [Number], line=-1)\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };

    TypeConstraint *c = constraint_create("y", CONSTRAINT_EQUAL, types, 1, -1);

    CU_ASSERT_PTR_NOT_NULL(c);
    CU_ASSERT_STRING_EQUAL(c->param_name, "y");
    CU_ASSERT_EQUAL(c->kind, CONSTRAINT_EQUAL);
    CU_ASSERT_EQUAL(list_count(c->allowed_types), 1);
    CU_ASSERT_EQUAL(list_count(c->lines), 0);

    printf("param_name: %s\n", c->param_name);
    printf("kind: %s\n", constraint_kind_to_string(c->kind));
    printf("allowed_types count: %zu\n", list_count(c->allowed_types));
    printf("lines count: %zu\n", list_count(c->lines));

    char *cstr = constraint_to_string(c);
    printf("constraint_to_string: %s\n", cstr);
    free(cstr);

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_create_null_name(void)
{
    printf("\n\n------------ Inicio test: test_constraint_create_null_name ------------\n");
    global_tables_init();

    printf("Caso: constraint_create(NULL, ...)\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };

    TypeConstraint *c = constraint_create(NULL, CONSTRAINT_CONFORMS, types, 1, 1);
    CU_ASSERT_PTR_NULL(c);
    printf("Resultado: NULL (esperado)\n");

    global_tables_destroy();
}

void test_constraint_create_null_types(void)
{
    printf("\n\n------------ Inicio test: test_constraint_create_null_types ------------\n");
    global_tables_init();

    printf("Caso: constraint_create('x', ..., NULL, ...)\n");

    TypeConstraint *c = constraint_create("x", CONSTRAINT_CONFORMS, NULL, 1, 1);
    CU_ASSERT_PTR_NULL(c);
    printf("Resultado: NULL (esperado)\n");

    global_tables_destroy();
}

void test_constraint_create_zero_count(void)
{
    printf("\n\n------------ Inicio test: test_constraint_create_zero_count ------------\n");
    global_tables_init();

    printf("Caso: constraint_create('x', ..., count=0)\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };

    TypeConstraint *c = constraint_create("x", CONSTRAINT_CONFORMS, types, 0, 1);
    CU_ASSERT_PTR_NULL(c);
    printf("Resultado: NULL (esperado)\n");

    global_tables_destroy();
}

void test_constraint_create_initial_valid(void)
{
    printf("\n\n------------ Inicio test: test_constraint_create_initial_valid ------------\n");
    global_tables_init();

    printf("Caso: constraint_create_initial('x')\n");

    TypeConstraint *c = constraint_create_initial("x");

    CU_ASSERT_PTR_NOT_NULL(c);
    CU_ASSERT_STRING_EQUAL(c->param_name, "x");
    CU_ASSERT_EQUAL(c->kind, CONSTRAINT_CONFORMS);
    CU_ASSERT_EQUAL(list_count(c->allowed_types), 1);
    CU_ASSERT_PTR_EQUAL(list_get(c->allowed_types, 0), type_table_lookup_by_tag(global_type_table, HULK_OBJECT));
    CU_ASSERT_EQUAL(list_count(c->lines), 0);

    printf("param_name: %s\n", c->param_name);
    printf("kind: %s\n", constraint_kind_to_string(c->kind));
    printf("allowed_types count: %zu\n", list_count(c->allowed_types));
    printf("  allowed_types[0]: %s\n", ((TypeDescriptor *)list_get(c->allowed_types, 0))->name);
    printf("lines count: %zu\n", list_count(c->lines));

    char *cstr = constraint_to_string(c);
    printf("constraint_to_string: %s\n", cstr);
    free(cstr);

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_create_initial_null(void)
{
    printf("\n\n------------ Inicio test: test_constraint_create_initial_null ------------\n");
    global_tables_init();

    printf("Caso: constraint_create_initial(NULL)\n");

    TypeConstraint *c = constraint_create_initial(NULL);
    CU_ASSERT_PTR_NULL(c);
    printf("Resultado: NULL (esperado)\n");

    global_tables_destroy();
}

void test_constraint_clone_no_lines(void)
{
    printf("\n\n------------ Inicio test: test_constraint_clone_no_lines ------------\n");
    global_tables_init();

    printf("Caso: clonar restriccion sin lineas\n");

    TypeConstraint *original = constraint_create_initial("y");
    TypeConstraint *clone = constraint_clone(original);

    CU_ASSERT_PTR_NOT_NULL(clone);
    CU_ASSERT_STRING_EQUAL(clone->param_name, "y");
    CU_ASSERT_EQUAL(clone->kind, CONSTRAINT_CONFORMS);
    CU_ASSERT_EQUAL(list_count(clone->allowed_types), 1);
    CU_ASSERT_EQUAL(list_count(clone->lines), 0);

    printf("Direccion original: %p, clone: %p\n", (void *)original, (void *)clone);
    CU_ASSERT_PTR_NOT_EQUAL(original, clone);

    constraint_destroy(original);
    constraint_destroy(clone);
    global_tables_destroy();
}

void test_constraint_clone_single_line(void)
{
    printf("\n\n------------ Inicio test: test_constraint_clone_single_line ------------\n");
    global_tables_init();

    printf("Caso: clonar restriccion con una sola linea\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };
    TypeConstraint *original = constraint_create("x", CONSTRAINT_EQUAL, types, 1, 42);
    TypeConstraint *clone = constraint_clone(original);

    CU_ASSERT_PTR_NOT_NULL(clone);
    CU_ASSERT_STRING_EQUAL(clone->param_name, "x");
    CU_ASSERT_EQUAL(clone->kind, CONSTRAINT_EQUAL);
    CU_ASSERT_EQUAL(list_count(clone->allowed_types), 1);
    CU_ASSERT_EQUAL(list_count(clone->lines), 1);
    CU_ASSERT_EQUAL((int)(intptr_t)list_get(clone->lines, 0), 42);

    printf("Direccion original: %p, clone: %p\n", (void *)original, (void *)clone);
    CU_ASSERT_PTR_NOT_EQUAL(original, clone);

    constraint_destroy(original);
    constraint_destroy(clone);
    global_tables_destroy();
}

void test_constraint_clone_multiple_lines(void)
{
    printf("\n\n------------ Inicio test: test_constraint_clone_multiple_lines ------------\n");
    global_tables_init();

    printf("Caso: clonar restriccion con multiples lineas\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types[] = { num, str };
    TypeConstraint *original = constraint_create("z", CONSTRAINT_CONFORMS, types, 2, 10);
    list_append(original->lines, (void *)(intptr_t)20);
    list_append(original->lines, (void *)(intptr_t)30);

    TypeConstraint *clone = constraint_clone(original);

    CU_ASSERT_PTR_NOT_NULL(clone);
    CU_ASSERT_STRING_EQUAL(clone->param_name, "z");
    CU_ASSERT_EQUAL(list_count(clone->allowed_types), 2);
    CU_ASSERT_EQUAL(list_count(clone->lines), 3);
    CU_ASSERT_EQUAL((int)(intptr_t)list_get(clone->lines, 0), 10);
    CU_ASSERT_EQUAL((int)(intptr_t)list_get(clone->lines, 1), 20);
    CU_ASSERT_EQUAL((int)(intptr_t)list_get(clone->lines, 2), 30);

    printf("Direccion original: %p, clone: %p\n", (void *)original, (void *)clone);
    CU_ASSERT_PTR_NOT_EQUAL(original, clone);

    constraint_destroy(original);
    constraint_destroy(clone);
    global_tables_destroy();
}

void test_constraint_clone_null(void)
{
    printf("\n\n------------ Inicio test: test_constraint_clone_null ------------\n");
    global_tables_init();

    printf("Caso: constraint_clone(NULL)\n");
    TypeConstraint *clone = constraint_clone(NULL);
    CU_ASSERT_PTR_NULL(clone);
    printf("Resultado: NULL (esperado)\n");

    global_tables_destroy();
}

void test_constraint_is_conflicted_false(void)
{
    printf("\n\n------------ Inicio test: test_constraint_is_conflicted_false ------------\n");
    global_tables_init();

    printf("Caso: restriccion CONFORMS no esta en conflicto\n");
    TypeConstraint *c = constraint_create_initial("x");
    bool result = constraint_is_conflicted(c);
    CU_ASSERT_FALSE(result);
    printf("constraint_is_conflicted: %s\n", result ? "true" : "false");

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_is_conflicted_true(void)
{
    printf("\n\n------------ Inicio test: test_constraint_is_conflicted_true ------------\n");
    global_tables_init();

    printf("Caso: restriccion CONFLICT si esta en conflicto\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types1[] = { num };
    TypeDescriptor *types2[] = { str };

    TypeConstraint *a = constraint_create("x", CONSTRAINT_EQUAL, types1, 1, 1);
    TypeConstraint *b = constraint_create("x", CONSTRAINT_EQUAL, types2, 1, 2);
    TypeConstraint *conflict = constraint_merge(a, b);

    bool result = constraint_is_conflicted(conflict);
    CU_ASSERT_TRUE(result);
    printf("constraint_is_conflicted: %s\n", result ? "true" : "false");

    constraint_destroy(a);
    constraint_destroy(b);
    constraint_destroy(conflict);
    global_tables_destroy();
}

void test_constraint_is_conflicted_null(void)
{
    printf("\n\n------------ Inicio test: test_constraint_is_conflicted_null ------------\n");
    global_tables_init();

    printf("Caso: constraint_is_conflicted(NULL)\n");
    bool result = constraint_is_conflicted(NULL);
    CU_ASSERT_FALSE(result);
    printf("constraint_is_conflicted(NULL): %s\n", result ? "true" : "false");

    global_tables_destroy();
}

void test_constraint_last_line_with_lines(void)
{
    printf("\n\n------------ Inicio test: test_constraint_last_line_with_lines ------------\n");
    global_tables_init();

    printf("Caso: restriccion con varias lineas\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };
    TypeConstraint *c = constraint_create("x", CONSTRAINT_CONFORMS, types, 1, 10);
    list_append(c->lines, (void *)(intptr_t)20);
    list_append(c->lines, (void *)(intptr_t)30);

    int last = constraint_last_line(c);
    CU_ASSERT_EQUAL(last, 30);
    printf("Ultima linea: %d\n", last);

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_last_line_no_lines(void)
{
    printf("\n\n------------ Inicio test: test_constraint_last_line_no_lines ------------\n");
    global_tables_init();

    printf("Caso: restriccion sin lineas\n");
    TypeConstraint *c = constraint_create_initial("x");
    int last = constraint_last_line(c);
    CU_ASSERT_EQUAL(last, -1);
    printf("Ultima linea: %d\n", last);

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_last_line_null(void)
{
    printf("\n\n------------ Inicio test: test_constraint_last_line_null ------------\n");
    global_tables_init();

    printf("Caso: constraint_last_line(NULL)\n");
    int last = constraint_last_line(NULL);
    CU_ASSERT_EQUAL(last, -1);
    printf("Ultima linea: %d\n", last);

    global_tables_destroy();
}

void test_constraint_cmp_by_name_equal(void)
{
    printf("\n\n------------ Inicio test: test_constraint_cmp_by_name_equal ------------\n");
    global_tables_init();

    printf("Caso: mismo nombre\n");
    TypeConstraint *a = constraint_create_initial("x");
    TypeConstraint *b = constraint_create_initial("x");

    int result = constraint_cmp_by_name(a, b);
    CU_ASSERT_EQUAL(result, 0);
    printf("cmp('x', 'x') = %d\n", result);

    constraint_destroy(a);
    constraint_destroy(b);
    global_tables_destroy();
}

void test_constraint_cmp_by_name_different(void)
{
    printf("\n\n------------ Inicio test: test_constraint_cmp_by_name_different ------------\n");
    global_tables_init();

    printf("Caso: nombres diferentes\n");
    TypeConstraint *a = constraint_create_initial("x");
    TypeConstraint *b = constraint_create_initial("y");

    int result = constraint_cmp_by_name(a, b);
    CU_ASSERT_NOT_EQUAL(result, 0);
    printf("cmp('x', 'y') = %d\n", result);

    constraint_destroy(a);
    constraint_destroy(b);
    global_tables_destroy();
}

void test_constraint_cmp_by_name_null(void)
{
    printf("\n\n------------ Inicio test: test_constraint_cmp_by_name_null ------------\n");
    global_tables_init();

    printf("Caso: una restriccion NULL\n");
    TypeConstraint *a = constraint_create_initial("x");

    int result = constraint_cmp_by_name(a, NULL);
    CU_ASSERT_EQUAL(result, -1);
    printf("cmp('x', NULL) = %d\n", result);

    result = constraint_cmp_by_name(NULL, a);
    CU_ASSERT_EQUAL(result, -1);
    printf("cmp(NULL, 'x') = %d\n", result);

    constraint_destroy(a);
    global_tables_destroy();
}

void test_constraint_merge_equal_equal_non_empty(void)
{
    printf("\n\n------------ Inicio test: test_constraint_merge_equal_equal_non_empty ------------\n");
    global_tables_init();

    printf("Merge EQUAL + EQUAL con interseccion no vacia\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    TypeDescriptor *types1[] = { num, obj };
    TypeDescriptor *types2[] = { obj };

    TypeConstraint *a = constraint_create("x", CONSTRAINT_EQUAL, types1, 2, 1);
    TypeConstraint *b = constraint_create("x", CONSTRAINT_EQUAL, types2, 1, 2);
    TypeConstraint *merged = constraint_merge(a, b);

    CU_ASSERT_PTR_NOT_NULL(merged);
    CU_ASSERT_EQUAL(merged->kind, CONSTRAINT_EQUAL);
    CU_ASSERT_EQUAL(list_count(merged->allowed_types), 1);
    CU_ASSERT_PTR_EQUAL(list_get(merged->allowed_types, 0), obj);

    char *cstr = constraint_to_string(merged);
    printf("Merged: %s\n", cstr);
    free(cstr);

    constraint_destroy(a);
    constraint_destroy(b);
    constraint_destroy(merged);
    global_tables_destroy();
}

void test_constraint_merge_equal_equal_conflict(void)
{
    printf("\n\n------------ Inicio test: test_constraint_merge_equal_equal_conflict ------------\n");
    global_tables_init();

    printf("Merge EQUAL + EQUAL con interseccion vacia -> CONFLICT\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types1[] = { num };
    TypeDescriptor *types2[] = { str };

    TypeConstraint *a = constraint_create("x", CONSTRAINT_EQUAL, types1, 1, 1);
    TypeConstraint *b = constraint_create("x", CONSTRAINT_EQUAL, types2, 1, 2);
    TypeConstraint *merged = constraint_merge(a, b);

    CU_ASSERT_PTR_NOT_NULL(merged);
    CU_ASSERT_EQUAL(merged->kind, CONSTRAINT_CONFLICT);
    CU_ASSERT_TRUE(constraint_is_conflicted(merged));

    char *cstr = constraint_to_string(merged);
    printf("Merged: %s\n", cstr);
    free(cstr);

    constraint_destroy(a);
    constraint_destroy(b);
    constraint_destroy(merged);
    global_tables_destroy();
}

void test_constraint_merge_conforms_conforms(void)
{
    printf("\n\n------------ Inicio test: test_constraint_merge_conforms_conforms ------------\n");
    global_tables_init();

    printf("Merge CONFORMS + CONFORMS realista: {Object} con {Number, String}\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);

    TypeDescriptor *types1[] = { obj };
    TypeDescriptor *types2[] = { num, str };

    TypeConstraint *a = constraint_create("x", CONSTRAINT_CONFORMS, types1, 1, 1);
    TypeConstraint *b = constraint_create("x", CONSTRAINT_CONFORMS, types2, 2, 2);
    TypeConstraint *merged = constraint_merge(a, b);

    CU_ASSERT_PTR_NOT_NULL(merged);
    CU_ASSERT_EQUAL(merged->kind, CONSTRAINT_CONFORMS);
    CU_ASSERT_EQUAL(list_count(merged->allowed_types), 2);

    bool has_num = false, has_str = false;
    for (size_t i = 0; i < list_count(merged->allowed_types); i++)
    {
        TypeDescriptor *t = (TypeDescriptor *)list_get(merged->allowed_types, i);
        if (t == num) has_num = true;
        if (t == str) has_str = true;
    }
    CU_ASSERT_TRUE(has_num);
    CU_ASSERT_TRUE(has_str);

    char *cstr = constraint_to_string(merged);
    printf("Merged: %s\n", cstr);
    free(cstr);

    constraint_destroy(a);
    constraint_destroy(b);
    constraint_destroy(merged);
    global_tables_destroy();
}

void test_constraint_merge_equal_conforms(void)
{
    printf("\n\n------------ Inicio test: test_constraint_merge_equal_conforms ------------\n");
    global_tables_init();

    printf("Merge EQUAL + CONFORMS -> EQUAL con filtrado\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    TypeDescriptor *types_equal[] = { num, obj };
    TypeDescriptor *types_conf[] = { obj };

    TypeConstraint *a = constraint_create("x", CONSTRAINT_EQUAL, types_equal, 2, 1);
    TypeConstraint *b = constraint_create("x", CONSTRAINT_CONFORMS, types_conf, 1, 2);
    TypeConstraint *merged = constraint_merge(a, b);

    CU_ASSERT_PTR_NOT_NULL(merged);
    CU_ASSERT_EQUAL(merged->kind, CONSTRAINT_EQUAL);
    CU_ASSERT_EQUAL(list_count(merged->allowed_types), 2);

    char *cstr = constraint_to_string(merged);
    printf("Merged: %s\n", cstr);
    free(cstr);

    constraint_destroy(a);
    constraint_destroy(b);
    constraint_destroy(merged);
    global_tables_destroy();
}

void test_constraint_merge_different_names(void)
{
    printf("\n\n------------ Inicio test: test_constraint_merge_different_names ------------\n");
    global_tables_init();

    printf("Merge con nombres diferentes -> NULL\n");
    TypeConstraint *a = constraint_create_initial("x");
    TypeConstraint *b = constraint_create_initial("y");
    TypeConstraint *merged = constraint_merge(a, b);
    CU_ASSERT_PTR_NULL(merged);
    printf("merge('x','y') = NULL\n");

    constraint_destroy(a);
    constraint_destroy(b);
    global_tables_destroy();
}

void test_constraint_merge_with_conflict(void)
{
    printf("\n\n------------ Inicio test: test_constraint_merge_with_conflict ------------\n");
    global_tables_init();

    printf("Merge con un operando CONFLICT -> clona el conflicto\n");

    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types1[] = { num };
    TypeDescriptor *types2[] = { str };

    TypeConstraint *a = constraint_create("x", CONSTRAINT_EQUAL, types1, 1, 1);
    TypeConstraint *b = constraint_create("x", CONSTRAINT_EQUAL, types2, 1, 2);
    TypeConstraint *conflict = constraint_merge(a, b);
    TypeConstraint *c = constraint_create_initial("x");
    TypeConstraint *merged = constraint_merge(conflict, c);

    CU_ASSERT_PTR_NOT_NULL(merged);
    CU_ASSERT_EQUAL(merged->kind, CONSTRAINT_CONFLICT);
    printf("merge(CONFLICT, ...) kind = %s\n", constraint_kind_to_string(merged->kind));

    constraint_destroy(a);
    constraint_destroy(b);
    constraint_destroy(conflict);
    constraint_destroy(c);
    constraint_destroy(merged);
    global_tables_destroy();
}

void test_constraint_merge_null(void)
{
    printf("\n\n------------ Inicio test: test_constraint_merge_null ------------\n");
    global_tables_init();

    printf("Merge con NULL -> NULL\n");
    TypeConstraint *a = constraint_create_initial("x");
    TypeConstraint *merged = constraint_merge(a, NULL);
    CU_ASSERT_PTR_NULL(merged);
    printf("merge(a, NULL) = NULL\n");

    merged = constraint_merge(NULL, a);
    CU_ASSERT_PTR_NULL(merged);
    printf("merge(NULL, a) = NULL\n");

    constraint_destroy(a);
    global_tables_destroy();
}

void test_constraint_resolve_type_single(void)
{
    printf("\n\n------------ Inicio test: test_constraint_resolve_type_single ------------\n");
    global_tables_init();

    printf("Caso: restriccion con un solo tipo -> 0\n");
    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *types[] = { num };
    TypeConstraint *c = constraint_create("x", CONSTRAINT_EQUAL, types, 1, 1);

    TypeDescriptor *resolved = NULL;
    int result = constraint_resolve_type(c, &resolved);
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_PTR_EQUAL(resolved, num);
    printf("result = %d, type = %s\n", result, resolved->name);

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_resolve_type_conflict(void)
{
    printf("\n\n------------ Inicio test: test_constraint_resolve_type_conflict ------------\n");
    global_tables_init();

    printf("Caso: restriccion CONFLICT -> 1\n");
    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types1[] = { num };
    TypeDescriptor *types2[] = { str };

    TypeConstraint *a = constraint_create("x", CONSTRAINT_EQUAL, types1, 1, 1);
    TypeConstraint *b = constraint_create("x", CONSTRAINT_EQUAL, types2, 1, 2);
    TypeConstraint *conflict = constraint_merge(a, b);

    TypeDescriptor *resolved = NULL;
    int result = constraint_resolve_type(conflict, &resolved);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_PTR_NULL(resolved);
    printf("result = %d, type = %s\n", result, resolved ? resolved->name : "NULL");

    constraint_destroy(a);
    constraint_destroy(b);
    constraint_destroy(conflict);
    global_tables_destroy();
}

void test_constraint_resolve_type_multiple(void)
{
    printf("\n\n------------ Inicio test: test_constraint_resolve_type_multiple ------------\n");
    global_tables_init();

    printf("Caso: restriccion con multiples tipos -> 2\n");
    TypeDescriptor *num = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *str = type_table_lookup_by_tag(global_type_table, HULK_STRING);
    TypeDescriptor *types[] = { num, str };
    TypeConstraint *c = constraint_create("x", CONSTRAINT_CONFORMS, types, 2, 1);

    TypeDescriptor *resolved = NULL;
    int result = constraint_resolve_type(c, &resolved);
    CU_ASSERT_EQUAL(result, 2);
    CU_ASSERT_PTR_NULL(resolved);
    printf("result = %d, type = %s\n", result, resolved ? resolved->name : "NULL");

    constraint_destroy(c);
    global_tables_destroy();
}

void test_constraint_resolve_type_null(void)
{
    printf("\n\n------------ Inicio test: test_constraint_resolve_type_null ------------\n");
    global_tables_init();

    printf("Caso: constraint_resolve_type(NULL, ...) -> 1\n");
    TypeDescriptor *resolved = NULL;
    int result = constraint_resolve_type(NULL, &resolved);
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_PTR_NULL(resolved);
    printf("result = %d\n", result);

    result = constraint_resolve_type(NULL, NULL);
    CU_ASSERT_EQUAL(result, 1);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeConstraint ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeConstraint Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_constraint_create_valid", test_constraint_create_valid);
    CU_add_test(suite, "test_constraint_create_no_line", test_constraint_create_no_line);
    CU_add_test(suite, "test_constraint_create_null_name", test_constraint_create_null_name);
    CU_add_test(suite, "test_constraint_create_null_types", test_constraint_create_null_types);
    CU_add_test(suite, "test_constraint_create_zero_count", test_constraint_create_zero_count);
    CU_add_test(suite, "test_constraint_create_initial_valid", test_constraint_create_initial_valid);
    CU_add_test(suite, "test_constraint_create_initial_null", test_constraint_create_initial_null);
    CU_add_test(suite, "test_constraint_clone_no_lines", test_constraint_clone_no_lines);
    CU_add_test(suite, "test_constraint_clone_single_line", test_constraint_clone_single_line);
    CU_add_test(suite, "test_constraint_clone_multiple_lines", test_constraint_clone_multiple_lines);
    CU_add_test(suite, "test_constraint_clone_null", test_constraint_clone_null);
    CU_add_test(suite, "test_constraint_is_conflicted_false", test_constraint_is_conflicted_false);
    CU_add_test(suite, "test_constraint_is_conflicted_true", test_constraint_is_conflicted_true);
    CU_add_test(suite, "test_constraint_is_conflicted_null", test_constraint_is_conflicted_null);
    CU_add_test(suite, "test_constraint_last_line_with_lines", test_constraint_last_line_with_lines);
    CU_add_test(suite, "test_constraint_last_line_no_lines", test_constraint_last_line_no_lines);
    CU_add_test(suite, "test_constraint_last_line_null", test_constraint_last_line_null);
    CU_add_test(suite, "test_constraint_cmp_by_name_equal", test_constraint_cmp_by_name_equal);
    CU_add_test(suite, "test_constraint_cmp_by_name_different", test_constraint_cmp_by_name_different);
    CU_add_test(suite, "test_constraint_cmp_by_name_null", test_constraint_cmp_by_name_null);
    CU_add_test(suite, "test_constraint_merge_equal_equal_non_empty", test_constraint_merge_equal_equal_non_empty);
    CU_add_test(suite, "test_constraint_merge_equal_equal_conflict", test_constraint_merge_equal_equal_conflict);
    CU_add_test(suite, "test_constraint_merge_conforms_conforms", test_constraint_merge_conforms_conforms);
    CU_add_test(suite, "test_constraint_merge_equal_conforms", test_constraint_merge_equal_conforms);
    CU_add_test(suite, "test_constraint_merge_different_names", test_constraint_merge_different_names);
    CU_add_test(suite, "test_constraint_merge_with_conflict", test_constraint_merge_with_conflict);
    CU_add_test(suite, "test_constraint_merge_null", test_constraint_merge_null);
    CU_add_test(suite, "test_constraint_resolve_type_single", test_constraint_resolve_type_single);
    CU_add_test(suite, "test_constraint_resolve_type_conflict", test_constraint_resolve_type_conflict);
    CU_add_test(suite, "test_constraint_resolve_type_multiple", test_constraint_resolve_type_multiple);
    CU_add_test(suite, "test_constraint_resolve_type_null", test_constraint_resolve_type_null);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}