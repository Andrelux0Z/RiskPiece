/* Juego [nombre]
* Implementación del Hashmaps
* Estructuras de Datos
* II Semestre 2025
*/
#include "implementacion_hashmap.h"
#include "generacion_terreno.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
* Este struct es el nodo del hashmap. Tiene nombre, descripcion y paises.
*/
struct nodo {
    char* key;
    char* nombre_completo;
    char* descripcion;
    struct Territorio* paises;
};

/*
* Implementacion del hashmap, va a usar los nodos, capacidad maxima 
* (lo vamos a usar si llegamos a mas del 75% de longitud, cambiarlo) y longitud
*/
struct hashmap {
    nodo* nodos;
    int capacidad;
    int length;
};

/*
* Funcion para crear el hashmap, elegimos el length y capacidad. Luego,
* asignamos la memoria de cada nodo con calloc
*/
hashmap* hashmap_crear(void) {
    // Evitar sombreado de nombre de tipo y asignar el tamano correcto de la estructura
    hashmap* mapa = (hashmap*)calloc(1, sizeof(struct hashmap));
    if (!mapa) return NULL;
    mapa->length = 0;
    mapa->capacidad = 10; // se puede cambiar si crecen las problematicas

    mapa->nodos = (nodo*)calloc(mapa->capacidad, sizeof(nodo));
    if (!mapa->nodos) { free(mapa); return NULL; }

    return mapa;
}

/*
* Funcion para recibir un integer cuando se da un string (funcion hash)
* En esto usé un hash de Daniel J. Bernstein llamado DJB2, más info en docu
*/
unsigned int hashmap_djb2(char* string) {
    unsigned int hash = 5381; //unsigned lo que hace es no permitir negativos (igual que en Rust)
    int c;
    while((c = *string++)) {
        hash = ((hash << 5) + hash) + c; //esta logica está sumamente interesante, ver más info en docu
    }
    return hash; //luego hacerle el modulo 11
}

/*
* Funcion para encontrar un nodo con un key.
* Estamos haciendo linear probing.
*/
int hashmap_buscar_indice(hashmap* hashmap, char* key) {
    unsigned int hash = hashmap_djb2(key);

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
* Funcion para añadir un entry a el hashmap
* Estamos usando linear probing, por lo que si ya esta usado el index, seguimos
*/
int hashmap_insertar(hashmap* hashmap, char* key, char* nombre_completo, char* descripcion, Territorio* paises) {
    unsigned int hash = hashmap_djb2(key);
    int index = hash % hashmap -> capacidad;
    int inicio = index;

    // Si el espacio no esta vacio, hacer linear probing
    while (hashmap->nodos[index].key != NULL) {
        if (strcmp(hashmap->nodos[index].key, key) == 0) {
            free(hashmap->nodos[index].nombre_completo);
            free(hashmap->nodos[index].descripcion);
            
            hashmap->nodos[index].nombre_completo = malloc(strlen(nombre_completo) + 1);
            strcpy(hashmap->nodos[index].nombre_completo, nombre_completo);
            
            hashmap->nodos[index].descripcion = malloc(strlen(descripcion) + 1);
            strcpy(hashmap->nodos[index].descripcion, descripcion);
            
            hashmap->nodos[index].paises = paises;
            return index;
        }

        index++;
        if (index >= hashmap->capacidad) {
            index = 0;
        }
        if (index == inicio) {
            return -1; //esta lleno el hashmap
        }
    }

    // Agregar nuevo nodo
    hashmap -> nodos[index].key = malloc(strlen(key) + 1);
    strcpy(hashmap->nodos[index].key, key);
    
    hashmap->nodos[index].nombre_completo = malloc(strlen(nombre_completo) + 1);
    strcpy(hashmap->nodos[index].nombre_completo, nombre_completo);
    
    hashmap->nodos[index].descripcion = malloc(strlen(descripcion) + 1);
    strcpy(hashmap->nodos[index].descripcion, descripcion);
    
    hashmap->nodos[index].paises = paises;
    
    hashmap->length++;
    return index;
}

/*
* Funcion para obtener un nodo del hashmap por su key
*/
nodo* hashmap_obtener(hashmap* hashmap, char* key) {
    int index = hashmap_buscar_indice(hashmap, key);
    if (index == -1) {
        return NULL;
    }
    return &hashmap->nodos[index];
}

/*
* Funcion para eliminar el hashmap y liberar memoria
*/
int hashmap_eliminar(hashmap* hashmap) {
    if (hashmap == NULL) {
        return -1;
    }
    
    // Liberar toda la memoria asignada a cada nodo
    for (int i = 0; i < hashmap->capacidad; i++) {
        if (hashmap->nodos[i].key != NULL) {
            free(hashmap->nodos[i].key);
            free(hashmap->nodos[i].nombre_completo);
            free(hashmap->nodos[i].descripcion);
        }
    }

    free(hashmap->nodos);
    free(hashmap);

    return 0;
}

/*
* Funcion para eliminar un nodo específico del hashmap
*/
int hashmap_eliminar_key(hashmap* hashmap, char* key) {
    int index = hashmap_buscar_indice(hashmap, key);
    if (index == -1) {
        return -1; //no está el nodo
    }
    
    free(hashmap->nodos[index].key);
    free(hashmap->nodos[index].nombre_completo);
    free(hashmap->nodos[index].descripcion);
    
    hashmap->nodos[index].key = NULL;
    hashmap->nodos[index].nombre_completo = NULL;
    hashmap->nodos[index].descripcion = NULL;
    hashmap->nodos[index].paises = NULL;
    
    hashmap->length--;

    //hago esto para reinsertar las cosas después de eliminar, para reorganizar
    int siguiente = (index + 1) % hashmap->capacidad;
    while (hashmap->nodos[siguiente].key != NULL) {
        char* temp_key = hashmap->nodos[siguiente].key;
        char* temp_nombre = hashmap->nodos[siguiente].nombre_completo;
        char* temp_desc = hashmap->nodos[siguiente].descripcion;
        Territorio* temp_paises = hashmap->nodos[siguiente].paises;
        
        hashmap->nodos[siguiente].key = NULL;
        hashmap->nodos[siguiente].nombre_completo = NULL;
        hashmap->nodos[siguiente].descripcion = NULL;
        hashmap->nodos[siguiente].paises = NULL;
        hashmap->length--;
        
        // Reinsertar
    hashmap_insertar(hashmap, temp_key, temp_nombre, temp_desc, temp_paises);
        
        free(temp_key);
        free(temp_nombre);
        free(temp_desc);
        
        siguiente = (siguiente + 1) % hashmap->capacidad;
    }
    
    return 0;
}

/*
* Crea una lista enlazada de Territorio solo con el nombre partir de
* una lista variable terminada que termina en NULL.
* Esto no cambia la otra lista doblemente enlazada, solo crea otra. y el ... signitica que puede variar la cantidad de parametros.
*/
Territorio* paises(char* primero, ...) {
    if (!primero) return NULL;

    Territorio* cabeza = NULL;
    Territorio* ultimo = NULL;

    va_list ap;
    va_start(ap, primero);

    char* actual = primero;
    while (actual != NULL) {
        Territorio* t = calloc(1, sizeof(Territorio));
        if (!t) {
            va_end(ap);
            return cabeza; //devolvemos lo construido
        }

        strncpy(t->nombre, actual, sizeof(t->nombre) - 1);
        t->nombre[sizeof(t->nombre) - 1] = '\0';

        t->anterior = ultimo;
        t->siguiente = NULL;
        if (ultimo) {
            ultimo->siguiente = t;
        }
        if (!cabeza) {
            cabeza = t;
        }
        ultimo = t;

        actual = va_arg(ap, char*);
    }

    va_end(ap);
    return cabeza;
}