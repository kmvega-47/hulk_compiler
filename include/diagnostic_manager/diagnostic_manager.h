#ifndef HULK_DIAGNOSTIC_MANAGER_H
#define HULK_DIAGNOSTIC_MANAGER_H

#include "hulk_common.h"
#include "enums.h"

/**
 * @brief Gestor de diagnósticos del compilador HULK.
 *
 * Almacena los errores detectados durante la compilación.
 *
 * Campos:
 *   - errors: Lista de errores registrados (internos).
 */
typedef struct
{
    List *errors;
} diagnostic_manager_t;

/** @brief Gestor de diagnósticos global. */
extern diagnostic_manager_t *dm_global;

/**
 * @brief Crea un gestor de diagnósticos.
 *
 * @return Puntero al gestor creado, o NULL si falla la asignación.
 */
diagnostic_manager_t *dm_create(void);

/**
 * @brief Destruye un gestor de diagnósticos y libera toda su memoria.
 *
 * @param dm Puntero al gestor a destruir.
 */
void dm_destroy(diagnostic_manager_t *dm);

/**
 * @brief Añade un error al gestor de diagnósticos.
 *
 * @param dm     Gestor de diagnósticos.
 * @param type   Tipo de error.
 * @param line   Número de línea (0 si no aplica).
 * @param column Número de columna (0 si no aplica).
 * @param format Formato del mensaje (estilo printf).
 * @param ...    Argumentos para el formato.
 */
void dm_add_error(diagnostic_manager_t *dm, error_type_t type, int line, int column, const char *format, ...)
    __attribute__((format(printf, 5, 6)));

/**
 * @brief Verifica si hay errores registrados.
 *
 * @param dm Gestor de diagnósticos.
 * @return true si hay al menos un error, false en caso contrario.
 */
bool dm_has_errors(const diagnostic_manager_t *dm);

/**
 * @brief Obtiene la cantidad de errores registrados.
 *
 * @param dm Gestor de diagnósticos.
 * @return Número de errores.
 */
size_t dm_error_count(const diagnostic_manager_t *dm);

/**
 * @brief Imprime todos los errores registrados por stderr.
 *
 * Formato: (line,col) TYPE: message
 *
 * @param dm Gestor de diagnósticos.
 */
void dm_print_errors(const diagnostic_manager_t *dm);

#endif