#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_base_call_simple(void)
{
    printf("\n\n------------ Inicio test: test_inference_base_call_simple ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: type Person { name() => \"Name\"; } type Knight inherits Person { name() => \"Sir\" @@ base(); } Knight.name()\n");

    List *person_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *person_params_empty = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *person_body = (ASTNode *)ast_string_literal_create("Name", 2, 14);
    FunctionDefinitionNode *person_method = ast_function_definition_create("name", person_params_empty, "String", person_body, 2, 5);
    list_append(person_methods, person_method);

    List *person_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *person_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *person_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *person_type = ast_type_definition_create("Person", person_init, NULL, person_father_args, person_attrs, person_methods, 1, 1);

    List *knight_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *knight_params_empty = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *base_args = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *base_call = ast_base_call_create(base_args, 5, 30);
    ASTNode *knight_body = (ASTNode *)ast_binary_create(OP_CONCAT_WS, (ASTNode *)ast_string_literal_create("Sir", 5, 18), (ASTNode *)base_call, 5, 28);
    FunctionDefinitionNode *knight_method = ast_function_definition_create("name", knight_params_empty, "String", knight_body, 5, 5);
    list_append(knight_methods, knight_method);

    List *knight_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *knight_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *knight_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *knight_type = ast_type_definition_create("Knight", knight_init, "Person", knight_father_args, knight_attrs, knight_methods, 4, 1);

    List *type_defs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(type_defs, person_type);
    list_append(type_defs, knight_type);

    List *func_defs = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 10, 1);
    ProgramNode *program = ast_program_create(root, func_defs, type_defs, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_NOT_NULL(base_call->base.return_type);
    printf("Tipo inferido de base(): %s (esperado: String)\n", base_call->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(base_call->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));

    CU_ASSERT_PTR_NOT_NULL(base_call->type_name);
    CU_ASSERT_PTR_NOT_NULL(base_call->method_name);
    CU_ASSERT_STRING_EQUAL(base_call->type_name, "Knight");
    CU_ASSERT_STRING_EQUAL(base_call->method_name, "name");
    printf("type_name: %s, method_name: %s\n", base_call->type_name, base_call->method_name);

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_base_call_with_args(void)
{
    printf("\n\n------------ Inicio test: test_inference_base_call_with_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: base con argumentos\n");

    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_params, symbol_binding_create("x", "Number", NULL, 2, 10));
    ASTNode *a_body = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)ast_variable_reference_create("x", 2, 22), (ASTNode *)ast_number_literal_create(1.0, 2, 26), 2, 24);
    FunctionDefinitionNode *a_method = ast_function_definition_create("foo", a_params, "Number", a_body, 2, 5);
    list_append(a_methods, a_method);

    List *a_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *a_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *a_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *a_type = ast_type_definition_create("A", a_init, NULL, a_father_args, a_attrs, a_methods, 1, 1);

    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(b_params, symbol_binding_create("x", "Number", NULL, 5, 10));
    List *b_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(b_args, ast_variable_reference_create("x", 5, 28));
    BaseCallNode *base_call2 = ast_base_call_create(b_args, 5, 25);
    ASTNode *b_body = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)base_call2, (ASTNode *)ast_number_literal_create(2.0, 5, 32), 5, 30);
    FunctionDefinitionNode *b_method = ast_function_definition_create("foo", b_params, "Number", b_body, 5, 5);
    list_append(b_methods, b_method);

    List *b_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *b_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *b_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *b_type = ast_type_definition_create("B", b_init, "A", b_father_args, b_attrs, b_methods, 4, 1);

    List *type_defs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(type_defs, a_type);
    list_append(type_defs, b_type);

    List *func_defs = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 10, 1);
    ProgramNode *program = ast_program_create(root, func_defs, type_defs, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_NOT_NULL(base_call2->base.return_type);
    printf("Tipo inferido de base(x): %s (esperado: Number)\n", base_call2->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(base_call2->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_STRING_EQUAL(base_call2->type_name, "B");
    CU_ASSERT_STRING_EQUAL(base_call2->method_name, "foo");
    printf("type_name: %s, method_name: %s\n", base_call2->type_name, base_call2->method_name);

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_base_call_no_ancestor_method(void)
{
    printf("\n\n------------ Inicio test: test_inference_base_call_no_ancestor_method ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: base() cuando el ancestro no tiene el metodo\n");

    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *a_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *a_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *a_type = ast_type_definition_create("A", a_init, NULL, a_father_args, a_attrs, a_methods, 1, 1);

    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *b_args = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *base_call3 = ast_base_call_create(b_args, 4, 15);
    FunctionDefinitionNode *b_method = ast_function_definition_create("foo", b_params, NULL, (ASTNode *)base_call3, 4, 5);
    list_append(b_methods, b_method);

    List *b_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *b_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *b_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *b_type = ast_type_definition_create("B", b_init, "A", b_father_args, b_attrs, b_methods, 3, 1);

    List *type_defs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(type_defs, a_type);
    list_append(type_defs, b_type);

    List *func_defs = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 10, 1);
    ProgramNode *program = ast_program_create(root, func_defs, type_defs, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_NULL(base_call3->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Tipo inferido: NULL, Errores: 1 (esperado: ancestro no tiene metodo)\n");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_base_call_multilevel_inheritance(void)
{
    printf("\n\n------------ Inicio test: test_inference_base_call_multilevel_inheritance ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: herencia multinivel A -> B -> C, base() en C llama a B\n");

    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *a_body = (ASTNode *)ast_number_literal_create(1.0, 2, 14);
    FunctionDefinitionNode *a_method = ast_function_definition_create("value", a_params, "Number", a_body, 2, 5);
    list_append(a_methods, a_method);
    List *a_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *a_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *a_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *a_type = ast_type_definition_create("A", a_init, NULL, a_father_args, a_attrs, a_methods, 1, 1);

    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *b_args_empty = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *base_b = ast_base_call_create(b_args_empty, 5, 18);
    ASTNode *b_body = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)base_b, (ASTNode *)ast_number_literal_create(10.0, 5, 26), 5, 24);
    FunctionDefinitionNode *b_method = ast_function_definition_create("value", b_params, "Number", b_body, 5, 5);
    list_append(b_methods, b_method);
    List *b_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *b_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *b_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *b_type = ast_type_definition_create("B", b_init, "A", b_father_args, b_attrs, b_methods, 4, 1);

    List *c_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *c_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *c_args_empty = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *base_c = ast_base_call_create(c_args_empty, 8, 18);
    ASTNode *c_body = (ASTNode *)ast_binary_create(OP_ADD, (ASTNode *)base_c, (ASTNode *)ast_number_literal_create(100.0, 8, 26), 8, 24);
    FunctionDefinitionNode *c_method = ast_function_definition_create("value", c_params, "Number", c_body, 8, 5);
    list_append(c_methods, c_method);
    List *c_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *c_father_args = list_create(0, NULL, NULL, NULL, NULL);
    List *c_attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    TypeDefinitionNode *c_type = ast_type_definition_create("C", c_init, "B", c_father_args, c_attrs, c_methods, 7, 1);

    List *type_defs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(type_defs, a_type);
    list_append(type_defs, b_type);
    list_append(type_defs, c_type);

    List *func_defs = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 10, 1);
    ProgramNode *program = ast_program_create(root, func_defs, type_defs, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_NOT_NULL(base_c->base.return_type);
    printf("Tipo inferido de base() en C: %s (esperado: Number)\n", base_c->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(base_c->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_PTR_NOT_NULL(base_b->base.return_type);
    printf("Tipo inferido de base() en B: %s (esperado: Number)\n", base_b->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(base_b->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de BaseCallNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BaseCallNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_base_call_simple", test_inference_base_call_simple);
    CU_add_test(suite, "test_inference_base_call_with_args", test_inference_base_call_with_args);
    CU_add_test(suite, "test_inference_base_call_no_ancestor_method", test_inference_base_call_no_ancestor_method);
    CU_add_test(suite, "test_inference_base_call_multilevel_inheritance", test_inference_base_call_multilevel_inheritance);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}