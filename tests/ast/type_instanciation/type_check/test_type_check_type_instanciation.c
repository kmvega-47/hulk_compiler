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

void test_check_type_instanciation_no_args(void)
{
    printf("\n\n------------ Inicio test: test_check_type_instanciation_no_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: new Empty()\n");

    List *empty_params = list_create(0, NULL, NULL, NULL, NULL);
    UserTypeDescriptor *ut = user_type_create("Empty", type_table_lookup_by_tag(global_type_table, HULK_OBJECT), empty_params);
    type_table_insert(global_type_table, ut);

    List *args = list_create(0, NULL, NULL, NULL, NULL);
    TypeInstanciationNode *inst = ast_type_instanciation_create("Empty", args, 1, 5);

    ProgramNode *program = ast_program_create((ASTNode *)inst,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo de la instancia: %s\n", inst->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_instanciation_correct_args(void)
{
    printf("\n\n------------ Inicio test: test_check_type_instanciation_correct_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: new Point(3, 4)\n");

    List *point_params = list_create(0, NULL, NULL, NULL, NULL);
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    UserTypeDescriptor *ut = user_type_create("Point", type_table_lookup_by_tag(global_type_table, HULK_OBJECT), point_params);
    type_table_insert(global_type_table, ut);

    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(3.0, 1, 12);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(4.0, 1, 15);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);

    TypeInstanciationNode *inst = ast_type_instanciation_create("Point", args, 1, 5);

    ProgramNode *program = ast_program_create((ASTNode *)inst,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo arg1: %s\n", arg1->return_type->name);
    printf("Tipo arg2: %s\n", arg2->return_type->name);
    printf("Tipo de la instancia: %s\n", inst->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_instanciation_fewer_args(void)
{
    printf("\n\n------------ Inicio test: test_check_type_instanciation_fewer_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: new Point(3) -- error\n");

    List *point_params = list_create(0, NULL, NULL, NULL, NULL);
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    UserTypeDescriptor *ut = user_type_create("Point", type_table_lookup_by_tag(global_type_table, HULK_OBJECT), point_params);
    type_table_insert(global_type_table, ut);

    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(3.0, 2, 12);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);

    TypeInstanciationNode *inst = ast_type_instanciation_create("Point", args, 2, 5);

    ProgramNode *program = ast_program_create((ASTNode *)inst,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Esperados: 2, recibidos: 1\n");
    printf("Tipo de la instancia: %s\n", inst->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_instanciation_more_args(void)
{
    printf("\n\n------------ Inicio test: test_check_type_instanciation_more_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: new Point(3, 4, 5) -- error\n");

    List *point_params = list_create(0, NULL, NULL, NULL, NULL);
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    UserTypeDescriptor *ut = user_type_create("Point", type_table_lookup_by_tag(global_type_table, HULK_OBJECT), point_params);
    type_table_insert(global_type_table, ut);

    ASTNode *arg1 = (ASTNode *)ast_number_literal_create(3.0, 3, 12);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(4.0, 3, 15);
    ASTNode *arg3 = (ASTNode *)ast_number_literal_create(5.0, 3, 18);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);
    list_append(args, arg3);

    TypeInstanciationNode *inst = ast_type_instanciation_create("Point", args, 3, 5);

    ProgramNode *program = ast_program_create((ASTNode *)inst,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Esperados: 2, recibidos: 3\n");
    printf("Tipo de la instancia: %s\n", inst->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_instanciation_wrong_arg_type(void)
{
    printf("\n\n------------ Inicio test: test_check_type_instanciation_wrong_arg_type ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: new Point(\"hello\", 4) -- error\n");

    List *point_params = list_create(0, NULL, NULL, NULL, NULL);
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    UserTypeDescriptor *ut = user_type_create("Point", type_table_lookup_by_tag(global_type_table, HULK_OBJECT), point_params);
    type_table_insert(global_type_table, ut);

    ASTNode *arg1 = (ASTNode *)ast_string_literal_create("hello", 4, 12);
    ASTNode *arg2 = (ASTNode *)ast_number_literal_create(4.0, 4, 22);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);
    list_append(args, arg2);

    TypeInstanciationNode *inst = ast_type_instanciation_create("Point", args, 4, 5);

    ProgramNode *program = ast_program_create((ASTNode *)inst,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo arg1: %s (esperado Number)\n", arg1->return_type->name);
    printf("Tipo arg2: %s\n", arg2->return_type->name);
    printf("Tipo de la instancia: %s\n", inst->base.return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_instanciation_combined_errors(void)
{
    printf("\n\n------------ Inicio test: test_check_type_instanciation_combined_errors ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: new Point(\"hello\") -- error de cantidad y tipo\n");

    List *point_params = list_create(0, NULL, NULL, NULL, NULL);
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    list_append(point_params, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    UserTypeDescriptor *ut = user_type_create("Point", type_table_lookup_by_tag(global_type_table, HULK_OBJECT), point_params);
    type_table_insert(global_type_table, ut);

    ASTNode *arg1 = (ASTNode *)ast_string_literal_create("hello", 5, 12);
    List *args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(args, arg1);

    TypeInstanciationNode *inst = ast_type_instanciation_create("Point", args, 5, 5);

    ProgramNode *program = ast_program_create((ASTNode *)inst,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Esperados: 2, recibidos: 1\n");
    printf("Tipo arg1: %s (esperado Number)\n", arg1->return_type->name);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 2);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeInstanciationNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeInstanciationNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_type_instanciation_no_args", test_check_type_instanciation_no_args);
    CU_add_test(suite, "test_check_type_instanciation_correct_args", test_check_type_instanciation_correct_args);
    CU_add_test(suite, "test_check_type_instanciation_fewer_args", test_check_type_instanciation_fewer_args);
    CU_add_test(suite, "test_check_type_instanciation_more_args", test_check_type_instanciation_more_args);
    CU_add_test(suite, "test_check_type_instanciation_wrong_arg_type", test_check_type_instanciation_wrong_arg_type);
    CU_add_test(suite, "test_check_type_instanciation_combined_errors", test_check_type_instanciation_combined_errors);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}