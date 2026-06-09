#ifndef HULK_COMMON_H
#define HULK_COMMON_H

/* Bibliotecas estándar */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>

/* Collections */
#include "collections.h"

/**
 * @brief Formatea un string usando formato printf con va_list.
 * 
 * @param format Formato del mensaje (estilo printf).
 * @param args   Lista de argumentos variables.
 * @return char* Puntero al string formateado (debe liberarse con free).
 *               Retorna NULL si falla la asignación de memoria.
 * 
 * @note El caller es responsable de liberar el string retornado.
 */
char* str_format(const char *format, va_list args);

/**
 * @brief Convierte una cadena a mayúsculas en el lugar.
 *
 * @param str Cadena terminada en '\0' que se modificará.
 */
char* str_toupper(char *str);

#endif