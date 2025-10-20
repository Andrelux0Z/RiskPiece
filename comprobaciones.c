#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "generacion_terreno.h"
#include "jugadores.h"
#include "comprobaciones.h"

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
        return 1;
    }
    return 0;
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
            return 0; // hay más de tres territorios
        }
        actual = actual->siguiente;
    }
    return 1; // tres o menos
}


// Verifica si todas las estadisticas de un territorio son 3
void comprobar_eliminar_territorio_seguro(Territorio **cabeza, jugadorList *jugadores)
{
    if (!cabeza || !*cabeza) return;
    
    // Crear lista de territorios a eliminar
    char territorios_a_eliminar[10][4];
    int count = 0;
    
    // identificar territorios a eliminar
    Territorio *actual = *cabeza;
    while (actual != NULL && count < 10)
    {
        if (actual->A == 3 && actual->B == 3 && actual->C == 3)
        {
            strncpy(territorios_a_eliminar[count], actual->codigo, 3);
            territorios_a_eliminar[count][3] = '\0';
            count++;
        }
        actual = actual->siguiente;
    }
    

    // eliminar territorios
    for (int i = 0; i < count; i++)
    {
        printf("\nALERTA! El territorio %s ha colapsado por problemas extremos.\n", territorios_a_eliminar[i]);
        
        // mover a los jugadores a otro territorio
        if (jugadores && jugadores->inicio)
        {
            jugador *j = jugadores->inicio;
            while (j != NULL)
            {
                if (j->ubicacion && j->ubicacion->codigo && 
                    strcmp(j->ubicacion->codigo, territorios_a_eliminar[i]) == 0)
                {
                    // Mover al primer territorio disponible
                    j->ubicacion = *cabeza;
                    printf("%s tuvo que ser evacuado a %s (%s)\n", j->nombre, (*cabeza)->nombre, (*cabeza)->codigo);
                }
                j = j->sigt;
            }
        }
        
        //quitar conexiones de los territorios vecinos
        limpiar_conexiones_territorio(*cabeza, territorios_a_eliminar[i]);
        
        eliminarTerritorio(cabeza, territorios_a_eliminar[i]);
        // ver si lista no esté vacía
        if (!*cabeza)
        {
            break;
        }
    }
}

// Verifica si todas las estadisticas de un territorio son 3 (modo difícil con piratas)
void comprobar_eliminar_territorio_con_piratas(Territorio **cabeza, jugadorList *jugadores, pirataList *piratas)
{
    if (!cabeza || !*cabeza) return;
    
    // Crear lista de territorios a eliminar
    char territorios_a_eliminar[10][4];
    Territorio *territorios_colapsados[10];
    int count = 0;
    
    // identificar territorios a eliminar
    Territorio *actual = *cabeza;
    while (actual != NULL && count < 10)
    {
        if (actual->A == 3 && actual->B == 3 && actual->C == 3)
        {
            strncpy(territorios_a_eliminar[count], actual->codigo, 3);
            territorios_a_eliminar[count][3] = '\0';
            territorios_colapsados[count] = actual;
            count++;
        }
        actual = actual->siguiente;
    }
    

    // eliminar territorios
    for (int i = 0; i < count; i++)
    {
        printf("\nALERTA! El territorio %s ha colapsado por problemas extremos.\n", territorios_a_eliminar[i]);
        
        // mover a los jugadores a otro territorio
        if (jugadores && jugadores->inicio)
        {
            jugador *j = jugadores->inicio;
            while (j != NULL)
            {
                if (j->ubicacion && j->ubicacion->codigo && 
                    strcmp(j->ubicacion->codigo, territorios_a_eliminar[i]) == 0)
                {
                    // Mover al primer territorio disponible
                    j->ubicacion = *cabeza;
                    printf("%s tuvo que ser evacuado a %s (%s)\n", j->nombre, (*cabeza)->nombre, (*cabeza)->codigo);
                }
                j = j->sigt;
            }
        }
        
        // mover a los piratas a otro territorio
        if (piratas && piratas->inicio)
        {
            pirata *p = piratas->inicio;
            while (p != NULL)
            {
                if (p->ubicacion && p->ubicacion->codigo && 
                    strcmp(p->ubicacion->codigo, territorios_a_eliminar[i]) == 0)
                {
                    // Mover al primer territorio disponible diferente al que colapsa
                    Territorio *nuevo_territorio = encontrar_territorio_alternativo(*cabeza, territorios_a_eliminar[i]);
                    if (nuevo_territorio)
                    {
                        p->ubicacion = nuevo_territorio;
                        printf("Un pirata lo logro y escapo a %s (%s)\n", 
                               nuevo_territorio->nombre, 
                               nuevo_territorio->codigo);
                    }
                }
                p = p->sigt;
            }
        }
        
        // Crear un nuevo pirata en un territorio vecino del que colapsó
        if (piratas && territorios_colapsados[i] && territorios_colapsados[i]->cantidad_conexiones > 0)
        {
            int indice_vecino = rand() % territorios_colapsados[i]->cantidad_conexiones;
            const char *codigo_vecino = territorios_colapsados[i]->conexiones[indice_vecino];
            Territorio *territorio_pirata = buscarTerritorioPorCodigo(codigo_vecino, *cabeza);
            
            if (territorio_pirata)
            {
                agregarPirata(piratas, territorio_pirata);
                printf("¡Del caos surge un nuevo pirata en %s (%s)!\n", 
                       territorio_pirata->nombre, 
                       territorio_pirata->codigo);
            }
        }
        
        //quitar conexiones de los territorios vecinos
        limpiar_conexiones_territorio(*cabeza, territorios_a_eliminar[i]);
        
        eliminarTerritorio(cabeza, territorios_a_eliminar[i]);
        // ver si lista no esté vacía
        if (!*cabeza)
        {
            break;
        }
    }
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

// Funcion para ver problemas de memoria
void validar_integridad_lista(Territorio *cabeza, const char *contexto)
{
    if (!cabeza) {
        return; // lista vacia
    }
    
    int count = 0;
    Territorio *actual = cabeza;
    Territorio *anterior = NULL;
    
    while (actual != NULL && count < 20) // limite de seguridad
    {
        // verificar punteros
        if (actual->anterior != anterior) { 
            return; // puntero anterior inconsistente
        }
                
        anterior = actual;
        actual = actual->siguiente;
        count++;
    }
    
    if (count >= 20) {
        return; // posible bucle infinito
    }
}