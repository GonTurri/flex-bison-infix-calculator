%code top{
#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include "scanner.h"
#include "calculadora.h"
}

%union{
    double val;
    struct simbolo_tabla * reg;
}

%code provides{
void yyerror(const char *);
extern int yylexerrs;
}

%defines "parser.h"
%output "parser.c"



%token <val> NUMERO
%token <reg> IDENTIFICADOR FUNC
%type <val> expresion
%token SALIR VAR
%token MAS_IGUAL "+=" 
%token MENOS_IGUAL "-="
%token DIVIDIDO_IGUAL "/="
%token POR_IGUAL "*="



%right '=' "+=" "-+" "*="
%left '+' '-'
%left '/' '*'
%left NEG
%right '^'


%define parse.error verbose


%%
todo:       sesion                                  { if(yynerrs || yylexerrs || nsemnterrs) YYABORT; else YYACCEPT; };
sesion:     sesion linea | %empty;
linea:      expresion '\n'                          {printf ("%f\n", $1); }|
            VAR IDENTIFICADOR '\n'                  {if(!declarar(&$2)) printf("%s: %f\n",$2->lexema,$2->valor.nro);} |
            VAR IDENTIFICADOR  '=' expresion '\n'   {if(!declarar(&$2)) {$2->valor.nro = $4; printf("%s: %f\n",$2->lexema,$2->valor.nro);}} |
            SALIR {salir();}|                                 
            '\n' |                                  
            error '\n' ;
expresion:  expresion '+' expresion                 {$$ = $1 + $3;}|
            expresion '-' expresion                 {$$ = $1 - $3;} |
            expresion '*' expresion                 {$$ = $1 * $3;}|
            expresion '/' expresion                 {$$ = $1 / $3;}|
            expresion '^' expresion                 {$$ = pow ($1,$3);}|
            '-' expresion %prec NEG                 {$$ = -$2;}| 
            IDENTIFICADOR                           {if(verificar_id($1->lexema)) {liberarMiTad($1); YYERROR;} else $$ = $1->valor.nro;}|
            IDENTIFICADOR '=' expresion             {if(verificar_id($1->lexema)) {liberarMiTad($1); YYERROR;} else $$ = $1->valor.nro = $3;} | 
            NUMERO                                  {$$ = $1;} |
            '(' expresion ')'                       { $$ = $2;} | 
            IDENTIFICADOR "+=" expresion            {if(verificar_id($1->lexema)) {liberarMiTad($1); YYERROR;} else $$ = $1->valor.nro += $3;} |
            IDENTIFICADOR "-=" expresion            {if(verificar_id($1->lexema)) {liberarMiTad($1); YYERROR;} else $$ = $1->valor.nro -= $3;} |
            IDENTIFICADOR "*=" expresion            {if(verificar_id($1->lexema)) {liberarMiTad($1); YYERROR;} $$ = $1->valor.nro *= $3;} |
            IDENTIFICADOR "/=" expresion            {if(verificar_id($1->lexema)) {liberarMiTad($1); YYERROR;} else $$ = $1->valor.nro /= $3;} |
            FUNC '(' expresion ')'                  {$$ = $1->valor.func($3);} ;
%%

void yyerror(const char *s){
	printf("%s\n\n",s);
	return;
}



