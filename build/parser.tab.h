/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BUILD_PARSER_TAB_H_INCLUDED
# define YY_YY_BUILD_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FUNCTION = 258,                /* FUNCTION  */
    LET = 259,                     /* LET  */
    IN = 260,                      /* IN  */
    IF = 261,                      /* IF  */
    ELIF = 262,                    /* ELIF  */
    ELSE = 263,                    /* ELSE  */
    WHILE = 264,                   /* WHILE  */
    TYPE = 265,                    /* TYPE  */
    INHERITS = 266,                /* INHERITS  */
    NEW = 267,                     /* NEW  */
    TRUE = 268,                    /* TRUE  */
    FALSE = 269,                   /* FALSE  */
    NUMBER = 270,                  /* NUMBER  */
    STRING = 271,                  /* STRING  */
    ID = 272,                      /* ID  */
    PLUS = 273,                    /* PLUS  */
    MINUS = 274,                   /* MINUS  */
    STAR = 275,                    /* STAR  */
    SLASH = 276,                   /* SLASH  */
    PERCENT = 277,                 /* PERCENT  */
    POWER = 278,                   /* POWER  */
    CONCAT = 279,                  /* CONCAT  */
    CONCAT_WS = 280,               /* CONCAT_WS  */
    ASSIGN = 281,                  /* ASSIGN  */
    REASSIGN = 282,                /* REASSIGN  */
    EQUALS = 283,                  /* EQUALS  */
    NOT_EQUALS = 284,              /* NOT_EQUALS  */
    GREATER = 285,                 /* GREATER  */
    GREATER_EQ = 286,              /* GREATER_EQ  */
    LESS = 287,                    /* LESS  */
    LESS_EQ = 288,                 /* LESS_EQ  */
    AND = 289,                     /* AND  */
    OR = 290,                      /* OR  */
    NOT = 291,                     /* NOT  */
    ARROW = 292,                   /* ARROW  */
    LPAREN = 293,                  /* LPAREN  */
    RPAREN = 294,                  /* RPAREN  */
    LBRACE = 295,                  /* LBRACE  */
    RBRACE = 296,                  /* RBRACE  */
    SEMICOLON = 297,               /* SEMICOLON  */
    COLON = 298,                   /* COLON  */
    DOT = 299,                     /* DOT  */
    COMMA = 300,                   /* COMMA  */
    IS = 301,                      /* IS  */
    AS = 302,                      /* AS  */
    BASE = 303,                    /* BASE  */
    LEX_ERROR = 304                /* LEX_ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "src/parser/hulk_parser.y"

    double          number_value;
    char           *string_value;
    bool            bool_value;
    ASTNode        *node;
    List           *list;
    SymbolBinding  *binding;
    TypeBodyElem   *type_elem;
    TypeInheritance *type_inherit;

#line 124 "build/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_BUILD_PARSER_TAB_H_INCLUDED  */
