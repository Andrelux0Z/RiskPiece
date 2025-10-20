/* 
* Juego Societas
* Implementación de Turnos
* Estructuras de Datos
* II Semestre 2025
*/

#include "turnos.h"
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
    if (!t) {
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
void listar_vecinos(Territorio *t,Territorio *cabeza)
{
    if (!t) {
        return;
    }

    printf("Vecinos disponibles:\n");

    for (int i = 0; i < t->cantidad_conexiones; i++) {
        Territorio* vecino = buscarTerritorioPorCodigo(t->conexiones[i], cabeza); // Asegura que el código es válido
        printf("  %d) %s\n", i + 1, vecino->nombre);
    }
}

/*
* Esta funcion funciona para hacer el turno del usuario, compila varias funciones
* para hacer que todo funcione
*/
void ejecutar_turno(jugadorList *jugadores, Territorio *cabeza)
{
    if (!jugadores || !jugadores->inicio) {
        return;
    }

    jugador *j1 = jugadores->inicio;
    
    // Safety check: verify player's location still exists
    if (!j1->ubicacion) {
        printf("ERROR: Jugador %s sin ubicación válida. Buscar territorio alternativo...\n", j1->nombre);
        j1->ubicacion = encontrar_territorio_alternativo(cabeza, "");
        if (!j1->ubicacion) {
            printf("ERROR CRÍTICO: No hay territorios disponibles.\n");
            return;
        }
    }
    int acciones = 4;

    printf("\n================ TURNO DE %s ================\n", j1->nombre);
    mostrar_estado_territorio(j1->ubicacion);

    while (acciones > 0) {
        printf("Acciones restantes: %d\n", acciones);
        printf("Elige una accion:\n  1) Moverse a un pais vecino\n  2) Hacer proyecto (bajar A/B/C en 1)\n  3) Ver estado actual\n  4) Ver vecinos\n  5) Ver Tabla General\nSeleccion: ");

        int op = 0;

        if (scanf("%d", &op) != 1) {
            int c;

            while ((c = getchar()) != '\n' && c != EOF) {
            }

            printf("\nEntrada invalida.\n\n");
            continue;
        }

        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }

        switch (op) {
            case 1:
            {
                listar_vecinos(j1->ubicacion,cabeza);
                printf("Ingresa el numero del vecino (1-%d): ", j1->ubicacion->cantidad_conexiones);

                int choice = 0;

                if (scanf("%d", &choice) != 1) {
                    while ((c = getchar()) != '\n' && c != EOF) {
                    }

                    printf("\nEntrada invalida.\n\n");
                    break;
                }

                while ((c = getchar()) != '\n' && c != EOF) {
                }

                if (choice < 1 || choice > j1->ubicacion->cantidad_conexiones) {
                    printf("Opcion fuera de rango.\n\n");
                    break;
                }

                char *cod = j1->ubicacion->conexiones[choice - 1];
                Territorio *destino = buscarTerritorioPorCodigo(cod, cabeza);

                if (!destino) {
                    printf("\n¡ADVERTENCIA! El territorio %s ya no existe. Conexión actualizada.\n", cod);
                    // Remove invalid connection
                    for (int k = choice - 1; k < j1->ubicacion->cantidad_conexiones - 1; k++) {
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
                printf("Elige que problema bajar (A/B/C): \n");
                printf("Elige que descripcion deseas ver (1/2/3 respectivamente): \n");
                printf("O cualquier otra tecla para cancelar.\n");

                int ch = getchar();

                while ((c = getchar()) != '\n' && c != EOF) {
                }

                if (ch == EOF) {
                    printf("Entrada invalida.\n\n");
                    break;
                }

                char est = (char)ch;
                int valido = 0;

                switch (est) {
                    case 'A':
                    case 'a':
                    case 'B':
                    case 'b':
                    case 'C':
                    case 'c':
                        valido = 1;
                        break;
                    case 1:
                    case 2:
                    case 3:
                        //TODO HASHMAP PARA DESCRIPCIONES

                    default:
                        valido = 0;
                        break;
                }

                if (!valido) {
                    printf("Opcion invalida. Usa A, B o C.\n\n");
                    break;
                }

                if (est == 'A' || est == 'a') {
                    if (j1->ubicacion->A > 0) {
                        j1->ubicacion->A--;
                    }
                } else if (est == 'B' || est == 'b') {
                    if (j1->ubicacion->B > 0) {
                        j1->ubicacion->B--;
                    }
                } else if (est == 'C' || est == 'c') {
                    if (j1->ubicacion->C > 0) {
                        j1->ubicacion->C--;
                    }
                }

                printf("Proyecto aplicado en %s.\n  A:%d  B:%d  C:%d\n\n",
                       j1->ubicacion->nombre,
                       j1->ubicacion->A,
                       j1->ubicacion->B,
                       j1->ubicacion->C);

                acciones--;
                break;
            }
            case 3:
            {
                mostrar_estado_territorio(j1->ubicacion);
                break;
            }
            case 4:
            {
                listar_vecinos(j1->ubicacion,cabeza);
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
}


/*
* Esta funcion funciona para hacer el turno de la computadora (la ONU)
* para hacer que todo funcione
*/
void ejecutar_turno_onu(jugadorList *jugadores, Territorio *cabeza)
{
    if (!jugadores || !jugadores->inicio) {
        return;
    }

    jugador *j1 = jugadores->inicio; // humano
    jugador *onu = j1->sigt;         // se agregó justo después

    if (!onu) {
        return;
    }
    
    // Safety check: verify ONU's location still exists
    if (!onu->ubicacion) {
        printf("ERROR: ONU sin ubicación válida. Buscar territorio alternativo...\n");
        onu->ubicacion = encontrar_territorio_alternativo(cabeza, "");
        if (!onu->ubicacion) {
            printf("ERROR CRÍTICO: No hay territorios disponibles para ONU.\n");
            return;
        }
    }

    printf("================ TURNO DE %s ================\n\n", onu->nombre);

    // 4 acciones aleatorias
    int ultimo_vecino_idx = -1; // para no repetir el mismo vecino consecutivamente

    for (int a = 0; a < 4; a++) {
        int puede_mover = onu->ubicacion->cantidad_conexiones > 0;
        int puede_bajar = (onu->ubicacion->A > 0) || (onu->ubicacion->B > 0) || (onu->ubicacion->C > 0);
        int op;

        if (puede_mover && puede_bajar) {
            op = rand() % 2; // 0 mover, 1 bajar
        } else if (puede_mover) {
            op = 0;
        } else if (puede_bajar) {
            op = 1;
        } else {
            op = 1; // nada que hacer, pero intentara bajar
        }

        switch (op) {
            case 0:
            {
                if (puede_mover) {
                    int deg = onu->ubicacion->cantidad_conexiones;
                    int choice = rand() % deg;

                    if (deg > 1) {
                        // Evitar repetir el mismo vecino si hay opciones
                        for (int tries = 0; tries < 3 && choice == ultimo_vecino_idx; tries++) {
                            choice = rand() % deg;
                        }
                    }

                    ultimo_vecino_idx = choice;

                    char *cod = onu->ubicacion->conexiones[choice];
                    Territorio *destino = buscarTerritorioPorCodigo(cod, cabeza);

                    if (destino) {
                        printf("ONU se mueve de %s (%s) a %s (%s).\n", onu->ubicacion->nombre, onu->ubicacion->codigo, destino->nombre, destino->codigo);
                        onu->ubicacion = destino;
                    } else {
                        printf("ONU intenta moverse a %s pero el territorio ya no existe.\n", cod);
                        // Clean up invalid connection
                        for (int k = choice; k < onu->ubicacion->cantidad_conexiones - 1; k++) {
                            strcpy(onu->ubicacion->conexiones[k], onu->ubicacion->conexiones[k + 1]);
                        }
                        onu->ubicacion->cantidad_conexiones--;
                        if (onu->ubicacion->cantidad_conexiones > 0) {
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

                if (onu->ubicacion->A > 0) {
                    candidatos[cnt++] = 'A';
                }

                if (onu->ubicacion->B > 0) {
                    candidatos[cnt++] = 'B';
                }

                if (onu->ubicacion->C > 0) {
                    candidatos[cnt++] = 'C';
                }

                char est;

                if (cnt > 0) {
                    est = candidatos[rand() % cnt];
                } else {
                    int r = rand() % 3;

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
                }

                switch (est) {
                    case 'A':
                        if (onu->ubicacion->A > 0) {
                            onu->ubicacion->A--;
                        }
                        break;
                    case 'B':
                        if (onu->ubicacion->B > 0) {
                            onu->ubicacion->B--;
                        }
                        break;
                    case 'C':
                        if (onu->ubicacion->C > 0) {
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

/*
* Funcion que ejecuta el turno de todos los piratas
* Los piratas causan daño y aumentan +1 a las problemáticas en su territorio
*/
void ejecutar_turno_piratas(pirataList *piratas, Territorio *cabeza)
{
    if (!piratas || !piratas->inicio || !cabeza)
    {
        return;
    }

    printf("\n============ TURNO DE LOS PIRATAS ============\n");
    printf("Los piratas siembran el caos y aumentan las problematicas...\n\n");

    pirata *actual = piratas->inicio;
    int contador_piratas = 1;

    while (actual != NULL)
    {
        if (actual->ubicacion == NULL)
        {
            actual = actual->sigt;
            continue;
        }

        printf("-- Pirata %d en %s (%s) --\n", 
               contador_piratas, 
               actual->ubicacion->nombre, 
               actual->ubicacion->codigo);

        // Decidir acción aleatoria: 50% mover, 50% atacar
        int accion = rand() % 2;

        if (accion == 0 && actual->ubicacion->cantidad_conexiones > 0)
        {
            // Moverse a un territorio vecino aleatorio
            int indice_vecino = rand() % actual->ubicacion->cantidad_conexiones;
            const char *codigo_vecino = actual->ubicacion->conexiones[indice_vecino];
            Territorio *nuevo_territorio = buscarTerritorioPorCodigo(codigo_vecino, cabeza);

            if (nuevo_territorio != NULL)
            {
                actual->ubicacion = nuevo_territorio;
                printf("  -> El pirata se mueve a %s (%s)\n", 
                       nuevo_territorio->nombre, 
                       nuevo_territorio->codigo);
            }
        }

        // Atacar: aumentar una estadística aleatoria en +1
        int estadistica_atacar = rand() % 3;  // 0=A, 1=B, 2=C
        char stat_nombre;

        switch (estadistica_atacar)
        {
            case 0:
                if (actual->ubicacion->A < 3)
                {
                    actual->ubicacion->A++;
                }
                stat_nombre = 'A';
                break;
            case 1:
                if (actual->ubicacion->B < 3)
                {
                    actual->ubicacion->B++;
                }
                stat_nombre = 'B';
                break;
            case 2:
                if (actual->ubicacion->C < 3)
                {
                    actual->ubicacion->C++;
                }
                stat_nombre = 'C';
                break;
        }

        printf("  -> El pirata aumenta la problematica %c en +1!\n", stat_nombre);
        printf("     Estado: A:%d B:%d C:%d\n\n", 
               actual->ubicacion->A, 
               actual->ubicacion->B, 
               actual->ubicacion->C);

        actual = actual->sigt;
        contador_piratas++;
    }

    printf("Fin del turno de los piratas.\n\n");
}

