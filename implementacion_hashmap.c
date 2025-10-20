/*
 * Juego Societas
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

// Las definiciones de nodo y hashmap están en el header; no duplicarlas aquí.

/*
 * Funcion para crear el hashmap, elegimos el length y capacidad. Luego,
 * asignamos la memoria de cada nodo con calloc
 */
hashmap *hashmap_crear(void)
{
    // asignar el tamano correcto de la estructura
    hashmap *mapa = (hashmap *)calloc(1, sizeof(hashmap));
    if (!mapa)
        return NULL;
    mapa->length = 0;
    // Capacidad inicial: debe ser mayor al numero de problematicas (actualmente 16)
    // Para evitar colisiones excesivas y fallos por mapa lleno, usamos 32
    mapa->capacidad = 32;

    mapa->nodos = (nodo *)calloc(mapa->capacidad, sizeof(nodo));
    if (!mapa->nodos)
    {
        free(mapa);
        return NULL;
    }

    return mapa;
}

/*
 * Funcion para recibir un integer cuando se da un string (funcion hash)
 * En esto usé un hash de Daniel J. Bernstein llamado DJB2
 */
unsigned int hashmap_djb2(char *string)
{
    unsigned int hash = 5381; // unsigned lo que hace es no permitir negativos (igual que en Rust)
    int c;
    while ((c = *string++))
    {
        hash = ((hash << 5) + hash) + c; // esta logica está sumamente interesante
    }
    return hash; // luego hacerle el modulo 11
}

/*
 * Funcion para encontrar un nodo con un key.
 * Estamos haciendo linear probing.
 */
int hashmap_buscar_indice(hashmap *hashmap, char *key)
{
    unsigned int hash = hashmap_djb2(key);

    int index = hash % hashmap->capacidad;
    int inicio = index; // no es 0, es donde iniciamos la busqueda

    // Dato importante: si no lo encontramos, seguimos buscando para adelante porque estamos haciendo lineal probing. eso quiere decir que buscamos el próximo que esté vacío
    while (hashmap->nodos[index].key != NULL)
    {
        if (strcmp(hashmap->nodos[index].key, key) == 0)
        { // si da 0 significa que son iguales
            return index;
        }

        index++;
        if (index >= hashmap->capacidad)
        {
            index = 0; // regresamos al principio
        }
        if (index == inicio)
        {
            break;
        }
    }

    return -1;
}

/*
 * Funcion para añadir un entry a el hashmap
 * Estamos usando linear probing, por lo que si ya esta usado el index, seguimos
 */
int hashmap_insertar(hashmap *hashmap, char *key, char *nombre_completo, char *descripcion, Territorio *paises)
{
    unsigned int hash = hashmap_djb2(key);
    int index = hash % hashmap->capacidad;
    int inicio = index;

    // Si el espacio no esta vacio, hacer linear probing
    while (hashmap->nodos[index].key != NULL)
    {
        if (strcmp(hashmap->nodos[index].key, key) == 0)
        {
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
        if (index >= hashmap->capacidad)
        {
            index = 0;
        }
        if (index == inicio)
        {
            return -1; // esta lleno el hashmap
        }
    }

    // Agregar nuevo nodo
    hashmap->nodos[index].key = malloc(strlen(key) + 1);
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
nodo *hashmap_obtener(hashmap *hashmap, char *key)
{
    int index = hashmap_buscar_indice(hashmap, key);
    if (index == -1)
    {
        return NULL;
    }
    return &hashmap->nodos[index];
}

/*
 * Funcion para eliminar el hashmap y liberar memoria
 */
int hashmap_eliminar(hashmap *hashmap)
{
    if (hashmap == NULL)
    {
        return -1;
    }

    // Liberar toda la memoria asignada a cada nodo
    for (int i = 0; i < hashmap->capacidad; i++)
    {
        if (hashmap->nodos[i].key != NULL)
        {
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
int hashmap_eliminar_key(hashmap *hashmap, char *key)
{
    int index = hashmap_buscar_indice(hashmap, key);
    if (index == -1)
    {
        return -1; // no está el nodo
    }

    free(hashmap->nodos[index].key);
    free(hashmap->nodos[index].nombre_completo);
    free(hashmap->nodos[index].descripcion);

    hashmap->nodos[index].key = NULL;
    hashmap->nodos[index].nombre_completo = NULL;
    hashmap->nodos[index].descripcion = NULL;
    hashmap->nodos[index].paises = NULL;

    hashmap->length--;

    // hago esto para reinsertar las cosas después de eliminar, para reorganizar
    int siguiente = (index + 1) % hashmap->capacidad;
    while (hashmap->nodos[siguiente].key != NULL)
    {
        char *temp_key = hashmap->nodos[siguiente].key;
        char *temp_nombre = hashmap->nodos[siguiente].nombre_completo;
        char *temp_desc = hashmap->nodos[siguiente].descripcion;
        Territorio *temp_paises = hashmap->nodos[siguiente].paises;

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
Territorio *paises(char *primero, ...)
{
    if (!primero)
        return NULL;

    Territorio *cabeza = NULL;
    Territorio *ultimo = NULL;

    va_list ap;
    va_start(ap, primero);

    char *actual = primero;
    while (actual != NULL)
    {
        Territorio *t = calloc(1, sizeof(Territorio));
        if (!t)
        {
            va_end(ap);
            return cabeza; // devolvemos lo construido
        }

        strncpy(t->nombre, actual, sizeof(t->nombre) - 1);
        t->nombre[sizeof(t->nombre) - 1] = '\0';

        t->anterior = ultimo;
        t->siguiente = NULL;
        if (ultimo)
        {
            ultimo->siguiente = t;
        }
        if (!cabeza)
        {
            cabeza = t;
        }
        ultimo = t;

        actual = va_arg(ap, char *);
    }

    va_end(ap);
    return cabeza;
}

/*
 * Crea y precarga el hashmap de problematicas para el modo FACIL
 */
hashmap *crear_problematicas_facil(Territorio *cabeza)
{
    hashmap *problematicas = hashmap_crear();
    if (!problematicas)
        return NULL;

    hashmap_insertar(problematicas, "P1", "Sequia masiva",
                     "El pais de Alabasta ya es un lugar desierto, pero en los ultimos anos "
                     "el agua escacea tanto que hay gente muriendo de sed.",
                     cabeza);

    hashmap_insertar(problematicas, "P2", "Olvido social",
                     "Personas de todo el pais estan siendo borradas de la historia y "
                     "todos sus registros son eliminados por el gobierno.",
                     cabeza);

    hashmap_insertar(problematicas, "P3", "Esclavitud",
                     "Sistemas de explotacion laboral que privan de libertad y derechos basicos, "
                     "trabajos pesados forzados en condiciones inhumanas.",
                     cabeza);

    hashmap_insertar(problematicas, "P4", "Agua contaminada",
                     "El agua de todo el pais esta contaminada por quimicos toxicos "
                     "provenientes de gigantescas fabricas industriales.",
                     cabeza);

    hashmap_insertar(problematicas, "P5", "Pirateria maritima",
                     "Surgida inicialmente como una respuesta para proteger las aguas "
                     "territoriales de la pesca ilegal, se convirtio en un lucrativo negocio criminal.",
                     cabeza);

    hashmap_insertar(problematicas, "P6", "Conflictos entre clanes",
                     "Grupos organizados se pelean continuamente por controlar territorios, "
                     "siembran la violencia y dificultan cualquier intento de paz y desarrollo.",
                     cabeza);

    hashmap_insertar(problematicas, "P7", "Vigilancia masiva",
                     "Contener bandas y crimen; invertir en educacion/empleo "
                     "o usar fuerza con costo social.",
                     cabeza);

    hashmap_insertar(problematicas, "P8", "Perdida de la Individualidad",
                     "El objetivo del gobierno es crear una poblacion sumisa que ame al Gran Hermano y no cuestione nada. "
                     "La policia del Pensamiento se encarga de castigar cualquier desviacion.",
                     cabeza);

    hashmap_insertar(problematicas, "P9", "Manipulacion linguistica",
                     "La Neolengua busca reducir el lenguaje para eliminar pensamientos disidentes. "
                     "Se elimina vocabulario que no apoye la ideologia del gobierno.",
                     cabeza);

    hashmap_insertar(problematicas, "10", "Manipulacion mediante religion",
                     "El gobierno utiliza la religion como una herramienta de control social. "
                     "Se promueve una interpretacion de la fe que favorece al regimen.",
                     cabeza);

    hashmap_insertar(problematicas, "11", "Pobreza",
                     "Gran parte de la poblacion vive en condiciones precarias, sin acceso a "
                     "recursos basicos como alimentacion, vivienda o educacion. "
                     "La desigualdad economica aumenta cada ano.",
                     cabeza);

    hashmap_insertar(problematicas, "12", "Mafias en el gobierno",
                     "La corrupcion ha penetrado las instituciones del Estado. "
                     "Funcionarios y lideres politicos colaboran con organizaciones criminales "
                     "para enriquecerse y mantener el poder.",
                     cabeza);

    hashmap_insertar(problematicas, "13", "Castigos brutales",
                     "El sistema judicial aplica penas desproporcionadas y torturas "
                     "como medio de control. Los disidentes y criminales son castigados "
                     "publicamente para infundir miedo en la poblacion.",
                     cabeza);
    hashmap_insertar(problematicas, "14", "Abuso de drogas",
                     "El consumo de sustancias ilegales se ha incrementado drasticamente, "
                     "afectando especialmente a los jovenes. La falta de programas de prevencion "
                     "y rehabilitacion agrava el problema.",
                     cabeza);

    hashmap_insertar(problematicas, "15", "Trafico de personas",
                     "Redes criminales capturan y venden personas para explotacion laboral o sexual. "
                     "Las victimas son en su mayoria mujeres y ninos provenientes de zonas pobres.",
                     cabeza);
    hashmap_insertar(problematicas, "16", "Experimentacion con humanos",
                     "Instituciones cientificas y militares realizan pruebas secretas en personas "
                     "sin su consentimiento. Los experimentos causan graves danos fisicos y mentales "
                     "a las victimas.",
                     cabeza);

    return problematicas;
}
