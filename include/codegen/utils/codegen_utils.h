#ifndef HULK_CODEGEN_UTILS_H
#define HULK_CODEGEN_UTILS_H

#include <llvm-c/Core.h>
#include "user_type_descriptor.h"
#include "scope.h"

/**
 * @brief Convierte un TypeDescriptor de HULK a su LLVMTypeRef correspondiente.
 *
 * @param context Contexto LLVM.
 * @param type    Tipo HULK.
 * @return Tipo LLVM equivalente.
 */
LLVMTypeRef code_gen_get_llvm_type(LLVMContextRef context, TypeDescriptor *type);

/**
 * @brief Registra todas las funciones built-in en el módulo LLVM y la tabla global.
 *
 * Declara en el módulo las funciones runtime y asigna sus LLVMValueRef y LLVMTypeRef
 * en la tabla global de funciones.
 *
 * @param module  Módulo LLVM donde declarar las funciones.
 * @param context Contexto LLVM.
 */
void code_gen_register_builtins(LLVMModuleRef module, LLVMContextRef context);

/**
 * @brief Resuelve el nombre real de una función para codegen.
 *
 * Para "print", despacha según el tipo del argumento a _print_number,
 * _print_string o _print_bool. Para otras funciones retorna el mismo nombre.
 *
 * @param name     Nombre de la función.
 * @param arg_type Tipo del primer argumento (puede ser NULL).
 * @return Nombre real de la función a llamar.
 */
const char *code_gen_resolve_function_name(const char *name, TypeDescriptor *arg_type);

/**
 * @brief Genera una llamada a función desde la tabla global.
 *
 * @param builder   Builder LLVM posicionado.
 * @param name      Nombre de la función.
 * @param args      Array de valores LLVM ya evaluados.
 * @param arg_count Número de argumentos.
 * @return Valor retornado por la función, o NULL si es void o no encontrada.
 */
LLVMValueRef code_gen_build_call(LLVMBuilderRef builder, const char *name, LLVMValueRef *args, size_t arg_count);

/**
 * @brief Construye una operación binaria en LLVM IR.
 *
 * @param builder   Builder LLVM posicionado.
 * @param op        Operador binario HULK.
 * @param left      Valor LLVM izquierdo.
 * @param right     Valor LLVM derecho.
 * @return Valor LLVM resultante.
 */
LLVMValueRef code_gen_build_binary_op(LLVMBuilderRef builder, HulkOperator op, LLVMValueRef left, LLVMValueRef right);

/**
 * @brief Construye una operación de comparación en LLVM IR.
 *
 * Solo se permiten comparaciones entre Number.
 *
 * @param builder   Builder LLVM posicionado.
 * @param op        Operador de comparación HULK.
 * @param left      Valor LLVM izquierdo.
 * @param right     Valor LLVM derecho.
 * @return Valor LLVM resultante (i1).
 */
LLVMValueRef code_gen_build_comparison(LLVMBuilderRef builder, HulkOperator op, LLVMValueRef left, LLVMValueRef right);

/**
 * @brief Obtiene un valor por defecto para un tipo HULK (0 para Number, false para Bool, "" para String, NULL para otros).
 *
 * @param context Contexto LLVM.
 * @param type    Tipo HULK.
 * @return Valor LLVM por defecto.
 */
LLVMValueRef code_gen_get_default_value(LLVMContextRef context, TypeDescriptor *type);

/**
 * @brief Construye un nodo PHI para unificar los resultados de dos ramas condicionales.
 *
 * @param builder    Builder LLVM posicionado en el bloque de merge.
 * @param context    Contexto LLVM.
 * @param hulk_type  Tipo HULK del resultado.
 * @param then_val   Valor de la rama then (puede ser NULL si es Void).
 * @param end_then   Bloque final de la rama then.
 * @param else_val   Valor de la rama else (puede ser NULL si es Void o no hay else).
 * @param end_else   Bloque final de la rama else (NULL si no hay else).
 * @param start_block Bloque anterior al if (para el caso sin else).
 * @param has_else   Si hay rama else.
 * @return Valor LLVM resultante, o NULL si es Void.
 */
LLVMValueRef code_gen_build_conditional_phi(LLVMBuilderRef builder, LLVMContextRef context, TypeDescriptor *hulk_type, LLVMValueRef then_val, LLVMBasicBlockRef end_then, LLVMValueRef else_val, LLVMBasicBlockRef end_else, LLVMBasicBlockRef start_block, bool has_else);

/**
 * @brief Construye un PHI para el valor de retorno de un bucle while.
 *
 * Inicializa el PHI con el valor por defecto del tipo desde el bloque de entrada.
 *
 * @param builder     Builder LLVM posicionado en el bloque de condición.
 * @param context     Contexto LLVM.
 * @param hulk_type   Tipo HULK del while.
 * @param start_block Bloque anterior al while (para el valor inicial).
 * @return PHI creado, o NULL si el tipo es Void.
 */
LLVMValueRef code_gen_build_while_phi(LLVMBuilderRef builder, LLVMContextRef context, TypeDescriptor *hulk_type, LLVMBasicBlockRef start_block);

/**
 * @brief Añade un valor entrante al PHI del while desde el final del cuerpo.
 *
 * @param phi      PHI del while.
 * @param body_val Valor generado por el cuerpo (NULL si no hay).
 * @param body_end Bloque final del cuerpo.
 */
void code_gen_add_while_phi_incoming(LLVMValueRef phi, LLVMValueRef body_val, LLVMBasicBlockRef body_end);

/**
 * @brief Registra las constantes globales PI y E en el scope actual.
 *
 * Crea alloca para PI y E, les asigna su valor y las inserta en el scope.
 *
 * @param scope   Scope donde registrar.
 * @param builder Builder LLVM posicionado al inicio de main.
 * @param context Contexto LLVM.
 */
void code_gen_register_constants(Scope *scope, LLVMBuilderRef builder, LLVMContextRef context);

/**
 * @brief Declara el prototipo LLVM de una función de usuario en el módulo
 *        y lo registra en la tabla global de funciones.
 *
 * @param module  Módulo LLVM.
 * @param context Contexto LLVM.
 * @param name    Nombre de la función.
 */
void code_gen_register_user_function(LLVMModuleRef module, LLVMContextRef context, const char *name);

/**
 * @brief Construye el LLVMStructType para un tipo definido por el usuario.
 *
 * Recorre los atributos del padre (herencia) y los propios, creando un struct
 * con todos los campos en orden: primero los del padre, luego los propios.
 *
 * @param context     Contexto LLVM.
 * @param user_type   Tipo definido por el usuario.
 * @return Struct type creado.
 */
LLVMTypeRef code_gen_build_struct_type(LLVMContextRef context, UserTypeDescriptor *user_type);

/**
 * @brief Obtiene el índice de un atributo en el struct LLVM del tipo.
 *
 * Busca el atributo en el propio tipo (no en ancestros). El índice es
 * el offset del tipo + la posición del atributo en la lista de atributos propios.
 *
 * @param user_type Tipo definido por el usuario.
 * @param attr_name Nombre del atributo.
 * @return Índice en el struct, o -1 si no existe.
 */
int code_gen_get_attribute_index(UserTypeDescriptor *user_type, const char *attr_name);

#endif