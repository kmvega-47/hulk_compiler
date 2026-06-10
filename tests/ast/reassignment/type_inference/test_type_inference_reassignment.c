#include <CUnit/Basic.h>
#include "type_inference_visitor.h"
#include "free_visitor.h"

void test_inference_reassignment_correct(void)
{
    printf("\n\n------------ Inicio test: test_inference_reassignment_correct ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: let x = 1 in x := 42 (linea 1, columna 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init = (ASTNode *)ast_number_literal_create(1.0, 1, 9);
    list_append(bindings, symbol_binding_create("x", NULL, init, 1, 5));

    VariableReferenceNode *target = ast_variable_reference_create("x", 1, 15);
    ASTNode *new_val = (ASTNode *)ast_number_literal_create(42.0, 1, 19);
    ReassignmentNode *body = ast_reassignment_create((ASTNode *)target, new_val, 1, 17);

    LetInNode *node = ast_let_create(bindings, (ASTNode *)body, 1, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del nuevo valor: %s\n", new_val->return_type->name);
    printf("Tipo del target (x): %s\n", target->base.return_type->name);
    printf("Tipo de la reasignacion: %s\n", body->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(new_val->return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(target->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(body->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

void test_inference_reassignment_undefined(void)
{
    printf("\n\n------------ Inicio test: test_inference_reassignment_undefined ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: y := 5 (y no definida, linea 2, columna 1)\n");

    VariableReferenceNode *target = ast_variable_reference_create("y", 2, 1);
    ASTNode *value = (ASTNode *)ast_number_literal_create(5.0, 2, 5);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, value, 2, 3);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del target (y): %s\n", target->base.return_type ? target->base.return_type->name : "NULL");
    printf("Tipo de la reasignacion: %s\n", node->base.return_type ? node->base.return_type->name : "NULL");

    CU_ASSERT_PTR_NULL(target->base.return_type);
    CU_ASSERT_PTR_NULL(node->base.return_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    printf("Errores: %zu (esperado: 1)\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_reassignment_constant(void)
{
    printf("\n\n------------ Inicio test: test_inference_reassignment_constant ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: PI := 3.15 (PI es constante, linea 3, columna 1)\n");

    VariableReferenceNode *target = ast_variable_reference_create("PI", 3, 1);
    ASTNode *value = (ASTNode *)ast_number_literal_create(3.15, 3, 6);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, value, 3, 4);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del target (PI): %s\n", target->base.return_type->name);
    printf("Tipo de la reasignacion: %s\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(target->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(node->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1 - constante)\n", dm_error_count(dm_global));

    type_inference_visitor_destroy(iv);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_reassignment_self(void)
{
    printf("\n\n------------ Inicio test: test_inference_reassignment_self ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: self := new Point() (self no es asignable, linea 4, columna 1)\n");

    TypeInferenceVisitor *iv = type_inference_visitor_create();

    UserTypeDescriptor *point_type = user_type_create("Point",
        type_table_lookup_by_tag(global_type_table, HULK_OBJECT),
        list_create(0, NULL, NULL, NULL, NULL));
    scope_add_self_instance(iv->current_scope, (TypeDescriptor *)point_type);

    VariableReferenceNode *target = ast_variable_reference_create("self", 4, 1);
    ASTNode *value = (ASTNode *)ast_number_literal_create(0.0, 4, 8);
    ReassignmentNode *node = ast_reassignment_create((ASTNode *)target, value, 4, 6);

    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipo del target (self): %s\n", target->base.return_type->name);
    CU_ASSERT_PTR_EQUAL(target->base.return_type, (TypeDescriptor *)point_type);
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);
    printf("Errores: %zu (esperado: 1 - self no asignable)\n", dm_error_count(dm_global));

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)fv);
    free_visitor_destroy(fv);

    user_type_destroy(point_type);
    type_inference_visitor_destroy(iv);
    global_tables_destroy();
    dm_destroy(dm_global);
    dm_global = NULL;
}

void test_inference_reassignment_nested(void)
{
    printf("\n\n------------ Inicio test: test_inference_reassignment_nested ------------\n");
    dm_global = dm_create();
    global_tables_init();

    printf("Caso: let a = 0, b = 0, c = 0 in a := b := c := 47 (linea 5, columna 1)\n");

    List *bindings = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *init0 = (ASTNode *)ast_number_literal_create(0.0, 5, 9);
    list_append(bindings, symbol_binding_create("a", NULL, init0, 5, 5));
    init0 = (ASTNode *)ast_number_literal_create(0.0, 5, 16);
    list_append(bindings, symbol_binding_create("b", NULL, init0, 5, 12));
    init0 = (ASTNode *)ast_number_literal_create(0.0, 5, 23);
    list_append(bindings, symbol_binding_create("c", NULL, init0, 5, 19));

    ASTNode *val47 = (ASTNode *)ast_number_literal_create(47.0, 5, 31);
    VariableReferenceNode *target_c = ast_variable_reference_create("c", 5, 27);
    ReassignmentNode *reassign_c = ast_reassignment_create((ASTNode *)target_c, val47, 5, 29);

    VariableReferenceNode *target_b = ast_variable_reference_create("b", 5, 20);
    ReassignmentNode *reassign_b = ast_reassignment_create((ASTNode *)target_b, (ASTNode *)reassign_c, 5, 22);

    VariableReferenceNode *target_a = ast_variable_reference_create("a", 5, 13);
    ReassignmentNode *reassign_a = ast_reassignment_create((ASTNode *)target_a, (ASTNode *)reassign_b, 5, 15);

    LetInNode *node = ast_let_create(bindings, (ASTNode *)reassign_a, 5, 1);

    TypeInferenceVisitor *iv = type_inference_visitor_create();
    ast_accept((ASTNode *)node, (Visitor *)iv);

    printf("Tipos de las reasignaciones:\n");
    printf("  c := 47 : %s\n", reassign_c->base.return_type->name);
    printf("  b := ... : %s\n", reassign_b->base.return_type->name);
    printf("  a := ... : %s\n", reassign_a->base.return_type->name);
    printf("Tipo del let-in: %s\n", node->base.return_type->name);

    CU_ASSERT_PTR_EQUAL(reassign_c->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(reassign_b->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
    CU_ASSERT_PTR_EQUAL(reassign_a->base.return_type, type_table_lookup_by_tag(global_type_table, HULK_NUMBER));
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

int main(void)
{
    printf("\n=== Iniciando tests de ReassignmentNode (type_inference) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ReassignmentNode TypeInference Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_inference_reassignment_correct", test_inference_reassignment_correct);
    CU_add_test(suite, "test_inference_reassignment_undefined", test_inference_reassignment_undefined);
    CU_add_test(suite, "test_inference_reassignment_constant", test_inference_reassignment_constant);
    CU_add_test(suite, "test_inference_reassignment_self", test_inference_reassignment_self);
    CU_add_test(suite, "test_inference_reassignment_nested", test_inference_reassignment_nested);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}