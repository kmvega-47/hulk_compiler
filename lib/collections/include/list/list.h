#ifndef LIST_H
#define LIST_H

#include "vector.h"

// Constantes de configuración
#define INITIAL_LIST_CAPACITY 32     // Capacidad inicial por defecto
#define LIST_GROWTH_FACTOR 2         // Factor de crecimiento

typedef struct List {
    Vector* vector;
    size_t count;
} List;

/** 
 * @brief Crea una nueva lista.
 * 
 * @param size Número inicial de elementos en la lista.
 * @param elements Array de punteros a elementos para inicializar la lista. Si es NULL, la lista se inicializa vacía.
 * @param free_function Función para liberar los elementos cuando se destruya la lista. Si es NULL, el usuario es responsable de liberar los elementos.
 * @param cmp_function Función para comparar dos elementos. Si es NULL, no se podrán realizar comparaciones.
 * @param copy_function Función para copiar un elemento. Necesaria si se desea realizar una copia profunda de la lista.
 * 
 * @return List* Puntero a la nueva lista creada. 
 */
List* list_create(size_t size, void** elements, free_func free_function, cmp_func cmp_function, copy_func copy_function);

/**
* @brief Libera una lista y todos sus elementos usando free_function.
* @param listx Puntero a la lista a destruir.
* @note Si la lista no cuenta con función de liberación simplemente se libera la estructura.
*/
void list_destroy(List* listx);

/**
* @brief Convierte un vector en una lista
*
 * @param vec Vector a convertir
 * @return List* Nueva lista que contiene el vector
 * 
 * @note La lista toma posesión del vector
 * @note Se asume que todos los slots del vector contienen elementos válidos
*/
List *vector_to_list(Vector* vec);

/**
 * @brief Obtiene el número de elementos en la lista
 * 
 * @param listx Lista a consultar
 * @return size_t Número de elementos
 */
size_t list_count(const List *listx);

/**
 * @brief Obtiene el elemento en la posición especificada
 * 
 * @param listx Lista de donde obtener el elemento
 * @param index Índice del elemento (negativo para índices desde el final)
 * @return void* Elemento en la posición especificada, NULL si índice inválido
 */
void* list_get(const List *listx, int index);

/**
 * @brief Establece un elemento en la posición especificada
 * 
 * @param listx Lista donde establecer el elemento
 * @param index Índice de la posición
 * @param item Nuevo elemento a establecer
 * 
 * @note Libera el elemento anterior si free_func está definida
 */
void list_set(List *listx, int index, void *item);

/**
 * @brief Busca un elemento en la lista y devuelve su índice
 * 
 * @param listx Lista donde buscar
 * @param item Elemento a encontrar
 * @return int Índice del elemento, -1 si no se encuentra
 * 
 * @note Requiere que cmp_func esté definida
 */
int list_index_of(const List *listx, const void *item);

/**
 * @brief Verifica si un elemento está presente en una lista
 * 
 * @param listx Lista donde buscar
 * @param item Elemento a encontrar
 * 
 * @return 
 * - true : Si el elemento está en la lista
 * 
 * - false : Si el elemento no está en la lista
 * 
 * @note Requiere que cmp_func esté definida
 */
bool list_contains(const List *listx, const void *item);

/**
 * @brief Inserta un elemento en la posición especificada
 * 
 * @param listx Lista donde insertar
 * @param index Posición de inserción
 * @param item Elemento a insertar
 * 
 * @note Expande automáticamente la capacidad si es necesario
 * @note Los elementos se desplazan para hacer espacio
 * @note Para añadir al final, usar list_append()
 */
void list_insert(List *listx, int index, void *item);

/**
 * @brief Añade un elemento al final de la lista
 * 
 * @param listx Lista donde añadir
 * @param item Elemento a añadir
 * 
 * @note Equivale a list_insert(listx, list_count(listx), item)
 */
void list_append(List *listx, void *item);

/**
 * @brief Elimina y retorna el elemento en la posición especificada
 * 
 * @param listx Lista de donde eliminar
 * @param index Índice del elemento a eliminar
 * @return void* Elemento eliminado, NULL si error
 * 
 * @note El caller es responsable de liberar el elemento retornado
 */
void *list_remove(List *listx, int index);

/**
 * @brief Elimina y retorna el último elemento de la lista
 * 
 * @param listx Lista de donde eliminar
 * @return void* Último elemento eliminado
 * 
 * @note Equivale a list_remove(listx, -1)
 */
void *list_pop(List *listx);

/**
 * @brief Crea una copia de una lista.
 * 
 * @param listx Lista a clonar.
 * @return List* Puntero a la nueva lista clonada.
 * 
 * @note 
 *  - Si la lista cuenta con función de copia (`copy_function`), se realiza una copia profunda
 *    de cada elemento. El nuevo vector tendrá su propia copia de los elementos, por lo que
 *    las modificaciones en los elementos de la lista original no afectarán a la lista clonada,
 *    y viceversa.
 * 
 *  - Si la lista NO cuenta con función de copia, se realiza una copia superficial (se copian
 *    los punteros). En este caso, los elementos son compartidos entre ambas listas.
 * 
 *  - La nueva lista hereda las funciones `free_function`, `cmp_function` y `copy_function`
 *    de la lista original.
 */
List* list_clone(const List* listx);

/**
 * @brief Filtra los elementos de una lista según un predicado.
 * 
 * @param listx   Lista a filtrar (no se modifica).
 * @param filter  Función predicado que recibe un elemento y context, 
 *                retorna true si el elemento debe incluirse.
 * @param context Puntero opaco con datos adicionales para el filtro (puede ser NULL).
 * 
 * @return List* Nueva lista con los elementos que pasaron el filtro,
 *               o NULL si falla la asignación de memoria.
 * 
 * @note 
 *  - La nueva lista conserva el orden de los elementos originales.
 *  - Si la lista original cuenta con copy_function, se realiza copia profunda
 *    de los elementos filtrados.
 *  - Si no cuenta con copy_function, se realiza copia superficial.
 *  - La nueva lista hereda free_function, cmp_function y copy_function de la original.
 */
List* list_filter(const List* listx, filter_func filter, void* context);

/**
 * @brief Crea una nueva lista con la intersección de dos listas.
 * 
 * @param a Primera lista.
 * @param b Segunda lista.
 * 
 * @return List* Nueva lista con los elementos de a que también están en b,
 *               o NULL si falla la asignación de memoria.
 * 
 * @note 
 *  - Ambas listas deben tener una función de comparación (`cmp_function`).
 *  - La nueva lista conserva el orden de los elementos de la lista a.
 *  - La nueva lista hereda las funciones de la lista a (free, cmp, copy).
 *  - La copia de elementos sigue la misma semántica que list_filter.
 * 
 * @warning Si alguna lista no tiene cmp_function, se lanza un error.
 */
List* list_intersection(const List* a, const List* b);

/**
 * @brief Crea una nueva lista con la unión de dos listas (sin duplicados).
 * 
 * @param a Primera lista.
 * @param b Segunda lista.
 * 
 * @return List* Nueva lista con todos los elementos de a y los elementos de b 
 *               que no están en a, sin duplicados.
 * 
 * @note 
 *  - La lista 'a' debe tener una función de comparación (`cmp_function`).
 *  - La nueva lista conserva el orden: primero todos los elementos de a,
 *    luego los elementos de b que no estaban en a (en su orden original).
 *  - La nueva lista hereda las funciones de la lista a (free, cmp, copy).
 *  - La copia de elementos sigue la misma semántica que list_clone y list_filter.
 */
List* list_union(const List* a, const List* b);

/**
 * @brief Elimina todos los elementos de la lista sin destruirla.
 *
 * @param listx Lista a limpiar.
 *
 * @note Si la lista tiene free_func, los elementos se liberan.
 */
void list_clear(List* listx);

#endif // LIST_H