#ifndef TURNOS_H
#define TURNOS_H

#include "generacion_terreno.h"
#include "jugadores.h"

// Etiquetas para los indicadores A/B/C (mapeo de problematicas)
char* PROB_A_LABEL;
char* PROB_B_LABEL;
char* PROB_C_LABEL;

// Utilidades de estado/vecinos
void mostrar_estado_territorio(Territorio* t);
void listar_vecinos(Territorio* t);

// Acciones
void ejecutar_primer_turno(jugadorList* jugadores, Territorio* cabeza);
void ejecutar_turno_onu(jugadorList* jugadores, Territorio* cabeza);

#endif // TURNOS_H
