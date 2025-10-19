#include "menu.h"
#include "implementacion_hashmap.h"
#include "generacion_terreno.h"
#include "jugadores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// para no tener que ponerlo muchas veces
char* PROB_A_LABEL = "Contaminacion y cambio climatico";
char* PROB_B_LABEL = "Desigualdad social";
char* PROB_C_LABEL = "Corrupcion gubernamental";

// Funcion para limpiar el buffer
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funcion para hacer clear de terminal, ver info en docu
void limpiar_pantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Funcion para mostrar el banner del juego (solo sale 1 vez)
void mostrar_banner() {
    printf("\n");
    printf("  ====================================================================\n");
    printf("                              SOCIETAS\n");
    printf("                  Un juego de estrategia y decisiones\n");
    printf("  ====================================================================\n\n");
}

// ---- Helpers de turno (solo para este modulo) ----
static void mostrar_estado_territorio(Territorio* t) {
    if (!t) return;
    printf("\n== Territorio actual ==\n");
    printf("  %s (%s)\n", t->nombre, t->codigo);
    printf("    A:%d - %s\n", t->A, PROB_A_LABEL);
    printf("    B:%d - %s\n", t->B, PROB_B_LABEL);
    printf("    C:%d - %s\n\n", t->C, PROB_C_LABEL);
}

static void listar_vecinos(Territorio* t) {
    if (!t) return;
    printf("Vecinos disponibles:\n");
    for (int i = 0; i < t->cantidad_conexiones; i++) {
        printf("  %d) %s\n", i + 1, t->conexiones[i]);
    }
}

static int indice_vecino_valido(Territorio* t, int idx1based) {
    return t && idx1based >= 1 && idx1based <= t->cantidad_conexiones;
}

static void disminuir_problema(Territorio* t, char est) {
    if (!t) return;
    if (est == 'A' || est == 'a') { if (t->A > 0) t->A--; }
    else if (est == 'B' || est == 'b') { if (t->B > 0) t->B--; }
    else if (est == 'C' || est == 'c') { if (t->C > 0) t->C--; }
}

static void ejecutar_primer_turno(jugadorList* jugadores, Territorio* cabeza) {
    if (!jugadores || !jugadores->inicio) return;
    jugador* j1 = jugadores->inicio; // jugador humano
    int acciones = 4;

    printf("\n================ PRIMER TURNO (Jugador: %s) ================\n", j1->nombre);
    mostrar_estado_territorio(j1->ubicacion);

    while (acciones > 0) {
        printf("Acciones restantes: %d\n", acciones);
        printf("Elige una accion:\n");
        printf("  1) Moverse a un pais vecino\n");
        printf("  2) Hacer proyecto (bajar A/B/C en 1)\n");
        printf("  3) Ver estado actual\n");
        printf("  4) Ver vecinos\n");
        printf("Seleccion: ");
        int op = 0;
        if (scanf("%d", &op) != 1) { limpiar_buffer(); printf("\nEntrada invalida.\n\n"); continue; }
        limpiar_buffer();

        if (op == 1) {
            listar_vecinos(j1->ubicacion);
            printf("Ingresa el numero del vecino (1-%d): ", j1->ubicacion->cantidad_conexiones);
            int choice = 0;
            if (scanf("%d", &choice) != 1) { limpiar_buffer(); printf("\nEntrada invalida.\n\n"); continue; }
            limpiar_buffer();
            if (!indice_vecino_valido(j1->ubicacion, choice)) { printf("Opcion fuera de rango.\n\n"); continue; }
            const char* cod = j1->ubicacion->conexiones[choice - 1];
            Territorio* destino = buscarTerritorioPorCodigo(cod, cabeza);
            if (!destino) { printf("No se encontro el territorio destino.\n\n"); continue; }
            j1->ubicacion = destino;
            printf("Te moviste a %s (%s).\n\n", destino->nombre, destino->codigo);
            acciones--;
        } else if (op == 2) {
            printf("Elige que problema bajar (A/B/C)\n");
            printf("  A = %s\n  B = %s\n  C = %s\n", PROB_A_LABEL, PROB_B_LABEL, PROB_C_LABEL);
            printf("Seleccion: ");
            int c = getchar();
            limpiar_buffer();
            if (c == EOF) { printf("Entrada invalida.\n\n"); continue; }
            char est = (char)c;
            if (est!='A'&&est!='a'&&est!='B'&&est!='b'&&est!='C'&&est!='c') { printf("Opcion invalida. Usa A, B o C.\n\n"); continue; }
            disminuir_problema(j1->ubicacion, est);
            printf("Proyecto aplicado en %s.\n  A:%d (%s)  B:%d (%s)  C:%d (%s)\n\n",
                   j1->ubicacion->nombre,
                   j1->ubicacion->A, PROB_A_LABEL,
                   j1->ubicacion->B, PROB_B_LABEL,
                   j1->ubicacion->C, PROB_C_LABEL);
            acciones--;
        } else if (op == 3) {
            mostrar_estado_territorio(j1->ubicacion);
        } else if (op == 4) {
            listar_vecinos(j1->ubicacion);
            printf("\n");
        } else {
            printf("Opcion invalida.\n\n");
        }
    }

    printf("Turno finalizado. Posicion actual: %s (%s) | A:%d B:%d C:%d\n\n",
           j1->ubicacion->nombre, j1->ubicacion->codigo, j1->ubicacion->A, j1->ubicacion->B, j1->ubicacion->C);
}

// Funcion para mostrar informacion del juego
void mostrar_informacion() {
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
    printf("   Cada territorio enfrenta 3 problemas diferentes:\n");
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
void iniciar_modo_facil() {
    limpiar_pantalla();
    mostrar_banner();
    
    printf("======================================================================\n");
    printf("                MODO FACIL (acaso tienes miedo?)                \n");
    printf("======================================================================\n\n");
    
    // Inicializar semilla para numeros aleatorios
    srand(time(NULL));
    
    // Iniciar el juego facil
    Territorio* cabeza = construir_lista_ejemplo();
    valoresProblematicas(cabeza);
    
    printf("\n>> Estado inicial de los territorios:\n\n");
    imprimir_tabla(cabeza);
    
    // Crear lista de jugadores directamente
    jugadorList* jugadores = malloc(sizeof(jugadorList));
    if (!jugadores) {
        printf("Error: memoria insuficiente\n");
        return;
    }
    jugadores->inicio = NULL;
    jugadores->final = NULL;
    
    // Pedir nombre del jugador
    char nombreJugador[32];
    printf("\n>> CONFIGURACION DE JUGADORES\n");
    printf("--------------------------------------------------------------\n");
    printf("Ingresa tu nombre (max 31 caracteres): ");
    fgets(nombreJugador, sizeof(nombreJugador), stdin);
    
    // Eliminar el salto de linea del fgets
    size_t len = strlen(nombreJugador);
    if (len > 0 && nombreJugador[len-1] == '\n') {
        nombreJugador[len-1] = '\0';
    }
    
    // Si el nombre esta vacio, asignar un nombre por defecto
    if (strlen(nombreJugador) == 0) {
        strcpy(nombreJugador, "Jugador");
    }
    
    // Asignar territorio aleatorio al jugador
    int territorioAleatorio = rand() % 10 + 1;
    char codigoTerritorio[3];
    sprintf(codigoTerritorio, "%02d", territorioAleatorio);
    
    Territorio* ubicacionJugador = buscarTerritorioPorCodigo(codigoTerritorio, cabeza);
    if (ubicacionJugador != NULL) {
        agregarJugador(jugadores, ubicacionJugador, nombreJugador);
    }
    
    // Crear jugador 2 automatico: ONU en un territorio distinto
    int territorioONU;
    Territorio* ubicacionONU = NULL;
    do {
        territorioONU = rand() % 10 + 1;
        char codigoONU[3];
        sprintf(codigoONU, "%02d", territorioONU);
        ubicacionONU = buscarTerritorioPorCodigo(codigoONU, cabeza);
    } while (ubicacionONU == NULL || (ubicacionJugador != NULL && strcmp(ubicacionONU->codigo, ubicacionJugador->codigo) == 0));

    // Nombre fijo "ONU"
    agregarJugador(jugadores, ubicacionONU, "ONU");
    
    // Mostrar informacion de los jugadores
    mostrarJugadores(jugadores);
    
    // Crear hashmap de problematicas
    hashmap* problematicas = hashmap_crear();
    
    hashmap_insertar(problematicas, "P1", "Contaminacion y cambio climatico", 
                     "Reducir contaminacion equilibrando desarrollo y sostenibilidad; "
                     "malas decisiones aumentan temperatura y causan desastres.", cabeza);
    
    hashmap_insertar(problematicas, "P2", "Desigualdad social", 
                     "Gestionar recursos, empleo y educacion entre clases; "
                     "malas decisiones aumentan pobreza y protestas o colapsan la economia.", cabeza);
    
    hashmap_insertar(problematicas, "P3", "Corrupcion gubernamental", 
                     "Transparencia vs enriquecimiento ilicito; "
                     "la corrupcion reduce inversion y genera caos social.", cabeza);
    
    hashmap_insertar(problematicas, "P4", "Migracion y refugiados", 
                     "Decidir como integrar migrantes, administrar recursos y mantener orden; "
                     "afecta economia, diversidad y estabilidad.", cabeza);
    
    hashmap_insertar(problematicas, "P5", "Violencia y crimen organizado", 
                     "Contener bandas y crimen; invertir en educacion/empleo "
                     "o usar fuerza con costo social.", cabeza);
    
    
    // Primer turno del jugador 1
    ejecutar_primer_turno(jugadores, cabeza);

    printf("Presiona ENTER para volver al menu principal...");
    getchar();
    
    // Liberar memoria
    liberarJugadores(jugadores);
    hashmap_eliminar(problematicas);
    liberar_lista(cabeza);
}

// Funcion para iniciar el modo dificil
void iniciar_modo_dificil() {
    limpiar_pantalla();
    mostrar_banner();
    
    printf("======================================================================\n");
    printf("                  MODO DIFICIL (un verdadero reto!)               \n");
    printf("======================================================================\n\n");
    
    
    // inicia el juego
    Territorio* cabeza = construir_lista_ejemplo();
    valoresProblematicas(cabeza);
    
    printf("\nEstado inicial de los territorios:\n\n");
    imprimir_tabla(cabeza);
    
    hashmap* problematicas = hashmap_crear();
    
    hashmap_insertar(problematicas, "P1", "Contaminacion y cambio climatico", 
                     "Reducir contaminacion equilibrando desarrollo y sostenibilidad; "
                     "malas decisiones aumentan temperatura y causan desastres.", cabeza);
    
    hashmap_insertar(problematicas, "P2", "Desigualdad social", 
                     "Gestionar recursos, empleo y educacion entre clases; "
                     "malas decisiones aumentan pobreza y protestas o colapsan la economia.", cabeza);
    
    hashmap_insertar(problematicas, "P3", "Corrupcion gubernamental", 
                     "Transparencia vs enriquecimiento ilicito; "
                     "la corrupcion reduce inversion y genera caos social.", cabeza);
    
    hashmap_insertar(problematicas, "P4", "Migracion y refugiados", 
                     "Decidir como integrar migrantes, administrar recursos y mantener orden; "
                     "afecta economia, diversidad y estabilidad.", cabeza);
    
    hashmap_insertar(problematicas, "P5", "Violencia y crimen organizado", 
                     "Contener bandas y crimen; invertir en educacion/empleo "
                     "o usar fuerza con costo social.", cabeza);
    
    
    // TODO: seguir logica del juego (hacer problematicas mas complicadas, cosa de piratas)
    
    printf("Presiona ENTER para volver al menu principal...");
    limpiar_buffer();
    getchar();

    hashmap_eliminar(problematicas);
    liberar_lista(cabeza);
}

// Funcion principal para mostrar el menu
void mostrar_menu_principal() {
    int opcion;
    int salir = 0;
    
    while (!salir) {
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
        
        if (scanf("%d", &opcion) != 1) {
            limpiar_buffer();
            printf("\n  Emmm... eso no es valido. Presiona ENTER para continuar...");
            getchar();
            continue;
        }
        limpiar_buffer();
        
        switch(opcion) {
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
