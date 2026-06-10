#include "type_constraint.h"

static int type_cmp_by_tag_wrapper(const void *a, const void *b)
{
    const TypeDescriptor *type_a = (const TypeDescriptor *)a;
    const TypeDescriptor *type_b = (const TypeDescriptor *)b;
    return type_cmp(type_a, type_b);
}

TypeConstraint *constraint_create(const char *param_name, ConstraintKind kind, TypeDescriptor **types, size_t count, int line)
{
    if (!param_name || !types || count == 0)
        return NULL;

    TypeConstraint *constraint = (TypeConstraint *)malloc(sizeof(TypeConstraint));
    if (!constraint)
        return NULL;

    constraint->param_name = strdup(param_name);

    if (!constraint->param_name)
    {
        free(constraint);
        return NULL;
    }

    constraint->kind = kind;

    constraint->allowed_types = list_create(0, NULL, NULL, type_cmp_by_tag_wrapper, NULL);

    if (!constraint->allowed_types)
    {
        free(constraint->param_name);
        free(constraint);
        return NULL;
    }

    for (size_t i = 0; i < count; i++)
        list_append(constraint->allowed_types, types[i]);

    constraint->lines = list_create(0, NULL, NULL, NULL, NULL);

    if (!constraint->lines)
    {
        list_destroy(constraint->allowed_types);
        free(constraint->param_name);
        free(constraint);
        return NULL;
    }

    if (line >= 0)
        list_append(constraint->lines, (void *)(intptr_t)line);

    return constraint;
}

TypeConstraint *constraint_create_initial(const char *param_name)
{
    TypeDescriptor *types[] = { type_table_lookup_by_tag(global_type_table, HULK_OBJECT) };
    return constraint_create(param_name, CONSTRAINT_CONFORMS, types, 1, -1);
}

void constraint_destroy(TypeConstraint *constraint)
{
    if (!constraint)
        return;

    free(constraint->param_name);
    list_destroy(constraint->allowed_types);
    list_destroy(constraint->lines);

    free(constraint);
}

TypeConstraint *constraint_clone(const TypeConstraint *c)
{
    if (!c)
        return NULL;

    size_t type_count = list_count(c->allowed_types);

    TypeDescriptor **types = malloc(type_count * sizeof(TypeDescriptor *));

    if (!types)
        return NULL;

    for (size_t i = 0; i < type_count; i++)
        types[i] = (TypeDescriptor *)list_get(c->allowed_types, i);

    int first_line = (list_count(c->lines) > 0) ? (int)(intptr_t)list_get(c->lines, 0) : -1;

    TypeConstraint *clone = constraint_create(c->param_name, c->kind, types, type_count, first_line);
    free(types);

    if (!clone)
        return NULL;

    for (size_t i = 1; i < list_count(c->lines); i++)
    {
        int line = (int)(intptr_t)list_get(c->lines, i);
        list_append(clone->lines, (void *)(intptr_t)line);
    }

    return clone;
}

bool constraint_is_conflicted(const TypeConstraint *c)
{
    return c && c->kind == CONSTRAINT_CONFLICT;
}

int constraint_last_line(const TypeConstraint *c)
{
    if (!c || !c->lines || list_count(c->lines) == 0)
        return -1;

    return (int)(intptr_t)list_get(c->lines, list_count(c->lines) - 1);
}

int constraint_cmp_by_name(const TypeConstraint *a, const TypeConstraint *b)
{
    if (!a || !b || !a->param_name || !b->param_name)
        return -1;

    return strcmp(a->param_name, b->param_name);
}

static bool type_conforms_to_any(const void *element, void *context)
{
    const TypeDescriptor *type = (const TypeDescriptor *)element;
    List *target_types = (List *)context;

    for (size_t i = 0; i < list_count(target_types); i++)
    {
        TypeDescriptor *target = (TypeDescriptor *)list_get(target_types, i);

        if (type_conforms_to(type, target))
            return true;
    }

    return false;
}

static List *filter_types_by_conformance(List *types, List *target_types)
{
    if (!types || !target_types)
        return NULL;

    return list_filter(types, type_conforms_to_any, target_types);
}

static List *merge_lines(const TypeConstraint *a, const TypeConstraint *b)
{
    List *merged = list_create(0, NULL, NULL, NULL, NULL);

    if (!merged)
        return NULL;

    for (size_t i = 0; i < list_count(a->lines); i++)
        list_append(merged, list_get(a->lines, i));

    for (size_t i = 0; i < list_count(b->lines); i++)
        list_append(merged, list_get(b->lines, i));

    return merged;
}

static TypeConstraint *constraint_create_merged(const char *param_name, ConstraintKind kind, List *allowed_types, List *lines)
{
    TypeConstraint *constraint = (TypeConstraint *)malloc(sizeof(TypeConstraint));
    if (!constraint)
        return NULL;

    constraint->param_name = strdup(param_name);
    if (!constraint->param_name)
    {
        free(constraint);
        return NULL;
    }

    constraint->kind = kind;
    constraint->allowed_types = allowed_types;
    constraint->lines = lines;

    return constraint;
}

static bool resolve_merge(const TypeConstraint *a, const TypeConstraint *b, List **out_types, ConstraintKind *out_kind)
{
    if (a->kind == CONSTRAINT_EQUAL && b->kind == CONSTRAINT_EQUAL)
    {
        *out_types = list_intersection(a->allowed_types, b->allowed_types);
        *out_kind = CONSTRAINT_EQUAL;
    }
    
    else if (a->kind == CONSTRAINT_CONFORMS && b->kind == CONSTRAINT_CONFORMS)
    {
        List *filtered_a = filter_types_by_conformance(a->allowed_types, b->allowed_types);
        List *filtered_b = filter_types_by_conformance(b->allowed_types, a->allowed_types);

        if (!filtered_a || !filtered_b)
        {
            if (filtered_a) list_destroy(filtered_a);
            if (filtered_b) list_destroy(filtered_b);
            return false;
        }

        *out_types = list_union(filtered_a, filtered_b);

        list_destroy(filtered_a);
        list_destroy(filtered_b);

        *out_kind = CONSTRAINT_CONFORMS;
    }

    else
    {
        const TypeConstraint *equals = (a->kind == CONSTRAINT_EQUAL) ? a : b;
        const TypeConstraint *conforms = (a->kind == CONSTRAINT_EQUAL) ? b : a;

        *out_types = filter_types_by_conformance(equals->allowed_types, conforms->allowed_types);
        *out_kind = CONSTRAINT_EQUAL;
    }

    return *out_types != NULL;
}

TypeConstraint *constraint_merge(const TypeConstraint *a, const TypeConstraint *b)
{
    if (!a || !b)
        return NULL;

    if (strcmp(a->param_name, b->param_name) != 0)
        return NULL;

    if (a->kind == CONSTRAINT_CONFLICT)
        return constraint_clone(a);

    if (b->kind == CONSTRAINT_CONFLICT)
        return constraint_clone(b);

    List *compatible_types = NULL;
    ConstraintKind result_kind;

    if (!resolve_merge(a, b, &compatible_types, &result_kind))
        return NULL;

    List *lines = merge_lines(a, b);
    if (!lines)
    {
        list_destroy(compatible_types);
        return NULL;
    }

    if (list_count(compatible_types) == 0)
    {
        List *conflict_types = list_create(0, NULL, NULL, type_cmp_by_tag_wrapper, NULL);

        for (size_t i = 0; i < list_count(a->allowed_types); i++)
        {
            list_append(conflict_types, list_get(a->allowed_types, i));
        }

        for (size_t i = 0; i < list_count(b->allowed_types); i++)
        {
            list_append(conflict_types, list_get(b->allowed_types, i));
        }

        list_destroy(compatible_types);
        return constraint_create_merged(a->param_name, CONSTRAINT_CONFLICT, conflict_types, lines);
    }

    return constraint_create_merged(a->param_name, result_kind, compatible_types, lines);
}

int constraint_resolve_type(const TypeConstraint *c, TypeDescriptor **out_type)
{
    if (!c || !out_type)
        return 1;

    if (c->kind == CONSTRAINT_CONFLICT)
    {
        *out_type = NULL;
        return 1;
    }

    size_t count = list_count(c->allowed_types);

    if (count == 1)
    {
        *out_type = (TypeDescriptor *)list_get(c->allowed_types, 0);
        return 0;
    }

    *out_type = NULL;
    return 2;
}

char *constraint_to_string(const TypeConstraint *c)
{
    if (!c)
        return strdup("NULL");

    char buffer[1024] = "";

    strcat(buffer, c->param_name);
    strcat(buffer, " ");
    strcat(buffer, constraint_kind_to_string(c->kind));
    strcat(buffer, " {");

    for (size_t i = 0; i < list_count(c->allowed_types); i++)
    {
        TypeDescriptor *t = (TypeDescriptor *)list_get(c->allowed_types, i);
        strcat(buffer, t->name);

        if (i < list_count(c->allowed_types) - 1)
            strcat(buffer, ", ");
    }

    strcat(buffer, "} : lines (");

    for (size_t i = 0; i < list_count(c->lines); i++)
    {
        char line_str[16];
        int line = (int)(intptr_t)list_get(c->lines, i);

        snprintf(line_str, sizeof(line_str), "%d", line);
        strcat(buffer, line_str);
        
        if (i < list_count(c->lines) - 1)
            strcat(buffer, ", ");
    }

    strcat(buffer, ")");

    return strdup(buffer);
}