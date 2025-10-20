/*
TODO: Nombre colectivo anarquista
1. Colocar jugadores de forma aleatoria al inciar el juego
2. Turno de jugadores
    ->Moverse de territorio
    ->Implementar proyecto -> disminuir estadisticas del territorio
3. Aumentar de forma aleatoria las problematicas de los territorios
     ->Aumentar las estadisticas alrededor de los territorios que tienen 3 o mas en algun aspecto
4. Implementar pirata inicial
    ->Implementar paisos perdidos como nuevos piratas --Pensar lore al respecto a esto
*/
#include "generacion_terreno.h"
#include "jugadores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Funcion para buscar un territorio por su codigo
Territorio *buscarTerritorioPorCodigo(const char *codigo, Territorio *cabeza)
{
    Territorio *actual = cabeza;
    while (actual != NULL)
    {
        if (strcmp(actual->codigo, codigo) == 0)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

Territorio *buscarTerritorioPorNumero(int codigoNum, Territorio *cabeza)
{
    Territorio *actual = cabeza;
    char codigoStr[3];
    snprintf(codigoStr, sizeof(codigoStr), "%02d", codigoNum);

    while (actual != NULL)
    {
        if (strcmp(actual->codigo, codigoStr) == 0)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

// Funcion para agregar un jugador a la lista
void agregarJugador(jugadorList *lista, Territorio *ubicacion, char *nombre)
{
    // Crear nuevo jugador
    jugador *nuevoJugador = malloc(sizeof(jugador));
    if (!nuevoJugador)
    {
        printf("Error: memoria insuficiente\n");
        return;
    }

    // Asignar nombre al nuevo jugador
    strncpy(nuevoJugador->nombre, nombre, sizeof(nuevoJugador->nombre) - 1);
    nuevoJugador->nombre[sizeof(nuevoJugador->nombre) - 1] = '\0';

    // Asignar ubicacion
    nuevoJugador->ubicacion = ubicacion;
    nuevoJugador->turnos_restantes = 4;
    nuevoJugador->sigt = NULL;
    nuevoJugador->ant = NULL;

    // Si la lista de jugadores esta vacia
    if (lista->inicio == NULL)
    {
        lista->inicio = nuevoJugador;
        lista->final = nuevoJugador;
    }
    else
    {
        // Si la lista de jugadores no esta vacia
        lista->final->sigt = nuevoJugador;
        nuevoJugador->ant = lista->final;
        lista->final = nuevoJugador;
    }
}

// Funcion para mostrar todos los jugadores y sus ubicaciones
void mostrarJugadores(jugadorList *lista)
{
    if (lista == NULL || lista->inicio == NULL)
    {
        printf("No hay jugadores en la partida.\n");
        return;
    }

    printf("\n>> JUGADORES EN LA PARTIDA:\n");
    printf("--------------------------------------------------------------\n");

    jugador *actual = lista->inicio;
    while (actual != NULL)
    {
        printf("  Jugador: %-20s | Ubicacion: %s (%s)\n",
               actual->nombre,
               actual->ubicacion->nombre,
               actual->ubicacion->codigo);
        actual = actual->sigt;
    }
    printf("--------------------------------------------------------------\n\n");
}

// Funcion para liberar la memoria de la lista de jugadores
void liberarJugadores(jugadorList *lista)
{
    if (lista == NULL)
        return;

    jugador *actual = lista->inicio;
    while (actual != NULL)
    {
        jugador *siguiente = actual->sigt;
        free(actual);
        actual = siguiente;
    }
    free(lista);
}

/* Funcion que permite colocar a los piratas de forma aleatoria al iniciar el modo dificil
Crea piratas iniciales en territorios aleatorios
*/
void piratasInicial(pirataList *lista, Territorio *cabeza)
{
    if (!lista || !cabeza) return;

    // Contar territorios disponibles
    int total_territorios = 0;
    Territorio *temp = cabeza;
    while (temp != NULL)
    {
        total_territorios++;
        temp = temp->siguiente;
    }

    if (total_territorios == 0) return;

    // Crear 2 piratas iniciales en territorios aleatorios
    int num_piratas = (total_territorios >= 2) ? 2 : 1;
    
    for (int i = 0; i < num_piratas; i++)
    {
        int indice_random = rand() % total_territorios;
        Territorio *actual = cabeza;
        
        for (int j = 0; j < indice_random && actual != NULL; j++)
        {
            actual = actual->siguiente;
        }
        
        if (actual != NULL)
        {
            agregarPirata(lista, actual);
        }
    }
}

/* Funcion que permite agregar un pirata a la lista de piratas y asignar su ubicacion
Utilizar cuando muera un territorio y se cree un nuevo pirata en ese territorio (TODO O EL SIGUIENTE)
*/
void agregarPirata(pirataList *pirataList, Territorio *ubicacion)
{
    pirata *nuevoPirata = malloc(sizeof(pirata));
    if (!nuevoPirata)
    {
        printf("Error: memoria insuficiente\n");
        return;
    }

    strcpy(nuevoPirata->nombre, "Pirata");
    nuevoPirata->ubicacion = ubicacion;
    nuevoPirata->sigt = NULL;
    nuevoPirata->ant = NULL;

    // Si la lista de piratas esta vacia
    if (pirataList->inicio == NULL)
    {
        pirataList->inicio = nuevoPirata;
        pirataList->final = nuevoPirata;
    }
    else
    {
        // Si la lista de piratas no esta vacia
        pirataList->final->sigt = nuevoPirata;
        nuevoPirata->ant = pirataList->final;
        pirataList->final = nuevoPirata;
    }
}

// Funcion para mostrar todos los piratas y sus ubicaciones
void mostrarPiratas(pirataList *lista)
{
    if (lista == NULL || lista->inicio == NULL)
    {
        printf("No hay piratas en la partida.\n");
        return;
    }

    printf("\n>> PIRATAS EN LA PARTIDA:\n");
    printf("--------------------------------------------------------------\n");

    pirata *actual = lista->inicio;
    int contador = 1;
    while (actual != NULL)
    {
        printf("  Pirata %d | Ubicacion: %s (%s)\n",
               contador,
               actual->ubicacion->nombre,
               actual->ubicacion->codigo);
        actual = actual->sigt;
        contador++;
    }
    printf("--------------------------------------------------------------\n\n");
}

// Funcion para liberar la memoria de la lista de piratas
void liberarPiratas(pirataList *lista)
{
    if (lista == NULL)
        return;

    pirata *actual = lista->inicio;
    while (actual != NULL)
    {
        pirata *siguiente = actual->sigt;
        free(actual);
        actual = siguiente;
    }
    free(lista);
}
