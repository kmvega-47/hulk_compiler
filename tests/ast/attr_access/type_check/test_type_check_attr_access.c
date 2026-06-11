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

void test_check_attribute_access_number(void)
{
    printf("\n\n------------ Inicio test: test_check_attribute_access_number ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { x: Number = 0; getX() => self.x; }\n");

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *self_x = (ASTNode *)ast_attribute_access_create(
        (ASTNode *)ast_variable_reference_create("self", 1, 55), "x", 1, 60);
    FunctionDefinitionNode *method = ast_function_definition_create("getX", m_params, NULL, self_x, 1, 45);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(attrs, symbol_binding_create("x", "Number", (ASTNode *)ast_number_literal_create(0.0, 1, 30), 1, 12));

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        attrs, methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 2, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "getX", args, 2, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de self.x: %s\n", self_x->return_type ? self_x->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_attribute_access_string(void)
{
    printf("\n\n------------ Inicio test: test_check_attribute_access_string ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { name: String = \"\"; getName() => self.name; }\n");

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *self_name = (ASTNode *)ast_attribute_access_create(
        (ASTNode *)ast_variable_reference_create("self", 2, 55), "name", 2, 62);
    FunctionDefinitionNode *method = ast_function_definition_create("getName", m_params, NULL, self_name, 2, 45);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(attrs, symbol_binding_create("name", "String", (ASTNode *)ast_string_literal_create("", 2, 30), 2, 12));

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        attrs, methods, 2, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 3, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "getName", args, 3, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de self.name: %s\n", self_name->return_type ? self_name->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_attribute_access_bool(void)
{
    printf("\n\n------------ Inicio test: test_check_attribute_access_bool ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { flag: Bool = true; getFlag() => self.flag; }\n");

    List *m_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *self_flag = (ASTNode *)ast_attribute_access_create(
        (ASTNode *)ast_variable_reference_create("self", 3, 55), "flag", 3, 62);
    FunctionDefinitionNode *method = ast_function_definition_create("getFlag", m_params, NULL, self_flag, 3, 45);

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    list_append(methods, method);

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(attrs, symbol_binding_create("flag", "Bool", (ASTNode *)ast_bool_literal_create(true, 3, 30), 3, 12));

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        attrs, methods, 3, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *instance = (ASTNode *)ast_type_instanciation_create("A", list_create(0, NULL, NULL, NULL, NULL), 4, 5);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *call = (ASTNode *)ast_method_access_create(instance, "getFlag", args, 4, 7);

    ProgramNode *program = ast_program_create(call, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de self.flag: %s\n", self_flag->return_type ? self_flag->return_type->name : "NULL");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de AttributeAccessNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("AttributeAccessNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_attribute_access_number", test_check_attribute_access_number);
    CU_add_test(suite, "test_check_attribute_access_string", test_check_attribute_access_string);
    CU_add_test(suite, "test_check_attribute_access_bool", test_check_attribute_access_bool);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}