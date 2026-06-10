#include "enums.h"

static const char *ERROR_TYPE_STRINGS[] = {
    [ERROR_TYPE_LEXICAL]  = "LEXICAL",
    [ERROR_TYPE_SYNTACTIC]   = "SYNTACTIC",
    [ERROR_TYPE_SEMANTIC] = "SEMANTIC",
};

static const char *HULK_TYPE_STRINGS[] = {
    [HULK_OBJECT]       = "Object",
    [HULK_NUMBER]       = "Number",
    [HULK_STRING]       = "String",
    [HULK_BOOL]         = "Bool",
    [HULK_VOID]         = "Void",
    [HULK_USER_DEFINED] = "UserDefined",
};

static const char *AST_NODE_TYPE_STRINGS[] = {
    [NODE_LITERAL]            = "Literal",
    [NODE_UNARY_OPERATION]    = "UnaryOperation",
    [NODE_BINARY_OPERATION]   = "BinaryOperation",
    [NODE_EXPRESSION_BLOCK]   = "ExpressionBlock",
    [NODE_CONDITIONAL]        = "Conditional",
    [NODE_WHILE_LOOP]         = "WhileLoop",
    [NODE_LET_IN]             = "LetIn",
    [NODE_VARIABLE_REFERENCE] = "VariableReference",
    [NODE_REASSIGNMENT]       = "Reassignment",
    [NODE_FUNCTION_DEFINITION] = "FunctionDefinition",
    [NODE_FUNCTION_CALL]      = "FunctionCall",
    [NODE_TYPE_DEFINITION]    = "TypeDefinition",
    [NODE_ATTRIBUTE_ACCESS]   = "AttributeAccess",
    [NODE_METHOD_ACCESS]      = "MethodAccess",
    [NODE_TYPE_INSTANCIATION] = "TypeInstantiation",
    [NODE_BASE_CALL]          = "BaseCall",
    [NODE_IS]                 = "Is",
    [NODE_AS]                 = "As",
    [NODE_PROGRAM]            = "Program",
};

static const char *CONSTRAINT_KIND_STRINGS[] = {
    [CONSTRAINT_EQUAL]     = "=",
    [CONSTRAINT_CONFORMS]  = "<=",
    [CONSTRAINT_CONFLICT]  = "CONFLICT",
};

static const char *OPERATOR_STRINGS[] = {
    [OP_EQUAL]          = "==",
    [OP_NOT_EQUAL]      = "!=",
    [OP_LESS]           = "<",
    [OP_LESS_EQUAL]     = "<=",
    [OP_GREATER]        = ">",
    [OP_GREATER_EQUAL]  = ">=",
    [OP_NOT]            = "!",
    [OP_ADD]            = "+",
    [OP_SUB]            = "-",
    [OP_MULT]           = "*",
    [OP_DIV]            = "/",
    [OP_MOD]            = "%",
    [OP_EXP]            = "^",
    [OP_AND]            = "&",
    [OP_OR]             = "|",
    [OP_CONCAT]         = "@",
    [OP_CONCAT_WS]      = "@@",
};

const char* error_type_to_string(error_type_t type)
{
    if (type < 0 || type >= (int)(sizeof(ERROR_TYPE_STRINGS) / sizeof(ERROR_TYPE_STRINGS[0])))
        return "UNKNOWN";
        
    return ERROR_TYPE_STRINGS[type];
}

const char* hulk_type_to_string(HulkType type)
{
    if (type < 0 || type >= (int)(sizeof(HULK_TYPE_STRINGS) / sizeof(HULK_TYPE_STRINGS[0])))
        return "Undefined Type";
        
    return HULK_TYPE_STRINGS[type];
}

const char* ast_node_type_to_string(ASTNodeType type)
{
    if (type < 0 || type >= (int)(sizeof(AST_NODE_TYPE_STRINGS) / sizeof(AST_NODE_TYPE_STRINGS[0])))
        return "Unknown";
    return AST_NODE_TYPE_STRINGS[type];
}

const char* constraint_kind_to_string(ConstraintKind type)
{
    if (type < 0 || type >= (int)(sizeof(CONSTRAINT_KIND_STRINGS) / sizeof(CONSTRAINT_KIND_STRINGS[0])))
        return "?";
    return CONSTRAINT_KIND_STRINGS[type];
}


const char* operator_to_string(HulkOperator op)
{
    if (op < 0 || op >= (int)(sizeof(OPERATOR_STRINGS) / sizeof(OPERATOR_STRINGS[0])))
        return "?";
    return OPERATOR_STRINGS[op];
}

bool is_arithmetic_operator(HulkOperator op)
{
    return op == OP_ADD || op == OP_SUB || op == OP_MULT || op == OP_DIV || op == OP_MOD || op == OP_EXP;
}

bool is_comparison_operator(HulkOperator op)
{
    return op == OP_GREATER || op == OP_GREATER_EQUAL || op == OP_LESS ||
           op == OP_LESS_EQUAL || op == OP_EQUAL || op == OP_NOT_EQUAL;
}

bool is_logical_operator(HulkOperator op)
{
    return op == OP_AND || op == OP_OR;
}

bool is_string_operator(HulkOperator op)
{
    return op == OP_CONCAT || op == OP_CONCAT_WS;
}