#ifndef COMPROBACIONES_H
#define COMPROBACIONES_H

#include "generacion_terreno.h"

// Función para mostrar mensaje de victoria
void ganar();

// Función que verifica si todos los territorios tienen al menos una estadística igual a cero
void verificar_condicion_victoria(Territorio *cabeza);

#endif // COMPROBACIONES_H