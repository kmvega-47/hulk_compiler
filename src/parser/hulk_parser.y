%{

#include "diagnostic_manager.h"
#include "parser_types.h"

/* Variables globales del compilador */
ASTNode *ast_root = NULL;

extern int yylineno;

/* Prototipos de funciones del parser */
void yyerror(const char *msg);
int yylex(void);

void append_else(ConditionalNode *chain, ASTNode *final_else);

%}

/* Unión semántica: tipos de los valores que transportan tokens y reglas */
%union {
    double          number_value;
    char           *string_value;
    bool            bool_value;
    ASTNode        *node;
    List           *list;
    SymbolBinding  *binding;
    TypeBodyElem   *type_elem;
    TypeInheritance *type_inherit;
}

/* Palabras clave */
%token FUNCTION LET IN IF ELIF ELSE WHILE TYPE INHERITS NEW
%token <bool_value> TRUE FALSE

/* Literales con valor semántico */
%token <number_value> NUMBER
%token <string_value> STRING
%token <string_value> ID

/* Operadores y puntuación */
%token PLUS MINUS STAR SLASH PERCENT POWER
%token CONCAT CONCAT_WS
%token ASSIGN REASSIGN
%token EQUALS NOT_EQUALS GREATER GREATER_EQ LESS LESS_EQ
%token AND OR NOT
%token ARROW
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COLON DOT COMMA
%token IS AS BASE
%token LEX_ERROR

/* Precedencia y asociatividad (de menor a mayor) */
%right IN
%right IF ELIF ELSE
%right WHILE
%right REASSIGN
%left CONCAT CONCAT_WS
%nonassoc IS AS
%left OR
%left AND
%nonassoc EQUALS NOT_EQUALS GREATER GREATER_EQ LESS LESS_EQ
%left PLUS MINUS
%left STAR SLASH PERCENT
%right POWER
%right NOT
%left DOT

/* Tipo de los no terminales */
%type <node> program expression literal
%type <node> type_def
%type <type_inherit> type_inheritance
%type <node> elif_clauses else_clause method_def func_def method_body
%type <list> bindings
%type <list> expr_list
%type <list> args_list args_sequence_opt args_sequence args_list_opt
%type <list> func_def_list params_list params_sequence_opt params_sequence params_list_opt
%type <list> type_body_elements type_def_list
%type <type_elem> type_body_element attr_def
%type <binding> var_binding
%type <string_value> type_annotation_opt
%type <node> expr_block
%type <node> var_reference
%type <node> attr_access
%type <node> assignable

/* Axioma de la gramática */
%start program

/* Seguimiento de ubicaciones (línea/columna) y errores detallados */
%locations
%define parse.error verbose
%define parse.trace

%%

/* --- PROGRAMA PRINCIPAL --- */
program:
    type_def_list func_def_list expression SEMICOLON_opt
    {
        $$ = (ASTNode *)ast_program_create($3, $2, $1,
              @1.first_line, @1.first_column);

        ast_root = $$;
    }
;

SEMICOLON_opt: %empty | SEMICOLON ;

type_def_list:
    %empty
    {
        $$ = list_create(0, NULL, NULL, NULL, NULL);
    }

    | type_def_list type_def
    {
        $$ = $1;
        list_append($$, $2);
    }
;

type_def:
    TYPE ID params_list_opt type_inheritance LBRACE type_body_elements RBRACE
    {
        List *attr_list = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
        List *method_list = list_create(0, NULL, NULL, NULL, NULL);

        for (size_t i = 0; i < list_count($6); i++)
        {
            TypeBodyElem *elem = (TypeBodyElem *)list_get($6, i);

            if (elem->kind == 0)
                list_append(attr_list, elem->data.attr);
            else
                list_append(method_list, elem->data.method);
        }

        list_destroy($6);

        char *father_name = NULL;
        List *father_args = NULL;

        if ($4 != NULL)
        {
            father_name = $4->father_name;
            father_args = $4->father_args;
            free($4);
        }

        $$ = (ASTNode *)ast_type_definition_create($2, $3, father_name,
              father_args, attr_list, method_list, @1.first_line, @1.first_column);

        free($2);
        free(father_name);
    }
;

params_list_opt:
    %empty
    {
        $$ = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    }

    | params_list
    {
        $$ = $1;
    }
;

params_list:
    LPAREN params_sequence_opt RPAREN
    {
        $$ = $2;
    }
;

params_sequence_opt:
    %empty
    {
        $$ = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    }

    | params_sequence
    {
        $$ = $1;
    }
;

params_sequence:
    ID type_annotation_opt
    {
        SymbolBinding *sb = symbol_binding_create($1, $2, NULL, @1.first_line, @1.first_column);
        free($1);
        if ($2) free($2);

        $$ = list_create(1, (void *[]){ sb }, (free_func)symbol_binding_destroy, NULL, NULL);
    }

    | params_sequence COMMA ID type_annotation_opt
    {
        SymbolBinding *sb = symbol_binding_create($3, $4, NULL, @3.first_line, @3.first_column);
        free($3);
        if ($4) free($4);

        $$ = $1;
        list_append($$, sb);
    }
;

type_annotation_opt:
    %empty
    {
        $$ = NULL;
    }

    | COLON ID
    {
        $$ = $2;
    }
;

type_inheritance:
    %empty
    {
        $$ = NULL;
    }

    | INHERITS ID args_list_opt
    {
        TypeInheritance *inherit = (TypeInheritance *)malloc(sizeof(TypeInheritance));
        inherit->father_name = $2;
        inherit->father_args = $3;
        $$ = inherit;
    }
;

args_list_opt:
    %empty
    {
        $$ = list_create(0, NULL, NULL, NULL, NULL);
    }

    | args_list
    {
        $$ = $1;
    }
;

args_list:
    LPAREN args_sequence_opt RPAREN
    {
        $$ = $2;
    }
;

args_sequence_opt:
    %empty
    {
        $$ = list_create(0, NULL, NULL, NULL, NULL);
    }

    | args_sequence
    {
        $$ = $1;
    }
;

args_sequence:
    expression
    {
        $$ = list_create(1, (void *[]){ $1 }, NULL, NULL, NULL);
    }

    | args_sequence COMMA expression
    {
        $$ = $1;
        list_append($$, $3);
    }
;

type_body_elements:
    %empty
    {
        $$ = list_create(0, NULL, free, NULL, NULL);
    }

    | type_body_elements type_body_element
    {
        $$ = $1;
        list_append($$, $2);
    }
;

type_body_element:
    attr_def
    {
        $$ = $1;
    }

    | method_def
    {
        TypeBodyElem *elem = (TypeBodyElem *)malloc(sizeof(TypeBodyElem));
        elem->kind = 1;
        elem->data.method = $1;
        $$ = elem;
    }
;

attr_def:
    var_binding SEMICOLON
    {
        TypeBodyElem *elem = (TypeBodyElem *)malloc(sizeof(TypeBodyElem));
        elem->kind = 0;
        elem->data.attr = $1;
        $$ = elem;
    }
;

method_def:
    ID params_list type_annotation_opt method_body
    {
        $$ = (ASTNode *)ast_function_definition_create($1, $2,
               $3, $4, @1.first_line, @1.first_column);
        free($1);
        if ($3) free($3);
    }
;

method_body:
    ARROW expression SEMICOLON
    {
        $$ = $2;
    }

    | expr_block
    {
        $$ = $1;
    }
;

var_binding:
    ID type_annotation_opt ASSIGN expression
    {
        $$ = symbol_binding_create($1, $2, $4, @1.first_line, @1.first_column);
        free($1);
        if ($2) free($2);
    }
;

func_def_list:
    %empty
    {
        $$ = list_create(0, NULL, NULL, NULL, NULL);
    }

    | func_def_list func_def
    {
        $$ = $1;
        list_append($$, $2);
    }
;

func_def:
    FUNCTION method_def
    {
        $$ = $2;
    }
;

expression:
    var_reference
    {
        $$ = $1;
    }

    | literal
    {
        $$ = $1;
    }

    | LPAREN expression RPAREN
    {
        $$ = $2;
    }

    | NOT expression
    {
        $$ = (ASTNode *)ast_unary_create(OP_NOT, $2, @1.first_line, @1.first_column);
    }

    | MINUS expression %prec NOT
    {
        $$ = (ASTNode *)ast_unary_create(OP_SUB, $2, @1.first_line, @1.first_column);
    }

    | expression PLUS expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_ADD, $1, $3, @2.first_line, @2.first_column);
    }

    | expression MINUS expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_SUB, $1, $3, @2.first_line, @2.first_column);
    }

    | expression STAR expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_MULT, $1, $3, @2.first_line, @2.first_column);
    }

    | expression SLASH expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_DIV, $1, $3, @2.first_line, @2.first_column);
    }

    | expression PERCENT expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_MOD, $1, $3, @2.first_line, @2.first_column);
    }

    | expression POWER expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_EXP, $1, $3, @2.first_line, @2.first_column);
    }

    | expression CONCAT expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_CONCAT, $1, $3, @2.first_line, @2.first_column);
    }

    | expression CONCAT_WS expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_CONCAT_WS, $1, $3, @2.first_line, @2.first_column);
    }

    | expression GREATER expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_GREATER, $1, $3, @2.first_line, @2.first_column);
    }

    | expression LESS expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_LESS, $1, $3, @2.first_line, @2.first_column);
    }

    | expression GREATER_EQ expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_GREATER_EQUAL, $1, $3, @2.first_line, @2.first_column);
    }

    | expression LESS_EQ expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_LESS_EQUAL, $1, $3, @2.first_line, @2.first_column);
    }

    | expression EQUALS expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_EQUAL, $1, $3, @2.first_line, @2.first_column);
    }

    | expression NOT_EQUALS expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_NOT_EQUAL, $1, $3, @2.first_line, @2.first_column);
    }

    | expression AND expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_AND, $1, $3, @2.first_line, @2.first_column);
    }

    | expression OR expression
    {
        $$ = (ASTNode *)ast_binary_create(OP_OR, $1, $3, @2.first_line, @2.first_column);
    }

    | expr_block
    {
        $$ = $1;
    }

    | IF LPAREN expression RPAREN expression elif_clauses else_clause %prec IF
    {
        ASTNode *else_part = $6;

        if (else_part == NULL)
        {
            else_part = $7;
        }
        else
        {
            append_else((ConditionalNode *)else_part, $7);
        }

        $$ = (ASTNode *)ast_conditional_create($3, $5, else_part,
              @1.first_line, @1.first_column);
    }

    | WHILE LPAREN expression RPAREN expression %prec WHILE
    {
        $$ = (ASTNode *)ast_while_create($3, $5,
              @1.first_line, @1.first_column);
    }

    | LET bindings IN expression
    {
        $$ = (ASTNode *)ast_let_create($2, $4,
              @1.first_line, @1.first_column);
    }

    | ID args_list
    {
        $$ = (ASTNode *)ast_function_call_create($1, $2,
              @1.first_line, @1.first_column);
        free($1);
    }

    | expression DOT ID args_list
    {
        $$ = (ASTNode *)ast_method_access_create($1, $3, $4,
              @3.first_line, @3.first_column);
        free($3);
    }

    | attr_access
    {
        $$ = $1;
    }

    | NEW ID args_list
    {
        $$ = (ASTNode *)ast_type_instanciation_create($2, $3,
              @2.first_line, @2.first_column);
        free($2);
    }

    | assignable REASSIGN expression
    {
        $$ = (ASTNode *)ast_reassignment_create($1, $3, @2.first_line, @2.first_column);
    }

    | expression IS ID
    {
        $$ = (ASTNode *)ast_is_create($1, $3, @2.first_line, @2.first_column);
        free($3);
    }

    | expression AS ID
    {
        $$ = (ASTNode *)ast_as_create($1, $3, @2.first_line, @2.first_column);
        free($3);
    }

    | BASE args_list
    {
        $$ = (ASTNode *)ast_base_call_create($2, @1.first_line, @1.first_column);
    }
    ;

literal:
    NUMBER
    {
        $$ = (ASTNode *)ast_number_literal_create($1, @1.first_line, @1.first_column);
    }

    | STRING
    {
        $$ = (ASTNode *)ast_string_literal_create($1, @1.first_line, @1.first_column);
    }

    | TRUE
    {
        $$ = (ASTNode *)ast_bool_literal_create($1, @1.first_line, @1.first_column);
    }

    | FALSE
    {
        $$ = (ASTNode *)ast_bool_literal_create($1, @1.first_line, @1.first_column);
    }
;

expr_block:
    LBRACE RBRACE
    {
        $$ = (ASTNode *)ast_block_create(list_create(0, NULL, NULL, NULL, NULL), @1.first_line, @1.first_column);
    }
    | LBRACE expr_list SEMICOLON_opt RBRACE
    {
        $$ = (ASTNode *)ast_block_create($2, @1.first_line, @1.first_column);
    }
;

expr_list:
    expression
    {
        $$ = list_create(1, (void *[]){ $1 }, NULL, NULL, NULL);
    }
    | expr_list SEMICOLON expression
    {
        $$ = $1;
        list_append($$, $3);
    }
;

elif_clauses:
    %empty %prec IF
    {
        $$ = NULL;
    }

    | ELIF LPAREN expression RPAREN expression elif_clauses %prec ELIF
    {
        $$ = (ASTNode *)ast_conditional_create($3, $5, $6,
              @1.first_line, @1.first_column);
    }
;

else_clause:
    %empty %prec IF
    {
        $$ = NULL;
    }

    | ELSE expression %prec ELSE
    {
        $$ = $2;
    }
;

bindings:
    var_binding
    {
        $$ = list_create(1, (void *[]){ $1 }, (free_func)symbol_binding_destroy, NULL, NULL);
    }

    | bindings COMMA var_binding
    {
        $$ = $1;
        list_append($$, $3);
    }
;

assignable:
    var_reference
    | attr_access
;

var_reference:
    ID
    {
        $$ = (ASTNode *)ast_variable_reference_create($1, @1.first_line, @1.first_column);
        free($1);
    }
;

attr_access:
    expression DOT ID
    {
        $$ = (ASTNode *)ast_attribute_access_create($1, $3, @3.first_line, @3.first_column);
        free($3);
    }
;

%%

void yyerror(const char *msg)
{
    dm_add_error(dm_global, ERROR_TYPE_SYNTACTIC, yylineno, yylloc.first_column, "%s", msg);
}

void append_else(ConditionalNode *chain, ASTNode *final_else)
{
    ConditionalNode *curr = chain;
    while (curr->else_branch != NULL && curr->else_branch->node_type == NODE_CONDITIONAL)
    {
        curr = (ConditionalNode *)curr->else_branch;
    }
    curr->else_branch = final_else;
}