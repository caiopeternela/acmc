%{
#include "globals.h"
#include "util.h"
#include "scan.h"

// Declara a função yylex como estática
static int yylex(void);

// Armazena o lexema do identificador ou palavra reservada
char tokenString[MAXTOKENLEN+1];
%}

%option noyywrap

digito          [0-9]
numero          {digito}{digito}*
letra           [a-zA-Z]
identificador   {letra}{letra}*
newline         \n
whitespace      [ \t\r]+

%%

"void"          { return VOID; }
"if"            { return IF; }
"int"           { return INT; }
"return"        { return RETURN; }
"while"         { return WHILE; }
"else"          { return ELSE; }

"+"             { return MAIS; }
"-"             { return SUB; }
"*"             { return MULT; }
"/"             { return DIV; }
"="             { return IGUAL; }
"<"             { return MENOR; }
"<="            { return MENIG; }
">"             { return MAIOR; }
">="            { return MAIIG; }
"=="            { return IGDAD; }
"!="            { return DIFER; }


";"             { return PV; }
","             { return VIR; }
"("             { return APAR; }
")"             { return FPAR; }
"["             { return ACOL; }
"]"             { return FCOL; }
"{"             { return ACHAV; }
"}"             { return FCHAV; }

{numero}        { return NUM; }
{identificador} { return ID; }
{newline}       { lineno++; }
{whitespace}    { /* ignora espaços em branco */ }

"/*" {
    char c;
    int commentLine = lineno;
    char p = '\0';
    do {
        c = input();
        if (c == YYEOF) {
            printf("ERRO LÉXICO: Comentário não encerrado ('/*'). LINHA: %d\n", commentLine);
            exit(EXIT_FAILURE);
        }
        if (c == '\n')
            lineno++;
        if (p == '*' && c == '/')
            break;
        p = c;
    } while (1);
}

{letra}*[0-9]+[a-zA-Z0-9]* {
    printf("ERRO LÉXICO: identificador inválido '%s'. LINHA: %d\n", yytext, lineno);
    exit(EXIT_FAILURE);
}

. {
    printf("ERRO LÉXICO: identificador inválido '%s'. LINHA: %d\n", yytext, lineno);
    exit(EXIT_FAILURE);
}
%%

// Função que retorna o próximo token do arquivo fonte
TokenType getToken(void) {
    static int firstTime = TRUE;
    TokenType currentToken;
    if (firstTime) {
        firstTime = FALSE;
        lineno++;
        yyin = source;
        yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenString, yytext, MAXTOKENLEN);
    return currentToken;
}
