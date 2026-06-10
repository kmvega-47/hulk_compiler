#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_function_call_no_args(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_call_no_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: rand() (linea 1, columna 1)\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    FunctionCallNode *node = ast_function_call_create("rand", args, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_function_call_with_arg(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_call_with_arg ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: sqrt(16) (linea 2, columna 1)\n");

    ASTNode *arg = (ASTNode *)ast_number_literal_create(16.0, 2, 6);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg);
    FunctionCallNode *node = ast_function_call_create("sqrt", args, 2, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del argumento: %s (Number)\n", arg->return_type->name);
    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_function_call_complex_arg(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_call_complex_arg ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: sqrt(4 + 5) (linea 3, columna 1)\n");

    ASTNode *sum = (ASTNode *)ast_binary_create(OP_ADD,
        (ASTNode *)ast_number_literal_create(4.0, 3, 6),
        (ASTNode *)ast_number_literal_create(5.0, 3, 10), 3, 8);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, sum);
    FunctionCallNode *node = ast_function_call_create("sqrt", args, 3, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del argumento: %s (Number)\n", sum->return_type->name);
    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(sum->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_FALSE(dm_has_errors(dm_global));
    printf("Errores: %s (esperado: false)\n", dm_has_errors(dm_global) ? "true" : "false");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_function_call_undefined(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_call_undefined ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: noExiste() (linea 4, columna 1)\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    FunctionCallNode *node = ast_function_call_create("noExiste", args, 4, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: NULL)\n", node->base.return_type ? node->base.return_type->name : "NULL");
    CU_ASSERT_PTR_NULL(node->base.return_type);

    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1)\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_function_call_argument_count_mismatch(void)
{
    printf("\n\n------------ Inicio test: test_inference_function_call_argument_count_mismatch ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: sqrt(1, 2) (argumentos de mas, linea 5, columna 1)\n");

    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(1.0, 5, 6);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(2.0, 5, 9);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);
    FunctionCallNode *node = ast_function_call_create("sqrt", args, 5, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Number)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));

    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1 - argumentos incorrectos)\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    printf("\n=== Iniciando tests de FunctionCallNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("FunctionCallNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_function_call_no_args", test_inference_function_call_no_args);
    CU_add_test(suite, "test_inference_function_call_with_arg", test_inference_function_call_with_arg);
    CU_add_test(suite, "test_inference_function_call_complex_arg", test_inference_function_call_complex_arg);
    CU_add_test(suite, "test_inference_function_call_undefined", test_inference_function_call_undefined);
    CU_add_test(suite, "test_inference_function_call_argument_count_mismatch", test_inference_function_call_argument_count_mismatch);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}