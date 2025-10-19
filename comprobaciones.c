#include <stdio.h>
#include "generacion_terreno.h"

void ganar()
{
    printf("    ===================================\n");
    printf("                   ganaste             \n");
    printf("    ===================================\n");
    return;
}

// Función que verifica si todos los territorios tienen al menos una estadística igual a cero
void verificar_condicion_victoria(Territorio *cabeza)
{
    int contador = 0;
    int total_territorios = 0;
    Territorio *actual = cabeza;

    // Recorrer toda la lista doblemente enlazada (NO es circular)
    while (actual != NULL)
    {
        total_territorios++;

        // Verificar si al menos una estadística (A, B o C) es igual a cero
        if (actual->A == 0 || actual->B == 0 || actual->C == 0)
        {
            contador++;
        }

        actual = actual->siguiente; // Avanzar al siguiente territorio
    }

    // Si todos los territorios tienen al menos una estadística en cero
    if (contador == total_territorios)
    {
        ganar();
    }
}