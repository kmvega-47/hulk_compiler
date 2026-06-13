#ifndef HULK_CODE_GEN_VISITOR_H
#define HULK_CODE_GEN_VISITOR_H

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include "visitor.h"
#include "codegen_utils.h"

/**
 * @brief Visitor para generación de código LLVM.
 *
 * Recorre el AST y genera el IR de LLVM correspondiente.
 *
 * Campos:
 *   - base:              Visitor base.
 *   - context:           Contexto LLVM.
 *   - module:            Módulo LLVM donde se genera el código.
 *   - builder:           Builder LLVM para generar instrucciones.
 *   - current_scope:     Scope actual durante la generación de código.
 *   - current_self_type: Tipo definido por el usuario actual (dentro de un método), o NULL.
 */
typedef struct CodeGenVisitor
{
    Visitor base;
    LLVMContextRef context;
    LLVMModuleRef module;
    LLVMBuilderRef builder;
    Scope *current_scope;
    UserTypeDescriptor *current_self_type;
    List *allocated_ptrs;
} CodeGenVisitor;

/**
 * @brief Crea un nuevo visitor de generación de código.
 *
 * @param module_name Nombre del módulo LLVM.
 * @return Puntero al visitor creado, o NULL si falla la asignación.
 */
CodeGenVisitor *code_gen_visitor_create(const char *module_name);

/**
 * @brief Destruye el visitor de generación de código.
 *
 * @param visitor Puntero al visitor a destruir.
 */
void code_gen_visitor_destroy(CodeGenVisitor *visitor);

/**
 * @brief Escribe el IR generado a un archivo .ll y compila a ejecutable.
 *
 * @param visitor  CodeGenVisitor con el módulo generado.
 * @param filename Nombre del archivo de salida (sin extensión).
 */
void code_gen_visitor_compile(CodeGenVisitor *visitor, const char *filename);

#endif