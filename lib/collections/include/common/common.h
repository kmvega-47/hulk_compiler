#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> 
#include <stdbool.h>
#include <string.h> 

/**
 * @brief Lanza un error fatal y termina la ejecución del programa.
 *
 * @param error_name Nombre del tipo de error.
 * @param enviroment Entorno donde ocurrió el error.
 * @param msg Mensaje de error a mostrar.
 */
void raise_error(const char *error_name, const char *enviroment, const char *msg);

/**
 * @brief Callback para liberar un elemento.
 *
 * - Se utiliza cuando la estructura es dueña del elemento.
 */
typedef void (*free_func)(void *elem);

/**
 * @brief Callback para comparar dos elementos.`
 */
typedef int (*cmp_func)(const void *a, const void *b);

/**
 * @brief Callback para copiar un elemento.
 */
typedef void* (*copy_func)(const void *elem);

/**
 * @brief Función de filtrado para list_filter.
 * 
 * @param element Elemento a evaluar.
 * @param context Contexto adicional pasado a list_filter.
 * @return bool true si el elemento debe mantenerse, false si debe eliminarse.
 */
typedef bool (*filter_func)(const void* element, void* context);

/**
 * @brief Resuelve correctamente un índice(incluído valores negativos) en una estructura de un length dado
 * 
 * @param index Índice a resolver
 * 
 * @param len Longitud de la estructura
 * 
 * @param caller_name Nombre de la función que invoca al método
 * 
 * @note Lanza error si el length de la estrutura es 0 o si el índice queda fuera de rango
**/
size_t resolve_index(int index, size_t len, char* caller_name);

#endif // COMMON_H