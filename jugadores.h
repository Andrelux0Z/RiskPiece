#ifndef JUGADORES_H
#define JUGADORES_H

#include "generacion_terreno.h"

// Estructura que representa un jugador en el juego
typedef struct jugador {
    char nombre[32];
    Territorio* ubicacion;
    int turnos_restantes;
    struct jugador* sigt;
    struct jugador* ant;
} jugador;

typedef struct pirata {
    char nombre[32];
    Territorio* ubicacion;
    struct pirata* sigt;
    struct pirata* ant;
} pirata;

typedef struct pirataList {
    pirata* inicio;
    pirata* final;
} pirataList;

typedef struct jugadorList {
    jugador* inicio;
    jugador* final;
} jugadorList;

// Function declarations for jugadores.c
Territorio* buscarTerritorioPorCodigo(const char* codigo, Territorio* cabeza);
Territorio* buscarTerritorioPorNumero(int codigo, Territorio* cabeza);
void agregarJugador(jugadorList* lista, Territorio* ubicacion, char* nombre);
void mostrarJugadores(jugadorList* lista);
void liberarJugadores(jugadorList* lista);
void piratasInicial(Territorio* cabeza);
void agregarPirata(pirataList* pirataList, Territorio* ubicacion);

#endif