/*
 * Juego Societas
 * Implementación de Turnos
 * Estructuras de Datos
 * II Semestre 2025
 */

#include "turnos.h"
#include "implementacion_hashmap.h"
#include "generacion_terreno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Funcion para subir las estadisticas de 3 paises random
 */
/* Ya hay una funcion que cumple este proposito
void subir_aleatorio_en_tres_paises(Territorio *cabeza)
{
    int n = 0;

    for (Territorio *it = cabeza; it; it = it->siguiente) {
        n++;
    }

    if (n == 0) {
        return;
    }

    int maxIdx = (n > 32) ? 32 : n;
    int idx[32];

    for (int i = 0; i < maxIdx; i++) {
        idx[i] = i;
    }

    barajar(idx, maxIdx);

    int subir_cuantos = (maxIdx < 3) ? maxIdx : 3;

    for (int k = 0; k < subir_cuantos; k++) {
        int objetivo = idx[k];
        Territorio *t = cabeza;

        for (int j = 0; j < objetivo && t; j++) {
            t = t->siguiente;
        }

        if (!t) {
            continue;
        }

        int r = rand() % 3;
        char est;

        switch (r) {
            case 0:
                est = 'A';
                break;
            case 1:
                est = 'B';
                break;
            default:
                est = 'C';
                break;
        }

        aumentar_estadistica(cabeza, t->codigo, est);
    }
}
*/

/*
 * Funcion para mostrar el estado de los territorios en los que el jugador está posicionado
 */
void mostrar_estado_territorio(Territorio *t)
{
    if (!t)
    {
        return;
    }

    printf("\n== Territorio actual ==\n");
    printf("  %s (%s)\n", t->nombre, t->codigo);
    printf("    A:%d\n", t->A);
    printf("    B:%d\n", t->B);
    printf("    C:%d\n\n", t->C);
}

/*
 * Funcion que sirve para mostrar los paises vecinos.
 */
void listar_vecinos(Territorio *t, Territorio *cabeza)
{
    if (!t)
    {
        return;
    }

    printf("\nVecinos disponibles:\n");

    for (int i = 0; i < t->cantidad_conexiones; i++)
    {
        Territorio *vecino = buscarTerritorioPorCodigo(t->conexiones[i], cabeza); // Asegura que el código es válido
        printf("  %d) %s (%s)\n", i + 1, vecino->nombre, vecino->codigo);
    }
}

/*
 * Esta funcion funciona para hacer el turno del usuario, compila varias funciones
 * para hacer que todo funcione
 */
void ejecutar_turno(jugadorList *jugadores, Territorio *cabeza)
{
    if (!jugadores || !jugadores->inicio)
    {
        return;
    }

    jugador *j1 = jugadores->inicio;

    // Safety check: verify player's location still exists
    if (!j1->ubicacion)
    {
        printf("ERROR: Jugador %s sin ubicación válida. Buscar territorio alternativo...\n", j1->nombre);
        j1->ubicacion = encontrar_territorio_alternativo(cabeza, "");
        if (!j1->ubicacion)
        {
            printf("ERROR CRÍTICO: No hay territorios disponibles.\n");
            return;
        }
    }
    int acciones = 4;

    // Crear el mapa de problematicas (se usa para mostrar nombres/descripciones)
    hashmap *mapa_problemas = crear_problematicas_facil(cabeza);

    printf("\n================ TURNO DE %s ================\n", j1->nombre);
    mostrar_estado_territorio(j1->ubicacion);

    while (acciones > 0)
    {
        printf("Acciones restantes: %d\n", acciones);
        printf("Elige una accion:\n  1) Moverse a un pais vecino\n  2) Hacer proyecto (bajar A/B/C en 1)\n  3) Ver estado actual\n  4) Ver vecinos\n  5) Ver Tabla General\nSeleccion: ");

        int op = 0;

        if (scanf("%d", &op) != 1)
        {
            int c;

            while ((c = getchar()) != '\n' && c != EOF)
            {
            }

            printf("\nEntrada invalida.\n\n");
            continue;
        }

        int c;

        while ((c = getchar()) != '\n' && c != EOF)
        {
        }

        switch (op)
        {
        case 1:
        {
            listar_vecinos(j1->ubicacion, cabeza);
            printf("Ingresa el numero del vecino (1-%d): ", j1->ubicacion->cantidad_conexiones);

            int choice = 0;

            if (scanf("%d", &choice) != 1)
            {
                while ((c = getchar()) != '\n' && c != EOF)
                {
                }

                printf("\nEntrada invalida.\n\n");
                break;
            }

            while ((c = getchar()) != '\n' && c != EOF)
            {
            }

            if (choice < 1 || choice > j1->ubicacion->cantidad_conexiones)
            {
                printf("Opcion fuera de rango.\n\n");
                break;
            }

            char *cod = j1->ubicacion->conexiones[choice - 1];
            Territorio *destino = buscarTerritorioPorCodigo(cod, cabeza);

            if (!destino)
            {
                printf("\n¡ADVERTENCIA! El territorio %s ya no existe. Conexión actualizada.\n", cod);
                // Remove invalid connection
                for (int k = choice - 1; k < j1->ubicacion->cantidad_conexiones - 1; k++)
                {
                    strcpy(j1->ubicacion->conexiones[k], j1->ubicacion->conexiones[k + 1]);
                }
                j1->ubicacion->cantidad_conexiones--;
                j1->ubicacion->conexiones[j1->ubicacion->cantidad_conexiones][0] = '\0';
                printf("Selecciona otra opción.\n\n");
                break;
            }

            j1->ubicacion = destino;
            printf("Te moviste a %s (%s).\n\n", destino->nombre, destino->codigo);
            acciones--;
            break;
        }
        case 2:
        {
            // Submenu interactivo: ver descripciones (1/2/3) o aplicar proyecto (A/B/C)
            while (1)
            {
                printf("\n=== HACER PROYECTO ===\n");
                printf("Problemas en %s:\n", j1->ubicacion->nombre);

                for (int i = 0; i < 3; i++)
                {
                    char letra;
                    if (i == 0)
                    {
                        letra = 'A';
                    }
                    else if (i == 1)
                    {
                        letra = 'B';
                    }
                    else
                    {
                        letra = 'C';
                    }

                    const char *key = j1->ubicacion->problematicas[i];

                    if (key && key[0] != '\0' && mapa_problemas)
                    {
                        nodo *prob = hashmap_obtener(mapa_problemas, (char *)key);
                        if (prob)
                        {
                            printf("  %d) %c: %s\n", i + 1, letra, prob->nombre_completo);
                        }
                        else
                        {
                            printf("  %d) %c: Descripcion no disponible\n", i + 1, letra);
                        }
                    }
                    else
                    {
                        printf("  %d) %c: Sin problema asignado\n", i + 1, letra);
                    }
                }

                printf("\nOpciones: 1/2/3 ver descripcion | A/B/C aplicar proyecto | otra tecla para cancelar\n");
                printf("Seleccion: ");

                int ch = getchar();
                while ((c = getchar()) != '\n' && c != EOF)
                {
                }
                if (ch == EOF)
                {
                    printf("Entrada invalida.\n\n");
                    break;
                }

                if (ch == '1' || ch == '2' || ch == '3')
                {
                    int idx = ch - '1';
                    const char *key = j1->ubicacion->problematicas[idx];
                    if (key && key[0] != '\0' && mapa_problemas)
                    {
                        nodo *prob = hashmap_obtener(mapa_problemas, (char *)key);
                        if (prob)
                        {
                            char letra_desc;
                            if (idx == 0)
                            {
                                letra_desc = 'A';
                            }
                            else if (idx == 1)
                            {
                                letra_desc = 'B';
                            }
                            else
                            {
                                letra_desc = 'C';
                            }

                            printf("\n-- %c) %s --\n%s\n\n", letra_desc, prob->nombre_completo, prob->descripcion);
                        }
                        else
                        {
                            printf("\nNo hay descripcion para la clave %s.\n\n", key);
                        }
                    }
                    else
                    {
                        printf("\nNo hay problema asignado para esa opcion.\n\n");
                    }
                    // volver a mostrar el submenu sin consumir accion
                    continue;
                }

                char est = (char)ch;
                int aplicado = 0;
                if (est == 'A' || est == 'a')
                {
                    if (j1->ubicacion->A > 0)
                    {
                        j1->ubicacion->A--;
                        aplicado = 1;
                    }
                    else
                    {
                        printf("El problema A ya esta en 0.\n");
                    }
                }
                else if (est == 'B' || est == 'b')
                {
                    if (j1->ubicacion->B > 0)
                    {
                        j1->ubicacion->B--;
                        aplicado = 1;
                    }
                    else
                    {
                        printf("El problema B ya esta en 0.\n");
                    }
                }
                else if (est == 'C' || est == 'c')
                {
                    if (j1->ubicacion->C > 0)
                    {
                        j1->ubicacion->C--;
                        aplicado = 1;
                    }
                    else
                    {
                        printf("El problema C ya esta en 0.\n");
                    }
                }
                else
                {
                    printf("Accion cancelada.\n\n");
                    break; // salir del submenu sin consumir accion
                }

                if (aplicado)
                {
                    printf("Proyecto aplicado en %s.\n\n", j1->ubicacion->nombre);
                    acciones--;
                }
                break; // salir del submenu tras aplicar (consumio accion)
            }
            break;
        }
        case 3:
        {
            mostrar_estado_territorio(j1->ubicacion);
            break;
        }
        case 4:
        {
            listar_vecinos(j1->ubicacion, cabeza);
            printf("\n");
            break;
        }
        case 5:
        {
            imprimir_tabla(cabeza);
            printf("\n");
            break;
        }
        default:
        {
            printf("Opcion invalida.\n\n");
            break;
        }
        }
    }

    printf("Turno finalizado. Posicion actual: %s (%s) | A:%d B:%d C:%d\n",
           j1->ubicacion->nombre,
           j1->ubicacion->codigo,
           j1->ubicacion->A,
           j1->ubicacion->B,
           j1->ubicacion->C);

    seleccionar_territorio_estadistica_random(cabeza);

    printf("\nCambios globales tras el turno del jugador:\n\n");
    imprimir_tabla(cabeza);
    printf("\n");

    // Liberar el mapa local de problematicas
    if (mapa_problemas)
    {
        hashmap_eliminar(mapa_problemas);
    }
}

/*
 * Esta funcion funciona para hacer el turno de la computadora (la ONU)
 * para hacer que todo funcione
 */
void ejecutar_turno_onu(jugadorList *jugadores, Territorio *cabeza)
{
    if (!jugadores || !jugadores->inicio)
    {
        return;
    }

    jugador *j1 = jugadores->inicio; // humano
    jugador *onu = j1->sigt;         // se agregó justo después

    if (!onu)
    {
        return;
    }

    // Safety check: verify ONU's location still exists
    if (!onu->ubicacion)
    {
        printf("ERROR: ONU sin ubicación válida. Buscar territorio alternativo...\n");
        onu->ubicacion = encontrar_territorio_alternativo(cabeza, "");
        if (!onu->ubicacion)
        {
            printf("ERROR CRÍTICO: No hay territorios disponibles para ONU.\n");
            return;
        }
    }

    printf("================ TURNO DE %s ================\n\n", onu->nombre);

    // 4 acciones aleatorias
    int ultimo_vecino_idx = -1; // para no repetir el mismo vecino consecutivamente

    for (int a = 0; a < 4; a++)
    {
        int puede_mover = onu->ubicacion->cantidad_conexiones > 0;
        int puede_bajar = (onu->ubicacion->A > 0) || (onu->ubicacion->B > 0) || (onu->ubicacion->C > 0);
        int op;

        if (puede_mover && puede_bajar)
        {
            op = rand() % 2; // 0 mover, 1 bajar
        }
        else if (puede_mover)
        {
            op = 0;
        }
        else if (puede_bajar)
        {
            op = 1;
        }
        else
        {
            op = 1; // nada que hacer, pero intentara bajar
        }

        switch (op)
        {
        case 0:
        {
            if (puede_mover)
            {
                int deg = onu->ubicacion->cantidad_conexiones;
                int choice = rand() % deg;

                if (deg > 1)
                {
                    // Evitar repetir el mismo vecino si hay opciones
                    for (int tries = 0; tries < 3 && choice == ultimo_vecino_idx; tries++)
                    {
                        choice = rand() % deg;
                    }
                }

                ultimo_vecino_idx = choice;

                char *cod = onu->ubicacion->conexiones[choice];
                Territorio *destino = buscarTerritorioPorCodigo(cod, cabeza);

                if (destino)
                {
                    printf("ONU se mueve de %s (%s) a %s (%s).\n", onu->ubicacion->nombre, onu->ubicacion->codigo, destino->nombre, destino->codigo);
                    onu->ubicacion = destino;
                }
                else
                {
                    printf("ONU intenta moverse a %s pero el territorio ya no existe.\n", cod);
                    // Clean up invalid connection
                    for (int k = choice; k < onu->ubicacion->cantidad_conexiones - 1; k++)
                    {
                        strcpy(onu->ubicacion->conexiones[k], onu->ubicacion->conexiones[k + 1]);
                    }
                    onu->ubicacion->cantidad_conexiones--;
                    if (onu->ubicacion->cantidad_conexiones > 0)
                    {
                        onu->ubicacion->conexiones[onu->ubicacion->cantidad_conexiones][0] = '\0';
                    }
                }
            }
            break;
        }
        default:
        {
            // Escoger una estadistica que sea > 0 si es posible
            char candidatos[3];
            int cnt = 0;

            if (onu->ubicacion->A > 0)
            {
                candidatos[cnt++] = 'A';
            }

            if (onu->ubicacion->B > 0)
            {
                candidatos[cnt++] = 'B';
            }

            if (onu->ubicacion->C > 0)
            {
                candidatos[cnt++] = 'C';
            }

            char est;

            if (cnt > 0)
            {
                est = candidatos[rand() % cnt];
            }
            else
            {
                int r = rand() % 3;

                switch (r)
                {
                case 0:
                    est = 'A';
                    break;
                case 1:
                    est = 'B';
                    break;
                default:
                    est = 'C';
                    break;
                }
            }

            switch (est)
            {
            case 'A':
                if (onu->ubicacion->A > 0)
                {
                    onu->ubicacion->A--;
                }
                break;
            case 'B':
                if (onu->ubicacion->B > 0)
                {
                    onu->ubicacion->B--;
                }
                break;
            case 'C':
                if (onu->ubicacion->C > 0)
                {
                    onu->ubicacion->C--;
                }
                break;
            default:
                break;
            }

            printf("ONU aplica proyecto en %s: -1 en %c.\n", onu->ubicacion->nombre, est);
            break;
        }
        }
    }

    // mostramos los movimientos de la ONU
    printf("Estado ONU tras turno: %s (%s) | A:%d B:%d C:%d\n",
           onu->ubicacion->nombre,
           onu->ubicacion->codigo,
           onu->ubicacion->A,
           onu->ubicacion->B,
           onu->ubicacion->C);

    seleccionar_territorio_estadistica_random(cabeza);

    printf("\nCambios globales tras el turno de la ONU:\n\n");
    imprimir_tabla(cabeza);
    printf("\n");
}
