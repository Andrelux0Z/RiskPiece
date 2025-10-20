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

//Función para limitar un valor al rango de 0 a 3
int limitar_a_rango_0_a_3(int valor);

// Función para crear un nuevo territorio
Territorio *crear_territorio(const char *codigo, const char *nombre, const char conexiones[][3], int cantidad_conexiones, int A, int B, int C);

// Función para agregar un territorio al final de la lista
void agregar_territorio(Territorio **cabeza, Territorio *nodo);

// Funcion para inicializar random los valores de las problematicas
void valoresProblematicas(Territorio *cabeza);

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
void aumentar_estadistica(Territorio *cabeza, char *codigo, char estadistica);

//Funcion que permite aumentar la estadistica ABC de los vecinos de un territorios
void aumentar_estadistica_vecinos(Territorio *cabeza, char *codigo, char estadistica);


//Función para eliminar un territorio de la lista por su código
void eliminarTerritorio(Territorio *cabeza, const char *codigo);

void aumentar_estadistica_vecinos(Territorio *cabeza, char *codigo, char estadistica)
//Funcion usada en el muno para aumentar de forma random, una estadistica random
//despues de cada turno del jugador y la onu. Llama a la funcion aumentar_estadistica para el aumento como tal
void seleccionar_territorio_estadistica_random(Territorio *cabeza);

#endif