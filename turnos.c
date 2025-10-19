#include "turnos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Etiquetas compartidas (definidas aqui y declaradas extern en turnos.h)
const char* PROB_A_LABEL = "Contaminacion y cambio climatico";
const char* PROB_B_LABEL = "Desigualdad social";
const char* PROB_C_LABEL = "Corrupcion gubernamental";

static int indice_vecino_valido(Territorio* t, int idx1based) {
    return t && idx1based >= 1 && idx1based <= t->cantidad_conexiones;
}

static void disminuir_problema(Territorio* t, char est) {
    if (!t) return;
    if (est == 'A' || est == 'a') { if (t->A > 0) t->A--; }
    else if (est == 'B' || est == 'b') { if (t->B > 0) t->B--; }
    else if (est == 'C' || est == 'c') { if (t->C > 0) t->C--; }
}

static const char* label_de_est(char est) {
    if (est=='A'||est=='a') return PROB_A_LABEL;
    if (est=='B'||est=='b') return PROB_B_LABEL;
    return PROB_C_LABEL;
}

static void subir_aleatorio_en_tres_paises(Territorio* cabeza) {
    int n = 0;
    for (Territorio* it = cabeza; it; it = it->siguiente) n++;
    if (n == 0) return;
    int maxIdx = (n > 32) ? 32 : n;
    int idx[32];
    for (int i = 0; i < maxIdx; i++) idx[i] = i;
    barajar(idx, maxIdx);
    int subir_cuantos = (maxIdx < 3) ? maxIdx : 3;
    for (int k = 0; k < subir_cuantos; k++) {
        int objetivo = idx[k];
        Territorio* t = cabeza;
        for (int j = 0; j < objetivo && t; j++) t = t->siguiente;
        if (!t) continue;
        int r = rand() % 3;
        char est = (r==0)?'A':(r==1)?'B':'C';
        aumentar_estadistica(cabeza, t->codigo, est);
    }
}

void mostrar_estado_territorio(Territorio* t) {
    if (!t) return;
    printf("\n== Territorio actual ==\n");
    printf("  %s (%s)\n", t->nombre, t->codigo);
    printf("    A:%d - %s\n", t->A, PROB_A_LABEL);
    printf("    B:%d - %s\n", t->B, PROB_B_LABEL);
    printf("    C:%d - %s\n\n", t->C, PROB_C_LABEL);
}

void listar_vecinos(Territorio* t) {
    if (!t) return;
    printf("Vecinos disponibles:\n");
    for (int i = 0; i < t->cantidad_conexiones; i++) {
        printf("  %d) %s\n", i + 1, t->conexiones[i]);
    }
}

void ejecutar_primer_turno(jugadorList* jugadores, Territorio* cabeza) {
    if (!jugadores || !jugadores->inicio) return;
    jugador* j1 = jugadores->inicio;
    int acciones = 4;
    printf("\n================ PRIMER TURNO (Jugador: %s) ================\n", j1->nombre);
    mostrar_estado_territorio(j1->ubicacion);
    while (acciones > 0) {
        printf("Acciones restantes: %d\n", acciones);
        printf("Elige una accion:\n  1) Moverse a un pais vecino\n  2) Hacer proyecto (bajar A/B/C en 1)\n  3) Ver estado actual\n  4) Ver vecinos\nSeleccion: ");
        int op = 0; if (scanf("%d", &op) != 1) { int c; while((c=getchar())!='\n'&&c!=EOF); printf("\nEntrada invalida.\n\n"); continue; }
        int c; while((c=getchar())!='\n'&&c!=EOF);
        if (op == 1) {
            listar_vecinos(j1->ubicacion);
            printf("Ingresa el numero del vecino (1-%d): ", j1->ubicacion->cantidad_conexiones);
            int choice=0; if (scanf("%d", &choice) != 1) { while((c=getchar())!='\n'&&c!=EOF); printf("\nEntrada invalida.\n\n"); continue; }
            while((c=getchar())!='\n'&&c!=EOF);
            if (!indice_vecino_valido(j1->ubicacion, choice)) { printf("Opcion fuera de rango.\n\n"); continue; }
            const char* cod = j1->ubicacion->conexiones[choice - 1];
            Territorio* destino = buscarTerritorioPorCodigo(cod, cabeza);
            if (!destino) { printf("No se encontro el territorio destino.\n\n"); continue; }
            j1->ubicacion = destino;
            printf("Te moviste a %s (%s).\n\n", destino->nombre, destino->codigo);
            acciones--;
        } else if (op == 2) {
            printf("Elige que problema bajar (A/B/C)\n  A = %s\n  B = %s\n  C = %s\nSeleccion: ", PROB_A_LABEL, PROB_B_LABEL, PROB_C_LABEL);
            int ch = getchar(); while((c=getchar())!='\n'&&c!=EOF);
            if (ch == EOF) { printf("Entrada invalida.\n\n"); continue; }
            char est = (char)ch;
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
    printf("Turno finalizado. Posicion actual: %s (%s) | A:%d B:%d C:%d\n",
           j1->ubicacion->nombre, j1->ubicacion->codigo, j1->ubicacion->A, j1->ubicacion->B, j1->ubicacion->C);
    subir_aleatorio_en_tres_paises(cabeza);
    printf("\nCambios globales tras el turno del jugador:\n\n");
    imprimir_tabla(cabeza);
    printf("\n");
}

void ejecutar_turno_onu(jugadorList* jugadores, Territorio* cabeza) {
    if (!jugadores || !jugadores->inicio) return;
    jugador* j1 = jugadores->inicio; // humano
    jugador* onu = j1->sigt; // se agregó justo después
    if (!onu) return;
    printf("================ TURNO DE ( %s ) ================\n\n", onu->nombre);
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

        if (op == 0 && puede_mover) {
            int deg = onu->ubicacion->cantidad_conexiones;
            int choice = rand() % deg;
            if (deg > 1) {
                // Evitar repetir el mismo vecino si hay opciones
                for (int tries = 0; tries < 3 && choice == ultimo_vecino_idx; tries++) {
                    choice = rand() % deg;
                }
            }
            ultimo_vecino_idx = choice;
            const char* cod = onu->ubicacion->conexiones[choice];
            Territorio* destino = buscarTerritorioPorCodigo(cod, cabeza);
            if (destino) {
                printf("ONU se mueve de %s (%s) a %s (%s).\n", onu->ubicacion->nombre, onu->ubicacion->codigo, destino->nombre, destino->codigo);
                onu->ubicacion = destino;
            }
        } else {
            // Escoger una estadistica que sea > 0 si es posible
            char candidatos[3]; int cnt = 0;
            if (onu->ubicacion->A > 0) candidatos[cnt++] = 'A';
            if (onu->ubicacion->B > 0) candidatos[cnt++] = 'B';
            if (onu->ubicacion->C > 0) candidatos[cnt++] = 'C';
            char est;
            if (cnt > 0) {
                est = candidatos[rand() % cnt];
            } else {
                int r = rand() % 3; est = (r==0)?'A':(r==1)?'B':'C';
            }
            if (est=='A'&&onu->ubicacion->A>0) onu->ubicacion->A--; else
            if (est=='B'&&onu->ubicacion->B>0) onu->ubicacion->B--; else
            if (est=='C'&&onu->ubicacion->C>0) onu->ubicacion->C--;
            printf("ONU aplica proyecto en %s: -1 en %c (%s).\n", onu->ubicacion->nombre, est, label_de_est(est));
        }
    }
    // Mostrar estado de la ONU ANTES de que incrementen problemas globales
    printf("Estado ONU tras turno: %s (%s) | A:%d B:%d C:%d\n", onu->ubicacion->nombre, onu->ubicacion->codigo, onu->ubicacion->A, onu->ubicacion->B, onu->ubicacion->C);
    subir_aleatorio_en_tres_paises(cabeza);
    printf("\nCambios globales tras el turno de la ONU:\n\n");
    imprimir_tabla(cabeza);
    printf("\n");
}
