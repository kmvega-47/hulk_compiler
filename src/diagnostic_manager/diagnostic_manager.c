#include "diagnostic_manager.h"

/**
 * @brief Representa un error del compilador HULK (interno).
 *
 * Campos:
 * 
 *   - type:    Tipo de error (léxico, sintáctico o semántico).
 * 
 *   - message: Mensaje descriptivo del error.
 * 
 *   - line:    Número de línea donde ocurrió el error (0 si no aplica).
 * 
 *   - column:  Número de columna donde ocurrió el error (0 si no aplica).
 * 
 */
typedef struct
{
    error_type_t type;
    char *message;
    int line;
    int column;
} hulk_error_t;

diagnostic_manager_t *dm_global = NULL;

static void error_destroy(hulk_error_t *error)
{
    if (!error)
        return;

    free(error->message);
    free(error);
}

static void error_list_free(void *error)
{
    error_destroy((hulk_error_t *)error);
}

static hulk_error_t *error_create(error_type_t type, int line, int column, const char *format, ...)
    __attribute__((format(printf, 4, 5)));

static hulk_error_t *error_create(error_type_t type, int line, int column, const char *format, ...)
{
    hulk_error_t *error = (hulk_error_t *)malloc(sizeof(hulk_error_t));
    if (!error)
        return NULL;

    error->type = type;
    error->line = line;
    error->column = column;

    va_list args;
    va_start(args, format);
    error->message = str_format(format, args);
    va_end(args);

    if (!error->message)
    {
        free(error);
        return NULL;
    }

    return error;
}

diagnostic_manager_t *dm_create(void)
{
    diagnostic_manager_t *dm = (diagnostic_manager_t *)malloc(sizeof(diagnostic_manager_t));
    if (!dm)
        return NULL;

    dm->errors = list_create(0, NULL, error_list_free, NULL, NULL);
    if (!dm->errors)
    {
        free(dm);
        return NULL;
    }

    return dm;
}

void dm_destroy(diagnostic_manager_t *dm)
{
    if (!dm)
        return;

    if (dm->errors)
        list_destroy(dm->errors);

    free(dm);
}

void dm_add_error(diagnostic_manager_t *dm, error_type_t type, int line, int column, const char *format, ...)
{
    if (!dm || !dm->errors || !format)
        return;

    va_list args;
    va_start(args, format);
    char *message = str_format(format, args);
    va_end(args);

    if (!message)
        return;

    hulk_error_t *error = error_create(type, line, column, "%s", message);
    free(message);

    if (error)
        list_append(dm->errors, error);
}

bool dm_has_errors(const diagnostic_manager_t *dm)
{
    return dm && dm->errors && list_count(dm->errors) > 0;
}

size_t dm_error_count(const diagnostic_manager_t *dm)
{
    return (dm && dm->errors) ? list_count(dm->errors) : 0;
}

void dm_print_errors(const diagnostic_manager_t *dm)
{
    if (!dm_has_errors(dm))
        return;

    size_t count = dm_error_count(dm);

    for (size_t i = 0; i < count; i++)
    {
        hulk_error_t *err = (hulk_error_t *)list_get(dm->errors, i);
        fprintf(stderr, "(%d,%d) %s: %s\n", err->line, err->column, error_type_to_string(err->type), err->message);
    }
}