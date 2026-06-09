#include "hulk_common.h"
#include "diagnostic_manager.h"
#include "global_tables.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <file.hulk>\n", argv[0]);
        return 1;
    }

    dm_global = dm_create();
    if (!dm_global)
    {
        fprintf(stderr, "Error: no se pudo inicializar el gestor de diagnosticos\n");
        return 1;
    }

    global_tables_init();
    if (!global_type_table || !global_function_table)
    {
        fprintf(stderr, "Error: no se pudieron inicializar las tablas globales\n");
        dm_destroy(dm_global);
        return 1;
    }

    /* TODO: compilar argv[1] */

    dm_print_errors(dm_global);

    int exit_code = 0;
    if (dm_has_errors(dm_global))
        exit_code = 1;

    global_tables_destroy();
    dm_destroy(dm_global);

    return exit_code;
}