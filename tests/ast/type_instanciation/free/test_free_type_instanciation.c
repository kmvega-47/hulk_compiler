#include <CUnit/Basic.h>
#include "free_visitor.h"

void test_free_type_instanciation_no_args(void)
{
    printf("\n\n------------ Inicio test: test_free_type_instanciation_no_args ------------\n");
    global_tables_init();

    printf("Caso: new Point() (linea 1, columna 5)\n");
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create("Point", args, 1, 5);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_type_instanciation_multiple_args(void)
{
    printf("\n\n------------ Inicio test: test_free_type_instanciation_multiple_args ------------\n");
    global_tables_init();

    printf("Caso: new Point(3, 4) (linea 2, columna 5)\n");
    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(3.0, 2, 12);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(4.0, 2, 15);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);
    TypeInstanciationNode *node = ast_type_instanciation_create("Point", args, 2, 5);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

void test_free_type_instanciation_long_name(void)
{
    printf("\n\n------------ Inicio test: test_free_type_instanciation_long_name ------------\n");
    global_tables_init();

    const char *long_name = "VeryLongTypeName_WithUnderscore_1234567890";
    printf("Caso: new %s() (linea 3, columna 5)\n", long_name);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create(long_name, args, 3, 5);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeInstanciationNode (free) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeInstanciationNode Free Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_free_type_instanciation_no_args", test_free_type_instanciation_no_args);
    CU_add_test(suite, "test_free_type_instanciation_multiple_args", test_free_type_instanciation_multiple_args);
    CU_add_test(suite, "test_free_type_instanciation_long_name", test_free_type_instanciation_long_name);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}