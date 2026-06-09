#include "hulk_common.h"
#include "diagnostic_manager.h"
#include "global_tables.h"
#include "parser_types.h"
#include "parser.tab.h"
#include "print_visitor.h"
#include "free_visitor.h"

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
        return 1;

    dm_global = dm_create();
    if (!dm_global)
    {
        fprintf(stderr, "Error: no se pudo inicializar el gestor de diagnosticos\n");
        fclose(file);
        return 1;
    }

    global_tables_init();
    if (!global_type_table || !global_function_table)
    {
        fprintf(stderr, "Error: no se pudieron inicializar las tablas globales\n");
        fclose(file);
        dm_destroy(dm_global);
        return 1;
    }

    yyin = file;

    printf("[Parsing] : Ejecutado\n");

    int parse_error = yyparse();
    fclose(file);
    yylex_destroy();

    int exit_code = parse_error;

    dm_print_errors(dm_global);

    if (exit_code == 0 && ast_root)
    {
        PrintVisitor *pv = print_visitor_create();
        FreeVisitor *fv = free_visitor_create();

        printf("[AST Generado] :\n");
        ast_accept(ast_root, (Visitor *)pv);

        ast_accept(ast_root, (Visitor *)fv);
        ast_root = NULL;

        print_visitor_destroy(pv);
        free_visitor_destroy(fv);
    }

    if (dm_has_errors(dm_global))
        exit_code = 1;

    global_tables_destroy();
    dm_destroy(dm_global);

    printf("[EXIT CODE] : %d\n", exit_code);
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