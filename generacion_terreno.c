#include "generacion_terreno.h"
#include "implementacion_hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//La definición de Territorio ya estaba en el header 

// Limita un valor entero al rango [0,3]
int limitar_a_rango_0_a_3(int valor) {
	if (valor < 0) {
		return 0;
	}
	if (valor > 3) {
		return 3;
	}
	return valor;
}

// Crea y devuelve un nuevo Territorio 
Territorio* crear_territorio(const char* codigo, const char* nombre, const char conexiones[][3], int cantidad_conexiones, int A, int B, int C) {
    // Reserva memoria para el nuevo nodo y valida el resultado.
	Territorio* t = (Territorio*)malloc(sizeof(Territorio));
	if (!t) {
		printf("Error: memoria insuficiente\n"); 
		exit(EXIT_FAILURE);
	}

	// Copia "codigo" y "nombre" de forma segura (siempre NUL-terminados)
	strncpy(t->codigo, codigo, sizeof(t->codigo) - 1);
	t->codigo[sizeof(t->codigo) - 1] = '\0';
	strncpy(t->nombre, nombre, sizeof(t->nombre) - 1);
	t->nombre[sizeof(t->nombre) - 1] = '\0';

	int n = cantidad_conexiones;
	t->cantidad_conexiones = n;

	// Copia hasta 'n' códigos de conexión (2 chars + '\0') y limpia el resto
	for (int i = 0; i < n && i < 4; i++) {
		strncpy(t->conexiones[i], conexiones[i], sizeof(t->conexiones[i]) - 1);
		t->conexiones[i][sizeof(t->conexiones[i]) - 1] = '\0';
	}
	for (int i = n; i < 4; i++) {
		t->conexiones[i][0] = '\0';
	}

	// Limita las estadísticas de 0 a 3.
	t->A = limitar_a_rango_0_a_3(A);
	t->B = limitar_a_rango_0_a_3(B);
	t->C = limitar_a_rango_0_a_3(C);

	// Inicializa los punteros de la lista doblemente enlazada.
	t->anterior = t->siguiente = NULL;

	// Devuelve el nuevo territorio listo para insertarse en la lista.
	return t;
}

// Inserta un territorio al final de la lista
void agregar_territorio(Territorio** cabeza, Territorio* nodo) {
	if (!*cabeza) {
		*cabeza = nodo;
		return;
	}
	Territorio* actual = *cabeza;
	while (actual->siguiente) actual = actual->siguiente;
	actual->siguiente = nodo;
	nodo->anterior = actual;
}

void imprimir_tabla(Territorio* cabeza) {
    printf("Codigo  | Nombre         | Conexiones     | A | B | C\n");
    printf("------- + -------------- + -------------- + - + - + -\n");

    Territorio* actual = cabeza;
    while (actual) {
        // Imprimir conexiones en una sola cadena
        char conexiones_str[128] = "";
        for (int i = 0; i < actual->cantidad_conexiones; i++) {
            strcat(conexiones_str, actual->conexiones[i]);
            if (i < actual->cantidad_conexiones - 1)
                 strcat(conexiones_str, ", ");
        }

        // Imprimir una fila de la tabla
        printf("%-7s | %-14s | %-14s | %d | %d | %d\n",
               actual->codigo,
               actual->nombre,
               conexiones_str,
               actual->A,
               actual->B,
               actual->C);

        actual = actual->siguiente;
    }
}


// Libera la lista enlazada completa para evitar fugas de memoria
void liberar_lista(Territorio* cabeza) {
	Territorio* actual = cabeza;
	while (actual) {
		Territorio* siguiente = actual->siguiente;
		free(actual);
		actual = siguiente;
	}
}

// Rellena la lista con 10 territorios
Territorio* construir_lista_ejemplo(void) {
	Territorio* cabeza = NULL;

	const char c01[][3] = {"02","03"};
	const char c02[][3] = {"01","04"};
	const char c03[][3] = {"01","05","06"};
	const char c04[][3] = {"02","07"};
	const char c05[][3] = {"03","08"};
	const char c06[][3] = {"03","09"};
	const char c07[][3] = {"04","10"};
	const char c08[][3] = {"05"};
	const char c09[][3] = {"06"};
	const char c10[][3] = {"07"};

	agregar_territorio(&cabeza, crear_territorio("01", "Dressrosa",  c01, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("02", "Wano",  c02, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("03", "Punk Hazard",   c03, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("04", "Alabasta",   c04, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("05", "Pisos Picados",   c05, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("06", "Skypea",  c06, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("07", "Somalia",   c07, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("08", "Ba Sing Se",c08, 1, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("09", "Pharloom",   c09, 1, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("10", "Oceania",   c10, 1, 0, 0, 0));

	return cabeza;
}

// ejemplo para Marco

