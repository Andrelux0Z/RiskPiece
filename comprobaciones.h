#ifndef COMPROBACIONES_H
#define COMPROBACIONES_H

#include "generacion_terreno.h"

// Mensajes de fin de juego
void ganar(void);
void perder(void);

// Devuelve 1 si TODOS los territorios tienen al menos una estadística en 0
int comprobar_ganar(Territorio *cabeza);

// Retorna 1 si se pierde (mas de 3 territorios en la lista)
int comprobar_perder(Territorio *cabeza);

// Revisa si un territorio debe ser eliminado (todas sus estadisticas en 3)
void comprobar_eliminar_territorio(Territorio *cabeza);

// Comprueba si una estadistica dada es 3 en todos los territorios
int comprobar_tres_todos(Territorio *cabeza, char estadistica);

#endif