#include "function_table.h"

typedef struct FunctionEntry
{
    char *name;
    TypeDescriptor *return_type;
    List *param_types;
    LLVMValueRef fn;
    LLVMTypeRef func_type;
} FunctionEntry;

static int function_entry_cmp_by_name(const void *a, const void *b)
{
    const FunctionEntry *entry_a = (const FunctionEntry *)a;
    const FunctionEntry *entry_b = (const FunctionEntry *)b;

    return strcmp(entry_a->name, entry_b->name);
}

static FunctionEntry *function_entry_create(const char *name, TypeDescriptor *return_type, List *param_types)
{
    if (!name || !param_types)
        return NULL;

    FunctionEntry *entry = (FunctionEntry *)malloc(sizeof(FunctionEntry));
    if (!entry)
        return NULL;

    entry->name = strdup(name);
    if (!entry->name)
    {
        free(entry);
        return NULL;
    }

    entry->return_type = return_type;
    entry->param_types = param_types;
    entry->fn = NULL;
    entry->func_type = NULL;

    return entry;
}

static void function_entry_destroy(FunctionEntry *entry)
{
    if (!entry)
        return;

    free(entry->name);

    if (entry->param_types)
        list_destroy(entry->param_types);

    free(entry);
}

static FunctionEntry *function_table_search(FunctionTable *table, const char *name)
{
    if (!table || !name)
        return NULL;

    FunctionEntry search_key;
    search_key.name = (char *)name;

    int idx = list_index_of(table->functions, (const void *)&search_key);

    if (idx < 0)
        return NULL;

    return list_get(table->functions, idx);
}

char *function_table_build_method_name(const char *type_name, const char *method_name)
{
    if (!type_name || !method_name)
        return NULL;

    size_t len = strlen(type_name) + strlen(method_name) + 3;
    char *full_name = (char *)malloc(len);
    if (!full_name)
        return NULL;

    snprintf(full_name, len, "_%s_%s", type_name, method_name);
    return full_name;
}

FunctionTable *function_table_create(void)
{
    FunctionTable *table = (FunctionTable *)malloc(sizeof(FunctionTable));
    if (!table)
        return NULL;

    table->functions = list_create(0, NULL, (free_func)function_entry_destroy, function_entry_cmp_by_name, NULL);
    if (!table->functions)
    {
        free(table);
        return NULL;
    }

    return table;
}

void function_table_destroy(FunctionTable *table)
{
    if (!table)
        return;

    if (table->functions)
        list_destroy(table->functions);

    free(table);
}

bool function_table_exists(FunctionTable *table, const char *name)
{
    return function_table_search(table, name) != NULL;
}

TypeDescriptor *function_table_get_return_type(FunctionTable *table, const char *name, bool *found)
{
    FunctionEntry *entry = function_table_search(table, name);
    if (found)
        *found = (entry != NULL);

    if (!entry)
        return NULL;

    return entry->return_type;
}

List *function_table_get_params_types(FunctionTable *table, const char *name)
{
    FunctionEntry *entry = function_table_search(table, name);
    if (!entry)
        return NULL;

    return entry->param_types;
}

LLVMValueRef function_table_get_fn(FunctionTable *table, const char *name)
{
    FunctionEntry *entry = function_table_search(table, name);
    if (!entry)
        return NULL;

    return entry->fn;
}

LLVMTypeRef function_table_get_func_type(FunctionTable *table, const char *name)
{
    FunctionEntry *entry = function_table_search(table, name);
    if (!entry)
        return NULL;

    return entry->func_type;
}

void function_table_set_fn(FunctionTable *table, const char *name, LLVMValueRef fn)
{
    FunctionEntry *entry = function_table_search(table, name);
    if (!entry)
        return;

    entry->fn = fn;
}

void function_table_set_func_type(FunctionTable *table, const char *name, LLVMTypeRef func_type)
{
    FunctionEntry *entry = function_table_search(table, name);
    if (!entry)
        return;

    entry->func_type = func_type;
}

bool function_table_insert(FunctionTable *table, const char *name, TypeDescriptor *return_type, List *params_types)
{
    if (!table || !name  || !params_types)
        return false;

    if (function_table_exists(table, name))
    {
        list_destroy(params_types);
        return false;
    }

    FunctionEntry *entry = function_entry_create(name, return_type, params_types);
    if (!entry)
    {
        list_destroy(params_types);
        return false;
    }

    list_append(table->functions, entry);
    return true;
}

bool function_table_update(FunctionTable *table, const char *name, TypeDescriptor *new_return_type, List *new_params_types)
{
    if (!table || !name)
    {
        if (new_params_types)
            list_destroy(new_params_types);

        return false;
    }

    FunctionEntry *entry = function_table_search(table, name);
    if (!entry)
    {
        if (new_params_types)
            list_destroy(new_params_types);

        return false;
    }

    if (new_params_types)
    {
        if (entry->param_types)
            list_destroy(entry->param_types);

        entry->param_types = new_params_types;
    }

    if (new_return_type)
        entry->return_type = new_return_type;

    return true;
}