%{
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
#include "calculadora.h"
char msg[120]; //para poder armar mensajes a enviar a yyerror
%}

%option header-file="scanner.h"
%option outfile="scanner.c"
%option nounput
%option noinput

digito [0-9]
letra [a-zA-Z]
expo [eE][+-]?{digito}+
frac {digito}*\.{digito}+|{digito}+\.{digito}*
inval [0-9.]
otro [[:punct:]]{-}[()+\-/*^=]

%%
salir                                                       return SALIR;
var                                                         return VAR;
{letra}({digito}|{letra})*                                  {if(!(yylval.reg = obtener_simbolo(yytext))) llenar_registro(&yylval.reg,yytext); return yylval.reg->tipo==VAR? IDENTIFICADOR: FUNC;}
{digito}+{expo}?                                            |
{frac}{expo}?                                               {sscanf(yytext,"%lf",&yylval.val);return NUMERO;}             
"+="                                                        return MAS_IGUAL;
"-="                                                        return MENOS_IGUAL;
"*="                                                        return POR_IGUAL;
"/="                                                        return DIVIDIDO_IGUAL;
"="                                                         return '=';
"("                                                         return '(';
")"                                                         return ')';
"+"                                                         return '+';
"*"                                                         return '*';
"-"                                                         return '-';
"/"                                                         return '/';
"^"                                                         return '^'; 
{letra}({digito}|{letra})*{otro}+({digito}|{letra}|{otro})* {sprintf(msg,"Error lexico: identificador invalido: %s\n",yytext); yylexerrs++; yyerror(msg);}
{digito}+{expo}({letra}|{otro})({digito}|{letra}|{otro})*   |
{frac}({inval}|{letra}|{otro})+{expo}?({inval}|{letra}|{otro})*  {sprintf(msg,"Error lexico: REAL invalido: %s\n",yytext);yylexerrs++; yyerror(msg);}
{digito}+({letra}|{otro})({digito}|{letra}|{otro})*         {sprintf(msg,"Error lexico: constante invalida: %s\n",yytext);yylexerrs++; yyerror(msg);}
{otro}+                                                     {sprintf(msg,"Error lexico: cadena desconocida: %s\n",yytext);yylexerrs++; yyerror(msg);}
\n                                                          return '\n';
" "                                                         ;
%%