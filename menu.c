//prueba

#include "menu.h"
#include "implementacion_hashmap.h"
#include "generacion_terreno.h"
#include "jugadores.h"
#include "turnos.h"
#include "comprobaciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Etiquetas A/B/C definidas en turnos.c y expuestas por turnos.h

// Funcion para limpiar el buffer
void limpiar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Funcion para hacer clear de terminal
void limpiar_pantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Funcion para mostrar el banner del juego (solo sale 1 vez)
void mostrar_banner()
{
    printf("  ====================================================================\n");
    printf("                             ,,,,,,\n");
    printf("                           o#'9MMHb':'-,o,\n");
    printf("                        .oH\":HH$' \"' ' -*R&o,\n");
    printf("                       dMMM*\"\"'`'      .oM\"HM?.\n");
    printf("                     ,MMM'          \"HLbd< ?&H\\\n");
    printf("                    .:MH .\"\\          ` MM  MM&b\n");
    printf("                   . \"*H    -        &MMMMMMMMMH:\n");
    printf("                   .    dboo        MMMMMMMMMMMM.\n");
    printf("                   .   dMMMMMMb      *MMMMMMMMMP.\n");
    printf("                   .    MMMMMMMP        *MMMMMP .\n");
    printf("                        `#MMMMM           MM6P ,\n");
    printf("                    '    `MMMP\"           HM*`,\n");
    printf("                     '    :MM             .- ,\n");
    printf("                      '.   `#?..  .       ..'\n");
    printf("                         -.   .         .-\n");
    printf("                           ''-.oo,oo.-''\n");
    printf("  ====================================================================\n");
    printf("                              SOCIETAS\n");
    printf("                  Un juego de estrategia y decisiones\n");
    printf("  ====================================================================\n\n");
}

// Logica de turnos implementada en turnos.c

// Funcion para mostrar informacion del juego
void mostrar_informacion()
{
    limpiar_pantalla();
    mostrar_banner();

    printf("  ====================================================================\n");
    printf("                      INFORMACION DEL JUEGO                        \n");
    printf("  ====================================================================\n\n");

    printf(">> INFO DE SOCIETAS:\n");
    printf("   Societas es un juego de estrategia donde debes gestionar paises\n");
    printf("   y resolver problematicas sociales.\n\n");

    printf(">> OBJETIVO:\n");
    printf("   Mantener la estabilidad de los paises resolviendo problemas\n");
    printf("   y tomando decisiones estrategicas que afectan a las sociedades.\n\n");

    printf(">> PAISES:\n");
    printf("   El juego cuenta con 10 paises conectados entre si:\n");
    printf("   * Dressrosa        * Wano          * Punk Hazard\n");
    printf("   * Alabasta         * Pisos Picados * Skypea\n");
    printf("   * Somalia          * Ba Sing Se    * Pharloom\n");
    printf("   * Oceania\n\n");

    printf(">> PROBLEMAS:\n");
    printf("   Cada territorio enfrenta 3 problemas diferentes, entre los que se encuentran:\n");
    printf("   A: Contaminacion y cambio climatico\n");
    printf("   B: Desigualdad social\n");
    printf("   C: Corrupcion gubernamental\n");

    printf(">> ESTADISTICAS:\n");
    printf("   Cada territorio tiene tres indicadores A/B/C con valores de 0 a 3.\n");
    printf("   Mapeo actual: A = %s | B = %s | C = %s\n\n", "Contaminacion y cambio climatico", "Desigualdad social", "Corrupcion gubernamental");

    printf(">> MODOS DE JUEGO:\n");
    printf("   MODO FACIL: Mas recursos y tiempo para resolver problemas.\n");
    printf("   MODO DIFICIL: Problemas mas graves y un grupo adicional arruinando el mapa.\n\n");

    printf("======================================================================\n");
    printf("  Desarrollado por: Andres, Marco y Fabian para Estructuras de Datos.\n");
    printf("======================================================================\n\n");

    printf("Presiona ENTER para volver al menu principal...");
    limpiar_buffer();
    getchar();
}

// Funcion para iniciar el modo facil
void iniciar_modo_facil()
{
    limpiar_pantalla();
    mostrar_banner();

    printf("======================================================================\n");
    printf("                MODO FACIL (acaso tienes miedo?)                \n");
    printf("======================================================================\n\n");

    // La semilla ya se inicializo en main(); no reseedar aqui

    // Iniciar el juego facil
    Territorio *cabeza = construir_lista_ejemplo();
    valoresProblematicas(cabeza);

    printf("\n>> Estado inicial de los territorios:\n\n");
    imprimir_tabla(cabeza);

    // Crear lista de jugadores directamente
    jugadorList *jugadores = malloc(sizeof(jugadorList));
    if (!jugadores)
    {
        printf("Error: memoria insuficiente\n");
        return;
    }
    jugadores->inicio = NULL;
    jugadores->final = NULL;

    // Pedir nombre del jugador
    char nombreJugador[32];
    printf("\n>> CONFIGURACION DE JUGADORES\n");
    printf("--------------------------------------------------------------\n");
    printf("Ingresa el nombre del grupo anarquista al que perteneces (max 31 caracteres): ");
    fgets(nombreJugador, sizeof(nombreJugador), stdin);

    // Eliminar el salto de linea del fgets
    size_t len = strlen(nombreJugador);
    if (len > 0 && nombreJugador[len - 1] == '\n')
    {
        nombreJugador[len - 1] = '\0';
    }

    // Si el nombre esta vacio, asignar un nombre por defecto
    if (strlen(nombreJugador) == 0)
    {
        strcpy(nombreJugador, "Jugador");
    }

    // Asignar territorio aleatorio al jugador
    int territorioAleatorio = rand() % 10 + 1;
    char codigoTerritorio[3];
    sprintf(codigoTerritorio, "%02d", territorioAleatorio);

    Territorio *ubicacionJugador = buscarTerritorioPorCodigo(codigoTerritorio, cabeza);
    if (ubicacionJugador != NULL)
    {
        agregarJugador(jugadores, ubicacionJugador, nombreJugador);
    }

    // Crear jugador 2 automatico: ONU en un territorio distinto
    int territorioONU;
    Territorio *ubicacionONU = NULL;
    do
    {
        territorioONU = rand() % 10 + 1;
        char codigoONU[3];
        sprintf(codigoONU, "%02d", territorioONU);
        ubicacionONU = buscarTerritorioPorCodigo(codigoONU, cabeza);
    } while (ubicacionONU == NULL || (ubicacionJugador != NULL && strcmp(ubicacionONU->codigo, ubicacionJugador->codigo) == 0));

    // Nombre fijo "ONU"
    agregarJugador(jugadores, ubicacionONU, "ONU");

    // Mostrar informacion de los jugadores
    mostrarJugadores(jugadores);

    // Crear hashmap de problematicas mediante una funcion dedicada
    hashmap *problematicas = crear_problematicas_facil(cabeza);

    hashmap_insertar(problematicas, "P1", "Contaminacion y cambio climatico",
                     "Reducir contaminacion equilibrando desarrollo y sostenibilidad; "
                     "malas decisiones aumentan temperatura y causan desastres.",
                     cabeza);

    hashmap_insertar(problematicas, "P2", "Desigualdad social",
                     "Gestionar recursos, empleo y educacion entre clases; "
                     "malas decisiones aumentan pobreza y protestas o colapsan la economia.",
                     cabeza);

    hashmap_insertar(problematicas, "P3", "Corrupcion gubernamental",
                     "Transparencia vs enriquecimiento ilicito; "
                     "la corrupcion reduce inversion y genera caos social.",
                     cabeza);

    hashmap_insertar(problematicas, "P4", "Migracion y refugiados",
                     "Decidir como integrar migrantes, administrar recursos y mantener orden; "
                     "afecta economia, diversidad y estabilidad.",
                     cabeza);

    hashmap_insertar(problematicas, "P5", "Violencia y crimen organizado",
                     "Contener bandas y crimen; invertir en educacion/empleo "
                     "o usar fuerza con costo social.",
                     cabeza);


    int turno = 1;
    while(comprobar_perder(cabeza) == 0 && comprobar_ganar(cabeza) == 0) {
        printf("\n=== TURNO %d ===\n", turno);
        
        ejecutar_turno(jugadores, cabeza);
        //Aumentar estadistica random en territorio random para simular el paso del tiempo
        seleccionar_territorio_estadistica_random(cabeza);
        comprobar_eliminar_territorio_seguro(&cabeza, jugadores);

        if (comprobar_perder(cabeza) == 1 || comprobar_ganar(cabeza) == 1)
            break;

        ejecutar_turno_onu(jugadores, cabeza);
        // Aumentar estadistica random en territorio random para simular el paso del tiempo
        seleccionar_territorio_estadistica_random(cabeza);
        comprobar_eliminar_territorio_seguro(&cabeza, jugadores);
        
        turno++;
    }

    if(comprobar_perder(cabeza) == 1) {
        perder();
    }

    printf("Presiona ENTER para volver al menu principal...");
    getchar();

    // Liberar memoria
    liberarJugadores(jugadores);
    hashmap_eliminar(problematicas);
    liberar_lista(cabeza);
}

// Funcion para iniciar el modo dificil
void iniciar_modo_dificil()
{
    limpiar_pantalla();
    mostrar_banner();

    printf("======================================================================\n");
    printf("                  MODO DIFICIL (un verdadero reto!)               \n");
    printf("======================================================================\n\n");

    // inicia el juego
    Territorio *cabeza = construir_lista_ejemplo();
    valoresProblematicas(cabeza);

    printf("\nEstado inicial de los territorios:\n\n");
    imprimir_tabla(cabeza);

    // Crear lista de jugadores directamente
    jugadorList *jugadores = malloc(sizeof(jugadorList));
    if (!jugadores)
    {
        printf("Error: memoria insuficiente\n");
        return;
    }
    jugadores->inicio = NULL;
    jugadores->final = NULL;

    // Pedir nombre del jugador
    char nombreJugador[32];
    printf("\n>> CONFIGURACION DE JUGADORES\n");
    printf("--------------------------------------------------------------\n");
    printf("Ingresa el nombre del grupo anarquista al que perteneces (max 31 caracteres): ");
    fgets(nombreJugador, sizeof(nombreJugador), stdin);

    // Eliminar el salto de linea del fgets
    size_t len = strlen(nombreJugador);
    if (len > 0 && nombreJugador[len - 1] == '\n')
    {
        nombreJugador[len - 1] = '\0';
    }

    // Si el nombre esta vacio, asignar un nombre por defecto
    if (strlen(nombreJugador) == 0)
    {
        strcpy(nombreJugador, "Jugador");
    }

    // Asignar territorio aleatorio al jugador
    int territorioAleatorio = rand() % 10 + 1;
    char codigoTerritorio[3];
    sprintf(codigoTerritorio, "%02d", territorioAleatorio);

    Territorio *ubicacionJugador = buscarTerritorioPorCodigo(codigoTerritorio, cabeza);
    if (ubicacionJugador != NULL)
    {
        agregarJugador(jugadores, ubicacionJugador, nombreJugador);
    }

    // Crear jugador 2 automatico: ONU en un territorio distinto
    int territorioONU;
    Territorio *ubicacionONU = NULL;
    do
    {
        territorioONU = rand() % 10 + 1;
        char codigoONU[3];
        sprintf(codigoONU, "%02d", territorioONU);
        ubicacionONU = buscarTerritorioPorCodigo(codigoONU, cabeza);
    } while (ubicacionONU == NULL || (ubicacionJugador != NULL && strcmp(ubicacionONU->codigo, ubicacionJugador->codigo) == 0));

    // Nombre fijo "ONU"
    agregarJugador(jugadores, ubicacionONU, "ONU");

    // Mostrar informacion de los jugadores
    mostrarJugadores(jugadores);

    hashmap *problematicas = crear_problematicas_facil(cabeza);

    hashmap_insertar(problematicas, "P2", "Desigualdad social",
                     "Gestionar recursos, empleo y educacion entre clases; "
                     "malas decisiones aumentan pobreza y protestas o colapsan la economia.",
                     cabeza);

    hashmap_insertar(problematicas, "P3", "Corrupcion gubernamental",
                     "Transparencia vs enriquecimiento ilicito; "
                     "la corrupcion reduce inversion y genera caos social.",
                     cabeza);

    hashmap_insertar(problematicas, "P4", "Migracion y refugiados",
                     "Decidir como integrar migrantes, administrar recursos y mantener orden; "
                     "afecta economia, diversidad y estabilidad.",
                     cabeza);

    hashmap_insertar(problematicas, "P5", "Violencia y crimen organizado",
                     "Contener bandas y crimen; invertir en educacion/empleo "
                     "o usar fuerza con costo social.",
                     cabeza);

    int turno = 1;
    while(comprobar_perder(cabeza) == 0 && comprobar_ganar(cabeza) == 0) {
        printf("\n=== TURNO %d ===\n", turno);
        
        ejecutar_turno(jugadores, cabeza);
        //Aumentar estadistica random en territorio random para simular el paso del tiempo
        seleccionar_territorio_estadistica_random(cabeza);
        comprobar_eliminar_territorio_seguro(&cabeza, jugadores);

        if (comprobar_perder(cabeza) == 1 || comprobar_ganar(cabeza) == 1)
            break;

        ejecutar_turno_onu(jugadores, cabeza);
        // Aumentar estadistica random en territorio random para simular el paso del tiempo
        seleccionar_territorio_estadistica_random(cabeza);
        comprobar_eliminar_territorio_seguro(&cabeza, jugadores);
        
        turno++;
    }

    if(comprobar_perder(cabeza) == 1) {
        perder();
    }

    // TODO: agregar logica extra para modo dificil (A;adir piratas y que estos afecten el juego)
    // TODO: seguir logica del juego (hacer problematicas mas complicadas, cosa de piratas)

    printf("Presiona ENTER para volver al menu principal...");
    limpiar_buffer();
    getchar();

    // Liberar memoria
    liberarJugadores(jugadores);
    hashmap_eliminar(problematicas);
    liberar_lista(cabeza);
}

// Funcion principal para mostrar el menu
void mostrar_menu_principal()
{
    int opcion;
    int salir = 0;

    while (!salir)
    {
        limpiar_pantalla();
        mostrar_banner();

        printf("  ====================================================================\n");
        printf("                           MENU PRINCIPAL                           \n");
        printf("  ====================================================================\n\n");

        printf("    1 - Jugar modo FACIL\n");
        printf("    2 - Jugar modo DIFICIL\n");
        printf("    3 - Mas informacion del juego\n");
        printf("    4 - Salir\n\n");

        printf("  ====================================================================\n");
        printf("  Selecciona una opcion: ");

        if (scanf("%d", &opcion) != 1)
        {
            limpiar_buffer();
            printf("\n  Emmm... eso no es valido. Presiona ENTER para continuar...");
            getchar();
            continue;
        }
        limpiar_buffer();

        switch (opcion)
        {
        case 1:
            iniciar_modo_facil();
            break;
        case 2:
            iniciar_modo_dificil();
            break;
        case 3:
            mostrar_informacion();
            break;
        case 4:
            limpiar_pantalla();
            mostrar_banner();
            printf("  Gracias por jugar SOCIETAS!\n");
            printf("  El mundo te espera en la proxima...\n\n");
            salir = 1;
            break;
        default:
            printf("\n  Opcion invalida. Presiona ENTER para continuar...");
            getchar();
            break;
        }
    }
}
