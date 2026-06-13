#include "hulk_common.h"
#include "diagnostic_manager.h"
#include "global_tables.h"
#include "parser_types.h"
#include "parser.tab.h"
#include "print_visitor.h"
#include "free_visitor.h"
#include "type_inference_visitor.h"
#include "type_check_visitor.h"
#include "codegen_visitor.h"

extern FILE *yyin;
extern ASTNode *ast_root;
extern int yyparse(void);
extern int yylex_destroy(void);

static const char *load_source_file(int argc, char **argv, FILE **file);

int main(int argc, char **argv)
{
    FILE *file = NULL;
    const char *filename = load_source_file(argc, argv, &file);

    if (!filename)
        return EXIT_FAILURE;

    dm_global = dm_create();
    if (!dm_global)
    {
        fprintf(stderr, "Error: no se pudo inicializar el gestor de diagnosticos\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    global_tables_init();
    if (!global_type_table || !global_function_table)
    {
        fprintf(stderr, "Error: no se pudieron inicializar las tablas globales\n");
        fclose(file);
        dm_destroy(dm_global);
        return EXIT_FAILURE;
    }

    TypeInferenceVisitor *inference_visitor = type_inference_visitor_create();
    TypeCheckVisitor *check_visitor = type_check_visitor_create();
    CodeGenVisitor *codegen_visitor = code_gen_visitor_create("hulk_module");
    PrintVisitor *pv = print_visitor_create();
    FreeVisitor *fv = free_visitor_create();

    yyin = file;

    int parse_error = yyparse();
    fclose(file);
    yylex_destroy();

    if (parse_error == 0 && ast_root)
    {
        ast_accept(ast_root, (Visitor *)inference_visitor);

        if (!dm_has_errors(dm_global))
        {
            ast_accept(ast_root, (Visitor *)check_visitor);

            if (!dm_has_errors(dm_global))
            {
                ast_accept(ast_root, (Visitor *)codegen_visitor);
                code_gen_visitor_compile(codegen_visitor, "output");
            }
        }
    }

    dm_print_errors(dm_global);

    if (ast_root)
    {
        //ast_accept(ast_root, (Visitor *)pv);
        ast_accept(ast_root, (Visitor *)fv);
        ast_root = NULL;
    }

    int exit_code = dm_get_exit_code(dm_global);

    free_visitor_destroy(fv);
    print_visitor_destroy(pv);
    code_gen_visitor_destroy(codegen_visitor);
    type_check_visitor_destroy(check_visitor);
    type_inference_visitor_destroy(inference_visitor);

    global_tables_destroy();
    dm_destroy(dm_global);

    return exit_code;
}

static const char *load_source_file(int argc, char **argv, FILE **file)
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <file.hulk>\n", argv[0]);
        return NULL;
    }

    const char *filename = argv[1];
    const char *extension = strrchr(filename, '.');

    if (!extension || strcmp(extension, ".hulk") != 0)
    {
        fprintf(stderr, "Error: El archivo debe tener extension .hulk\n");
        return NULL;
    }

    *file = fopen(filename, "r");
    if (!*file)
    {
        fprintf(stderr, "Error: No se pudo abrir el archivo '%s'\n", filename);
        return NULL;
    }

    return filename;
}