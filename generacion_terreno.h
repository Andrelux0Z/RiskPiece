#ifndef GENERACION_TERRENO_H
#define GENERACION_TERRENO_H

/*
* Struct que define el territorio (esto es una lista doblemente enlazada)
* Cada nodo tiene un código, nombre, conexiones, A/B/C (cada problema), anterior/siguiente
*/
typedef struct Territorio
{
    char codigo[3];
    char nombre[32];
    char conexiones[4][3];
    int cantidad_conexiones;
    int A;
    int B;
    int C;
    struct Territorio *anterior;
    struct Territorio *siguiente;
} Territorio;

/*
* Funcion para crear un territorio, normalmente iniciamos con cabeza = NULL (como inicio).
* La función sirve para crear también los nodos, por lo que ocupamos código,
* nombre, conexiones (y su cantidad) y los valores de los 3 problemas.
* Retorna el territorio.
*/
Territorio *crear_territorio(const char *codigo, const char *nombre, const char conexiones[][3], int cantidad_conexiones, int A, int B, int C);

/*
* Funcion para agregar un territorio (nodo) creado con crear_territorio
* lo que hace es poner este nuevo nodo al final de la lista doblemente enlazada
*/
void agregar_territorio(Territorio **cabeza, Territorio *nodo);

/*
* Funcion para randomizar los valores de las problematicas.
*/
void valoresProblematicas(Territorio *cabeza);

/*
* Funcion que baraja, lo utilizamos para randomizar. usa un método llamado
* Fisher-Yates, cuyo objetivo es randomizar un array.
*/
void barajar(int *arreglo, int n);

/*
* Funcion para imprimir los territorios.
* Se manda cabeza, que es la lista doblemente enlazada que representa los territorios
*/
void imprimir_tabla(Territorio *cabeza);

/*
* Funcion para liberar la memoria utilizada, y evitar fugas de memoria
*/
void liberar_lista(Territorio *cabeza);

/*
* Funcion que usa las otras funciones para crear la lista de territorio,
* con 9 territorios interconectados entre sí. Es ejemplo, porque se puede
* reconstruir para hacer otros países
*/
Territorio *construir_lista_ejemplo(void);

/*
* Funcion que incrementa una estadística (A, B o C) del territorio con el código dado.
* Si la estadística ya es 3, se incrementan en 1 las otras dos, sin pasarse de 3.
*/
int aumentar_estadistica(Territorio *cabeza, const char *codigo, char estadistica);

#endif