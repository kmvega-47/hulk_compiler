#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

static UserTypeDescriptor *register_dummy_type(const char *name, List *param_types)
{
    TypeDescriptor *obj = type_table_lookup_by_tag(global_type_table, HULK_OBJECT);
    UserTypeDescriptor *ut = user_type_create(name, obj, param_types);
    type_table_insert(global_type_table, ut);
    return ut;
}

void test_inference_type_instanciation_no_args(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_instanciation_no_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: new Point() (linea 1, columna 5)\n");

    List *empty = list_create(0, NULL, NULL, NULL, NULL);
    register_dummy_type("Point", empty);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create("Point", args, 1, 5);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    TypeDescriptor *point_type = type_table_lookup_by_name(global_type_table, "Point");
    CU_ASSERT_PTR_NOT_NULL(point_type);
    CU_ASSERT_PTR_EQUAL(node->base.return_type, point_type);
    printf("Tipo inferido: %s (esperado: Point)\n", node->base.return_type->name);

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

void test_inference_type_instanciation_with_args(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_instanciation_with_args ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: new Point(3, 4) (linea 2, columna 5)\n");

    List *param_types = list_create(2,
        (void *[]){ type_table_lookup_by_tag(global_type_table, HULK_NUMBER),
                     type_table_lookup_by_tag(global_type_table, HULK_NUMBER) },
        NULL, NULL, NULL);
    register_dummy_type("Point", param_types);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(3.0, 2, 12);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(4.0, 2, 15);
    list_append(args, arg1);
    list_append(args, arg2);
    TypeInstanciationNode *node = ast_type_instanciation_create("Point", args, 2, 5);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    TypeDescriptor *point_type = type_table_lookup_by_name(global_type_table, "Point");
    CU_ASSERT_PTR_EQUAL(node->base.return_type, point_type);
    CU_ASSERT_PTR_EQUAL(arg1->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(arg2->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    printf("Tipo inferido: %s, args: Number, Number\n", node->base.return_type->name);

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

void test_inference_type_instanciation_undefined(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_instanciation_undefined ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: new NoExiste() (error, linea 3, columna 5)\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create("NoExiste", args, 3, 5);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Tipo inferido: NULL, Errores: 1\n");

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_type_instanciation_builtin(void)
{
    printf("\n\n------------ Inicio test: test_inference_type_instanciation_builtin ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: new Number() (error, linea 4, columna 5)\n");

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *node = ast_type_instanciation_create("Number", args, 4, 5);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    CU_ASSERT_PTR_NULL(node->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Tipo inferido: NULL, Errores: 1\n");

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
    printf("\n=== Iniciando tests de TypeInstanciationNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeInstanciationNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_type_instanciation_no_args", test_inference_type_instanciation_no_args);
    CU_add_test(suite, "test_inference_type_instanciation_with_args", test_inference_type_instanciation_with_args);
    CU_add_test(suite, "test_inference_type_instanciation_undefined", test_inference_type_instanciation_undefined);
    CU_add_test(suite, "test_inference_type_instanciation_builtin", test_inference_type_instanciation_builtin);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}