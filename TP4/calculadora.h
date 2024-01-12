#ifndef CALCULADORA_H_INCLUDED
#define CALCULADORA_H_INCLUDED



struct simbolo_tabla{
    char * lexema;
    int tipo;
    union{
        double nro;
        double (*func)(double);

    }valor;
    struct simbolo_tabla * sig;
};
 // POR AHORA IMPLEMENTO COMO LINKED LIST

extern struct simbolo_tabla* tabla_de_simbolos;

extern int nsemnterrs;


struct simbolo_tabla* agregar_simbolo(char* nombre, int tipo);
struct simbolo_tabla* obtener_simbolo(char * nombre);
int declarar(struct simbolo_tabla** sem_reg);
int verificar_id(char *nombre);
void llenar_registro(struct simbolo_tabla ** sem_reg,char* nombre);
void inicializar_tabla(void);
void liberarMemoriaTabla(void);
void liberarMiTad(struct simbolo_tabla * elem);
void salir (void);

int fue_declarado(char * nombre);

#endif