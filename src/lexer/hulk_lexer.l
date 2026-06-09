%{

#include "hulk_common.h"
#include "diagnostic_manager.h"
#include "parser_types.h"
#include "parser.tab.h"

char *process_string(const char *raw);

int yycolumn = 1;

#define YY_USER_ACTION                  \
    do {                                \
        yylloc.first_line = yylineno;   \
        yylloc.first_column = yycolumn;  \
        yylloc.last_line = yylineno;    \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;             \
    } while (0) ;

%}

%option noyywrap
%option nounput
%option noinput
%option yylineno
%option case-sensitive

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}[a-zA-Z0-9_]*
SPACE       [ \t\r]

%x COMMENT

%%

{SPACE}+            { /* ignorar */ }
\n                  { yycolumn = 1; }

"//".*              { /* comentario de línea */ }
"/*"                { BEGIN(COMMENT); }
<COMMENT>[^*]*      { /* ignorar */ }
<COMMENT>"*"+[^*/]* { /* ignorar */ }
<COMMENT>"*"+"/"    { BEGIN(INITIAL); }

"as"                { return AS; }
"base"              { return BASE; }
"function"          { return FUNCTION; }
"let"               { return LET; }
"in"                { return IN; }
"if"                { return IF; }
"is"                { return IS; }
"elif"              { return ELIF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"type"              { return TYPE; }
"inherits"          { return INHERITS; }
"new"               { return NEW; }
"true"              { yylval.bool_value = true;  return TRUE; }
"false"             { yylval.bool_value = false; return FALSE; }

"=>"                { return ARROW; }
"=="                { return EQUALS; }
"!="                { return NOT_EQUALS; }
">="                { return GREATER_EQ; }
"<="                { return LESS_EQ; }
":="                { return REASSIGN; }
"**"                { return POWER; }

"="                 { return ASSIGN; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }
"%"                 { return PERCENT; }
"^"                 { return POWER; }
"@"                 { return CONCAT; }
"@@"                { return CONCAT_WS; }
">"                 { return GREATER; }
"<"                 { return LESS; }
"&"                 { return AND; }
"|"                 { return OR; }
"!"                 { return NOT; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
";"                 { return SEMICOLON; }
":"                 { return COLON; }
"."                 { return DOT; }
","                 { return COMMA; }

{ID}                { 
                        yylval.string_value = strdup(yytext); 
                        return ID; 
                    }

{DIGIT}+("."{DIGIT}+)?  {
                            yylval.number_value = atof(yytext);
                            return NUMBER;
                        }

\"([^\"\\]|\\.)*\"      {
                            yylval.string_value = process_string(yytext);
                            if (!yylval.string_value) 
                            {
                                dm_add_error(dm_global, ERROR_TYPE_LEXICAL, yylineno, yycolumn - yyleng, "Invalid string literal");
                                return LEX_ERROR;
                            }
                            return STRING;
                        }

.                       {
                            dm_add_error(dm_global, ERROR_TYPE_LEXICAL, yylineno, yycolumn - 1, "Unexpected character '%c'", yytext[0]);
                            return LEX_ERROR;
                        }

%%

char *process_string(const char *raw)
{
    if (!raw || raw[0] != '\"')
        return NULL;

    size_t len = strlen(raw);
    if (len < 2 || raw[len-1] != '\"')
        return NULL;

    char *result = (char *)malloc(len - 1);
    if (!result)
        return NULL;

    size_t j = 0;
    for (size_t i = 1; i < len - 1; i++)
    {
        if (raw[i] == '\\' && i + 1 < len - 1)
        {
            i++;
            switch (raw[i])
            {
                case 'n':  result[j++] = '\n'; break;
                case 't':  result[j++] = '\t'; break;
                case '"':  result[j++] = '\"'; break;
                case '\\': result[j++] = '\\'; break;
                default:   result[j++] = raw[i]; break;
            }
        }
        else
        {
            result[j++] = raw[i];
        }
    }

    result[j] = '\0';
    return result;
}