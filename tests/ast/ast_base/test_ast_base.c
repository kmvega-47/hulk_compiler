#include <CUnit/Basic.h>
#include "ast.h"

void test_ast_base_init(void)
{
    printf("\n\n------------ Inicio test: test_ast_base_init ------------\n");
    global_tables_init();

    printf("Inicializando un ASTNode base y verificando campos\n");

    ASTNode node;
    ast_base_init(&node, NODE_LITERAL, 5, 10);

    CU_ASSERT_EQUAL(node.node_type, NODE_LITERAL);
    printf("node_type = %d (esperado: %d)\n", node.node_type, NODE_LITERAL);

    CU_ASSERT_PTR_NULL(node.return_type);
    printf("return_type = %p (esperado: NULL)\n", (void *)node.return_type);

    CU_ASSERT_EQUAL(node.line, 5);
    printf("line = %d (esperado: 5)\n", node.line);

    CU_ASSERT_EQUAL(node.column, 10);
    printf("column = %d (esperado: 10)\n", node.column);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de ASTNode (creacion base) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ASTNode Base Creation", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_ast_base_init", test_ast_base_init);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}