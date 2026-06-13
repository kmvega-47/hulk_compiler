#include "scope.h"

typedef struct
{
    char *name;
    TypeDescriptor *type;
    bool is_constant;
    bool is_parameter;
    bool is_self_instance;
    LLVMValueRef alloca;
    bool error_flag;
} SymbolEntry;

static SymbolEntry *symbol_entry_create(const char *name, TypeDescriptor *type, bool is_constant, bool is_parameter, bool is_self_instance)
{
    SymbolEntry *entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    if (!entry)
        return NULL;

    entry->name = strdup(name);
    if (!entry->name)
    {
        free(entry);
        return NULL;
    }

    entry->type = type;
    entry->is_constant = is_constant;
    entry->is_parameter = is_parameter;
    entry->is_self_instance = is_self_instance;
    entry->alloca = NULL;
    entry->error_flag = false;

    return entry;
}

static void free_symbol_entry(void *entry)
{
    SymbolEntry *sym_entry = (SymbolEntry *)entry;
    if (sym_entry)
    {
        free(sym_entry->name);
        free(sym_entry);
    }
}

static SymbolEntry *scope_lookup_local(Scope *scope, const char *name)
{
    if (!scope || !scope->symbols)
        return NULL;

    for (size_t i = 0; i < list_count(scope->symbols); i++)
    {
        SymbolEntry *entry = (SymbolEntry *)list_get(scope->symbols, i);
        if (strcmp(entry->name, name) == 0)
            return entry;
    }

    return NULL;
}

static SymbolEntry *scope_lookup_entry(Scope *scope, const char *name)
{
    while (scope)
    {
        SymbolEntry *entry = scope_lookup_local(scope, name);

        if (entry)
            return entry;

        scope = scope->parent;
    }

    return NULL;
}

static bool scope_add_entry(Scope *scope, const char *name, TypeDescriptor *type, bool is_constant, bool is_parameter, bool is_self_instance)
{
    if (!scope || !scope->symbols || !name)
        return false;

    if (scope_lookup_local(scope, name))
        return false;

    SymbolEntry *entry = symbol_entry_create(name, type, is_constant, is_parameter, is_self_instance);
    if (!entry)
        return false;

    list_append(scope->symbols, entry);
    return true;
}

Scope *scope_create(Scope *parent)
{
    Scope *scope = (Scope *)malloc(sizeof(Scope));
    if (!scope)
        return NULL;

    scope->parent = parent;
    scope->symbols = list_create(0, NULL, free_symbol_entry, NULL, NULL);
    if (!scope->symbols)
    {
        free(scope);
        return NULL;
    }

    return scope;
}

void scope_destroy(Scope *scope)
{
    if (!scope)
        return;

    if (scope->symbols)
        list_destroy(scope->symbols);

    free(scope);
}

bool scope_add_variable(Scope *scope, const char *name, TypeDescriptor *type)
{
    return scope_add_entry(scope, name, type, false, false, false);
}

bool scope_add_parameter(Scope *scope, const char *name, TypeDescriptor *type)
{
    return scope_add_entry(scope, name, type, false, true, false);
}

bool scope_add_constant(Scope *scope, const char *name, TypeDescriptor *type)
{
    return scope_add_entry(scope, name, type, true, false, false);
}

bool scope_add_self_instance(Scope *scope, TypeDescriptor *type)
{
    if (!scope || !scope->symbols)
        return false;

    return scope_add_entry(scope, "self", type, false, false, true);
}

TypeDescriptor *scope_lookup(Scope *scope, const char *name, bool *found)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    if (found)
        *found = (entry != NULL);

    return entry ? entry->type : NULL;
}

bool scope_is_constant(Scope *scope, const char *name)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    return entry ? entry->is_constant : false;
}

bool scope_is_parameter(Scope *scope, const char *name)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    return entry ? entry->is_parameter : false;
}

bool scope_is_self_instance(Scope *scope, const char *name)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    return entry ? entry->is_self_instance : false;
}

void scope_mark_variable_error(Scope *scope, const char *name)
{
    if (!scope || !name)
        return;

    SymbolEntry *entry = scope_lookup_local(scope, name);

    if (entry)
    {
        entry->error_flag = true;
    }
    
    else
    {
        SymbolEntry *new_entry = symbol_entry_create(name, NULL, false, false, false);
        if (new_entry)
        {
            new_entry->error_flag = true;
            list_append(scope->symbols, new_entry);
        }
    }
}

bool scope_is_error_flag(Scope *scope, const char *name)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    return entry ? entry->error_flag : false;
}


void scope_update_symbol(Scope *scope, const char *name, TypeDescriptor *type)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    if (entry)
        entry->type = type;
}

bool scope_lookup_current(Scope *scope, const char *name)
{
    return scope_lookup_local(scope, name) != NULL;
}

void scope_destroy_until(Scope *scope, Scope *target)
{
    while (scope != target && scope != NULL)
    {
        Scope *parent = scope->parent;
        scope_destroy(scope);
        scope = parent;
    }
}

LLVMValueRef scope_get_alloca(Scope *scope, const char *name)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    return entry ? entry->alloca : NULL;
}

void scope_set_alloca(Scope *scope, const char *name, LLVMValueRef alloca)
{
    SymbolEntry *entry = scope_lookup_entry(scope, name);
    if (entry)
        entry->alloca = alloca;
}

bool scope_add_variable_with_alloca(Scope *scope, const char *name, TypeDescriptor *type, LLVMValueRef alloca)
{
    if (!scope_add_variable(scope, name, type))
        return false;

    scope_set_alloca(scope, name, alloca);
    return true;
}