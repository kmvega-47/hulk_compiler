#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_program_minimal(void)
{
    printf("\n\n------------ Inicio test: test_inference_program_minimal ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Programa: 42; (linea 1)\n");

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 1, 1);
    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(program->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Tipo del programa: Number\n");

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

void test_inference_program_functions(void)
{
    printf("\n\n------------ Inicio test: test_inference_program_functions ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Programa con funciones: foo(), bar(x: Number): Number, main() que llama a ambas\n");

    List *params_foo = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body_foo = (ASTNode *)ast_number_literal_create(10.0, 1, 20);
    FunctionDefinitionNode *foo = ast_function_definition_create("foo", params_foo, NULL, body_foo, 1, 1);

    List *params_bar = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params_bar, symbol_binding_create("x", "Number", NULL, 2, 14));
    ASTNode *x_ref = (ASTNode *)ast_variable_reference_create("x", 2, 30);
    ASTNode *two = (ASTNode *)ast_number_literal_create(2.0, 2, 34);
    ASTNode *body_bar = (ASTNode *)ast_binary_create(OP_MULT, x_ref, two, 2, 32);
    FunctionDefinitionNode *bar = ast_function_definition_create("bar", params_bar, "Number", body_bar, 2, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, foo);
    list_append(funcs, bar);

    List *root_exprs = list_create(0, NULL, NULL, NULL, NULL);
    List *args_foo_call = list_create(0, NULL, NULL, NULL, NULL);
    FunctionCallNode *call_foo = ast_function_call_create("foo", args_foo_call, 3, 1);
    list_append(root_exprs, (ASTNode *)call_foo);

    List *args_bar_call = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args_bar_call, ast_number_literal_create(5.0, 3, 6));
    FunctionCallNode *call_bar = ast_function_call_create("bar", args_bar_call, 3, 4);
    list_append(root_exprs, (ASTNode *)call_bar);

    ASTNode *root = (ASTNode *)ast_block_create(root_exprs, 3, 1);
    List *types = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_EQUAL(foo->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    CU_ASSERT_PTR_EQUAL(bar->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_VOID));
    printf("Tipos de definiciones: Void\n");

    bool found = false;
    TypeDescriptor *ret_foo = function_table_get_return_type(global_function_table, "foo", &found);
    CU_ASSERT_TRUE(found);
    CU_ASSERT_PTR_EQUAL(ret_foo, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Retorno de foo: Number\n");

    TypeDescriptor *ret_bar = function_table_get_return_type(global_function_table, "bar", &found);
    CU_ASSERT_TRUE(found);
    CU_ASSERT_PTR_EQUAL(ret_bar, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Retorno de bar: Number\n");

    CU_ASSERT_PTR_EQUAL(call_foo->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(call_bar->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Tipos de llamadas: Number, Number\n");

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

void test_inference_program_types_and_methods(void)
{
    printf("\n\n------------ Inicio test: test_inference_program_types_and_methods ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Programa con tipo Point(x: Number, y: Number) { ... } y expresion new + llamadas a metodos\n");

    List *init_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(init_params, symbol_binding_create("x", "Number", NULL, 1, 12));
    list_append(init_params, symbol_binding_create("y", "Number", NULL, 1, 24));

    List *attributes = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init_x = (ASTNode *)ast_variable_reference_create("x", 1, 30);
    ASTNode *init_y = (ASTNode *)ast_variable_reference_create("y", 1, 42);
    list_append(attributes, symbol_binding_create("x", NULL, init_x, 1, 5));
    list_append(attributes, symbol_binding_create("y", NULL, init_y, 1, 11));

    List *methods = list_create(0, NULL, NULL, NULL, NULL);

    List *params_getX = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *self_x = (ASTNode *)ast_attribute_access_create(
        (ASTNode *)ast_variable_reference_create("self", 1, 60), "x", 1, 65);
    FunctionDefinitionNode *getX = ast_function_definition_create("getX", params_getX, NULL, self_x, 1, 50);
    list_append(methods, getX);

    List *params_setX = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(params_setX, symbol_binding_create("v", "Number", NULL, 1, 77));
    ASTNode *self_x_target = (ASTNode *)ast_attribute_access_create(
        (ASTNode *)ast_variable_reference_create("self", 1, 85), "x", 1, 90);
    ASTNode *v_ref = (ASTNode *)ast_variable_reference_create("v", 1, 95);
    ASTNode *body_setX = (ASTNode *)ast_reassignment_create(self_x_target, v_ref, 1, 87);
    FunctionDefinitionNode *setX = ast_function_definition_create("setX", params_setX, NULL, body_setX, 1, 70);
    list_append(methods, setX);

    TypeDefinitionNode *point_type = ast_type_definition_create("Point", init_params, NULL,
        list_create(0, NULL, NULL, NULL, NULL), attributes, methods, 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, point_type);

    List *args_new = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args_new, ast_number_literal_create(3.0, 2, 15));
    list_append(args_new, ast_number_literal_create(4.0, 2, 18));
    TypeInstanciationNode *new_point = ast_type_instanciation_create("Point", args_new, 2, 5);

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(bindings, symbol_binding_create("p", NULL, (ASTNode *)new_point, 2, 5));

    ASTNode *p_ref1 = (ASTNode *)ast_variable_reference_create("p", 2, 25);
    List *args_getX = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *call_getX = ast_method_access_create(p_ref1, "getX", args_getX, 2, 27);

    ASTNode *p_ref2 = (ASTNode *)ast_variable_reference_create("p", 2, 38);
    List *args_setX = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args_setX, ast_number_literal_create(10.0, 2, 45));
    MethodAccessNode *call_setX = ast_method_access_create(p_ref2, "setX", args_setX, 2, 40);

    ASTNode *p_ref3 = (ASTNode *)ast_variable_reference_create("p", 2, 50);
    List *args_getX2 = list_create(0, NULL, NULL, NULL, NULL);
    MethodAccessNode *call_getX2 = ast_method_access_create(p_ref3, "getX", args_getX2, 2, 52);

    List *body_exprs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(body_exprs, (ASTNode *)call_getX);
    list_append(body_exprs, (ASTNode *)call_setX);
    list_append(body_exprs, (ASTNode *)call_getX2);
    ASTNode *block = (ASTNode *)ast_block_create(body_exprs, 2, 22);
    ASTNode *let_in = (ASTNode *)ast_let_create(bindings, block, 2, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    ProgramNode *program = ast_program_create(let_in, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    TypeDescriptor *pt = type_table_lookup_by_name(global_type_table, "Point");
    CU_ASSERT_PTR_NOT_NULL(pt);
    CU_ASSERT_EQUAL(pt->tag, HULK_USER_DEFINED);
    printf("Tipo Point registrado\n");

    CU_ASSERT_PTR_EQUAL(call_getX->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(call_setX->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(call_getX2->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Tipos de metodos: Number, Number, Number\n");

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

void test_inference_program_errors(void)
{
    printf("\n\n------------ Inicio test: test_inference_program_errors ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Programa con errores: funcion duplicada, tipo duplicado, herencia invalida\n");

    List *params_foo1 = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body_foo1 = (ASTNode *)ast_number_literal_create(1.0, 1, 20);
    FunctionDefinitionNode *foo1 = ast_function_definition_create("foo", params_foo1, NULL, body_foo1, 1, 1);

    List *params_foo2 = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body_foo2 = (ASTNode *)ast_number_literal_create(2.0, 2, 20);
    FunctionDefinitionNode *foo2 = ast_function_definition_create("foo", params_foo2, NULL, body_foo2, 2, 1);

    List *funcs = list_create(0, NULL, NULL, NULL, NULL);
    list_append(funcs, foo1);
    list_append(funcs, foo2);

    TypeDefinitionNode *typeA = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 3, 1);

    TypeDefinitionNode *typeA_dup = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 4, 1);

    TypeDefinitionNode *typeB = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), "NoExiste",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 5, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, typeA);
    list_append(types, typeA_dup);
    list_append(types, typeB);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 6, 1);
    ProgramNode *program = ast_program_create(root, funcs, types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu\n", dm_error_count(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 3);

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
    printf("\n=== Iniciando tests de ProgramNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ProgramNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_program_minimal", test_inference_program_minimal);
    CU_add_test(suite, "test_inference_program_functions", test_inference_program_functions);
    CU_add_test(suite, "test_inference_program_types_and_methods", test_inference_program_types_and_methods);
    CU_add_test(suite, "test_inference_program_errors", test_inference_program_errors);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}