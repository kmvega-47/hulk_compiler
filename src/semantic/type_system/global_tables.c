#include "global_tables.h"

TypeTable *global_type_table = NULL;
FunctionTable *global_function_table = NULL;

static void register_builtin_unary(const char *name, TypeDescriptor *return_type, TypeDescriptor *param_type)
{
    List *params = list_create(1, (void *[]){param_type}, NULL, NULL, NULL);
    function_table_insert(global_function_table, name, return_type, params);
}

static void register_builtin_functions(void)
{
    TypeDescriptor *void_type = type_table_lookup_by_tag(global_type_table, HULK_VOID);
    TypeDescriptor *num_type  = type_table_lookup_by_tag(global_type_table, HULK_NUMBER);
    TypeDescriptor *obj_type  = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);

    register_builtin_unary("print", void_type, obj_type);
    register_builtin_unary("sqrt", num_type, num_type);
    register_builtin_unary("sin", num_type, num_type);
    register_builtin_unary("cos", num_type, num_type);
    register_builtin_unary("exp", num_type, num_type);

    List *log_params = list_create(2, (void *[]){num_type, num_type}, NULL, NULL, NULL);
    function_table_insert(global_function_table, "log", num_type, log_params);

    List *rand_params = list_create(0, NULL, NULL, NULL, NULL);
    function_table_insert(global_function_table, "rand", num_type, rand_params);
}

void global_tables_init(void)
{
    global_type_table = type_table_create();
    global_function_table = function_table_create();

    register_builtin_functions();
}

void global_tables_destroy(void)
{
    if (global_function_table)
    {
        function_table_destroy(global_function_table);
        global_function_table = NULL;
    }

    if (global_type_table)
    {
        type_table_destroy(global_type_table);
        global_type_table = NULL;
    }
}

TypeDescriptor *get_type_from_annotation(const char *type_name)
{
    if (!type_name)
        return NULL;

    return type_table_lookup_by_name(global_type_table, type_name);
}