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

void test_check_type_definition_no_father_no_args(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_no_father_no_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type Simple {}\n");

    TypeDefinitionNode *type_def = ast_type_definition_create("Simple",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 1, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 2, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_inherits_object(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_inherits_object ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type Child inherits Object {}\n");

    TypeDefinitionNode *type_def = ast_type_definition_create("Child",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "Object",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 2, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 3, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_user_father_no_args(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_user_father_no_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A {}  type B inherits A {}\n");

    TypeDefinitionNode *a_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 3, 1);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A",
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 3, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 4, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_correct_father_args(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_correct_father_args ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A(x: Number) {}  type B inherits A(42)\n");

    List *a_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_init, symbol_binding_create("x", "Number", NULL, 4, 10));
    TypeDefinitionNode *a_def = ast_type_definition_create("A", a_init, NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 4, 1);

    List *father_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(father_args, ast_number_literal_create(42.0, 4, 35));
    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A", father_args,
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 4, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 5, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_wrong_arg_type(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_wrong_arg_type ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A(x: Number) {}  type B inherits A(\"hello\") -- error\n");

    List *a_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_init, symbol_binding_create("x", "Number", NULL, 5, 10));
    TypeDefinitionNode *a_def = ast_type_definition_create("A", a_init, NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 5, 1);

    List *father_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(father_args, ast_string_literal_create("hello", 5, 35));
    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A", father_args,
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 5, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 6, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo esperado por A.x: Number\n");
    printf("Tipo del argumento: String\n");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_attr_no_annotation(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_attr_no_annotation ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type Person { name = \"John\"; }\n");

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *name_init = (ASTNode *)ast_string_literal_create("John", 6, 25);
    list_append(attrs, symbol_binding_create("name", NULL, name_init, 6, 12));

    TypeDefinitionNode *type_def = ast_type_definition_create("Person",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        attrs,
        list_create(0, NULL, NULL, NULL, NULL), 6, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 7, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_attr_conformant(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_attr_conformant ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type Person { name: String = \"John\"; }\n");

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *name_init = (ASTNode *)ast_string_literal_create("John", 7, 30);
    list_append(attrs, symbol_binding_create("name", "String", name_init, 7, 12));

    TypeDefinitionNode *type_def = ast_type_definition_create("Person",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        attrs,
        list_create(0, NULL, NULL, NULL, NULL), 7, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 8, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_attr_non_conformant(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_attr_non_conformant ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type Person { age: Number = \"John\"; } -- error\n");

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *age_init = (ASTNode *)ast_string_literal_create("John", 8, 30);
    list_append(attrs, symbol_binding_create("age", "Number", age_init, 8, 12));

    TypeDefinitionNode *type_def = ast_type_definition_create("Person",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        attrs,
        list_create(0, NULL, NULL, NULL, NULL), 8, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 9, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo anotado: Number\n");
    printf("Tipo del inicializador: String\n");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_multiple_attrs_one_error(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_multiple_attrs_one_error ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { a: Number = 1; b: String = 2; } -- error en b\n");

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *a_init = (ASTNode *)ast_number_literal_create(1.0, 9, 25);
    ASTNode *b_init = (ASTNode *)ast_number_literal_create(2.0, 9, 45);
    list_append(attrs, symbol_binding_create("a", "Number", a_init, 9, 12));
    list_append(attrs, symbol_binding_create("b", "String", b_init, 9, 32));

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        attrs,
        list_create(0, NULL, NULL, NULL, NULL), 9, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 10, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Atributo a: Number := Number\n");
    printf("Atributo b: String := Number\n");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_method_ok(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_method_ok ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { f() => 42; }\n");

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_number_literal_create(42.0, 10, 25);
    FunctionDefinitionNode *method = ast_function_definition_create("f", mp, NULL, body, 10, 15);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 10, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 11, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 0);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_method_return_mismatch(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_method_return_mismatch ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A { f(): Number => \"hello\"; } -- error\n");

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *body = (ASTNode *)ast_string_literal_create("hello", 11, 35);
    FunctionDefinitionNode *method = ast_function_definition_create("f", mp, "Number", body, 11, 15);
    list_append(methods, method);

    TypeDefinitionNode *type_def = ast_type_definition_create("A",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        methods, 11, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, type_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 12, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Tipo anotado del retorno: Number\n");
    printf("Tipo del cuerpo: String\n");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 1);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

void test_check_type_definition_combined_errors(void)
{
    printf("\n\n------------ Inicio test: test_check_type_definition_combined_errors ------------\n");
    global_tables_init();
    dm_global = dm_create();

    printf("Caso: type A(x: Number) {}  type B inherits A(\"bad\") { a: Number = true; f(): Number => \"no\"; }\n");

    List *a_init = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(a_init, symbol_binding_create("x", "Number", NULL, 12, 10));
    TypeDefinitionNode *a_def = ast_type_definition_create("A", a_init, NULL,
        list_create(0, NULL, NULL, NULL, NULL),
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        list_create(0, NULL, NULL, NULL, NULL), 12, 1);

    List *father_args = list_create(0, NULL, NULL, NULL, NULL);
    list_append(father_args, ast_string_literal_create("bad", 12, 40));

    List *attrs = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    list_append(attrs, symbol_binding_create("a", "Number", (ASTNode *)ast_bool_literal_create(true, 12, 60), 12, 50));

    List *methods = list_create(0, NULL, NULL, NULL, NULL);
    List *mp = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    ASTNode *method_body = (ASTNode *)ast_string_literal_create("no", 12, 85);
    FunctionDefinitionNode *method = ast_function_definition_create("f", mp, "Number", method_body, 12, 75);
    list_append(methods, method);

    TypeDefinitionNode *b_def = ast_type_definition_create("B",
        list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL),
        "A", father_args, attrs, methods, 12, 1);

    List *types = list_create(0, NULL, NULL, NULL, NULL);
    list_append(types, a_def);
    list_append(types, b_def);

    ASTNode *root = (ASTNode *)ast_number_literal_create(42.0, 13, 1);
    ProgramNode *program = ast_program_create(root, list_create(0, NULL, NULL, NULL, NULL), types, 1, 1);

    size_t errors = run_inference_and_check((ASTNode *)program);

    printf("Error 1: argumento del padre (String vs Number)\n");
    printf("Error 2: atributo a (Bool vs Number)\n");
    printf("Error 3: retorno del metodo f (String vs Number)\n");

    dm_print_errors(dm_global);
    CU_ASSERT_EQUAL(errors, 3);

    FreeVisitor *fv = free_visitor_create();
    ast_accept((ASTNode *)program, (Visitor *)fv);
    free_visitor_destroy(fv);

    dm_destroy(dm_global);
    global_tables_destroy();
}

int main(void)
{
    printf("\n=== Iniciando tests de TypeDefinitionNode (type_check) ===\n");

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TypeDefinitionNode TypeCheck Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_check_type_definition_no_father_no_args", test_check_type_definition_no_father_no_args);
    CU_add_test(suite, "test_check_type_definition_inherits_object", test_check_type_definition_inherits_object);
    CU_add_test(suite, "test_check_type_definition_user_father_no_args", test_check_type_definition_user_father_no_args);
    CU_add_test(suite, "test_check_type_definition_correct_father_args", test_check_type_definition_correct_father_args);
    CU_add_test(suite, "test_check_type_definition_wrong_arg_type", test_check_type_definition_wrong_arg_type);
    CU_add_test(suite, "test_check_type_definition_attr_no_annotation", test_check_type_definition_attr_no_annotation);
    CU_add_test(suite, "test_check_type_definition_attr_conformant", test_check_type_definition_attr_conformant);
    CU_add_test(suite, "test_check_type_definition_attr_non_conformant", test_check_type_definition_attr_non_conformant);
    CU_add_test(suite, "test_check_type_definition_multiple_attrs_one_error", test_check_type_definition_multiple_attrs_one_error);
    CU_add_test(suite, "test_check_type_definition_method_ok", test_check_type_definition_method_ok);
    CU_add_test(suite, "test_check_type_definition_method_return_mismatch", test_check_type_definition_method_return_mismatch);
    CU_add_test(suite, "test_check_type_definition_combined_errors", test_check_type_definition_combined_errors);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    printf("=== Tests finalizados ===\n");
    return CU_get_error();
}