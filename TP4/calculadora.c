#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "calculadora.h"


char buffer [120]; //para poder armar mensajes a enviar a yyerror


//BUSCA EN LA LISTA UN REGISTRO CON ESE LEXEMA DE NOMBRE SI LO ENCUENTRA LO DEVUELVE, SI NO DEVUELVE 0
struct simbolo_tabla* obtener_simbolo(char * nombre){
    struct simbolo_tabla* aux = tabla_de_simbolos;
    while (aux != 0){
        if(!strcmp(nombre, aux->lexema)) return aux;  
        aux = aux->sig;
    }
    return 0;
}


//ESTE EJEMPLO AGREGA AL PRINCIPIO DE LA LISTA, SE PODRIA HACER AGREGANDO AL FINAL Y TE DEVUELVE EL PUNTERO AL OBJETO AGREGADO
struct simbolo_tabla* agregar_simbolo(char* nombre, int tipo){
    struct simbolo_tabla* aux = malloc(sizeof(struct simbolo_tabla));
    // aux->lexema = malloc(strlen(nombre) + 1);
    // strcpy(aux->lexema,nombre);
    aux->lexema = strdup(nombre);
    aux->tipo = tipo;
    aux->valor.nro = 0;
    aux->sig = tabla_de_simbolos;
    tabla_de_simbolos = aux;
    return aux;
}

struct reg_funcion
{
  char *nombre;
  double (*func)(double);
};

struct reg_funcion funciones_aritmeticas[]
  = {
      {"sin", sin},
      {"cos", cos},
      {"tan",tan},
      {"asin",asin},
      {"acos",acos},
      {"atan", atan},
      {"log", log},
      {"sqrt", sqrt},
      {0, 0}
    };

struct reg_constante{
    char * nombre;
    double valor;
};

struct reg_constante constantes[]= {{"e",M_E},{"pi",M_PI},{0,0}};


// me devuelve si un ID ya fue declarado
int fue_declarado(char * nombre){
    return obtener_simbolo(nombre) != 0;
}

int declarar(struct simbolo_tabla ** sem_reg){
    struct simbolo_tabla * aux = *sem_reg;
    if(fue_declarado(aux->lexema)) {
        nsemnterrs++;
        yyerror("Error, identificador ya declarado como variable");
        return 1;
    }
    *sem_reg = agregar_simbolo(aux->lexema,VAR);
    return 0;
}

int verificar_id(char *nombre){
    if(!fue_declarado(nombre)){
        sprintf(buffer,"Error: ID %s no declarado",nombre);
        nsemnterrs++;
        yyerror(buffer);
        return 1;
    }
    return 0;
}

void llenar_registro(struct simbolo_tabla ** sem_reg,char* nombre){
    struct simbolo_tabla * aux;
    *sem_reg = malloc(sizeof(sem_reg));
    aux = *sem_reg;
    aux->lexema = strdup(nombre);
    aux->tipo = VAR;
}

void inicializar_tabla(void){
    struct simbolo_tabla * aux;
    for(int i=0;funciones_aritmeticas[i].nombre != 0; i++){
        aux = agregar_simbolo(funciones_aritmeticas[i].nombre,FUNC);
        aux->valor.func = funciones_aritmeticas[i].func;
    }
    for(int i=0;constantes[i].nombre !=0 ; i++){
        aux = agregar_simbolo(constantes[i].nombre,VAR);
        aux->valor.nro = constantes[i].valor;
    }
}

void liberarMemoriaTabla(void){
    struct simbolo_tabla * aux;
    while(tabla_de_simbolos){
        aux = tabla_de_simbolos;
        tabla_de_simbolos = tabla_de_simbolos->sig;
        free(aux);
    }
    tabla_de_simbolos = 0;
}

void salir(void){
    liberarMemoriaTabla();
    exit(0);
}
