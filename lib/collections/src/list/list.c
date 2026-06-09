#include "list.h"

List* list_create(size_t size, void** elements, free_func free_function, cmp_func cmp_function, copy_func copy_function) {
    List* list = (List*)malloc(sizeof(List));

    if (list == NULL)
        raise_error("MemoryError", "list_create", "failed to allocate list");

    // Caso lista vacía
    if (size == 0) {
        list->vector = vector_create(0, NULL, free_function, cmp_function, copy_function);
        if (list->vector == NULL) {
            free(list);
            raise_error("MemoryError", "list_create", "failed to allocate vector for empty list");
        }
        list->count = 0;
        return list;
    }

    // Crear vector con capacidad inicial, pero solo 'size' elementos inicializados
    size_t initial_capacity = (size > INITIAL_LIST_CAPACITY) ? size : INITIAL_LIST_CAPACITY;
    
    // Crear array temporal con capacidad inicial, rellenando con NULLs
    void** temp_elements = (void**)calloc(initial_capacity, sizeof(void*));
    if (temp_elements == NULL) {
        free(list);
        raise_error("MemoryError", "list_create", "failed to allocate temporal elements");
    }
    
    // Copiar los elementos iniciales
    if (elements != NULL) {
        for (size_t i = 0; i < size; i++) {
            temp_elements[i] = elements[i];
        }
    }
    
    // Crear vector con capacidad preestablecida
    list->vector = vector_create(initial_capacity, temp_elements, free_function, cmp_function, copy_function);
    
    free(temp_elements);
    
    if (list->vector == NULL) {
        free(list);
        raise_error("MemoryError", "list_create", "failed to allocate vector for list");
    }
    list->count = size;
    return list;
}

void list_destroy(List* listx) {
    if (listx == NULL)
        raise_error("NullReferenceError", "list_destroy", "list cannot be null");

    vector_destroy(listx->vector);
    free(listx);
}

List *vector_to_list(Vector *vec) {
    
    if (!vec) 
        raise_error("NullReferenceError", "vector_to_list", "vector cannot be null");

    List *listx = malloc(sizeof(List));

    if (listx == NULL) 
        raise_error("MemoryError", "vector_to_list", "failed to allocate list");

    listx->vector = vec;
    listx->count = vec->size;

    return listx;
}

size_t list_count(const List *listx) {
    if (!listx)
        raise_error("NullReferenceError", "list_count", "list cannot be null");

    return listx->count;  
}

void* list_get(const List *listx, int index) {
    if (!listx)
        raise_error("NullReferenceError", "list_get", "list cannot be null");

    return vector_get(listx->vector, index);
}

void list_set(List *listx, int index, void *item) {
    if (!listx)
        raise_error("NullReferenceError", "list_set", "list cannot be null");

    vector_set(listx->vector, index, item);
}

int list_index_of(const List *listx, const void *item) {
    if (!listx)
        raise_error("NullReferenceError", "list_index_of", "list cannot be null");

    return vector_index_of(listx->vector, item);
}

bool list_contains(const List *listx, const void *item) {
    if (!listx)
        raise_error("NullReferenceError", "list_contains", "list cannot be null");

    return vector_contains(listx->vector, item);
}

void list_insert(List *listx, int index, void *item) {
    if (!listx)
        raise_error("NullReferenceError", "list_insert", "list cannot be null");
    
    size_t idx = resolve_index(index, listx->count + 1, "list_insert");
    
    // Expansión automática si está llena
    if (listx->count >= listx->vector->size) {
        size_t new_capacity = listx->vector->size * LIST_GROWTH_FACTOR;
        if (new_capacity == 0) {
            new_capacity = INITIAL_LIST_CAPACITY;
        }
        vector_resize(listx->vector, new_capacity);
    }
    
    // Desplazar elementos
    for (size_t i = listx->count; i > idx; i--) {
        listx->vector->data[i] = listx->vector->data[i - 1];
    }
    
    // Insertar nuevo elemento
    listx->vector->data[idx] = item;
    listx->count++;
}

void list_append(List *listx, void *item) {
    if (!listx)
        raise_error("NullReferenceError", "list_append", "list cannot be null");

    list_insert(listx, listx->count, item);
}

void *list_remove(List *listx, int index) {
    if (!listx)
        raise_error("NullReferenceError", "list_remove", "list cannot be null");

    size_t idx = resolve_index(index, listx->count, "list_remove");
    
    void *removed_item = listx->vector->data[idx];
    
    // Desplazar elementos para llenar el hueco
    if (idx < listx->count - 1) {
        memmove(&listx->vector->data[idx], &listx->vector->data[idx + 1], (listx->count - idx - 1) * sizeof(void*));
    }
    
    listx->count--;
    
    if(listx->count > 0)
        listx->vector->data[listx->count] = NULL;
        
    return removed_item;
}

void *list_pop(List *listx) {
    if (!listx)
        raise_error("NullReferenceError", "list_pop", "list cannot be null");

    return list_remove(listx, -1);
}

List* list_clone(const List* listx) 
{
    if (listx == NULL) 
        raise_error("NullReferenceError", "list_clone", "list cannot be null");
    
    List* clone = (List*)malloc(sizeof(List));
    if (clone == NULL) 
        raise_error("MemoryError", "list_clone", "failed to allocate list");
    
    clone->vector = vector_clone(listx->vector);
    clone->count = listx->count;
    
    return clone;
}

List* list_filter(const List* listx, filter_func filter, void* context) 
{
    if (listx == NULL) 
        raise_error("NullReferenceError", "list_filter", "list cannot be null");
    
    if (filter == NULL) 
        raise_error("InvalidArgumentError", "list_filter", "filter function cannot be null");
    
    // Crear lista vacía con las mismas funciones
    List* filtered_list = list_create(0, NULL, listx->vector->free_function, listx->vector->cmp_function, listx->vector->copy_function);
    
    if (filtered_list == NULL) 
        return NULL;
    
    // Recorrer y añadir elementos que pasan el filtro
    for (size_t i = 0; i < listx->count; i++) 
    {
        void* element = vector_get(listx->vector, i);
        
        if (filter(element, context)) 
        {
            // Si hay copy_function, hacer copia profunda
            if (listx->vector->copy_function != NULL && element != NULL) 
            {
                void* copy = listx->vector->copy_function(element);
                list_append(filtered_list, copy);
            } 
            else 
            {
                list_append(filtered_list, element);  // Copia superficial
            }
        }
    }
    
    return filtered_list;
}

/**
 * @brief Función de filtro que verifica si un elemento está en otra lista usand parámetros void*.
 */
static bool element_in_list(const void* element, void* context) 
{
    const List* other_list = (const List*)context;
    return list_contains(other_list, element);
}

List* list_intersection(const List* a, const List* b) {
    if (a == NULL || b == NULL) 
        raise_error("NullReferenceError", "list_intersection", "lists cannot be null");
    
    // Verificar que ambas listas tienen cmp_function
    if (a->vector->cmp_function == NULL || b->vector->cmp_function == NULL) 
        raise_error("InvalidOperationError", "list_intersection", 
                    "both lists must have a comparison function");
    
    // Filtrar a manteniendo elementos que están en b
    return list_filter(a, element_in_list, (void*)b);
}

List* list_union(const List* a, const List* b) 
{
    if (a == NULL || b == NULL) 
        raise_error("NullReferenceError", "list_union", "lists cannot be null");
    
    if (a->vector->cmp_function == NULL) 
        raise_error("InvalidOperationError", "list_union", "list 'a' must have a comparison function");
    
    // Empezar con clon de a
    List* result = list_clone(a);
    
    if (!result) 
        return NULL;
    
    // Añadir elementos de b que no estén ya en result
    for (size_t i = 0; i < b->count; i++) 
    {
        void* element = vector_get(b->vector, i);
        
        if (!list_contains(result, element)) 
        {
            if (b->vector->copy_function != NULL && element != NULL) 
            {
                void* copy = b->vector->copy_function(element);
                list_append(result, copy);
            } 
            
            else 
            {
                list_append(result, element);
            }
        }
    }
    
    return result;
}

void list_clear(List* listx)
{
    if (!listx)
        return;

    for (size_t i = 0; i < listx->count; i++)
    {
        vector_set(listx->vector, i, NULL);
    }

    listx->count = 0;
}