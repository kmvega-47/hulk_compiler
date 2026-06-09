#include <CUnit/Basic.h>
#include "diagnostic_manager.h"

void test_dm_create(void)
{
    printf("\n\n------------ Inicio test: dm_create ------------\n");
    printf("Caso: creación del diagnostic manager\n");

    diagnostic_manager_t *dm = dm_create();

    CU_ASSERT_PTR_NOT_NULL(dm);
    CU_ASSERT_PTR_NOT_NULL(dm->errors);
    CU_ASSERT_EQUAL(list_count(dm->errors), 0);
    CU_ASSERT_FALSE(dm_has_errors(dm));
    CU_ASSERT_EQUAL(dm_error_count(dm), 0);

    dm_destroy(dm);
}

void test_dm_add_error(void)
{
    printf("\n\n------------ Inicio test: dm_add_error ------------\n");
    printf("Caso: añadir un error\n");

    diagnostic_manager_t *dm = dm_create();

    dm_add_error(dm, ERROR_TYPE_SEMANTIC, 10, 0, "Type mismatch: expected '%s', got '%s'", "Number", "String");

    CU_ASSERT_TRUE(dm_has_errors(dm));
    CU_ASSERT_EQUAL(dm_error_count(dm), 1);

    dm_destroy(dm);
}

void test_dm_multiple_errors(void)
{
    printf("\n\n------------ Inicio test: dm_multiple_errors ------------\n");
    printf("Caso: múltiples errores\n");

    diagnostic_manager_t *dm = dm_create();

    dm_add_error(dm, ERROR_TYPE_SEMANTIC, 10, 0, "Error 1");
    dm_add_error(dm, ERROR_TYPE_SYNTACTIC, 20, 5, "Error 2");
    dm_add_error(dm, ERROR_TYPE_LEXICAL, 30, 10, "Error 3");

    CU_ASSERT_TRUE(dm_has_errors(dm));
    CU_ASSERT_EQUAL(dm_error_count(dm), 3);

    dm_destroy(dm);
}

void test_dm_empty(void)
{
    printf("\n\n------------ Inicio test: dm_empty ------------\n");
    printf("Caso: gestor sin errores\n");

    diagnostic_manager_t *dm = dm_create();

    CU_ASSERT_FALSE(dm_has_errors(dm));
    CU_ASSERT_EQUAL(dm_error_count(dm), 0);

    dm_destroy(dm);
}

void test_dm_destroy_null(void)
{
    printf("\n\n------------ Inicio test: dm_destroy_null ------------\n");
    printf("Caso: destroy con NULL no crashea\n");

    dm_destroy(NULL);
    CU_PASS("dm_destroy(NULL) executed without crashing");
}

void test_dm_add_error_null_dm(void)
{
    printf("\n\n------------ Inicio test: dm_add_error_null_dm ------------\n");
    printf("Caso: añadir error con dm NULL no crashea\n");

    dm_add_error(NULL, ERROR_TYPE_SEMANTIC, 10, 0, "Should not crash");
    CU_PASS("dm_add_error(NULL, ...) executed without crashing");
}

void test_dm_has_errors_null(void)
{
    printf("\n\n------------ Inicio test: dm_has_errors_null ------------\n");
    printf("Caso: dm_has_errors con NULL retorna false\n");

    CU_ASSERT_FALSE(dm_has_errors(NULL));
}

void test_dm_error_count_null(void)
{
    printf("\n\n------------ Inicio test: dm_error_count_null ------------\n");
    printf("Caso: dm_error_count con NULL retorna 0\n");

    CU_ASSERT_EQUAL(dm_error_count(NULL), 0);
}

void test_dm_print_errors(void)
{
    printf("\n\n------------ Inicio test: dm_print_errors ------------\n");
    printf("Caso: dm_print_errors no crashea\n");

    diagnostic_manager_t *dm = dm_create();

    dm_add_error(dm, ERROR_TYPE_SEMANTIC, 10, 5, "Error message 1");
    dm_add_error(dm, ERROR_TYPE_LEXICAL, 1, 0, "Invalid character '%c'", '@');

    printf("--- Testing dm_print_errors (should appear on stderr) ---\n");
    dm_print_errors(dm);

    CU_PASS("dm_print_errors executed without crashing");

    dm_destroy(dm);
}

void test_dm_print_errors_empty(void)
{
    printf("\n\n------------ Inicio test: dm_print_errors_empty ------------\n");
    printf("Caso: dm_print_errors con lista vacía no crashea\n");

    diagnostic_manager_t *dm = dm_create();

    printf("--- Testing dm_print_errors on empty manager ---\n");
    dm_print_errors(dm);

    CU_PASS("dm_print_errors on empty manager executed without crashing");

    dm_destroy(dm);
}

void test_dm_print_errors_null(void)
{
    printf("\n\n------------ Inicio test: dm_print_errors_null ------------\n");
    printf("Caso: dm_print_errors con NULL no crashea\n");

    dm_print_errors(NULL);
    CU_PASS("dm_print_errors(NULL) executed without crashing");
}

void test_dm_global(void)
{
    printf("\n\n------------ Inicio test: dm_global ------------\n");
    printf("Caso: variable global dm_global existe\n");

    CU_ASSERT_PTR_NULL(dm_global);

    dm_global = dm_create();
    CU_ASSERT_PTR_NOT_NULL(dm_global);

    dm_add_error(dm_global, ERROR_TYPE_SEMANTIC, 42, 7, "Global error test");
    CU_ASSERT_TRUE(dm_has_errors(dm_global));
    CU_ASSERT_EQUAL(dm_error_count(dm_global), 1);

    dm_destroy(dm_global);
    dm_global = NULL;
}

int main(void)
{
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Diagnostic Manager Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "test_dm_create", test_dm_create);
    CU_add_test(suite, "test_dm_add_error", test_dm_add_error);
    CU_add_test(suite, "test_dm_multiple_errors", test_dm_multiple_errors);
    CU_add_test(suite, "test_dm_empty", test_dm_empty);
    CU_add_test(suite, "test_dm_destroy_null", test_dm_destroy_null);
    CU_add_test(suite, "test_dm_add_error_null_dm", test_dm_add_error_null_dm);
    CU_add_test(suite, "test_dm_has_errors_null", test_dm_has_errors_null);
    CU_add_test(suite, "test_dm_error_count_null", test_dm_error_count_null);
    CU_add_test(suite, "test_dm_print_errors", test_dm_print_errors);
    CU_add_test(suite, "test_dm_print_errors_empty", test_dm_print_errors_empty);
    CU_add_test(suite, "test_dm_print_errors_null", test_dm_print_errors_null);
    CU_add_test(suite, "test_dm_global", test_dm_global);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}