#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_collector_base_call_no_args(void)
{
    printf("\n\n------------ Inicio test: test_collector_base_call_no_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { foo() => 1; } type B inherits A { foo() => base(); }  f(x) => new B().foo()\n");

    // type A { foo() => 1; }
    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *a_body = (ASTNode *)ast_number_literal_create(1.0, 2, 12);
    list_append(a_methods, ast_function_definition_create("foo", a_params, "Number", a_body, 2, 5));

    TypeDefinitionNode *typeA = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    // type B inherits A { foo() => base(); }
    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *base_args = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *base_call = ast_base_call_create(base_args, 5, 18);
    list_append(b_methods, ast_function_definition_create("foo", b_params, NULL, (ASTNode *)base_call, 5, 5));

    TypeDefinitionNode *typeB = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 4, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, typeA);
    list_append(types, typeB);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 7, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_NOT_NULL(base_call->base.return_type);
    printf("Tipo inferido de base(): %s (esperado: Number)\n", base_call->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(base_call->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_STRING_EQUAL(base_call->type_name, "B");
    CU_ASSERT_STRING_EQUAL(base_call->method_name, "foo");
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

void test_collector_base_call_with_args(void)
{
    printf("\n\n------------ Inicio test: test_collector_base_call_with_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { add(x: Number) => x + 1; } type B inherits A { add(x: Number) => base(x); }  f(p) => new B().add(p)\n");

    // type A { add(x: Number) => x + 1; }
    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_params, symbol_binding_create("x", "Number", NULL, 2, 10));
    ASTNode *a_x = (ASTNode *)ast_variable_reference_create("x", 2, 22);
    ASTNode *a_body = (ASTNode *)ast_binary_create(OP_ADD, a_x, (ASTNode *)ast_number_literal_create(1.0, 2, 26), 2, 24);
    list_append(a_methods, ast_function_definition_create("add", a_params, "Number", a_body, 2, 5));

    TypeDefinitionNode *typeA = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    // type B inherits A { add(x: Number) => base(x); }
    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(b_params, symbol_binding_create("x", "Number", NULL, 5, 10));
    List *b_args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *b_x = (ASTNode *)ast_variable_reference_create("x", 5, 22);
    list_append(b_args, b_x);
    BaseCallNode *base_call2 = ast_base_call_create(b_args, 5, 18);
    list_append(b_methods, ast_function_definition_create("add", b_params, NULL, (ASTNode *)base_call2, 5, 5));

    TypeDefinitionNode *typeB = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 4, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, typeA);
    list_append(types, typeB);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 7, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_NOT_NULL(base_call2->base.return_type);
    printf("Tipo inferido de base(x): %s (esperado: Number)\n", base_call2->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(base_call2->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

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

void test_collector_base_call_with_unannotated_param(void)
{
    printf("\n\n------------ Inicio test: test_collector_base_call_with_unannotated_param ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { add(x: Number) => x; } type B inherits A { add(y) => base(y); }  -->  y Number\n");

    // type A { add(x: Number) => x; }
    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *a_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_params, symbol_binding_create("x", "Number", NULL, 2, 10));
    ASTNode *a_x = (ASTNode *)ast_variable_reference_create("x", 2, 22);
    list_append(a_methods, ast_function_definition_create("add", a_params, "Number", a_x, 2, 5));

    TypeDefinitionNode *typeA = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    // type B inherits A { add(y) => base(y); }
    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(b_params, symbol_binding_create("y", NULL, NULL, 5, 10));
    List *b_args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *b_y = (ASTNode *)ast_variable_reference_create("y", 5, 22);
    list_append(b_args, b_y);
    BaseCallNode *base_call3 = ast_base_call_create(b_args, 5, 18);
    list_append(b_methods, ast_function_definition_create("add", b_params, NULL, (ASTNode *)base_call3, 5, 5));

    TypeDefinitionNode *typeB = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 4, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, typeA);
    list_append(types, typeB);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 7, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    // Verificar que el parámetro y de B.add se infirió como Number
    char *full_name = function_table_build_method_name("B", "add");
    List *param_types = function_table_get_params_types(global_function_table, full_name);
    free(full_name);
    CU_ASSERT_PTR_NOT_NULL(param_types);
    CU_ASSERT_EQUAL(list_count(param_types), 1);
    TypeDescriptor *y_type = (TypeDescriptor *)list_get(param_types, 0);
    printf("Tipo inferido para y: %s (esperado: Number)\n", y_type ? y_type->name : "NULL");
    CU_ASSERT_PTR_EQUAL(y_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_PTR_NOT_NULL(base_call3->base.return_type);
    printf("Tipo inferido de base(y): %s (esperado: Number)\n", base_call3->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(base_call3->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

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

void test_collector_base_call_no_ancestor(void)
{
    printf("\n\n------------ Inicio test: test_collector_base_call_no_ancestor ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("type A { } type B inherits A { foo() => base(); }  -->  error\n");

    List *a_methods = list_create(0, NULL, NULL, NULL, NULL);
    TypeDefinitionNode *typeA = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        a_methods, 1, 1);

    List *b_methods = list_create(0, NULL, NULL, NULL, NULL);
    List *b_params = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    List *b_args = list_create(0, NULL, NULL, NULL, NULL);
    BaseCallNode *base_call4 = ast_base_call_create(b_args, 4, 15);
    list_append(b_methods, ast_function_definition_create("foo", b_params, NULL, (ASTNode *)base_call4, 4, 5));

    TypeDefinitionNode *typeB = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL), "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        b_methods, 3, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, typeA);
    list_append(types, typeB);

    ASTNode *root = (ASTNode *)ast_number_literal_create(0.0, 6, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)iv);

    CU_ASSERT_PTR_NULL(base_call4->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Tipo inferido: NULL, Errores: %zu (esperado: >= 1)\n", dm_error_count(dm_global));
    CU_ASSERT(dm_error_count(dm_global) >= 1);

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
    printf("\n=== Iniciando tests de ConstraintCollector (base_call) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ConstraintCollector BaseCall Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_collector_base_call_no_args", test_collector_base_call_no_args);
    CU_add_test(suite, "test_collector_base_call_with_args", test_collector_base_call_with_args);
    CU_add_test(suite, "test_collector_base_call_with_unannotated_param", test_collector_base_call_with_unannotated_param);
    CU_add_test(suite, "test_collector_base_call_no_ancestor", test_collector_base_call_no_ancestor);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}