#include <CUnit/Basic.h>
#include "type_check_visitor.h"
#include "type_inference_visitor.h"
#include "free_visitor.h"
#include "print_visitor.h"
#include "function_table.h"

static size_t run_inference_and_check(ASTNode *node)
{
    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept(node, (Visitor *)iv);
    type_inference_visitor_destroy(iv);

    printf("\n--- AST despues de inferencia ---\n");
    PrintVisitor *pv = print_visitor_create();
    ast_accept(node, (Visitor *)pv);
    print_visitor_destroy(pv);

    if (!dm_has_errors(dm_global))
    {
        TypeCheckVisitor *cv = type_check_visitor_create();
        ast_accept(node, (Visitor *)cv);
        type_check_visitor_destroy(cv);
    }

    return dm_error_count(dm_global);
}

void test_check_function_definition_return_annotation_incorrect(void)
{
    printf("\n\n------------ Inicio test: test_check_function_definition_return_annotation_incorrect ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: function bad(x: Number): Bool => x + 1\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params, symbol_binding_create("x", "Number", NULL, 1, 17));

    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 1, 35);
    ASTNode *one = (ASTNode *)ast_number_literal_create(1.0, 1, 39);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_ADD, x_ref, one, 1, 37);
    FunctionDefinitionNode *f = ast_function_definition_create("bad", params, "Bool", body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(1.0, 2, 5));
    ASTNode *root = (ASTNode *)ast_function_call_create("bad", args, 2, 1);

    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_function_definition_error_in_body(void)
{
    printf("\n\n------------ Inicio test: test_check_function_definition_error_in_body ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: function f() => true + 1\n");

    List *params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    LiteralNode *bad_l = ast_bool_literal_create(true, 1, 18);
    LiteralNode *bad_r = ast_number_literal_create(1.0, 1, 24);
    ASTNode *body = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)bad_l, (ASTNode *)bad_r, 1, 22);
    FunctionDefinitionNode *f = ast_function_definition_create("f", params, NULL, body, 1, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, f);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_function_call_create("f", args, 2, 1);

    ProgramNode *program = ast_program_create(root, funcs, list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de FunctionDefinitionNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionDefinitionNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_function_definition_return_annotation_incorrect", test_check_function_definition_return_annotation_incorrect);
    CU_add_test(suite, "test_check_function_definition_error_in_body", test_check_function_definition_error_in_body);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}