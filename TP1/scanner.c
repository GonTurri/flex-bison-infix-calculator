#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

char lexema[200];

// posibles estados del AFD
enum estados {
    INICIAL,
    RECONOCIENDO_ID,
    RECONOCIENDO_CONST,
    ARRANCA_CON_CERO,
    RECONOCIENDO_CONST_HEXA,
    RECONOCIENDO_CONST_MAL_FORMADA,
    RECONOCIENDO_ERR_GRAL,
    IDENTIFICADOR_OK = 100,
    CONSTANTE_OK,
    HEXADECIMAL_OK,
    FIN_DE_TEXTO_OK,
    ERR_CONST_MAL_FORMADA = 200,
    ERR_GRAL
};

// tipos de caracteres
enum columnas {
    CARACTER_CERO,
    DIGITS_1_AL_9,
    LETRAS_HEXA,
    EQUIS,
    LETRAS_NO_HEXA,
    FDTEXT,
    ESPACIO,
    OTROS
};

typedef enum estados estados;
typedef enum columnas columnas;

const estados TABLA_TRANSICION[7][8] = {
    {3, 2, 1, 1, 1, 103, 0, 6},
    {1, 1, 1, 1, 1, 100, 100, 100},
    {2, 2, 5, 5, 5, 101, 101, 101},
    {2, 2, 5, 4, 5, 101, 101, 101},
    {4, 4, 4, 102, 102, 102, 102, 102},
    {5, 5, 5, 5, 5, 200, 200, 200},
    {201, 201, 201, 201, 201, 201, 201, 6}
};

int aceptor(estados estado){
    return estado >= IDENTIFICADOR_OK && estado <= FIN_DE_TEXTO_OK;
}

int centinela(estados estado) {
    return estado != FIN_DE_TEXTO_OK && (estado >= IDENTIFICADOR_OK && estado <= ERR_GRAL);
}

int debo_parar(estados estado){
    return estado >= 100;
}

 columnas tipo_caracter(int caracter){
    if(caracter == '0') return CARACTER_CERO;
    
    if(isdigit(caracter)) return DIGITS_1_AL_9;
    
    if(isxdigit(caracter)) return LETRAS_HEXA;
    
    if(caracter == 'x' || caracter == 'X') return EQUIS;
    
    if(isalpha(caracter)) return LETRAS_NO_HEXA;
    
    if(caracter == EOF) return FDTEXT;
    
    if(isspace(caracter)) return ESPACIO;
    
    return OTROS;
}

int debo_agregar(estados estado){
    return estado != INICIAL && estado != FIN_DE_TEXTO_OK;
}

char* nombre_token(token t) {
    switch (t)
    {
        case IDENTIFICADOR:
            return "Identificador";
            break;
        case ENTERO:
            return "Entero";
            break;
        case HEXA:
            return "Hexadecimal";
            break;
        case ERROR_ENTERO:
            return "Entero Mal formado";
            break;
        case ERROR_GRAL:
            return "Error general";
            break;
        default:
            fprintf(stderr, "ERROR: token invalido");
            exit(1);
    }
}

token scanner(void) {
    estados estado = INICIAL;
    int caracter;
    int indice_lexema = 0;
    token t = ERROR_GRAL;

    while(!debo_parar(estado)) {
        caracter =  getchar();
        estado = TABLA_TRANSICION[estado][tipo_caracter(caracter)];
        
        if(centinela(estado)) lexema[indice_lexema] = '\0';
        
        // va a llenar el array lexema hasta el 200, despues va a seguir analizando, pero ya no va a guardar los caracteres.
        // se podria modificar a un array mas grande o capaz ir variando su tamanio, pero el profe dijo que no importa de todos modos
        else if(debo_agregar(estado) && indice_lexema <= 200) {
            lexema[indice_lexema] = caracter;
            indice_lexema++;
        }
    }
    
    if(centinela(estado)) ungetc(caracter, stdin);
    
    switch (estado)
    {
        case IDENTIFICADOR_OK:
            t = IDENTIFICADOR;
            break;
        case CONSTANTE_OK:
            t = ENTERO;
            break;
        case HEXADECIMAL_OK:
            t = HEXA;
            break;
        case FIN_DE_TEXTO_OK:
            t = FDT;
            break;
        case ERR_CONST_MAL_FORMADA:
            t = ERROR_ENTERO;
            break;
        case ERR_GRAL:
            t = ERROR_GRAL;
            break;  
        default:
            fprintf(stderr, "ERROR: no existe el token");
            exit(1);
            break;
    }
    
    return t;
}

