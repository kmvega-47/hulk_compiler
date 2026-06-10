#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_literal_number(void)
{
    printf("\n\n------------ Inicio test: test_inference_literal_number ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: 42.0 (linea 1, columna 1)\n");

    LiteralNode *node = ast_number_literal_create(42.0, 1, 1);
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

void test_inference_literal_string(void)
{
    printf("\n\n------------ Inicio test: test_inference_literal_string ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: \"Hola mundo\" (linea 2, columna 3)\n");

    LiteralNode *node = ast_string_literal_create("Hola mundo", 2, 3);
    TypeInferenceVisitor *iv = type_inference_visitor_create();

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: String)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_STRING));

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

void test_inference_literal_bool(void)
{
    printf("\n\n------------ Inicio test: test_inference_literal_bool ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: true (linea 3, columna 5)\n");

    LiteralNode *node = ast_bool_literal_create(true, 3, 5);
    TypeInferenceVisitor *iv = type_inference_visitor_create();

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo inferido: %s (esperado: Bool)\n", node->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_BOOL));

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

int main(void)
{
    printf("\n=== Iniciando tests de LiteralNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LiteralNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_literal_number", test_inference_literal_number);
    CU_add_test(suite, "test_inference_literal_string", test_inference_literal_string);
    CU_add_test(suite, "test_inference_literal_bool", test_inference_literal_bool);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}