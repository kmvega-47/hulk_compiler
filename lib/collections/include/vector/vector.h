#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

typedef struct {
    void** data;
    size_t size;
    free_func free_function;
    cmp_func cmp_function; 
    copy_func copy_function;   
} Vector;

/**
 * @brief Crea un nuevo vector con un tamaño fijo.
 *
 * @param size Tamaño del vector (Debe ser mayor que 0).
 *
 * @param elements Array de punteros a elementos para inicializar el vector. Si es NULL, todas las posiciones se inicializan con NULL.
 *
 * @param free_function Función para liberar los elementos cuando se destruya el vector. Si es NULL, el usuario es responsable de liberar los elementos.
 * 
 * @param cmp_function Función para comparar dos elementos. Si es NULL, no se podrán realizar comparaciones.
 * 
 * @param copy_function Función para copiar un elemento. Necesaria si se desea realizar una copia profunda del vector.
 *
 * @return Vector* Puntero al nuevo vector creado.
 *
 * @note 
 *  - El vector copia los punteros del array elements, no toma posesión del array original, el cual debe ser liberado por el usuario.
 * 
 *  - Si se especifica free_function, el vector obtiene el ownership de los elementos y se encargará de liberarlos automáticamente al destruir el vector o al reemplazarlos con vector_set.
 * 
 *  - Si free_function es NULL, el vector no libera los elementos, y el usuario es responsable de gestionar la memoria de los mismos.
 * 
 *  - Si se especifica cmp_function, el vector podrá realizar operaciones de comparación entre elementos (como vector_index_of o vector_contains).
 * 
 *  - Si se especifica copy_function, el vector podrá realizar copias profundas de los elementos cuando sea necesario.
 */
Vector* vector_create(size_t size, void** elements, free_func free_function, cmp_func cmp_function, copy_func copy_function);

/**
 * @brief Libera el elemento del vector y asigna NULL en la posición especificada.
 *        Se permiten índices negativos para indexar desde el final.
 *
 * @param v Puntero al vector.
 * @param index Índice del elemento a destruir.
 * 
 * @note Si el vector no cuenta con función de liberación solamente se asigna NULL en la posición.
 *
 */
void vector_destroy_at(Vector *v, int index);

/**
 * @brief Libera un vector y todos sus elementos usando free_function.
 *
 * @param v Puntero al vector a destruir.
 * 
 * @note Si el vector no cuenta con función de liberación simplemente se libera la estructura.
 */
void vector_destroy(Vector *v);

/** 
 * @brief Obtiene el número de elementos en el vector.
 * @param v Puntero al vector.
 * @return size_t Número de elementos en el vector.
*/
size_t vector_length(const Vector *v);


/**
 * @brief Obtiene el elemento en la posición especificada.
 *
 * @param v Puntero al vector.
 * @param index Índice del elemento (soporta índices negativos para acceder desde el final).
 *
 * @return void* Elemento en la posición especificada, o NULL si no hay elemento.
 *
 * @note Si el índice está fuera de rango, la función lanza un error.
 */
void* vector_get(Vector *v, int index);

/** 
 * @brief Inserta un elemento en la posición especificada.
 * 
 * @param v Puntero al vector.
 * 
 * @param index Índice donde se insertará el elemento (soporta índices negativos para acceder desde el final).
 * 
 * @param element Puntero al elemento a insertar.
 * 
 * @return void* Elemento almacenado en la posición, o NULL si no había elemento.
 * 
 * @note 
 *  - Si el índice está fuera de rango, la función lanza un error.
 * 
 *  - Si se especificó función de liberación, el elemento que se encuentre en la posición será liberado automáticamente antes de ser reemplazado por el nuevo elemento.
 */
void vector_set(Vector *v, int index, void *element);

/**
 * @brief Obtiene el índice del primer elemento que coincide con el elemento dado, utilizando la función de comparación cmp_function.
 * @param v Puntero al vector.
 * @param element Puntero al elemento a buscar.
 * @return int Índice del primer elemento que coincide, o -1 si no se encuentra.
 * @note Si el vector no cuenta con función de comparación, esta función no estará disponible y lanzará un error si se intenta usarla.
 */
int vector_index_of(const Vector *v, const void *element);

/**
 * @brief Verifica si el vector contiene un elemento específico utilizando la función de comparación cmp_function.
 * @param v Puntero al vector.
 * @param element Puntero al elemento a buscar.
 * @return bool true si el elemento se encuentra en el vector, false en caso contrario.
 * @note Si el vector no cuenta con función de comparación, esta función no estará disponible y lanzará un error si se intenta usarla.
 */
bool vector_contains(const Vector *v, const void *element);

/*  
* @brief Crea una copia profunda de un vector utilizando la función de copia copy_function para duplicar cada elemento.
 * @param v Puntero al vector a clonar.
 * @return Vector* Puntero al nuevo vector clonado.
 * @note 
 *  - Si el vector no cuenta con función de copia, esta función no estará disponible y lanzará un error si se intenta usarla.
 *  - El nuevo vector tendrá su propia copia de los elementos, por lo que las modificaciones en el vector original no afectarán al vector clonado, y viceversa.
*/
Vector *vector_clone(const Vector *v);

/** 
 * @brief Modifica el tamaño de un vector.
 * @param v Puntero al vector a redimensionar.
 * @param new_size Nuevo tamaño del vector.
 * @note Si el nuevo tamaño es menor que el tamaño actual, los elementos excedentes serán destruidos.
 */
void vector_resize(Vector *v, size_t new_size);

#endif // VECTOR_H