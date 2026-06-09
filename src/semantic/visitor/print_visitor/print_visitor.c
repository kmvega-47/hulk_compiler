#include "print_visitor.h"

static void print_indent(int level)
{
    for (int i = 0; i < level; i++)
        printf("   ");
}

static void *visit_literal_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    LiteralNode *literal = (LiteralNode *)node;

    print_indent(pv->indent_level);

    if (literal->base.return_type == type_table_lookup_by_tag(global_type_table, HULK_NUMBER))
        printf("NumberLiteral: %g [Number]\n", literal->value.number_value);

    else if (literal->base.return_type == type_table_lookup_by_tag(global_type_table, HULK_STRING))
        printf("StringLiteral: \"%s\" [String]\n", literal->value.string_value);

    else if (literal->base.return_type == type_table_lookup_by_tag(global_type_table, HULK_BOOL))
        printf("BoolLiteral: %s [Bool]\n", literal->value.bool_value ? "true" : "false");

    return NULL;
}

static void *visit_unary_operation_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    UnaryOperationNode *unary = (UnaryOperationNode *)node;

    print_indent(pv->indent_level);

    printf("UnaryOperation: %s [%s]\n", 
            operator_to_string(unary->operator), unary->base.return_type ? unary->base.return_type->name : "?");

    pv->indent_level++;
    ast_accept(unary->operand, visitor);
    pv->indent_level--;

    return NULL;
}

static void *visit_binary_operation_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    BinaryOperationNode *bin = (BinaryOperationNode *)node;

    print_indent(pv->indent_level);

    printf("BinaryOperation: %s [%s]\n",
           operator_to_string(bin->operator), bin->base.return_type ? bin->base.return_type->name : "?");

    pv->indent_level++;
    ast_accept(bin->left, visitor);
    ast_accept(bin->right, visitor);
    pv->indent_level--;

    return NULL;
}

static void *visit_expression_block_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    ExpressionBlockNode *block = (ExpressionBlockNode *)node;

    print_indent(pv->indent_level);

    printf("ExpressionBlock [%s]:\n",
           block->base.return_type ? block->base.return_type->name : "?");

    print_indent(pv->indent_level);

    printf("{\n");

    if (list_count(block->expressions) > 0)
    {
        pv->indent_level++;

        for (size_t i = 0; i < list_count(block->expressions); i++)
        {
            ASTNode *expr = (ASTNode *)list_get(block->expressions, i);
            ast_accept(expr, visitor);
        }

        pv->indent_level--;
    }

    print_indent(pv->indent_level);
    printf("}\n");

    return NULL;
}

static void *visit_conditional_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    ConditionalNode *cond = (ConditionalNode *)node;

    print_indent(pv->indent_level);

    printf("Conditional [%s]:\n",
           cond->base.return_type ? cond->base.return_type->name : "?");

    pv->indent_level++;
    print_indent(pv->indent_level);

    printf("if:\n");

    pv->indent_level++;
    ast_accept(cond->condition, visitor);
    pv->indent_level--;

    print_indent(pv->indent_level);
    printf("then:\n");

    pv->indent_level++;
    ast_accept(cond->then_branch, visitor);
    pv->indent_level--;

    if (cond->else_branch)
    {
        print_indent(pv->indent_level);

        printf("else:\n");

        pv->indent_level++;
        ast_accept(cond->else_branch, visitor);
        pv->indent_level--;
    }
    pv->indent_level--;

    return NULL;
}

static void *visit_while_loop_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    WhileLoopNode *loop = (WhileLoopNode *)node;

    print_indent(pv->indent_level);

    printf("WhileLoop [%s]:\n",
           loop->base.return_type ? loop->base.return_type->name : "?");

    pv->indent_level++;
    print_indent(pv->indent_level);

    printf("condition:\n");

    pv->indent_level++;
    ast_accept(loop->condition, visitor);
    pv->indent_level--;

    if (loop->body)
    {
        print_indent(pv->indent_level);

        printf("body:\n");

        pv->indent_level++;
        ast_accept(loop->body, visitor);
        pv->indent_level--;
    }

    pv->indent_level--;

    return NULL;
}

static void *visit_let_in_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    LetInNode *let_in = (LetInNode *)node;

    print_indent(pv->indent_level);

    printf("LetIn [%s]:\n",
           let_in->base.return_type ? let_in->base.return_type->name : "?");

    pv->indent_level++;
    print_indent(pv->indent_level);

    printf("bindings:\n");

    pv->indent_level++;

    for (size_t i = 0; i < list_count(let_in->bindings); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(let_in->bindings, i);

        print_indent(pv->indent_level);
        printf("%s", binding->name);

        if (binding->type_name)
            printf(" : %s", binding->type_name);

        printf(" =\n");

        pv->indent_level++;

        if (binding->initializer)
            ast_accept(binding->initializer, visitor);

        pv->indent_level--;
    }

    pv->indent_level--;

    print_indent(pv->indent_level);
    printf("in:\n");

    pv->indent_level++;
    ast_accept(let_in->body, visitor);
    pv->indent_level--;

    pv->indent_level--;

    return NULL;
}

static void *visit_variable_reference_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    VariableReferenceNode *var_ref = (VariableReferenceNode *)node;

    print_indent(pv->indent_level);

    printf("VariableReference: %s [%s]\n",
           var_ref->name, var_ref->base.return_type ? var_ref->base.return_type->name : "?");

    return NULL;
}

static void *visit_reassignment_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    ReassignmentNode *reassign = (ReassignmentNode *)node;

    print_indent(pv->indent_level);
    printf("Reassignment [%s]:\n",
           reassign->base.return_type ? reassign->base.return_type->name : "?");

    pv->indent_level++;

    ast_accept(reassign->target, visitor);

    print_indent(pv->indent_level);
    printf(":=\n");

    ast_accept(reassign->value, visitor);

    pv->indent_level--;
    return NULL;
}

static void *visit_function_definition_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    FunctionDefinitionNode *func = (FunctionDefinitionNode *)node;

    print_indent(pv->indent_level);

    printf("FunctionDefinition: %s [%s]\n",
           func->name, func->base.return_type ? func->base.return_type->name : "?");

    pv->indent_level++;

    print_indent(pv->indent_level);

    printf("params:\n");
    pv->indent_level++;

    for (size_t i = 0; i < list_count(func->params); i++)
    {
        SymbolBinding *binding = (SymbolBinding *)list_get(func->params, i);

        print_indent(pv->indent_level);
        printf("%s", binding->name);

        if (binding->type_name)
            printf(" : %s", binding->type_name);

        printf("\n");
    }
    pv->indent_level--;

    if (func->return_type_annotation)
    {
        print_indent(pv->indent_level);
        printf("return: %s\n", func->return_type_annotation);
    }

    print_indent(pv->indent_level);
    printf("body:\n");

    pv->indent_level++;
    ast_accept(func->body, visitor);
    pv->indent_level--;

    pv->indent_level--;
    return NULL;
}

static void *visit_function_call_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    FunctionCallNode *func = (FunctionCallNode *)node;

    print_indent(pv->indent_level);

    printf("FunctionCall: %s [%s]\n",
           func->name, func->base.return_type ? func->base.return_type->name : "?");

    pv->indent_level++;

    if (list_count(func->args) > 0)
    {
        print_indent(pv->indent_level);

        printf("Args:\n");
        pv->indent_level++;

        for (size_t i = 0; i < list_count(func->args); i++)
        {
            ASTNode *arg = (ASTNode *)list_get(func->args, i);
            ast_accept(arg, visitor);
        }

        pv->indent_level--;
    }

    pv->indent_level--;
    return NULL;
}

static void *visit_type_definition_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    TypeDefinitionNode *type = (TypeDefinitionNode *)node;

    print_indent(pv->indent_level);

    printf("TypeDefinition: %s [%s]",
           type->name, type->base.return_type ? type->base.return_type->name : "?");

    if (type->father_name)
        printf(" inherits %s", type->father_name);

    printf("\n");
    pv->indent_level++;

    if (list_count(type->init_params) > 0)
    {
        print_indent(pv->indent_level);
        printf("init_params:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(type->init_params); i++)
        {
            SymbolBinding *b = (SymbolBinding *)list_get(type->init_params, i);
            print_indent(pv->indent_level);

            printf("%s", b->name);

            if (b->type_name)
                printf(" : %s", b->type_name);

            printf("\n");
        }

        pv->indent_level--;
    }

    if (list_count(type->father_init_args) > 0)
    {
        print_indent(pv->indent_level);

        printf("father_init_args:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(type->father_init_args); i++)
        {
            ASTNode *arg = (ASTNode *)list_get(type->father_init_args, i);
            ast_accept(arg, visitor);
        }

        pv->indent_level--;
    }

    if (list_count(type->attributes) > 0)
    {
        print_indent(pv->indent_level);
        printf("attributes:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(type->attributes); i++)
        {
            SymbolBinding *b = (SymbolBinding *)list_get(type->attributes, i);

            print_indent(pv->indent_level);
            printf("%s", b->name);

            if (b->type_name)
                printf(" : %s", b->type_name);

            if (b->initializer)
            {
                printf(" =\n");
                pv->indent_level++;
                ast_accept(b->initializer, visitor);
                pv->indent_level--;
            }

            else
            {
                printf("\n");
            }
        }
        pv->indent_level--;
    }

    if (list_count(type->methods) > 0)
    {
        print_indent(pv->indent_level);
        printf("methods:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(type->methods); i++)
        {
            ASTNode *method = (ASTNode *)list_get(type->methods, i);
            ast_accept(method, visitor);
        }

        pv->indent_level--;
    }

    pv->indent_level--;
    return NULL;
}

static void *visit_attribute_access_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    AttributeAccessNode *attr = (AttributeAccessNode *)node;

    print_indent(pv->indent_level);

    printf("AttributeAccess: %s [%s]\n",
           attr->attribute_name, attr->base.return_type ? attr->base.return_type->name : "?");

    pv->indent_level++;
    ast_accept(attr->target, visitor);
    pv->indent_level--;

    return NULL;
}

static void *visit_type_instanciation_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    TypeInstanciationNode *inst = (TypeInstanciationNode *)node;

    print_indent(pv->indent_level);

    printf("TypeInstanciation: %s [%s]\n",
           inst->type_name, inst->base.return_type ? inst->base.return_type->name : "?");

    if (list_count(inst->args) > 0)
    {
        pv->indent_level++;

        print_indent(pv->indent_level);
        printf("args:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(inst->args); i++)
        {
            ASTNode *arg = (ASTNode *)list_get(inst->args, i);
            ast_accept(arg, visitor);
        }

        pv->indent_level -= 2;
    }

    return NULL;
}

static void *visit_method_access_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    MethodAccessNode *method = (MethodAccessNode *)node;

    print_indent(pv->indent_level);

    printf("MethodAccess: %s [%s]\n",
           method->method_name, method->base.return_type ? method->base.return_type->name : "?");

    pv->indent_level++;

    print_indent(pv->indent_level);
    printf("Target:\n");

    pv->indent_level++;
    ast_accept(method->target, visitor);
    pv->indent_level--;

    if (list_count(method->args) > 0)
    {
        print_indent(pv->indent_level);
        printf("Args:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(method->args); i++)
        {
            ASTNode *arg = (ASTNode *)list_get(method->args, i);
            ast_accept(arg, visitor);
        }

        pv->indent_level--;
    }

    pv->indent_level--;
    return NULL;
}

static void *visit_program_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    ProgramNode *program = (ProgramNode *)node;

    print_indent(pv->indent_level);

    printf("ProgramNode [%s]:\n",
           program->base.return_type ? program->base.return_type->name : "?");

    pv->indent_level++;

    if (list_count(program->type_definitions) > 0)
    {
        print_indent(pv->indent_level);
        printf("type_definitions:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(program->type_definitions); i++)
        {
            ASTNode *type_def = (ASTNode *)list_get(program->type_definitions, i);
            ast_accept(type_def, visitor);
        }

        pv->indent_level--;
    }

    if (list_count(program->function_definitions) > 0)
    {
        print_indent(pv->indent_level);
        printf("function_definitions:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(program->function_definitions); i++)
        {
            ASTNode *def = (ASTNode *)list_get(program->function_definitions, i);
            ast_accept(def, visitor);
        }

        pv->indent_level--;
    }

    print_indent(pv->indent_level);
    printf("root:\n");

    pv->indent_level++;
    ast_accept(program->root, visitor);
    pv->indent_level--;

    pv->indent_level--;
    return NULL;
}

static void *visit_base_call_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    BaseCallNode *base_call = (BaseCallNode *)node;

    print_indent(pv->indent_level);

    printf("BaseCall [%s]\n",
           base_call->base.return_type ? base_call->base.return_type->name : "?");

    if (list_count(base_call->args) > 0)
    {
        pv->indent_level++;

        print_indent(pv->indent_level);
        printf("Args:\n");

        pv->indent_level++;

        for (size_t i = 0; i < list_count(base_call->args); i++)
        {
            ASTNode *arg = (ASTNode *)list_get(base_call->args, i);
            ast_accept(arg, visitor);
        }
        
        pv->indent_level -= 2;
    }

    return NULL;
}

static void *visit_is_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    IsNode *is_node = (IsNode *)node;

    print_indent(pv->indent_level);

    printf("Is: %s [%s]\n",
           is_node->type_name, is_node->base.return_type ? is_node->base.return_type->name : "?");

    pv->indent_level++;
    ast_accept(is_node->target, visitor);
    pv->indent_level--;

    return NULL;
}

static void *visit_as_node(Visitor *visitor, ASTNode *node)
{
    PrintVisitor *pv = (PrintVisitor *)visitor;
    AsNode *as_node = (AsNode *)node;

    print_indent(pv->indent_level);

    printf("As: %s [%s]\n",
        as_node->type_name, as_node->base.return_type ? as_node->base.return_type->name : "?");
        
    pv->indent_level++;
    ast_accept(as_node->target, visitor);
    pv->indent_level--;

    return NULL;
}

PrintVisitor *print_visitor_create(void)
{
    PrintVisitor *visitor = (PrintVisitor *)malloc(sizeof(PrintVisitor));
    if (!visitor)
        return NULL;

    visitor_init(&visitor->base);

    VisitorFunc funcs[] = { (VisitorFunc)visit_literal_node, (VisitorFunc)visit_unary_operation_node, (VisitorFunc)visit_binary_operation_node, (VisitorFunc)visit_expression_block_node, (VisitorFunc)visit_conditional_node, (VisitorFunc)visit_while_loop_node, (VisitorFunc)visit_let_in_node, (VisitorFunc)visit_variable_reference_node, (VisitorFunc)visit_reassignment_node, (VisitorFunc)visit_function_definition_node, (VisitorFunc)visit_function_call_node, (VisitorFunc)visit_type_definition_node, (VisitorFunc)visit_attribute_access_node, (VisitorFunc)visit_method_access_node, (VisitorFunc)visit_type_instanciation_node, (VisitorFunc)visit_base_call_node, (VisitorFunc)visit_is_node, (VisitorFunc)visit_as_node, (VisitorFunc)visit_program_node };
    visitor_register_functions(&visitor->base, funcs);

    visitor->indent_level = 0;
    return visitor;
}

void print_visitor_destroy(PrintVisitor *visitor)
{
    if (!visitor)
        return;

    visitor_destroy(&visitor->base);
    free(visitor);
}