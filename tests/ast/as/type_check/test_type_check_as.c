#include <CUnit/Basic.h>
#include "type_check_visitor.h"
#include "type_inference_visitor.h"
#include "free_visitor.h"

static size_t run_inference_and_check(ASTNode *node)
{
    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept(node, (Visitor *)iv);
    type_inference_visitor_destroy(iv);

    if (!dm_has_errors(dm_global))
    {
        TypeCheckVisitor *cv = type_check_visitor_create();
        ast_accept(node, (Visitor *)cv);
        type_check_visitor_destroy(cv);
    }

    return dm_error_count(dm_global);
}

void test_check_as_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_as_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A {} let x: A = new A() in x as A\n");

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 16);
    list_append(bindings, symbol_binding_create("x", "A", init, 2, 5));

    VariableReferenceNode *x_ref = ast_variable_reference_create("x", 2, 21);
    AsNode *as_node = ast_as_create((ASTNode *)x_ref, "A", 2, 23);
    LetInNode *let = ast_let_create(bindings, (ASTNode *)as_node, 2, 1);

    ProgramNode *program = ast_program_create((ASTNode *)let, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de x: %s\n", x_ref->base.return_type->name);
    printf("Tipo de x as A: %s\n", as_node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_as_subtype(void)
{
    printf("\n\n------------ Inicio test: test_check_as_subtype ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A {} type B inherits A {} let x: A = new B() in x as B\n");

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_type_instanciation_create("B", list_create(0, NULL, NULL, NULL, NULL), 2, 16);
    list_append(bindings, symbol_binding_create("x", "A", init, 2, 5));

    VariableReferenceNode *x_ref = ast_variable_reference_create("x", 2, 21);
    AsNode *as_node = ast_as_create((ASTNode *)x_ref, "B", 2, 23);
    LetInNode *let = ast_let_create(bindings, (ASTNode *)as_node, 2, 1);

    ProgramNode *program = ast_program_create((ASTNode *)let, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de x: %s\n", x_ref->base.return_type->name);
    printf("Tipo de x as B: %s\n", as_node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_as_with_is_guard(void)
{
    printf("\n\n------------ Inicio test: test_check_as_with_is_guard ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A {} type B inherits A {} let x: A = new B() in if (x is B) x as B else x\n");

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_type_instanciation_create("B", list_create(0, NULL, NULL, NULL, NULL), 2, 16);
    list_append(bindings, symbol_binding_create("x", "A", init, 2, 5));

    VariableReferenceNode *x1 = ast_variable_reference_create("x", 2, 25);
    IsNode *is_check = ast_is_create((ASTNode *)x1, "B", 2, 27);

    VariableReferenceNode *x2 = ast_variable_reference_create("x", 2, 36);
    AsNode *as_node = ast_as_create((ASTNode *)x2, "B", 2, 38);

    VariableReferenceNode *x3 = ast_variable_reference_create("x", 2, 46);
    ConditionalNode *cond = ast_conditional_create((ASTNode *)is_check, (ASTNode *)as_node, (ASTNode *)x3, 2, 20);

    LetInNode *let = ast_let_create(bindings, (ASTNode *)cond, 2, 1);

    ProgramNode *program = ast_program_create((ASTNode *)let, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de x: %s\n", x1->base.return_type->name);
    printf("Tipo de x is B: %s\n", is_check->base.return_type->name);
    printf("Tipo de x as B: %s\n", as_node->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_as_undefined_type(void)
{
    printf("\n\n------------ Inicio test: test_check_as_undefined_type ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: 42 as Foo (tipo no definido)\n");

    LiteralNode *num = ast_number_literal_create(42.0, 1, 1);
    AsNode *as_node = ast_as_create((ASTNode *)num, "Foo", 1, 4);

    ProgramNode *program = ast_program_create((ASTNode *)as_node,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo del target: %s\n", num->base.return_type->name);
    printf("Tipo de 42 as Foo: %s\n", as_node->base.return_type ? as_node->base.return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT(errors >= 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_as_chain(void)
{
    printf("\n\n------------ Inicio test: test_check_as_chain ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A {} type B inherits A {} type C inherits B {} let x: A = new C() in (x as B) as C\n");

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    TypeDefinitionNode *c_def = ast_type_definition_create("C",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "B",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);
    list_append(types, c_def);

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_type_instanciation_create("C", list_create(0, NULL, NULL, NULL, NULL), 2, 16);
    list_append(bindings, symbol_binding_create("x", "A", init, 2, 5));

    VariableReferenceNode *x_ref = ast_variable_reference_create("x", 2, 21);
    AsNode *as_b = ast_as_create((ASTNode *)x_ref, "B", 2, 24);
    AsNode *as_c = ast_as_create((ASTNode *)as_b, "C", 2, 31);
    LetInNode *let = ast_let_create(bindings, (ASTNode *)as_c, 2, 1);

    ProgramNode *program = ast_program_create((ASTNode *)let, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de x: %s\n", x_ref->base.return_type->name);
    printf("Tipo de x as B: %s\n", as_b->base.return_type->name);
    printf("Tipo de (x as B) as C: %s\n", as_c->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_as_error_in_target(void)
{
    printf("\n\n------------ Inicio test: test_check_as_error_in_target ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A {} (true + 1) as A (error en el target)\n");

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);

    LiteralNode *bad_l = ast_bool_literal_create(true, 2, 2);
    LiteralNode *bad_r = ast_number_literal_create(1.0, 2, 8);
    ASTNode *bad_target = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)bad_l, (ASTNode *)bad_r, 2, 6);
    AsNode *as_node = ast_as_create(bad_target, "A", 2, 11);

    ProgramNode *program = ast_program_create((ASTNode *)as_node, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo del target: %s\n", bad_target->return_type->name);
    printf("Tipo de as: %s\n", as_node->base.return_type ? as_node->base.return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT(errors >= 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de AsNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AsNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_as_correct", test_check_as_correct);
    CU_add_test(suite, "test_check_as_subtype", test_check_as_subtype);
    CU_add_test(suite, "test_check_as_with_is_guard", test_check_as_with_is_guard);
    CU_add_test(suite, "test_check_as_undefined_type", test_check_as_undefined_type);
    CU_add_test(suite, "test_check_as_chain", test_check_as_chain);
    CU_add_test(suite, "test_check_as_error_in_target", test_check_as_error_in_target);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}