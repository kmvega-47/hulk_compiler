#include "symbol_binding.h"

SymbolBinding *symbol_binding_create(const char *name, const char *type_name, ASTNode *initializer, int line, int column)
{
    if (!name)
        return NULL;

    SymbolBinding *binding = (SymbolBinding *)malloc(sizeof(SymbolBinding));
    if (!binding)
        return NULL;

    binding->name = strdup(name);
    if (!binding->name)
    {
        free(binding);
        return NULL;
    }

    if (type_name)
    {
        binding->type_name = strdup(type_name);
        if (!binding->type_name)
        {
            free(binding->name);
            free(binding);
            return NULL;
        }
    }
    else
    {
        binding->type_name = NULL;
    }

    binding->initializer = initializer;
    binding->return_type = NULL;
    binding->line = line;
    binding->column = column;

    return binding;
}

void symbol_binding_destroy(SymbolBinding *binding)
{
    if (!binding)
        return;

    free(binding->name);
    free(binding->type_name);
    free(binding);
}

List *get_unannotated_params(List *bindings)
{
    List *names = list_create(0, NULL, NULL, NULL, NULL);
    if (!names)
        return NULL;

    for (size_t i = 0; i < list_count(bindings); i++)
    {
        SymbolBinding *sb = (SymbolBinding *)list_get(bindings, i);
        if (!sb->type_name)
            list_append(names, sb->name);
    }

    return names;
}

List *build_param_types(List *bindings, diagnostic_manager_t *dm)
{
    List *param_types = list_create(0, NULL, NULL, NULL, NULL);
    if (!param_types)
        return NULL;

    for (size_t i = 0; i < list_count(bindings); i++)
    {
        SymbolBinding *sb = (SymbolBinding *)list_get(bindings, i);
        TypeDescriptor *type = get_type_from_annotation(sb->type_name);

        if (!type && sb->type_name)
        {
            dm_add_error(dm, ERROR_TYPE_SEMANTIC, sb->line, sb->column, "Undefined type '%s'", sb->type_name);
        }

        list_append(param_types, type);
    }

    return param_types;
}