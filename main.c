#include <stdio.h>
#include "calculadora.h"
#include "parser.h"

int nsemnterrs =0;
int yylexerrs = 0;
struct simbolo_tabla * tabla_de_simbolos = 0;

int main(void){
    inicializar_tabla();
    yyparse();
    liberarMemoriaTabla();
    return 0;
}