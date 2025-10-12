#ifndef IMPLEMENTACION_HASHMAP_H
#define IMPLEMENTACION_HASHMAP_H

#include "generacion_terreno.h"

//TODO: poner la docu

typedef struct nodo nodo;
typedef struct hashmap hashmap;

hashmap* crear_hashmap(void);
int agregar_h(hashmap* hashmap, char* key, char* nombre_completo, char* descripcion, struct Territorio* paises);
nodo* obtener_h(hashmap* hashmap, char* key);
int encontrar_n(hashmap* hashmap, char* key);
int eliminar_nodo_h(hashmap* hashmap, char* key);
int eliminar_hashmap(hashmap* hashmap);

unsigned int djb2(char* string);

#endif