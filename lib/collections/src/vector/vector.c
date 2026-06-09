#include "vector.h"

Vector* vector_create(size_t size, void** elements, free_func free_function, cmp_func cmp_function, copy_func copy_function)
{
    Vector* v = (Vector*)malloc(sizeof(Vector));
    if (v == NULL) raise_error("MemoryError", "vector_create", "failed to allocate vector");
    
    v->size = size;
    v->free_function = free_function;
    v->cmp_function = cmp_function;
    v->copy_function = copy_function;
    
    // Manejar size = 0
    if (size == 0) {
        v->data = NULL;
        return v;
    }
    
    // Para size > 0
    v->data = (void**)malloc(size * sizeof(void*));
    if (v->data == NULL) {
        free(v);
        raise_error("MemoryError", "vector_create", "failed to allocate data array");
    }
    
    if (elements != NULL) {
        for (size_t i = 0; i < size; i++) {
            v->data[i] = elements[i];
        }
    } else {
        for (size_t i = 0; i < size; i++) {
            v->data[i] = NULL;
        }
    }
    
    return v;
}

void vector_destroy_at(Vector *v, int index) 
{
    if (v == NULL)
        raise_error("NullReferenceError", "vector_destroy_at", "vector cannot be null.");

    size_t idx = resolve_index(index, v->size, "vector_destroy_at");

    // Si se especificó free_function se libera el elemento.
    if (v->free_function && v->data[idx]) 
    {
        v->free_function(v->data[idx]);
    }

    // Setea el elemento en NULL para evitar danglings pointers.
    v->data[idx] = NULL;
}

void vector_destroy(Vector *v) 
{
    if (v == NULL) return;

    // Liberar elementos
    if (v->free_function)
    {
        for (size_t i = 0; i < v->size; i++)
        {
            vector_destroy_at(v, i);
        }
    }

    // Liberar estructura
    free(v->data);
    free(v);  
}

size_t vector_length(const Vector *v) 
{
    if (v == NULL) 
        raise_error("NullReferenceError", "vector_length", "vector cannot be null");
    
    return v->size;
}

void* vector_get(Vector *v, int index) 
{
    if (v == NULL) {
        raise_error("NullReferenceError", "vector_get", "vector cannot be null");
        return NULL;
    }

    size_t idx = resolve_index(index, v->size, "vector_get");
    return v->data[idx];
}

void vector_set(Vector *v, int index, void *element) 
{
    if (v == NULL) {
        raise_error("NullReferenceError", "vector_set", "vector cannot be null");
    }

    size_t idx = resolve_index(index, v->size, "vector_set");

    // Liberar el elemento actual si se especificó free_function.
    vector_destroy_at(v, index);

    v->data[idx] = element;
}

int vector_index_of(const Vector *v, const void *element) 
{
    if (v == NULL) 
        raise_error("NullReferenceError", "vector_index_of", "vector cannot be null");
    
    if (v->cmp_function == NULL) 
        raise_error("InvalidOperationError", "vector_index_of", "vector does not have a comparison function");

    for (size_t i = 0; i < v->size; i++) 
    {
        if (v->data[i] != NULL && v->cmp_function(v->data[i], element) == 0) 
        {
            return (int)i;
        }
    }

    return -1; // No se encontró el elemento
}

bool vector_contains(const Vector *v, const void *element) 
{
    if (v == NULL) 
        raise_error("NullReferenceError", "vector_contains", "vector cannot be null");
    
    if (v->cmp_function == NULL) 
        raise_error("InvalidOperationError", "vector_contains", "vector does not have a comparison function");

    return vector_index_of(v, element) != -1;
}

Vector* vector_clone(const Vector *v) 
{
    if (v == NULL) 
        raise_error("NullReferenceError", "vector_clone", "vector cannot be null");

    Vector* clone = vector_create(v->size, NULL, v->free_function, v->cmp_function, v->copy_function);

    for (size_t i = 0; i < v->size; i++) 
    {
        if (v->data[i] != NULL) 
        {
            if (v->copy_function != NULL)
                clone->data[i] = v->copy_function(v->data[i]);
            else
                clone->data[i] = v->data[i];  // copia superficial
        }
    }

    return clone;
}

void vector_resize(Vector *v, size_t new_size) 
{
    if (v == NULL) 
        raise_error("NullReferenceError", "vector_resize", "vector cannot be null");
    
    if (new_size == 0) {
        fprintf(stderr, "WARNING: vector_resize called with new_size=0 from %p\n", __builtin_return_address(0));
        // Liberar todos los elementos si hay función de liberación
        if (v->free_function) {
            for (size_t i = 0; i < v->size; i++) {
                if (v->data[i]) {
                    v->free_function(v->data[i]);
                }
            }
        }
        // Liberar el array de datos
        if (v->data) {
            free(v->data);
            v->data = NULL;
        }
        v->size = 0;
        return;
    }

    // Si el nuevo tamaño es menor, destruir los elementos excedentes
    if (new_size < v->size && v->free_function) 
    {
        for (size_t i = new_size; i < v->size; i++) 
        {
            vector_destroy_at(v, i);
        }
    }

    void** new_data = (void**)realloc(v->data, new_size * sizeof(void*));
    
    if (new_data == NULL) 
        raise_error("MemoryError", "vector_resize", "failed to reallocate data array");

    // Si el nuevo tamaño es mayor, inicializar los nuevos elementos en NULL
    if (new_size > v->size) 
    {
        for (size_t i = v->size; i < new_size; i++) 
        {
            new_data[i] = NULL;
        }
    }

    v->data = new_data;
    v->size = new_size;
}