#ifndef IMPLEMENTACION_HASHMAP_H
#define IMPLEMENTACION_HASHMAP_H

#include "generacion_terreno.h"

/*
 * Este struct es el nodo del hashmap. Tiene nombre, descripcion y paises.
 */
typedef struct
{
    char *key;
    char *nombre_completo;
    char *descripcion;
    struct Territorio *paises;
} nodo;

/*
 * Implementacion del hashmap, va a usar los nodos, capacidad maxima
 * (lo vamos a usar si llegamos a mas del 75% de longitud, cambiarlo) y longitud
 */
typedef struct
{
    nodo *nodos;
    int capacidad;
    int length;
} hashmap;

/*
 * Funcion para crear el hashmap, elegimos el length y capacidad. Luego,
 * asignamos la memoria de cada nodo con calloc
 */
hashmap *hashmap_crear(void);

/*
 * Funcion para añadir un entry a el hashmap
 * Estamos usando linear probing, por lo que si ya esta usado el index, seguimos
 */
int hashmap_insertar(hashmap *mapa, char *key, char *nombre_completo, char *descripcion, struct Territorio *paises);

/*
 * Funcion para obtener un nodo del hashmap por su key
 */
nodo *hashmap_obtener(hashmap *mapa, char *key);

/*
 * Funcion para encontrar un nodo con un key.
 * Estamos haciendo linear probing.
 */
int hashmap_buscar_indice(hashmap *mapa, char *key);

/*
 * Funcion para eliminar un nodo específico del hashmap
 */
int hashmap_eliminar_key(hashmap *mapa, char *key);

/*
 * Funcion para eliminar el hashmap y liberar memoria
 */
int hashmap_eliminar(hashmap *mapa);

/*
 * Funcion para recibir un integer cuando se da un string (funcion hash)
 * En esto usé un hash de Daniel J. Bernstein llamado DJB2
 */
unsigned int hashmap_djb2(char *string);

/*
 * Crea una lista enlazada de Territorio solo con el nombre partir de
 * una lista variable terminada que termina en NULL.
 * Esto no cambia la otra lista doblemente enlazada, solo crea otra. y
 * el ... signitica que puede variar la cantidad de parametros.
 */
Territorio *paises(char *primero, ...);

/*
 * Inicializa y devuelve un hashmap pre-cargado con las problematicas
 * del modo FACIL. Devuelve NULL si falla la asignacion de memoria.
 */
hashmap *crear_problematicas_facil(struct Territorio *cabeza);

#endif