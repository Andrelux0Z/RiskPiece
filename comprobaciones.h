#ifndef COMPROBACIONES_H
#define COMPROBACIONES_H

#include "generacion_terreno.h"

// Mensajes de fin de juego
void ganar(void);
void perder(void);

// Devuelve 1 si TODOS los territorios tienen al menos una estadística en 0
int comprobar_ganar(Territorio *cabeza);

// Retorna 1 si se pierde
int comprobar_perder(Territorio *cabeza);

#endif