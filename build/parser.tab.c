/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/parser/hulk_parser.y"


#include "diagnostic_manager.h"
#include "parser_types.h"

/* Variables globales del compilador */
ASTNode *ast_root = NULL;

extern int yylineno;

/* Prototipos de funciones del parser */
void yyerror(const char *msg);
int yylex(void);

void append_else(ConditionalNode *chain, ASTNode *final_else);


#line 89 "build/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_FUNCTION = 3,                   /* FUNCTION  */
  YYSYMBOL_LET = 4,                        /* LET  */
  YYSYMBOL_IN = 5,                         /* IN  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_ELIF = 7,                       /* ELIF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_TYPE = 10,                      /* TYPE  */
  YYSYMBOL_INHERITS = 11,                  /* INHERITS  */
  YYSYMBOL_NEW = 12,                       /* NEW  */
  YYSYMBOL_TRUE = 13,                      /* TRUE  */
  YYSYMBOL_FALSE = 14,                     /* FALSE  */
  YYSYMBOL_NUMBER = 15,                    /* NUMBER  */
  YYSYMBOL_STRING = 16,                    /* STRING  */
  YYSYMBOL_ID = 17,                        /* ID  */
  YYSYMBOL_PLUS = 18,                      /* PLUS  */
  YYSYMBOL_MINUS = 19,                     /* MINUS  */
  YYSYMBOL_STAR = 20,                      /* STAR  */
  YYSYMBOL_SLASH = 21,                     /* SLASH  */
  YYSYMBOL_PERCENT = 22,                   /* PERCENT  */
  YYSYMBOL_POWER = 23,                     /* POWER  */
  YYSYMBOL_CONCAT = 24,                    /* CONCAT  */
  YYSYMBOL_CONCAT_WS = 25,                 /* CONCAT_WS  */
  YYSYMBOL_ASSIGN = 26,                    /* ASSIGN  */
  YYSYMBOL_REASSIGN = 27,                  /* REASSIGN  */
  YYSYMBOL_EQUALS = 28,                    /* EQUALS  */
  YYSYMBOL_NOT_EQUALS = 29,                /* NOT_EQUALS  */
  YYSYMBOL_GREATER = 30,                   /* GREATER  */
  YYSYMBOL_GREATER_EQ = 31,                /* GREATER_EQ  */
  YYSYMBOL_LESS = 32,                      /* LESS  */
  YYSYMBOL_LESS_EQ = 33,                   /* LESS_EQ  */
  YYSYMBOL_AND = 34,                       /* AND  */
  YYSYMBOL_OR = 35,                        /* OR  */
  YYSYMBOL_NOT = 36,                       /* NOT  */
  YYSYMBOL_ARROW = 37,                     /* ARROW  */
  YYSYMBOL_LPAREN = 38,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 39,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 40,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 41,                    /* RBRACE  */
  YYSYMBOL_SEMICOLON = 42,                 /* SEMICOLON  */
  YYSYMBOL_COLON = 43,                     /* COLON  */
  YYSYMBOL_DOT = 44,                       /* DOT  */
  YYSYMBOL_COMMA = 45,                     /* COMMA  */
  YYSYMBOL_IS = 46,                        /* IS  */
  YYSYMBOL_AS = 47,                        /* AS  */
  YYSYMBOL_BASE = 48,                      /* BASE  */
  YYSYMBOL_LEX_ERROR = 49,                 /* LEX_ERROR  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_program = 51,                   /* program  */
  YYSYMBOL_SEMICOLON_opt = 52,             /* SEMICOLON_opt  */
  YYSYMBOL_type_def_list = 53,             /* type_def_list  */
  YYSYMBOL_type_def = 54,                  /* type_def  */
  YYSYMBOL_params_list_opt = 55,           /* params_list_opt  */
  YYSYMBOL_params_list = 56,               /* params_list  */
  YYSYMBOL_params_sequence_opt = 57,       /* params_sequence_opt  */
  YYSYMBOL_params_sequence = 58,           /* params_sequence  */
  YYSYMBOL_type_annotation_opt = 59,       /* type_annotation_opt  */
  YYSYMBOL_type_inheritance = 60,          /* type_inheritance  */
  YYSYMBOL_args_list_opt = 61,             /* args_list_opt  */
  YYSYMBOL_args_list = 62,                 /* args_list  */
  YYSYMBOL_args_sequence_opt = 63,         /* args_sequence_opt  */
  YYSYMBOL_args_sequence = 64,             /* args_sequence  */
  YYSYMBOL_type_body_elements = 65,        /* type_body_elements  */
  YYSYMBOL_type_body_element = 66,         /* type_body_element  */
  YYSYMBOL_attr_def = 67,                  /* attr_def  */
  YYSYMBOL_method_def = 68,                /* method_def  */
  YYSYMBOL_method_body = 69,               /* method_body  */
  YYSYMBOL_var_binding = 70,               /* var_binding  */
  YYSYMBOL_func_def_list = 71,             /* func_def_list  */
  YYSYMBOL_func_def = 72,                  /* func_def  */
  YYSYMBOL_expression = 73,                /* expression  */
  YYSYMBOL_literal = 74,                   /* literal  */
  YYSYMBOL_expr_block = 75,                /* expr_block  */
  YYSYMBOL_expr_list = 76,                 /* expr_list  */
  YYSYMBOL_elif_clauses = 77,              /* elif_clauses  */
  YYSYMBOL_else_clause = 78,               /* else_clause  */
  YYSYMBOL_bindings = 79,                  /* bindings  */
  YYSYMBOL_assignable = 80,                /* assignable  */
  YYSYMBOL_var_reference = 81,             /* var_reference  */
  YYSYMBOL_attr_access = 82                /* attr_access  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   485

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  88
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  159

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   304


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    97,    97,   106,   106,   109,   114,   122,   158,   163,
     170,   177,   182,   189,   198,   210,   215,   222,   227,   237,
     242,   249,   256,   261,   268,   273,   281,   286,   294,   299,
     309,   319,   329,   334,   341,   350,   355,   363,   370,   375,
     380,   385,   390,   395,   400,   405,   410,   415,   420,   425,
     430,   435,   440,   445,   450,   455,   460,   465,   470,   475,
     480,   497,   503,   509,   516,   523,   528,   535,   540,   546,
     552,   559,   564,   569,   574,   581,   585,   592,   596,   604,
     609,   617,   622,   629,   634,   642,   643,   647,   655
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "FUNCTION", "LET",
  "IN", "IF", "ELIF", "ELSE", "WHILE", "TYPE", "INHERITS", "NEW", "TRUE",
  "FALSE", "NUMBER", "STRING", "ID", "PLUS", "MINUS", "STAR", "SLASH",
  "PERCENT", "POWER", "CONCAT", "CONCAT_WS", "ASSIGN", "REASSIGN",
  "EQUALS", "NOT_EQUALS", "GREATER", "GREATER_EQ", "LESS", "LESS_EQ",
  "AND", "OR", "NOT", "ARROW", "LPAREN", "RPAREN", "LBRACE", "RBRACE",
  "SEMICOLON", "COLON", "DOT", "COMMA", "IS", "AS", "BASE", "LEX_ERROR",
  "$accept", "program", "SEMICOLON_opt", "type_def_list", "type_def",
  "params_list_opt", "params_list", "params_sequence_opt",
  "params_sequence", "type_annotation_opt", "type_inheritance",
  "args_list_opt", "args_list", "args_sequence_opt", "args_sequence",
  "type_body_elements", "type_body_element", "attr_def", "method_def",
  "method_body", "var_binding", "func_def_list", "func_def", "expression",
  "literal", "expr_block", "expr_list", "elif_clauses", "else_clause",
  "bindings", "assignable", "var_reference", "attr_access", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-77)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-87)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -77,    12,     7,   -77,    -4,   -77,    61,   -22,     1,     2,
      -8,    13,    35,   -77,   -77,   -77,   -77,    16,   142,   142,
     142,   104,    16,   -77,   204,   -77,   -77,    -5,    29,    30,
      41,    48,   -77,   -22,   -77,    18,   -77,     3,   142,   142,
      16,   142,   -77,    19,    19,   234,   -77,   384,    24,   -77,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   -77,    51,    52,    55,
     -77,   142,    18,    40,    36,    65,    43,    18,    67,    59,
     142,     2,   264,   294,   -77,    47,    42,   384,   -77,   142,
      49,     5,     5,     6,     6,     6,     6,   404,   404,   144,
     144,   144,   144,   144,   144,   441,   424,    16,   -77,   -77,
     384,   -77,   -77,    71,    16,   -77,   -26,   -77,   142,   384,
     -77,   142,   142,   -77,   142,   384,   -77,   -77,    18,   -77,
     -77,   -10,   142,   -77,   -77,   384,   174,   384,   384,   -77,
     -28,   -77,   -77,   -77,   -77,    50,   324,    56,    81,   -77,
     -77,   142,   142,   -77,   354,   384,   142,   174,   -77
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,    35,     1,     0,     6,     0,     8,     0,     0,
       0,     0,     0,    73,    74,    71,    72,    87,     0,     0,
       0,     0,     0,    36,     3,    39,    59,     0,    38,    65,
      11,    17,     9,     0,    37,    15,    83,     0,     0,     0,
       0,    22,    63,    42,    41,     0,    75,    77,     3,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     0,     0,     0,
       2,     0,    15,     0,    12,     0,     0,    15,     0,     0,
       0,     0,     0,     0,    66,     0,    23,    24,    40,     4,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    55,
      56,    51,    53,    52,    54,    57,    58,    88,    68,    69,
      67,    13,    10,     0,    19,    26,     0,    16,     0,    62,
      84,     0,     0,    21,     0,    78,    76,    64,    15,    18,
      20,     0,     0,    31,    33,    34,    79,    61,    25,    14,
      15,     7,    27,    28,    29,     0,     0,     0,    81,    30,
      32,     0,     0,    60,     0,    82,     0,    79,    80
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -77,   -77,    45,   -77,   -77,   -77,    88,   -77,   -77,   -68,
     -77,   -77,   -16,   -77,   -77,   -77,   -77,   -77,   -35,   -77,
     -76,   -77,   -77,   -18,   -77,   -14,   -77,   -52,   -77,   -77,
     -77,   -77,   -77
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    70,     2,     5,    31,    77,    73,    74,    79,
      76,   129,    42,    85,    86,   131,   142,   143,    34,   133,
      36,     6,    23,    24,    25,    26,    48,   148,   153,    37,
      27,    28,    29
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,    44,    45,    47,   111,   120,    49,   140,    80,   116,
      30,   132,     3,     7,    21,    78,    30,     4,    33,    35,
      82,    83,    71,    87,    84,    52,    53,    54,    55,    55,
      38,   141,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,    81,    67,
      67,    39,    40,   110,    41,   145,   -85,   -86,    72,    75,
     139,    78,   119,    67,     8,     9,    89,    10,   107,   108,
      11,   125,   109,    12,    13,    14,    15,    16,    17,   112,
      18,   113,   114,   115,   117,   118,   123,   124,   128,   152,
     126,   127,   149,    90,   151,    32,   144,    19,   130,    20,
     135,    21,   134,   136,   137,   158,   138,     0,     9,    22,
      10,     0,     0,    11,   146,     0,    12,    13,    14,    15,
      16,    17,     0,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,   155,     0,     0,     0,   157,     0,
      19,     0,    20,     0,    21,    46,     9,     0,    10,     0,
       0,    11,    22,     0,    12,    13,    14,    15,    16,    17,
       0,    18,    50,    51,    52,    53,    54,    55,     0,     0,
       0,     0,   -87,   -87,   -87,   -87,   -87,   -87,    19,     0,
      20,   147,    21,     0,     0,     0,     0,     0,    67,     0,
      22,     0,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,    69,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,     0,     0,     0,     0,    66,     0,    67,     0,
      68,    69,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,     0,    88,     0,     0,     0,     0,    67,     0,
      68,    69,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,     0,   121,     0,     0,     0,     0,    67,     0,
      68,    69,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,     0,   122,     0,     0,     0,     0,    67,     0,
      68,    69,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,     0,     0,     0,     0,   150,     0,    67,     0,
      68,    69,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,     0,   156,     0,     0,     0,     0,    67,     0,
      68,    69,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,    50,    51,    52,    53,    54,    55,    67,     0,
      68,    69,    58,    59,    60,    61,    62,    63,    64,    65,
       0,     0,    50,    51,    52,    53,    54,    55,    67,     0,
      68,    69,    58,    59,    60,    61,    62,    63,    64,    50,
      51,    52,    53,    54,    55,     0,     0,     0,    67,    58,
      59,    60,    61,    62,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67
};

static const yytype_int16 yycheck[] =
{
      18,    19,    20,    21,    72,    81,    22,    17,     5,    77,
      38,    37,     0,    17,    40,    43,    38,    10,    17,    17,
      38,    39,    27,    41,    40,    20,    21,    22,    23,    23,
      38,    41,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    45,    44,
      44,    38,    17,    71,    38,   131,    27,    27,    17,    11,
     128,    43,    80,    44,     3,     4,    42,     6,    17,    17,
       9,    89,    17,    12,    13,    14,    15,    16,    17,    39,
      19,    45,    17,    40,    17,    26,    39,    45,    17,     8,
      41,   107,    42,    48,    38,     7,   131,    36,   114,    38,
     118,    40,   116,   121,   122,   157,   124,    -1,     4,    48,
       6,    -1,    -1,     9,   132,    -1,    12,    13,    14,    15,
      16,    17,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,    -1,    -1,    -1,   156,    -1,
      36,    -1,    38,    -1,    40,    41,     4,    -1,     6,    -1,
      -1,     9,    48,    -1,    12,    13,    14,    15,    16,    17,
      -1,    19,    18,    19,    20,    21,    22,    23,    -1,    -1,
      -1,    -1,    28,    29,    30,    31,    32,    33,    36,    -1,
      38,     7,    40,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      48,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      46,    47,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    44,    -1,
      46,    47,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    47,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    47,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    47,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    44,    -1,
      46,    47,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    47,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    18,    19,    20,    21,    22,    23,    44,    -1,
      46,    47,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    18,    19,    20,    21,    22,    23,    44,    -1,
      46,    47,    28,    29,    30,    31,    32,    33,    34,    18,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    44,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    51,    53,     0,    10,    54,    71,    17,     3,     4,
       6,     9,    12,    13,    14,    15,    16,    17,    19,    36,
      38,    40,    48,    72,    73,    74,    75,    80,    81,    82,
      38,    55,    56,    17,    68,    17,    70,    79,    38,    38,
      17,    38,    62,    73,    73,    73,    41,    73,    76,    62,
      18,    19,    20,    21,    22,    23,    24,    25,    28,    29,
      30,    31,    32,    33,    34,    35,    42,    44,    46,    47,
      52,    27,    17,    57,    58,    11,    60,    56,    43,    59,
       5,    45,    73,    73,    62,    63,    64,    73,    39,    42,
      52,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    17,    17,    17,
      73,    59,    39,    45,    17,    40,    59,    17,    26,    73,
      70,    39,    39,    39,    45,    73,    41,    62,    17,    61,
      62,    65,    37,    69,    75,    73,    73,    73,    73,    59,
      17,    41,    66,    67,    68,    70,    73,     7,    77,    42,
      42,    38,     8,    78,    73,    73,    39,    73,    77
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    52,    52,    53,    53,    54,    55,    55,
      56,    57,    57,    58,    58,    59,    59,    60,    60,    61,
      61,    62,    63,    63,    64,    64,    65,    65,    66,    66,
      67,    68,    69,    69,    70,    71,    71,    72,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    74,    74,    74,    74,    75,    75,    76,    76,    77,
      77,    78,    78,    79,    79,    80,    80,    81,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     0,     1,     0,     2,     7,     0,     1,
       3,     0,     1,     2,     4,     0,     2,     0,     3,     0,
       1,     3,     0,     1,     1,     3,     0,     2,     1,     1,
       2,     4,     3,     1,     4,     0,     2,     2,     1,     1,
       3,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       7,     5,     4,     2,     4,     1,     3,     3,     3,     3,
       2,     1,     1,     1,     1,     2,     4,     1,     3,     0,
       6,     0,     2,     1,     3,     1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: type_def_list func_def_list expression SEMICOLON_opt  */
#line 98 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_program_create((yyvsp[-1].node), (yyvsp[-2].list), (yyvsp[-3].list),
              (yylsp[-3]).first_line, (yylsp[-3]).first_column);

        ast_root = (yyval.node);
    }
#line 1725 "build/parser.tab.c"
    break;

  case 5: /* type_def_list: %empty  */
#line 110 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(0, NULL, NULL, NULL, NULL);
    }
#line 1733 "build/parser.tab.c"
    break;

  case 6: /* type_def_list: type_def_list type_def  */
#line 115 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-1].list);
        list_append((yyval.list), (yyvsp[0].node));
    }
#line 1742 "build/parser.tab.c"
    break;

  case 7: /* type_def: TYPE ID params_list_opt type_inheritance LBRACE type_body_elements RBRACE  */
#line 123 "src/parser/hulk_parser.y"
    {
        List *attr_list = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
        List *method_list = list_create(0, NULL, NULL, NULL, NULL);

        for (size_t i = 0; i < list_count((yyvsp[-1].list)); i++)
        {
            TypeBodyElem *elem = (TypeBodyElem *)list_get((yyvsp[-1].list), i);

            if (elem->kind == 0)
                list_append(attr_list, elem->data.attr);
            else
                list_append(method_list, elem->data.method);
        }

        list_destroy((yyvsp[-1].list));

        char *father_name = NULL;
        List *father_args = NULL;

        if ((yyvsp[-3].type_inherit) != NULL)
        {
            father_name = (yyvsp[-3].type_inherit)->father_name;
            father_args = (yyvsp[-3].type_inherit)->father_args;
            free((yyvsp[-3].type_inherit));
        }

        (yyval.node) = (ASTNode *)ast_type_definition_create((yyvsp[-5].string_value), (yyvsp[-4].list), father_name,
              father_args, attr_list, method_list, (yylsp[-6]).first_line, (yylsp[-6]).first_column);

        free((yyvsp[-5].string_value));
        free(father_name);
    }
#line 1779 "build/parser.tab.c"
    break;

  case 8: /* params_list_opt: %empty  */
#line 159 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    }
#line 1787 "build/parser.tab.c"
    break;

  case 9: /* params_list_opt: params_list  */
#line 164 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[0].list);
    }
#line 1795 "build/parser.tab.c"
    break;

  case 10: /* params_list: LPAREN params_sequence_opt RPAREN  */
#line 171 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-1].list);
    }
#line 1803 "build/parser.tab.c"
    break;

  case 11: /* params_sequence_opt: %empty  */
#line 178 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(0, NULL, (free_func)symbol_binding_destroy, NULL, NULL);
    }
#line 1811 "build/parser.tab.c"
    break;

  case 12: /* params_sequence_opt: params_sequence  */
#line 183 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[0].list);
    }
#line 1819 "build/parser.tab.c"
    break;

  case 13: /* params_sequence: ID type_annotation_opt  */
#line 190 "src/parser/hulk_parser.y"
    {
        SymbolBinding *sb = symbol_binding_create((yyvsp[-1].string_value), (yyvsp[0].string_value), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[-1].string_value));
        if ((yyvsp[0].string_value)) free((yyvsp[0].string_value));

        (yyval.list) = list_create(1, (void *[]){ sb }, (free_func)symbol_binding_destroy, NULL, NULL);
    }
#line 1831 "build/parser.tab.c"
    break;

  case 14: /* params_sequence: params_sequence COMMA ID type_annotation_opt  */
#line 199 "src/parser/hulk_parser.y"
    {
        SymbolBinding *sb = symbol_binding_create((yyvsp[-1].string_value), (yyvsp[0].string_value), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[-1].string_value));
        if ((yyvsp[0].string_value)) free((yyvsp[0].string_value));

        (yyval.list) = (yyvsp[-3].list);
        list_append((yyval.list), sb);
    }
#line 1844 "build/parser.tab.c"
    break;

  case 15: /* type_annotation_opt: %empty  */
#line 211 "src/parser/hulk_parser.y"
    {
        (yyval.string_value) = NULL;
    }
#line 1852 "build/parser.tab.c"
    break;

  case 16: /* type_annotation_opt: COLON ID  */
#line 216 "src/parser/hulk_parser.y"
    {
        (yyval.string_value) = (yyvsp[0].string_value);
    }
#line 1860 "build/parser.tab.c"
    break;

  case 17: /* type_inheritance: %empty  */
#line 223 "src/parser/hulk_parser.y"
    {
        (yyval.type_inherit) = NULL;
    }
#line 1868 "build/parser.tab.c"
    break;

  case 18: /* type_inheritance: INHERITS ID args_list_opt  */
#line 228 "src/parser/hulk_parser.y"
    {
        TypeInheritance *inherit = (TypeInheritance *)malloc(sizeof(TypeInheritance));
        inherit->father_name = (yyvsp[-1].string_value);
        inherit->father_args = (yyvsp[0].list);
        (yyval.type_inherit) = inherit;
    }
#line 1879 "build/parser.tab.c"
    break;

  case 19: /* args_list_opt: %empty  */
#line 238 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(0, NULL, NULL, NULL, NULL);
    }
#line 1887 "build/parser.tab.c"
    break;

  case 20: /* args_list_opt: args_list  */
#line 243 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[0].list);
    }
#line 1895 "build/parser.tab.c"
    break;

  case 21: /* args_list: LPAREN args_sequence_opt RPAREN  */
#line 250 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-1].list);
    }
#line 1903 "build/parser.tab.c"
    break;

  case 22: /* args_sequence_opt: %empty  */
#line 257 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(0, NULL, NULL, NULL, NULL);
    }
#line 1911 "build/parser.tab.c"
    break;

  case 23: /* args_sequence_opt: args_sequence  */
#line 262 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[0].list);
    }
#line 1919 "build/parser.tab.c"
    break;

  case 24: /* args_sequence: expression  */
#line 269 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(1, (void *[]){ (yyvsp[0].node) }, NULL, NULL, NULL);
    }
#line 1927 "build/parser.tab.c"
    break;

  case 25: /* args_sequence: args_sequence COMMA expression  */
#line 274 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-2].list);
        list_append((yyval.list), (yyvsp[0].node));
    }
#line 1936 "build/parser.tab.c"
    break;

  case 26: /* type_body_elements: %empty  */
#line 282 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(0, NULL, free, NULL, NULL);
    }
#line 1944 "build/parser.tab.c"
    break;

  case 27: /* type_body_elements: type_body_elements type_body_element  */
#line 287 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-1].list);
        list_append((yyval.list), (yyvsp[0].type_elem));
    }
#line 1953 "build/parser.tab.c"
    break;

  case 28: /* type_body_element: attr_def  */
#line 295 "src/parser/hulk_parser.y"
    {
        (yyval.type_elem) = (yyvsp[0].type_elem);
    }
#line 1961 "build/parser.tab.c"
    break;

  case 29: /* type_body_element: method_def  */
#line 300 "src/parser/hulk_parser.y"
    {
        TypeBodyElem *elem = (TypeBodyElem *)malloc(sizeof(TypeBodyElem));
        elem->kind = 1;
        elem->data.method = (yyvsp[0].node);
        (yyval.type_elem) = elem;
    }
#line 1972 "build/parser.tab.c"
    break;

  case 30: /* attr_def: var_binding SEMICOLON  */
#line 310 "src/parser/hulk_parser.y"
    {
        TypeBodyElem *elem = (TypeBodyElem *)malloc(sizeof(TypeBodyElem));
        elem->kind = 0;
        elem->data.attr = (yyvsp[-1].binding);
        (yyval.type_elem) = elem;
    }
#line 1983 "build/parser.tab.c"
    break;

  case 31: /* method_def: ID params_list type_annotation_opt method_body  */
#line 320 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_function_definition_create((yyvsp[-3].string_value), (yyvsp[-2].list),
               (yyvsp[-1].string_value), (yyvsp[0].node), (yylsp[-3]).first_line, (yylsp[-3]).first_column);
        free((yyvsp[-3].string_value));
        if ((yyvsp[-1].string_value)) free((yyvsp[-1].string_value));
    }
#line 1994 "build/parser.tab.c"
    break;

  case 32: /* method_body: ARROW expression SEMICOLON  */
#line 330 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 2002 "build/parser.tab.c"
    break;

  case 33: /* method_body: expr_block  */
#line 335 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2010 "build/parser.tab.c"
    break;

  case 34: /* var_binding: ID type_annotation_opt ASSIGN expression  */
#line 342 "src/parser/hulk_parser.y"
    {
        (yyval.binding) = symbol_binding_create((yyvsp[-3].string_value), (yyvsp[-2].string_value), (yyvsp[0].node), (yylsp[-3]).first_line, (yylsp[-3]).first_column);
        free((yyvsp[-3].string_value));
        if ((yyvsp[-2].string_value)) free((yyvsp[-2].string_value));
    }
#line 2020 "build/parser.tab.c"
    break;

  case 35: /* func_def_list: %empty  */
#line 351 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(0, NULL, NULL, NULL, NULL);
    }
#line 2028 "build/parser.tab.c"
    break;

  case 36: /* func_def_list: func_def_list func_def  */
#line 356 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-1].list);
        list_append((yyval.list), (yyvsp[0].node));
    }
#line 2037 "build/parser.tab.c"
    break;

  case 37: /* func_def: FUNCTION method_def  */
#line 364 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2045 "build/parser.tab.c"
    break;

  case 38: /* expression: var_reference  */
#line 371 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2053 "build/parser.tab.c"
    break;

  case 39: /* expression: literal  */
#line 376 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2061 "build/parser.tab.c"
    break;

  case 40: /* expression: LPAREN expression RPAREN  */
#line 381 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 2069 "build/parser.tab.c"
    break;

  case 41: /* expression: NOT expression  */
#line 386 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_unary_create(OP_NOT, (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2077 "build/parser.tab.c"
    break;

  case 42: /* expression: MINUS expression  */
#line 391 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_unary_create(OP_SUB, (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2085 "build/parser.tab.c"
    break;

  case 43: /* expression: expression PLUS expression  */
#line 396 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_ADD, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2093 "build/parser.tab.c"
    break;

  case 44: /* expression: expression MINUS expression  */
#line 401 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_SUB, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2101 "build/parser.tab.c"
    break;

  case 45: /* expression: expression STAR expression  */
#line 406 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_MULT, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2109 "build/parser.tab.c"
    break;

  case 46: /* expression: expression SLASH expression  */
#line 411 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_DIV, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2117 "build/parser.tab.c"
    break;

  case 47: /* expression: expression PERCENT expression  */
#line 416 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_MOD, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2125 "build/parser.tab.c"
    break;

  case 48: /* expression: expression POWER expression  */
#line 421 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_EXP, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2133 "build/parser.tab.c"
    break;

  case 49: /* expression: expression CONCAT expression  */
#line 426 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_CONCAT, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2141 "build/parser.tab.c"
    break;

  case 50: /* expression: expression CONCAT_WS expression  */
#line 431 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_CONCAT_WS, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2149 "build/parser.tab.c"
    break;

  case 51: /* expression: expression GREATER expression  */
#line 436 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_GREATER, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2157 "build/parser.tab.c"
    break;

  case 52: /* expression: expression LESS expression  */
#line 441 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_LESS, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2165 "build/parser.tab.c"
    break;

  case 53: /* expression: expression GREATER_EQ expression  */
#line 446 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_GREATER_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2173 "build/parser.tab.c"
    break;

  case 54: /* expression: expression LESS_EQ expression  */
#line 451 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_LESS_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2181 "build/parser.tab.c"
    break;

  case 55: /* expression: expression EQUALS expression  */
#line 456 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2189 "build/parser.tab.c"
    break;

  case 56: /* expression: expression NOT_EQUALS expression  */
#line 461 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_NOT_EQUAL, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2197 "build/parser.tab.c"
    break;

  case 57: /* expression: expression AND expression  */
#line 466 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_AND, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2205 "build/parser.tab.c"
    break;

  case 58: /* expression: expression OR expression  */
#line 471 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_binary_create(OP_OR, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2213 "build/parser.tab.c"
    break;

  case 59: /* expression: expr_block  */
#line 476 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2221 "build/parser.tab.c"
    break;

  case 60: /* expression: IF LPAREN expression RPAREN expression elif_clauses else_clause  */
#line 481 "src/parser/hulk_parser.y"
    {
        ASTNode *else_part = (yyvsp[-1].node);

        if (else_part == NULL)
        {
            else_part = (yyvsp[0].node);
        }
        else
        {
            append_else((ConditionalNode *)else_part, (yyvsp[0].node));
        }

        (yyval.node) = (ASTNode *)ast_conditional_create((yyvsp[-4].node), (yyvsp[-2].node), else_part,
              (yylsp[-6]).first_line, (yylsp[-6]).first_column);
    }
#line 2241 "build/parser.tab.c"
    break;

  case 61: /* expression: WHILE LPAREN expression RPAREN expression  */
#line 498 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_while_create((yyvsp[-2].node), (yyvsp[0].node),
              (yylsp[-4]).first_line, (yylsp[-4]).first_column);
    }
#line 2250 "build/parser.tab.c"
    break;

  case 62: /* expression: LET bindings IN expression  */
#line 504 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_let_create((yyvsp[-2].list), (yyvsp[0].node),
              (yylsp[-3]).first_line, (yylsp[-3]).first_column);
    }
#line 2259 "build/parser.tab.c"
    break;

  case 63: /* expression: ID args_list  */
#line 510 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_function_call_create((yyvsp[-1].string_value), (yyvsp[0].list),
              (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[-1].string_value));
    }
#line 2269 "build/parser.tab.c"
    break;

  case 64: /* expression: expression DOT ID args_list  */
#line 517 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_method_access_create((yyvsp[-3].node), (yyvsp[-1].string_value), (yyvsp[0].list),
              (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[-1].string_value));
    }
#line 2279 "build/parser.tab.c"
    break;

  case 65: /* expression: attr_access  */
#line 524 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2287 "build/parser.tab.c"
    break;

  case 66: /* expression: NEW ID args_list  */
#line 529 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_type_instanciation_create((yyvsp[-1].string_value), (yyvsp[0].list),
              (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[-1].string_value));
    }
#line 2297 "build/parser.tab.c"
    break;

  case 67: /* expression: assignable REASSIGN expression  */
#line 536 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_reassignment_create((yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2305 "build/parser.tab.c"
    break;

  case 68: /* expression: expression IS ID  */
#line 541 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_is_create((yyvsp[-2].node), (yyvsp[0].string_value), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[0].string_value));
    }
#line 2314 "build/parser.tab.c"
    break;

  case 69: /* expression: expression AS ID  */
#line 547 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_as_create((yyvsp[-2].node), (yyvsp[0].string_value), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        free((yyvsp[0].string_value));
    }
#line 2323 "build/parser.tab.c"
    break;

  case 70: /* expression: BASE args_list  */
#line 553 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_base_call_create((yyvsp[0].list), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2331 "build/parser.tab.c"
    break;

  case 71: /* literal: NUMBER  */
#line 560 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_number_literal_create((yyvsp[0].number_value), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2339 "build/parser.tab.c"
    break;

  case 72: /* literal: STRING  */
#line 565 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_string_literal_create((yyvsp[0].string_value), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2347 "build/parser.tab.c"
    break;

  case 73: /* literal: TRUE  */
#line 570 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_bool_literal_create((yyvsp[0].bool_value), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2355 "build/parser.tab.c"
    break;

  case 74: /* literal: FALSE  */
#line 575 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_bool_literal_create((yyvsp[0].bool_value), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2363 "build/parser.tab.c"
    break;

  case 75: /* expr_block: LBRACE RBRACE  */
#line 582 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_block_create(list_create(0, NULL, NULL, NULL, NULL), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2371 "build/parser.tab.c"
    break;

  case 76: /* expr_block: LBRACE expr_list SEMICOLON_opt RBRACE  */
#line 586 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_block_create((yyvsp[-2].list), (yylsp[-3]).first_line, (yylsp[-3]).first_column);
    }
#line 2379 "build/parser.tab.c"
    break;

  case 77: /* expr_list: expression  */
#line 593 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(1, (void *[]){ (yyvsp[0].node) }, NULL, NULL, NULL);
    }
#line 2387 "build/parser.tab.c"
    break;

  case 78: /* expr_list: expr_list SEMICOLON expression  */
#line 597 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-2].list);
        list_append((yyval.list), (yyvsp[0].node));
    }
#line 2396 "build/parser.tab.c"
    break;

  case 79: /* elif_clauses: %empty  */
#line 605 "src/parser/hulk_parser.y"
    {
        (yyval.node) = NULL;
    }
#line 2404 "build/parser.tab.c"
    break;

  case 80: /* elif_clauses: ELIF LPAREN expression RPAREN expression elif_clauses  */
#line 610 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_conditional_create((yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node),
              (yylsp[-5]).first_line, (yylsp[-5]).first_column);
    }
#line 2413 "build/parser.tab.c"
    break;

  case 81: /* else_clause: %empty  */
#line 618 "src/parser/hulk_parser.y"
    {
        (yyval.node) = NULL;
    }
#line 2421 "build/parser.tab.c"
    break;

  case 82: /* else_clause: ELSE expression  */
#line 623 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2429 "build/parser.tab.c"
    break;

  case 83: /* bindings: var_binding  */
#line 630 "src/parser/hulk_parser.y"
    {
        (yyval.list) = list_create(1, (void *[]){ (yyvsp[0].binding) }, (free_func)symbol_binding_destroy, NULL, NULL);
    }
#line 2437 "build/parser.tab.c"
    break;

  case 84: /* bindings: bindings COMMA var_binding  */
#line 635 "src/parser/hulk_parser.y"
    {
        (yyval.list) = (yyvsp[-2].list);
        list_append((yyval.list), (yyvsp[0].binding));
    }
#line 2446 "build/parser.tab.c"
    break;

  case 87: /* var_reference: ID  */
#line 648 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_variable_reference_create((yyvsp[0].string_value), (yylsp[0]).first_line, (yylsp[0]).first_column);
        free((yyvsp[0].string_value));
    }
#line 2455 "build/parser.tab.c"
    break;

  case 88: /* attr_access: expression DOT ID  */
#line 656 "src/parser/hulk_parser.y"
    {
        (yyval.node) = (ASTNode *)ast_attribute_access_create((yyvsp[-2].node), (yyvsp[0].string_value), (yylsp[0]).first_line, (yylsp[0]).first_column);
        free((yyvsp[0].string_value));
    }
#line 2464 "build/parser.tab.c"
    break;


#line 2468 "build/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 662 "src/parser/hulk_parser.y"


void yyerror(const char *msg)
{
    dm_add_error(dm_global, ERROR_TYPE_SYNTACTIC, yylineno, yylloc.first_column, "%s", msg);
}

void append_else(ConditionalNode *chain, ASTNode *final_else)
{
    ConditionalNode *curr = chain;
    while (curr->else_branch != NULL && curr->else_branch->node_type == NODE_CONDITIONAL)
    {
        curr = (ConditionalNode *)curr->else_branch;
    }
    curr->else_branch = final_else;
}
