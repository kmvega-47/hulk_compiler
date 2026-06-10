#ifndef HULK_USER_TYPE_DESCRIPTOR_H
#define HULK_USER_TYPE_DESCRIPTOR_H

#include "type_descriptor.h"
#include <llvm-c/Core.h>

/**
 * @brief Descriptor para tipos definidos por el usuario.
 *
 * Extiende TypeDescriptor para representar tipos definidos en HULK.
 * Incluye campos necesarios tanto para el análisis semántico como para
 * la generación de código LLVM.
 *
 * Campos:
 *   - base:              TypeDescriptor base (tag = HULK_USER_DEFINED).
 *
 *   - param_types:       Lista de TypeDescriptor* con los tipos de los
 *                        parámetros de inicialización.
 *
 *   - attribute_names:   Lista de strings con los nombres de los atributos.
 *
 *   - attribute_types:   Lista de TypeDescriptor* con los tipos de los atributos.
 *
 *   - method_names:      Lista de strings con los nombres de los métodos.
 *
 *   - struct_type:       Tipo struct de LLVM para este tipo.
 *
 *   - constructor:       Función constructora LLVM para este tipo.
 *
 *   - constructor_type:  Tipo de la función constructora LLVM.
 *
 *   - offset:            Offset en la tabla de métodos virtuales.
 *                        Calculado como: 0 si el padre no es UserTypeDescriptor,
 *                        offset del padre + cantidad de atributos del padre si lo es.
 */
typedef struct UserTypeDescriptor
{
    TypeDescriptor base;
    List *param_types;
    List *attribute_names;
    List *attribute_types;
    List *method_names;
    LLVMTypeRef struct_type;
    LLVMValueRef constructor;
    LLVMTypeRef constructor_type;
    int offset;
} UserTypeDescriptor;

/**
 * @brief Crea un nuevo descriptor de tipo definido por el usuario.
 *
 * @param name        Nombre del tipo (se copia internamente y se pasa a mayúsculas).
 * @param parent      Tipo del que hereda (NULL para Object).
 * @param param_types Lista de TypeDescriptor* con los tipos de los parámetros de
 *                    inicialización. Toma ownership. No puede ser NULL.
 * @return Puntero al descriptor creado, o NULL si falla la asignación.
 */
UserTypeDescriptor *user_type_create(const char *name, TypeDescriptor *parent, List *param_types);

/**
 * @brief Destruye un descriptor de tipo definido por el usuario.
 *
 * @param type Puntero al descriptor a destruir. Si es NULL, no hace nada.
 */
void user_type_destroy(UserTypeDescriptor *type);

/**
 * @brief Convierte un TypeDescriptor a UserTypeDescriptor de forma segura.
 *
 * @param type TypeDescriptor a convertir.
 * @return Puntero a UserTypeDescriptor si el tag es HULK_USER_DEFINED, NULL en caso contrario.
 */
UserTypeDescriptor *type_to_user_defined(TypeDescriptor *type);

/**
 * @brief Obtiene los tipos de los parámetros de inicialización.
 *
 * @param type Descriptor de tipo.
 * @return Lista de TypeDescriptor* (no toma ownership).
 */
List *user_type_get_param_types(const UserTypeDescriptor *type);

/**
 * @brief Actualiza el tipo de uno de los parámetros de inicialización.
 *
 * @param type           Tipo definido por el usuario.
 * @param param_idx      Índice del parámetro a actualizar.
 * @param new_param_type Nuevo tipo a asignar al parámetro.
 */
void user_type_update_param_type(UserTypeDescriptor *type, int param_idx, TypeDescriptor *new_param_type);

/**
 * @brief Busca un atributo en el tipo definido por el usuario y retorna su tipo.
 *
 * @param type Descriptor de tipo.
 * @param name Nombre del atributo.
 * @return Tipo del atributo si existe, NULL en caso contrario.
 */
TypeDescriptor *user_type_lookup_attribute(const UserTypeDescriptor *type, const char *name);

/**
 * @brief Agrega un atributo al tipo definido por el usuario.
 *
 * @param type      Descriptor de tipo.
 * @param name      Nombre del atributo.
 * @param attr_type Tipo del atributo.
 */
void user_type_add_attribute(UserTypeDescriptor *type, const char *name, TypeDescriptor *attr_type);

/**
 * @brief Actualiza el tipo de un atributo existente.
 *
 * Si el atributo no existe, lo agrega.
 *
 * @param type      Descriptor del tipo.
 * @param name      Nombre del atributo.
 * @param attr_type Nuevo tipo del atributo.
 */
void user_type_update_attribute(UserTypeDescriptor *type, const char *name, TypeDescriptor *attr_type);

/**
 * @brief Verifica si un método existe en el tipo (busca en ancestros).
 *
 * @param type Descriptor del tipo.
 * @param name Nombre del método.
 * @return true si el método existe, false en caso contrario.
 */
bool user_type_has_method(const UserTypeDescriptor *type, const char *name);

/**
 * @brief Agrega un método a la lista de métodos del tipo.
 *
 * @param type Descriptor del tipo.
 * @param name Nombre del método.
 */
void user_type_add_method(UserTypeDescriptor *type, const char *name);

/**
 * @brief Busca el ancestro más cercano que tenga un método con el nombre dado.
 *
 * @param type        Tipo desde donde iniciar la búsqueda (se excluye a sí mismo).
 * @param method_name Nombre del método a buscar.
 * @return UserTypeDescriptor del ancestro que tiene el método, o NULL si no se encuentra.
 */
UserTypeDescriptor *user_type_find_ancestor_with_method(const UserTypeDescriptor *type, const char *method_name);

#endif