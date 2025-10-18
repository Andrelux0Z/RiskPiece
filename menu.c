#include "menu.h"
#include "implementacion_hashmap.h"
#include "generacion_terreno.h"
#include <stdio.h>
#include <stdlib.h>

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

// Funcion para mostrar información del juego
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
    printf("   P1: Contaminacion y cambio climatico\n");
    printf("   P2: Desigualdad social\n");
    printf("   P3: Corrupcion gubernamental\n");
    printf("   P4: Migracion y refugiados\n");
    printf("   P5: Violencia y crimen organizado\n\n");
    
    printf(">> ESTADISTICAS:\n");
    printf("   Cada territorio tiene tres niveles de problemas (A, B, C)\n");
    printf("   con valores del 0 al 3. La idea es no dejar que pase de C.\n\n");
    
    printf(">> MODOS DE JUEGO:\n");
    printf("   MODO FACIL: Mas recursos y tiempo para resolver problemas.\n");
    printf("   MODO DIFICIL: Problemas más graves y un grupo adicional arruniando el mapa.\n\n");
    
    printf("======================================================================\n");
    printf("  Desarrollado por: Andrés, Marco y Fabián para Estructuras de Datos.\n");
    printf("======================================================================\n\n");
    
    printf("Presiona ENTER para volver al menú principal...");
    limpiar_buffer();
    getchar();
}

// Funcion para iniciar el modo fácil
void iniciar_modo_facil() {
    limpiar_pantalla();
    mostrar_banner();
    
    printf("======================================================================\n");
    printf("                MODO FACIL (acaso tienes miedo?)                \n");
    printf("======================================================================\n\n");
    
    
    // incia el juego facil
    Territorio* cabeza = construir_lista_ejemplo();
    valoresProblematicas(cabeza);
    
    printf("\nEstado inicial de los territorios:\n\n");
    imprimir_tabla(cabeza);
    
    hashmap* problematicas = hashmap_crear();
    
    hashmap_insertar(problematicas, "P1", "Contaminación y cambio climático", 
                     "Reducir contaminación equilibrando desarrollo y sostenibilidad; "
                     "malas decisiones aumentan temperatura y causan desastres.", cabeza);
    
    hashmap_insertar(problematicas, "P2", "Desigualdad social", 
                     "Gestionar recursos, empleo y educación entre clases; "
                     "malas decisiones aumentan pobreza y protestas o colapsan la economía.", cabeza);
    
    hashmap_insertar(problematicas, "P3", "Corrupción gubernamental", 
                     "Transparencia vs enriquecimiento ilícito; "
                     "la corrupción reduce inversión y genera caos social.", cabeza);
    
    hashmap_insertar(problematicas, "P4", "Migración y refugiados", 
                     "Decidir cómo integrar migrantes, administrar recursos y mantener orden; "
                     "afecta economía, diversidad y estabilidad.", cabeza);
    
    hashmap_insertar(problematicas, "P5", "Violencia y crimen organizado", 
                     "Contener bandas y crimen; invertir en educacion/empleo "
                     "o usar fuerza con costo social.", cabeza);
    
    
    // TODO: seguir la logica del juego
    
    printf("Presiona ENTER para volver al menú principal...");
    limpiar_buffer();
    getchar();
    
    hashmap_eliminar(problematicas);
    liberar_lista(cabeza);
}

// Función para iniciar el modo dificil
void iniciar_modo_dificil() {
    limpiar_pantalla();
    mostrar_banner();
    
    printf("======================================================================\n");
    printf("                  MODO DIFICIL (un verdadero reto!)               \n");
    printf("+======================================================================+\n\n");
    
    
    // inicia el juego
    Territorio* cabeza = construir_lista_ejemplo();
    valoresProblematicas(cabeza);
    
    printf("\nEstado inicial de los territorios:\n\n");
    imprimir_tabla(cabeza);
    
    hashmap* problematicas = hashmap_crear();
    
    hashmap_insertar(problematicas, "P1", "Contaminación y cambio climático", 
                     "Reducir contaminación equilibrando desarrollo y sostenibilidad; "
                     "malas decisiones aumentan temperatura y causan desastres.", cabeza);
    
    hashmap_insertar(problematicas, "P2", "Desigualdad social", 
                     "Gestionar recursos, empleo y educación entre clases; "
                     "malas decisiones aumentan pobreza y protestas o colapsan la economía.", cabeza);
    
    hashmap_insertar(problematicas, "P3", "Corrupción gubernamental", 
                     "Transparencia vs enriquecimiento ilícito; "
                     "la corrupción reduce inversión y genera caos social.", cabeza);
    
    hashmap_insertar(problematicas, "P4", "Migración y refugiados", 
                     "Decidir cómo integrar migrantes, administrar recursos y mantener orden; "
                     "afecta economía, diversidad y estabilidad.", cabeza);
    
    hashmap_insertar(problematicas, "P5", "Violencia y crimen organizado", 
                     "Contener bandas y crimen; invertir en educacion/empleo "
                     "o usar fuerza con costo social.", cabeza);
    
    
    // TODO: seguir logica del juego (hacer problemáticas más complicadas, cosa de piratas)
    
    printf("Presiona ENTER para volver al menú principal...");
    limpiar_buffer();
    getchar();

    hashmap_eliminar(problematicas);
    liberar_lista(cabeza);
}

// Función principal para mostrar el menú
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
