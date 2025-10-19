#include <stdio.h>
#include "generacion_terreno.h"

void ganar(void)
{
    printf("\n==================== VICTORIA ====================\n");
    printf("           Felicidades, has ganado!\n");
    printf("=================================================\n\n");
}

void perder(void)
{
    printf("\n===================== DERROTA ====================\n");
    printf("           Lo sentimos, has perdido.\n");
    printf("=================================================\n\n");
}

// Devuelve 1 si TODOS los territorios tienen al menos una estadística en 0. 0 en caso contrario
int comprobar_ganar(Territorio *cabeza)
{
    int contador = 0;
    int total_territorios = 0;
    Territorio *actual = cabeza;

    while (actual != NULL)
    {
        total_territorios++;

        // Verificar si al menos una estadística (A, B o C) es igual a cero
        if (actual->A == 0 || actual->B == 0 || actual->C == 0)
        {
            contador++;
        }
        actual = actual->siguiente;
    }

    // Si todos los territorios tienen al menos una estadística en cero
    if (contador == total_territorios)
    {
        ganar();
    }
}

// Función que comprueba condición de perder: hay más de tres territorios en la lista
// Retorna 1 si se cumple la condición de perder, 0 en caso contrario
int comprobar_perder(Territorio *cabeza)
{
    int cont = 0;
    Territorio *actual = cabeza;
    while (actual != NULL)
    {
        cont++;
        if (cont > 3)
        {
            return 1; // hay más de tres territorios
        }
        actual = actual->siguiente;
    }
    return 0; // tres o menos
}

// Verifica si todas las estadisticas de un territorio son 3
int comprobar_eliminar_territorio(Territorio *cabeza)
{
    Territorio *actual = cabeza;
    while (actual != NULL)
    {
        if (actual->A != 3 || actual->B != 3 || actual->C != 3)
        {
            return 0; // Si encuentra una estadística que no es 3, retorna 0
        }
        actual = actual->siguiente;
    }
    return 1; // Si todas las estadísticas son 3, retorna 1
}

// Verifica que una estadistica dada sea 3 en todos los territorios
int comprobar_tres_todos(Territorio *cabeza, char estadistica)
{
    Territorio *actual = cabeza;
    while (actual != NULL)
    {
        int valor = 0;
        if (estadistica == 'A')
            valor = actual->A;
        else if (estadistica == 'B')
            valor = actual->B;
        else if (estadistica == 'C')
            valor = actual->C;

        if (valor != 3)
            return 0; // Si encuentra un valor que no es 3, retorna 0

        actual = actual->siguiente;
    }
    return 1; // Si todos los territorios tienen 3 en la estadística dada, retorna 1
}