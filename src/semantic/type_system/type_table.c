#include "type_table.h"

static int type_cmp_by_name(const void *a, const void *b)
{
    const TypeDescriptor *type_a = (const TypeDescriptor *)a;
    const TypeDescriptor *type_b = (const TypeDescriptor *)b;

    return strcmp(type_a->name, type_b->name);
}

static bool register_builtin(TypeTable *table, HulkType tag, const char *name, TypeDescriptor *parent)
{
    TypeDescriptor *type = type_create(tag, name, parent);
    if (!type)
        return false;

    vector_set(table->builtin_types, tag, type);
    return true;
}

static bool type_table_register_builtins(TypeTable *table)
{
    if (!register_builtin(table, HULK_OBJECT, "Object", NULL))
        return false;

    TypeDescriptor *obj = type_table_lookup_by_tag(table, HULK_OBJECT);

    if (!register_builtin(table, HULK_NUMBER, "Number", obj))
        return false;

    if (!register_builtin(table, HULK_STRING, "String", obj))
        return false;

    if (!register_builtin(table, HULK_BOOL, "Bool", obj))
        return false;

    if (!register_builtin(table, HULK_VOID, "Void", obj))
        return false;

    return true;
}

TypeTable *type_table_create(void)
{
    TypeTable *table = (TypeTable *)malloc(sizeof(TypeTable));
    if (!table)
        return NULL;

    table->builtin_types = vector_create(BUILTIN_TYPES_COUNT, NULL, (free_func)type_destroy, type_cmp_by_name, NULL);
    if (!table->builtin_types)
    {
        free(table);
        return NULL;
    }

    table->user_types = list_create(0, NULL, (free_func)user_type_destroy, type_cmp_by_name, NULL);
    if (!table->user_types)
    {
        type_table_destroy(table);
        return NULL;
    }

    if (!type_table_register_builtins(table))
    {
        type_table_destroy(table);
        return NULL;
    }

    return table;
}

void type_table_destroy(TypeTable *table)
{
    if (!table)
        return;

    if (table->builtin_types)
        vector_destroy(table->builtin_types);

    if (table->user_types)
        list_destroy(table->user_types);

    free(table);
}

TypeDescriptor *type_table_lookup_by_tag(TypeTable *table, HulkType tag)
{
    if (!table)
        return NULL;

    if (tag >= 0 && (size_t)tag < BUILTIN_TYPES_COUNT)
        return (TypeDescriptor *)vector_get(table->builtin_types, tag);

    return NULL;
}

TypeDescriptor *type_table_lookup_by_name(TypeTable *table, const char *name)
{
    if (!table || !name)
        return NULL;

    TypeDescriptor key;
    key.name = (char *)name;

    int idx = vector_index_of(table->builtin_types, &key);
    if (idx != -1)
        return (TypeDescriptor *)vector_get(table->builtin_types, idx);

    idx = list_index_of(table->user_types, &key);
    if (idx != -1)
        return (TypeDescriptor *)list_get(table->user_types, idx);

    return NULL;
}

bool type_table_insert(TypeTable *table, UserTypeDescriptor *type)
{
    if (!table || !type)
        return false;

    if (type_table_lookup_by_name(table, type->base.name) != NULL)
        return false;

    list_append(table->user_types, type);
    return true;
}