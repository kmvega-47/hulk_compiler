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

void test_check_reassignment_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_reassignment_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x = 1 in x := 42\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(1.0, 1, 9);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));

    VariableReferenceNode *target = ast_variable_reference_create("x", 1, 14);
    ASTNode *value = (ASTNode *)ast_number_literal_create(42.0, 1, 18);
    ReassignmentNode *reassign = ast_reassignment_create((ASTNode *)target, value, 1, 16);

    LetInNode *let = ast_let_create(bindings, (ASTNode *)reassign, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)let);

    printf("Tipo target: %s\n", target->base.return_type->name);
    printf("Tipo value: %s\n", value->return_type->name);
    printf("Tipo reassign: %s\n", reassign->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)let, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_reassignment_incompatible(void)
{
    printf("\n\n------------ Inicio test: test_check_reassignment_incompatible ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: let x: Number = 1 in x := true\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(1.0, 1, 19);
    list_append(bindings, symbol_binding_create("x", "Number", init, 1, 5));

    VariableReferenceNode *target = ast_variable_reference_create("x", 1, 24);
    ASTNode *value = (ASTNode *)ast_bool_literal_create(true, 1, 28);
    ReassignmentNode *reassign = ast_reassignment_create((ASTNode *)target, value, 1, 26);

    LetInNode *let = ast_let_create(bindings, (ASTNode *)reassign, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)let);

    printf("Tipo target: %s\n", target->base.return_type->name);
    printf("Tipo value: %s\n", value->return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)let, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_reassignment_to_attribute_correct(void)
{
    printf("\n\n------------ Inicio test: test_check_reassignment_to_attribute_correct ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { x = 0; set(v) => self.x := v; }\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init0 = (ASTNode *)ast_number_literal_create(0.0, 1, 15);
    list_append(attrs, symbol_binding_create("x", NULL, init0, 1, 12));

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(m_params, symbol_binding_create("v", NULL, NULL, 1, 22));

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 1, 35);
    ASTNode *self_x = (ASTNode *)ast_attribute_access_create(self_ref, "x", 1, 40);
    ASTNode *v_ref = (ASTNode *)ast_variable_reference_create("v", 1, 45);
    ASTNode *m_body = (ASTNode *)ast_reassignment_create(self_x, v_ref, 1, 42);

    FunctionDefinitionNode *method = ast_function_definition_create("set", m_params, NULL, m_body, 1, 18);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attrs, methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_number_literal_create(42.0, 2, 8));
    ASTNode *root = (ASTNode *)ast_method_access_create(instance, "set", args, 2, 3);

    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de self.x: %s\n", self_x->return_type->name);
    printf("Tipo de v: %s\n", v_ref->return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_reassignment_to_attribute_incompatible(void)
{
    printf("\n\n------------ Inicio test: test_check_reassignment_to_attribute_incompatible ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { x: Number = 0; set(v: String) => self.x := v; }\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init0 = (ASTNode *)ast_number_literal_create(0.0, 1, 25);
    list_append(attrs, symbol_binding_create("x", "Number", init0, 1, 12));

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(m_params, symbol_binding_create("v", "String", NULL, 1, 32));

    ASTNode *self_ref = (ASTNode *)ast_variable_reference_create("self", 1, 53);
    ASTNode *self_x = (ASTNode *)ast_attribute_access_create(self_ref, "x", 1, 58);
    ASTNode *v_ref = (ASTNode *)ast_variable_reference_create("v", 1, 63);
    ASTNode *m_body = (ASTNode *)ast_reassignment_create(self_x, v_ref, 1, 60);

    FunctionDefinitionNode *method = ast_function_definition_create("set", m_params, NULL, m_body, 1, 28);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attrs, methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 1);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, ast_string_literal_create("hello", 2, 8));
    ASTNode *root = (ASTNode *)ast_method_access_create(instance, "set", args, 2, 3);

    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de self.x: %s\n", self_x->return_type->name);
    printf("Tipo de v: %s\n", v_ref->return_type->name);

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
    printf("\n=== Iniciando tests de ReassignmentNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ReassignmentNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_reassignment_correct", test_check_reassignment_correct);
    CU_add_test(suite, "test_check_reassignment_incompatible", test_check_reassignment_incompatible);
    CU_add_test(suite, "test_check_reassignment_to_attribute_correct", test_check_reassignment_to_attribute_correct);
    CU_add_test(suite, "test_check_reassignment_to_attribute_incompatible", test_check_reassignment_to_attribute_incompatible);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}