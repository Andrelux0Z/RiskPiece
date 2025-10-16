#ifndef IMPLEMENTACION_HASHMAP_H
#define IMPLEMENTACION_HASHMAP_H

#include "generacion_terreno.h"

//TODO: poner la docu

typedef struct nodo nodo;
typedef struct hashmap hashmap;

// API clara y consistente
hashmap* hashmap_crear(void);
int hashmap_insertar(hashmap* mapa, char* key, char* nombre_completo, char* descripcion, struct Territorio* paises);
nodo* hashmap_obtener(hashmap* mapa, char* key);
int hashmap_buscar_indice(hashmap* mapa, char* key);
int hashmap_eliminar_key(hashmap* mapa, char* key);
int hashmap_eliminar(hashmap* mapa);

// Función hash (DJB2)
unsigned int hashmap_djb2(char* string);

// Construye una lista enlazada de Territorio solo con el campo nombre a partir de nombres terminados en NULL
Territorio* paises(char* primero, ...);

#endif