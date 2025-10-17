#ifndef JUGADORES_H
#define JUGADORES_H

#include "generacion_terreno.h"

// Estructura que representa un territorio en el juego
typedef struct jugador {
    char nombre[32];
    Territorio* ubicacion;
    int turnos_restantes = 4;
    jugador* sigt;
    jugador* ant;
} jugador;

typedef struct pirata {
    char nombre[32];
    Territorio* ubicacion;
    pirata* sigt;
    pirata* ant;
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
void crearJugadoresInicial(Territorio* cabeza);
void agregarJugador(jugadorList* jugadorList, Territorio* ubicacion, char* nombre);
void piratasInicial(Territorio* cabeza);
void agregarPirata(pirataList* pirataList, Territorio* ubicacion);

#endif