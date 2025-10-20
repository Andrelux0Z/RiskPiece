#ifndef TURNOS_H
#define TURNOS_H

#include "generacion_terreno.h"
#include "jugadores.h"

/*
* Funcion para mostrar el estado de los territorios en los que el jugador está posicionado
*/
void mostrar_estado_territorio(Territorio *t);

/*
* Funcion que sirve para mostrar los paises vecinos.
*/
void listar_vecinos(Territorio *t);

/*
* Esta funcion funciona para hacer el turno del usuario, compila varias funciones
* para hacer que todo funcione (dice primera porque así se implemento inicialmente
* luego nos dimos cuenta del error pero no cambiamos las cosas jaja)
*/
void ejecutar_primer_turno(jugadorList *jugadores, Territorio *cabeza);

/*
* Esta funcion funciona para hacer el turno de la computadora (la ONU)
* para hacer que todo funcione (dice primera porque así se implemento inicialmente
* luego nos dimos cuenta del error pero no cambiamos las cosas jaja)
*/
void ejecutar_turno_onu(jugadorList *jugadores, Territorio *cabeza);

#endif