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

// Función para limitar un valor al rango de 0 a 3
int limitar_a_rango_0_a_3(int valor);

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

// Funcion auxiliar para barajar (Fisher-Yates shuffle) usada en valoresProblematicas
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

// Funcion que permite aumentar la estadistica ABC de los vecinos de un territorio
void aumentar_estadistica_vecinos(Territorio *cabeza, const char *codigo, char estadistica);

// Función para eliminar un territorio de la lista por su código (puede actualizar la cabeza)
void eliminarTerritorio(Territorio **cabeza, const char *codigo);

// Funcion usada para aumentar de forma random una estadistica random
// despues de cada turno del jugador y la ONU.
void seleccionar_territorio_estadistica_random(Territorio *cabeza);

// Helper functions for safe territory deletion
Territorio *encontrar_territorio_alternativo(Territorio *cabeza, const char *codigo_eliminado);
void limpiar_conexiones_territorio(Territorio *cabeza, const char *codigo_eliminado);

#endif
