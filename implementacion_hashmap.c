/* Juego [nombre]
* Implementación del Hashmaps
* Estructuras de Datos
* II Semestre 2025
*/
#include "implementacion_hashmap.h"
#include "generacion_terreno.h"

#include <stdio.h>
#include <stdlib.h>

/*
* Este struct es el nodo del hashmap. Tiene nombre, descripcion y paises.
*/
typedef struct {
    char* key;
    char* nombre_completo;
    char* descripcion;
    struct Territorio* paises;
} nodo;

/*
* Implementacion del hashmap, va a usar los nodos, capacidad maxima 
* (lo vamos a usar si llegamos a mas del 75% de longitud, cambiarlo) y longitud
*/
typedef struct {
    nodo* nodos;
    int capacidad;
    int length;
} hashmap;

/*
* Funcion para crear el hashmap, elegimos el length y capacidad. Luego,
* asignamos la memoria de cada nodo con calloc
*/
hashmap* crear_hashmap() {
    hashmap* hashmap = calloc(1, sizeof(hashmap));
    hashmap -> length = 0;
    hashmap -> capacidad = 10; //se puede cambiar al añadir más problemas, pongo 10 porque hay 5 problemas (50% capacidad, evitamos colisiones)

    hashmap -> nodos = calloc(hashmap -> capacidad, sizeof(nodo));

    return hashmap;
}

/*
* Funcion para recibir un integer cuando se da un string (funcion hash)
* En esto usé un hash de Daniel J. Bernstein llamado DJB2, más info en docu
*/
unsigned int djb2(char* string) {
    unsigned int hash = 5381; //unsigned lo que hace es no permitir negativos (igual que en Rust)
    int c;
    while((c = *string++)) {
        hash = ((hash << 5) + hash) + c; //esta logica está sumamente interesante, ver más info en docu
    }
    return hash; //luego hacerle el modulo 11
}

int encontrar_h(hashmap* hashmap, char* key) {
    unsigned int hash = djb2(key);

    int index = hash % hashmap -> capacidad;
    int inicio = index; // no es 0, es donde iniciamos la busqueda

    // Dato importante: si no lo encontramos, seguimos buscando para adelante porque estamos haciendo lineal probing. eso quiere decir que buscamos el próximo que esté vacío
    while (hashmap -> nodos[index].key != NULL) {
        if (strcmp(hashmap -> nodos[index].key, key) == 0) { // si da 0 significa que son iguales
            return index;
        }

        index++;
        if (index >= hashmap -> capacidad) {
            index = 0; //regresamos al principio
        }
        if (index == inicio) {
            break;
        }
    }

    return -1;
}

/*
* Funcion para eliminar el hashmap y liberar memoria
*/
int eliminar_hashmap(hashmap* hashmap) {
    //eliminar keys, luego nodos y finalmente todo el hashmap
    for (int i = 0; i < hashmap -> capacidad; i++) {
        free(hashmap -> nodos[i].key);
    }

    free(hashmap -> nodos);
    free(hashmap);

    return 0;
}


