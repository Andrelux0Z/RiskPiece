#ifndef GENERACION_TERRENO_H
#define GENERACION_TERRENO_H

// Estructura que representa un territorio en el juego
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

// Función para crear un nuevo territorio
Territorio *crear_territorio(const char *codigo, const char *nombre, const char conexiones[][3], int cantidad_conexiones, int A, int B, int C);

// Función para agregar un territorio al final de la lista
void agregar_territorio(Territorio **cabeza, Territorio *nodo);

// Funcion para inicializar random los valores de las problematicas
void valoresProblematicas(Territorio *cabeza);

// Incrementa una estadística (A/B/C) del territorio; si está al máximo, propaga a vecinos
void aumentar_estadistica_vecinos(Territorio *cabeza, const char *codigo, char estadistica);

// Funcion auxiliar para barajar (Fisher-Yates shuffle) usada en valoresProblematicas
void barajar(int *arreglo, int n);

// Función para imprimir la tabla de territorios
void imprimir_tabla(Territorio *cabeza);

// Función para liberar la memoria de la lista enlazada
void liberar_lista(Territorio *cabeza);

// Función para construir una lista de ejemplo con 10 territorios
Territorio *construir_lista_ejemplo(void);

// Incrementa una estadística (A, B o C) de un territorio identificado por su código.
// Si la estadística indicada ya es 3, se incrementan en 1 las otras dos, sin pasarse de 3.
int aumentar_estadistica_dificil(Territorio *cabeza, const char *codigo, char estadistica);

#endif