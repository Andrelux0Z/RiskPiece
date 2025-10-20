// prueba

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

/* Introduccion para modo facil */
static void mostrar_introduccion_facil(const char *grupo)
{
    limpiar_pantalla();
    mostrar_banner();

    printf("======================================================================\n");
    printf("                      INTRODUCCION - MODO FACIL                        \n");
    printf("======================================================================\n\n");

    printf("Hace tiempo las naciones se tambaleaban por diversas problematicas sumamente graves, como la esclavitud y el abuso de drogas.\n");
    printf("Tu grupo anarquista, %s, ha surgido como una chispa de cambio: organizada,\n", grupo);
    printf("valiente y decidida a devolverle dignidad a la gente de estas diversas regiones. El estado actual de\n");
    printf("los territorios exige acciones concretas e inmmediatas: proyectos comunitarios, redes\n");
    printf("de apoyo y campañas de concienciacion.\n\n");

    printf("En este contexto, cuentas con recursos y tiempo para planear con calma.\n");
    printf("esto no debe frenar tu impulso, ya que esto no es un juego, la vida de las personas\n");
    printf("de cada una de las personas de estas regiones depende de tus decisiones.\n\n");
    printf("Tu liderazgo inspira a la poblacion local, y la ONU respondera con\n");
    printf("mayor predisposicion para financiar y ejecutar proyectos cuando el\n");
    printf("movimiento muestre resultados claros. Cada victoria local aumenta la\n");
    printf("participacion internacional y acelera la recuperacion.\n\n");


    printf("Como ejemplos breves: podriais construir cisternas y sistemas de riego\n");
    printf("para mitigar sequias, organizar programas de memoria y archivos\n");
    printf("comunitarios para combatir el olvido social, o lanzar iniciativas de\n");
    printf("empleo local para reducir la pobreza. Estas acciones muestran impacto\n");
    printf("y ayudan a atraer apoyo internacional para proyectos mas grandes.\n\n");

    printf("Presiona ENTER para continuar y tomar tu primer paso hacia el cambio...");
    limpiar_buffer();
    getchar();
}

/* Introduccion para modo dificil */
static void mostrar_introduccion_dificil(const char *grupo)
{
    limpiar_pantalla();
    mostrar_banner();

    printf("======================================================================\n");
    printf("                     INTRODUCCION - MODO DIFICIL                       \n");
    printf("======================================================================\n\n");

    printf("La decadencia se ha acelerado: violencia organizada, instituciones podridas\n");
    printf("y el colapso ambiental amenazan con arrastrarlo todo. %s, tu grupo\n", grupo);
    printf("anarquista encarna la rabia y la esperanza a la vez. Se debe no solo buscar la idea \n");
    printf("de destruir de forma superficial el problema, sino construir alternativas reales desde la base.\n\n");

    printf("En este contexto, las dificultades son mayores: menos recursos, problemas\n");
    printf("mas complejos y actores hostiles. Sin embargo, cuando la accion imponente de tu grupo\n");
    printf("impacte positivamente en un territorio, la ONU puede verse forzada a\n");
    printf("intervenir y lanzar proyectos de apoyo. Vuestras campañas publicas y\n");
    printf("vuestras alianzas son la clave para atraer a la comunidad internacional.\n\n");

    printf("Manten la moral alta: cada accion cuenta y puede cambiar la reaccion de la ONU.\n");
    
    printf("Por ejemplo, en situaciones criticas se debe priorizar el reparto de agua,\n");
    printf("organizar operativos de liberacion frente a la esclavitud y distribuir\n");
    printf("kits de potabilizacion para fuentes contaminadas. Intervenciones veloces\n");
    printf("como estas demuestran impacto y pueden provocar la implicacion de la ONU.\n\n");
    printf("Presiona ENTER para aceptar la carga y empezar a forjar el cambio...");
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

    //Mostrar introduccion especifica del modo facil para el grupo ingresado.
    mostrar_introduccion_facil(nombreJugador);

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

    int turno = 1;
    while (comprobar_perder(cabeza) == 0 && comprobar_ganar(cabeza) == 0)
    {
        printf("\n=== TURNO %d ===\n", turno);

        ejecutar_turno(jugadores, cabeza);
        // Aumentar estadistica random en territorio random para simular el paso del tiempo
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

    if (comprobar_perder(cabeza) == 1)
    {
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

    // Crear lista de piratas
    pirataList *piratas = malloc(sizeof(pirataList));
    if (!piratas)
    {
        free(jugadores); // no hay memoria, hacemos free
        return;
    }
    piratas->inicio = NULL;
    piratas->final = NULL;

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

    //Mostrar introduccion especifica del modo dificil para el grupo ingresado.
    mostrar_introduccion_dificil(nombreJugador);


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

    // Nombre fijo ONU
    agregarJugador(jugadores, ubicacionONU, "ONU");

    // Mostrar informacion de los jugadores
    mostrarJugadores(jugadores);

    // Inicializar piratas en el modo dificil
    piratasInicial(piratas, cabeza);
    mostrarPiratas(piratas);

    hashmap *problematicas = crear_problematicas_facil(cabeza);

    int turno = 1;
    while (comprobar_perder(cabeza) == 0 && comprobar_ganar(cabeza) == 0)
    {
        printf("\n=== TURNO %d ===\n", turno);

        ejecutar_turno(jugadores, cabeza);
        // Aumentar estadistica random en territorio random para simular el paso del tiempo
        seleccionar_territorio_estadistica_random(cabeza);
        comprobar_eliminar_territorio_con_piratas(&cabeza, jugadores, piratas);

        if (comprobar_perder(cabeza) == 1 || comprobar_ganar(cabeza) == 1)
            break;

        ejecutar_turno_onu(jugadores, cabeza);
        // Aumentar estadistica random en territorio random para simular el paso del tiempo
        seleccionar_territorio_estadistica_random(cabeza);
        comprobar_eliminar_territorio_con_piratas(&cabeza, jugadores, piratas);

        if (comprobar_perder(cabeza) == 1 || comprobar_ganar(cabeza) == 1)
            break;

        // Turno de los piratas - causan daño y aumentan problematicas
        ejecutar_turno_piratas(piratas, cabeza);
        comprobar_eliminar_territorio_con_piratas(&cabeza, jugadores, piratas);
        
        turno++;
    }

    if (comprobar_perder(cabeza) == 1)
    {
        perder();
    }

    printf("Presiona ENTER para volver al menu principal...");
    limpiar_buffer();
    getchar();

    // Liberar memoria
    liberarJugadores(jugadores);
    liberarPiratas(piratas);
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
