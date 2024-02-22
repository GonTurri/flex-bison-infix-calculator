# TP 4: FLEX Y BISON

## Analizador semántico para una calculadora utilizando las herramientas Flex y Bison.

Recomendable correr con entorno de linux
La calculadora te permite declarar y asignar variables, usar funciones logaritmo y trigonometricas y viene con mecanismo de deteccion de errores

# Como correr

``` shell
    make
    ./calculadora <archivo de entrada opcional
```
# Ejemplo entrada con salida

Entrada: 

```
var a = 1
var b = 2
var c = 3
var z = (a += (b *= c)) / 2
a
b
c
log(10)
cos(pi)
sin(pi)
salir
```

Salida: 

```
a: 1.000000
b: 2.000000
c: 3.000000
z: 3.500000
7.000000
6.000000
3.000000
2.302585
-1.000000
0.000000
```



