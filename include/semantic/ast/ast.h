#ifndef HULK_AST_H
#define HULK_AST_H

#include "global_tables.h"
#include "symbol_binding.h"

/**
 * @brief Nodo base del AST.
 * 
 * Todos los nodos específicos deben tener este struct como primer campo.
 * 
 * Campos:
 * 
 *   - node_type:  Tipo de nodo del AST.
 * 
 *   - return_type: Tipo de retorno inferido para este nodo.
 * 
 *   - line:       Número de línea en el código fuente.
 * 
 *   - column:     Número de columna en el código fuente.
 */
typedef struct ASTNode
{
    ASTNodeType node_type;
    TypeDescriptor *return_type;
    int line;
    int column;
} ASTNode;

/**
 * @brief Nodo para literales (Number, String, Bool).
 * 
 * Campos:
 * 
 *   - base:  Nodo base del AST.
 * 
 *   - value: Valor del literal (number_value, string_value o bool_value).
 */
typedef struct LiteralNode
{
    ASTNode base;
    union
    {
        double number_value;
        char *string_value;
        bool bool_value;
    } value;
} LiteralNode;

/**
 * @brief Nodo para operaciones unarias.
 * 
 * Campos:
 * 
 *   - base:     Nodo base del AST.
 * 
 *   - operator: Operador a aplicar.
 * 
 *   - operand:  Operando (toma ownership).
 */
typedef struct UnaryOperationNode
{
    ASTNode base;
    HulkOperator operator;
    ASTNode *operand;
} UnaryOperationNode;

/**
 * @brief Nodo para operaciones binarias.
 * 
 * Campos:
 * 
 *   - base:     Nodo base del AST.
 * 
 *   - operator: Operador a aplicar.
 * 
 *   - left:     Operando izquierdo (toma ownership). No puede ser NULL.
 * 
 *   - right:    Operando derecho (toma ownership). No puede ser NULL.
 */
typedef struct BinaryOperationNode
{
    ASTNode base;
    HulkOperator operator;
    ASTNode *left;
    ASTNode *right;
} BinaryOperationNode;

/**
 * @brief Nodo para bloque de expresiones.
 * 
 * Representa una secuencia de expresiones delimitadas por { }.
 * El tipo del bloque es el tipo de la última expresión.
 * 
 * Campos:
 * 
 *   - base:        Nodo base del AST.
 * 
 *   - expressions: Lista de ASTNode* con las expresiones del bloque.
 */
typedef struct ExpressionBlockNode
{
    ASTNode base;
    List *expressions;
} ExpressionBlockNode;

/**
 * @brief Nodo para condicional if-else.
 * 
 * Representa una expresión condicional if-else.
 * El tipo del condicional es el LCA de then_branch y else_branch.
 * Si no hay else_branch, el tipo es el de then_branch.
 * 
 * Campos:
 * 
 *   - base:        Nodo base del AST.
 * 
 *   - condition:   Condición del if (toma ownership). No puede ser NULL.
 * 
 *   - then_branch: Rama then (toma ownership). No puede ser NULL.
 * 
 *   - else_branch: Rama else (toma ownership, puede ser NULL).
 */
typedef struct ConditionalNode
{
    ASTNode base;
    ASTNode *condition;
    ASTNode *then_branch;
    ASTNode *else_branch;
} ConditionalNode;

/**
 * @brief Nodo para bucle while.
 * 
 * Representa un bucle while. La condición debe ser de tipo Bool.
 * El tipo del bucle es el tipo del cuerpo, o Void si no tiene cuerpo.
 * 
 * Campos:
 * 
 *   - base:      Nodo base del AST.
 * 
 *   - condition: Condición del bucle (toma ownership). No puede ser NULL.
 * 
 *   - body:      Cuerpo del bucle (toma ownership, puede ser NULL).
 */
typedef struct WhileLoopNode
{
    ASTNode base;
    ASTNode *condition;
    ASTNode *body;
} WhileLoopNode;

/**
 * @brief Nodo para expresión let-in.
 * 
 * Representa una expresión let-in que introduce variables locales con
 * tipo anotado opcional y expresiones de inicialización.
 * 
 * Campos:
 * 
 *   - base:     Nodo base del AST.
 * 
 *   - bindings: Lista de SymbolBinding* con las declaraciones de variables.
 *               No puede estar vacía. La lista debe crearse con
 *               free_function = symbol_binding_destroy.
 * 
 *   - body:     Expresión cuerpo del let-in (toma ownership). No puede ser NULL.
 */
typedef struct LetInNode
{
    ASTNode base;
    List *bindings;
    ASTNode *body;
} LetInNode;

/**
 * @brief Nodo para referencia a variable.
 * 
 * Representa el uso de una variable definida previamente.
 * El tipo se resuelve buscando en el scope durante la inferencia.
 * 
 * Campos:
 * 
 *   - base: Nodo base del AST.
 * 
 *   - name: Nombre de la variable referenciada.
 */
typedef struct VariableReferenceNode
{
    ASTNode base;
    char *name;
} VariableReferenceNode;

/**
 * @brief Nodo para reasignación de variable.
 * 
 * Representa la reasignación de una variable existente con un nuevo valor.
 * La variable debe estar definida previamente y no puede ser constante.
 * 
 * Campos:
 * 
 *   - base:   Nodo base del AST.
 * 
 *   - target: Expresión a reasignar.
 * 
 *   - value:  Expresión del nuevo valor (toma ownership). No puede ser NULL.
 */
typedef struct ReassignmentNode
{
    ASTNode base;
    ASTNode *target;
    ASTNode *value;
} ReassignmentNode;

/**
 * @brief Nodo para definición de función.
 *
 * Campos:
 * 
 *   - base:                   Nodo base del AST.
 * 
 *   - name:                   Nombre de la función.
 * 
 *   - params:                 Lista de SymbolBinding* con los parámetros.
 * 
 *   - return_type_annotation: Anotación del tipo de retorno (puede ser NULL).
 * 
 *   - body:                   Expresión del cuerpo.
 */
typedef struct FunctionDefinitionNode
{
    ASTNode base;
    char *name;
    List *params;
    char *return_type_annotation;
    ASTNode *body;
} FunctionDefinitionNode;

/**
 * @brief Nodo para llamada a función.
 *
 * Campos:
 * 
 *   - base: Nodo base del AST.
 * 
 *   - name: Nombre de la función (copia interna).
 * 
 *   - args: Lista de ASTNode* con los argumentos (toma ownership).
 *           No puede ser NULL (usar lista vacía para llamadas sin argumentos).
 */
typedef struct FunctionCallNode
{
    ASTNode base;
    char *name;
    List *args;
} FunctionCallNode;

/**
 * @brief Nodo para definición de tipo.
 *
 * Representa una definición de tipo con nombre, parámetros de inicialización,
 * herencia, atributos y métodos.
 *
 * Campos:
 * 
 *   - base:             Nodo base del AST.
 * 
 *   - name:             Nombre del tipo.
 * 
 *   - init_params:      Lista de SymbolBinding* con los parámetros de inicialización.
 *                       No puede ser NULL (usar lista vacía si no hay).
 * 
 *   - father_name:      Nombre del tipo padre (NULL o "Object").
 * 
 *   - father_init_args: Lista de ASTNode* con los argumentos para inicializar al padre.
 *                       No puede ser NULL (usar lista vacía si no hay).
 * 
 *   - attributes:       Lista de SymbolBinding* con los atributos.
 *                       No puede ser NULL (usar lista vacía si no hay).
 * 
 *   - methods:          Lista de FunctionDefinitionNode* con los métodos.
 *                       No puede ser NULL (usar lista vacía si no hay).
 */
typedef struct TypeDefinitionNode
{
    ASTNode base;
    char *name;
    List *init_params;
    char *father_name;
    List *father_init_args;
    List *attributes;
    List *methods;
} TypeDefinitionNode;

/**
 * @brief Nodo para acceso a un atributo (objeto.attr).
 *
 * Representa la evaluación de un acceso a un atributo de una expresión
 * (normalmente `self`). El target debe ser una expresión que evalúa a un
 * objeto con atributos.
 *
 * Campos:
 * 
 *   - base:           Nodo base del AST.
 * 
 *   - target:         Expresión del objeto sobre el que se accede (toma ownership).
 *                     No puede ser NULL.
 * 
 *   - attribute_name: Nombre del atributo a acceder.
 */
typedef struct AttributeAccessNode
{
    ASTNode base;
    ASTNode *target;
    char *attribute_name;
} AttributeAccessNode;

/**
 * @brief Nodo para acceso a un método (obj.method(args)).
 *
 * Representa la llamada a un método de un objeto. El target debe evaluar a un
 * objeto que tenga un método con el nombre y la cantidad de argumentos adecuados.
 *
 * Campos:
 * 
 *   - base:        Nodo base del AST.
 * 
 *   - target:      Expresión del objeto sobre el que se invoca el método (toma ownership).
 *                  No puede ser NULL.
 * 
 *   - method_name: Nombre del método (copia interna). No puede ser NULL.
 * 
 *   - args:        Lista de ASTNode* con los argumentos (toma ownership).
 *                  No puede ser NULL (usar lista vacía para cero argumentos).
 */
typedef struct MethodAccessNode
{
    ASTNode base;
    ASTNode *target;
    char *method_name;
    List *args;
} MethodAccessNode;

/**
 * @brief Nodo para la instanciación de un tipo.
 *
 * Representa la creación de una nueva instancia de un tipo definido por el usuario,
 * por ejemplo: `new Point(3, 4)`.
 *
 * Campos:
 * 
 *   - base:      Nodo base del AST.
 * 
 *   - type_name: Nombre del tipo a instanciar.
 * 
 *   - args:      Lista de ASTNode* con los argumentos de inicialización (toma ownership).
 *                No puede ser NULL (usar lista vacía si no hay argumentos).
 */
typedef struct TypeInstanciationNode
{
    ASTNode base;
    char *type_name;
    List *args;
} TypeInstanciationNode;

/**
 * @brief Nodo para llamada a la implementación base de un método (base()).
 *
 * Representa una llamada al método del mismo nombre en el tipo padre.
 * Solo es válido dentro de un método de un tipo definido por el usuario.
 *
 * Campos:
 * 
 *   - base:        Nodo base del AST.
 * 
 *   - method_name: Nombre del método desde el que se invoca.
 * 
 *   - type_name:   Nombre del tipo actual (para resolver el padre).
 * 
 *   - args:        Lista de ASTNode* con los argumentos (toma ownership).
 *                  No puede ser NULL (usar lista vacía para cero argumentos).
 */
typedef struct BaseCallNode
{
    ASTNode base;
    char *method_name;
    char *type_name;
    List *args;
    bool setted;
} BaseCallNode;

/**
 * @brief Nodo para expresión de comprobación de tipo (is).
 *
 * Representa la expresión `expr is Type`, que verifica si el resultado
 * de `expr` es una instancia de `Type` o de un subtipo.
 *
 * Campos:
 * 
 *   - base:      Nodo base del AST.
 * 
 *   - target:    Expresión a comprobar (toma ownership).
 * 
 *   - type_name: Nombre del tipo contra el que se comprueba.
 */
typedef struct IsNode
{
    ASTNode base;
    ASTNode *target;
    char *type_name;
} IsNode;

/**
 * @brief Nodo para expresión de conversión de tipo (as).
 *
 * Representa la expresión `expr as Type`, que convierte el resultado
 * de `expr` al tipo `Type` en tiempo de ejecución.
 *
 * Campos:
 * 
 *   - base:      Nodo base del AST.
 * 
 *   - target:    Expresión a convertir (toma ownership).
 * 
 *   - type_name: Nombre del tipo al que se convierte.
 */
typedef struct AsNode
{
    ASTNode base;
    ASTNode *target;
    char *type_name;
} AsNode;

/**
 * @brief Nodo raíz del programa.
 * 
 * Contiene las listas de definiciones de funciones y tipos declarados en el programa,
 * así como la expresión raíz que constituye el cuerpo del programa.
 * 
 * Campos:
 * 
 *   - base:                 Nodo base del AST.
 * 
 *   - root:                 Expresión raíz del programa (toma ownership). No puede ser NULL.
 * 
 *   - function_definitions: Lista de FunctionDefinitionNode* con las funciones definidas (toma ownership).
 *                           No puede ser NULL (usar lista vacía si no hay funciones).
 * 
 *   - type_definitions:     Lista de TypeDefinitionNode* con los tipos definidos (toma ownership).
 *                           No puede ser NULL (usar lista vacía si no hay tipos).
 */
typedef struct ProgramNode
{
    ASTNode base;
    ASTNode *root;
    List *function_definitions;
    List *type_definitions;
} ProgramNode;

/**
 * @brief Inicializa los campos base de un nodo AST.
 * 
 * @param node Puntero al nodo a inicializar.
 * @param node_type Tipo de nodo AST.
 * @param line Número de línea donde aparece el nodo.
 * @param column Número de columna donde aparece el nodo.
 */
void ast_base_init(ASTNode *node, ASTNodeType node_type, int line, int column);

/**
 * @brief Crea un nodo literal de tipo Number.
 * 
 * @param value Valor numérico.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
LiteralNode *ast_number_literal_create(double value, int line, int column);

/**
 * @brief Crea un nodo literal de tipo String.
 * 
 * @param value Cadena de texto (se copia internamente).
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
LiteralNode *ast_string_literal_create(const char *value, int line, int column);

/**
 * @brief Crea un nodo literal de tipo Bool.
 * 
 * @param value Valor booleano.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
LiteralNode *ast_bool_literal_create(bool value, int line, int column);

/**
 * @brief Crea un nodo de operación unaria.
 * 
 * @param op Operador unario.
 * @param operand Operando (toma ownership). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o si operand es NULL.
 */
UnaryOperationNode *ast_unary_create(HulkOperator op, ASTNode *operand, int line, int column);

/**
 * @brief Crea un nodo de operación binaria.
 * 
 * @param op Operador binario.
 * @param left Operando izquierdo (toma ownership). No puede ser NULL.
 * @param right Operando derecho (toma ownership). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún operando es NULL.
 */
BinaryOperationNode *ast_binary_create(HulkOperator op, ASTNode *left, ASTNode *right, int line, int column);

/**
 * @brief Crea un nodo de bloque de expresiones.
 * 
 * @param expressions Lista de expresiones (toma ownership). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o expressions es NULL.
 */
ExpressionBlockNode *ast_block_create(List *expressions, int line, int column);

/**
 * @brief Crea un nodo condicional if-else.
 * 
 * @param condition Condición (toma ownership). No puede ser NULL.
 * @param then_branch Rama then (toma ownership). No puede ser NULL.
 * @param else_branch Rama else (toma ownership, puede ser NULL).
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o condition/then_branch son NULL.
 */
ConditionalNode *ast_conditional_create(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int line, int column);

/**
 * @brief Crea un nodo de bucle while.
 * 
 * @param condition Condición del bucle (toma ownership). No puede ser NULL.
 * @param body Cuerpo del bucle (toma ownership, puede ser NULL).
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o condition es NULL.
 */
WhileLoopNode *ast_while_create(ASTNode *condition, ASTNode *body, int line, int column);

/**
 * @brief Crea un nodo let-in.
 * 
 * @param bindings Lista de SymbolBinding* (toma ownership). No puede ser NULL ni vacía. Debe tener free_function = symbol_binding_destroy.
 * @param body Expresión cuerpo (toma ownership). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación, bindings está vacío o body es NULL.
 */
LetInNode *ast_let_create(List *bindings, ASTNode *body, int line, int column);

/**
 * @brief Crea un nodo de referencia a variable.
 * 
 * @param name Nombre de la variable (se copia internamente).
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
VariableReferenceNode *ast_variable_reference_create(const char *name, int line, int column);

/**
 * @brief Crea un nodo de reasignación.
 * 
 * @param target Nodo asignable (VariableReference o AttributeAccess) (toma ownership).
 * @param value  Expresión del nuevo valor (toma ownership).
 * @param line   Línea del operador :=.
 * @param column Columna del operador :=.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
ReassignmentNode *ast_reassignment_create(ASTNode *target, ASTNode *value, int line, int column);

/**
 * @brief Crea un nodo de definición de función.
 *
 * @param name                   Nombre de la función (se copia internamente). No puede ser NULL.
 * @param params                 Lista de SymbolBinding* con los parámetros (toma ownership).
 *                               No puede ser NULL. La lista debe tener free_func = symbol_binding_destroy.
 * @param return_type_annotation Anotación del tipo de retorno (se copia, puede ser NULL).
 * @param body                   Expresión del cuerpo (toma ownership). No puede ser NULL.
 * @param line                   Número de línea donde se define la función.
 * @param column                 Número de columna donde se define la función.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún parámetro obligatorio es NULL.
 */
FunctionDefinitionNode *ast_function_definition_create(const char *name, List *params, const char *return_type_annotation, ASTNode *body, int line, int column);

/**
 * @brief Crea un nodo de llamada a función.
 *
 * @param name   Nombre de la función (se copia internamente). No puede ser NULL.
 * @param args   Lista de ASTNode* con los argumentos (toma ownership).
 *               No puede ser NULL (usar lista vacía para llamadas sin argumentos).
 * @param line   Número de línea de la llamada.
 * @param column Número de columna de la llamada.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún parámetro obligatorio es NULL.
 */
FunctionCallNode *ast_function_call_create(const char *name, List *args, int line, int column);

/**
 * @brief Crea un nodo de definición de tipo.
 *
 * @param name Nombre del tipo (se copia internamente). No puede ser NULL.
 * @param init_params Lista de SymbolBinding* con los parámetros de inicialización (toma ownership). No puede ser NULL.
 * @param father_name Nombre del tipo padre (se copia, puede ser NULL; se asigna "Object" por defecto).
 * @param father_init_args Lista de ASTNode* con los argumentos del padre (toma ownership). No puede ser NULL.
 * @param attributes Lista de SymbolBinding* con los atributos (toma ownership). No puede ser NULL.
 * @param methods Lista de FunctionDefinitionNode* con los métodos (toma ownership). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún parámetro obligatorio es NULL.
 */
TypeDefinitionNode *ast_type_definition_create(const char *name, List *init_params, const char *father_name, List *father_init_args, List *attributes, List *methods, int line, int column);

/**
 * @brief Crea un nodo de acceso a un atributo.
 *
 * @param target Expresión del objeto (toma ownership). No puede ser NULL.
 * @param attribute_name Nombre del atributo (se copia internamente). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún parámetro es NULL.
 */
AttributeAccessNode *ast_attribute_access_create(ASTNode *target, const char *attribute_name, int line, int column);

/**
 * @brief Crea un nodo de acceso a método.
 *
 * @param target      Expresión del objeto (toma ownership). No puede ser NULL.
 * @param method_name Nombre del método (se copia internamente). No puede ser NULL.
 * @param args        Lista de argumentos (toma ownership). No puede ser NULL (usar lista vacía).
 * @param line        Número de línea.
 * @param column      Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún parámetro obligatorio es NULL.
 */
MethodAccessNode *ast_method_access_create(ASTNode *target, const char *method_name, List *args, int line, int column);

/**
 * @brief Crea un nodo de instanciación de tipo.
 *
 * @param type_name Nombre del tipo (se copia internamente). No puede ser NULL.
 * @param args Lista de argumentos (toma ownership). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún parámetro obligatorio es NULL.
 */
TypeInstanciationNode *ast_type_instanciation_create(const char *type_name, List *args, int line, int column);

/**
 * @brief Crea un nodo de llamada a base().
 *
 * @param args   Lista de argumentos (toma ownership). No puede ser NULL.
 * @param line   Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
BaseCallNode *ast_base_call_create(List *args, int line, int column);

/**
 * @brief Crea un nodo de expresión is.
 *
 * @param target    Expresión a comprobar (toma ownership). No puede ser NULL.
 * @param type_name Nombre del tipo (se copia internamente). No puede ser NULL.
 * @param line      Número de línea.
 * @param column    Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
IsNode *ast_is_create(ASTNode *target, const char *type_name, int line, int column);

/**
 * @brief Crea un nodo de expresión as.
 *
 * @param target    Expresión a convertir (toma ownership). No puede ser NULL.
 * @param type_name Nombre del tipo (se copia internamente). No puede ser NULL.
 * @param line      Número de línea.
 * @param column    Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación.
 */
AsNode *ast_as_create(ASTNode *target, const char *type_name, int line, int column);

/**
 * @brief Crea un nodo de programa.
 * 
 * @param root Expresión raíz del programa (toma ownership). No puede ser NULL.
 * @param function_definitions Lista de definiciones de funciones (toma ownership). No puede ser NULL.
 * @param type_definitions Lista de definiciones de tipos (toma ownership). No puede ser NULL.
 * @param line Número de línea.
 * @param column Número de columna.
 * @return Puntero al nodo creado, o NULL si falla la asignación o algún parámetro es NULL.
 */
ProgramNode *ast_program_create(ASTNode *root, List *function_definitions, List *type_definitions, int line, int column);

#endif