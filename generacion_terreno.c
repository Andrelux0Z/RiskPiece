#include "generacion_terreno.h"
#include "implementacion_hashmap.h"
#include "jugadores.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
* Struct que define el territorio (esto es una lista doblemente enlazada)
* Cada nodo tiene un código, nombre, conexiones, A/B/C (cada problema), anterior/siguiente
*/
typedef struct Territorio
{
    char codigo[3];
    char nombre[32];
    char conexiones[4][3];
    int cantidad_conexiones;
    int A;
    int B;
    int C;
    struct Territorio *anterior;
    struct Territorio *siguiente;
} Territorio;

/*
* Funcion auxiliar para limitar el rango de los valores de las problematicas,
* retorna el int.
*/
int limitar_a_rango_0_a_3(int valor)
{
	if (valor < 0)
	{
		return 0;
	}
	if (valor > 3)
	{
		return 3;
	}
	return valor;
}

/*
* Funcion para crear un territorio, normalmente iniciamos con cabeza = NULL (como inicio).
* La función sirve para crear también los nodos, por lo que ocupamos código,
* nombre, conexiones (y su cantidad) y los valores de los 3 problemas.
* Retorna el territorio.
*/
Territorio *crear_territorio(char *codigo, char *nombre, char conexiones[][3], int cantidad_conexiones, int A, int B, int C)
{
	// Reserva memoria para el nuevo nodo y valida el resultado.
	Territorio *t = (Territorio *)malloc(sizeof(Territorio));
	if (!t)
	{
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
	for (int i = 0; i < n && i < 4; i++)
	{
		strncpy(t->conexiones[i], conexiones[i], sizeof(t->conexiones[i]) - 1);
		t->conexiones[i][sizeof(t->conexiones[i]) - 1] = '\0';
	}
	for (int i = n; i < 4; i++)
	{
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

/*
* Funcion para agregar un territorio (nodo) creado con crear_territorio
* lo que hace es poner este nuevo nodo al final de la lista doblemente enlazada
*/
void agregar_territorio(Territorio **cabeza, Territorio *nodo)
{
	if (!*cabeza)
	{
		*cabeza = nodo;
		return;
	}
	Territorio *actual = *cabeza;
	while (actual->siguiente)
		actual = actual->siguiente;
	actual->siguiente = nodo;
	nodo->anterior = actual;
}

/*
* Funcion para randomizar los valores de las problematicas.
* Nota: buscarTerritorioPorCodigo(const char*, Territorio*) esta implementada en jugadores.c
*/
void valoresProblematicas(Territorio *cabeza)
{
	int n = 9; // Numero de territorios
	int indices[n];
	int contador = 0;

	Territorio *actual = cabeza;

	for (int i = 0; i < n; i++)
	{
		indices[i] = i; // Llenar el arreglo con índices
	}

	barajar(indices, n); // Barajar los índices (Fisher-Yates shuffle)
	actual = cabeza;

	for (int i = 0; i < n; i++)
	{
		actual->codigo[1] = indices[i]; // Asignar códigos barajados
		actual = actual->siguiente;
	}

	actual = cabeza;

	while (actual)
	{
		if (actual->codigo[1] < 3)
		{
			// Caso 1: Valores de 1 en todos los aspectos
			actual->A = 1;
			actual->B = 1;
			actual->C = 1;
		}
		else if (actual->codigo[1] < 6)
		{
			// Caso 2: Un aspecto con 2, los otros con 1
			int aspecto = rand() % 3; // Elegir aleatoriamente el aspecto que será 2
			actual->A = (aspecto == 0) ? 2 : 1;
			actual->B = (aspecto == 1) ? 2 : 1;
			actual->C = (aspecto == 2) ? 2 : 1;
		}
		else
		{
			// Caso 3: Valores distintos en cada aspecto
			actual->A = 1 + rand() % 3; // Valores entre 1 y 3
			do
			{
				actual->B = 1 + rand() % 3;
			} while (actual->B == actual->A); // Asegurar que B sea distinto de A
			do
			{
				actual->C = 1 + rand() % 3;
			} while (actual->C == actual->A || actual->C == actual->B); // Asegurar que C sea distinto de A y B
		}

		actual = actual->siguiente;
	}

	actual = cabeza;

	for (int i = 0; i < n; i++)
	{
		char temp = '0' + i + 1;  // Convertir a carácter
		actual->codigo[1] = temp; // Reasignar códigos en orden
		actual = actual->siguiente;
	}
}

/*
* Funcion que baraja, lo utilizamos para randomizar. usa un método llamado
* Fisher-Yates, cuyo objetivo es randomizar un array.
*/
void barajar(int *arreglo, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		int temp = arreglo[i];
		arreglo[i] = arreglo[j];
		arreglo[j] = temp;
	}
}

/*
* Funcion para imprimir los territorios.
* Se manda cabeza, que es la lista doblemente enlazada que representa los territorios
*/
void imprimir_tabla(Territorio *cabeza)
{
	printf("Codigo  | Nombre         | Conexiones     | A | B | C\n");
	printf("------- + -------------- + -------------- + - + - + -\n");

	Territorio *actual = cabeza;
	while (actual)
	{
		// Imprimir conexiones en una sola cadena
		char conexiones_str[128] = "";
		for (int i = 0; i < actual->cantidad_conexiones; i++)
		{
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

/*
* Funcion para liberar la memoria utilizada, y evitar fugas de memoria
*/
void liberar_lista(Territorio *cabeza)
{
	Territorio *actual = cabeza;
	while (actual)
	{
		Territorio *siguiente = actual->siguiente;
		free(actual);
		actual = siguiente;
	}
}

/*
* Funcion que usa las otras funciones para crear la lista de territorio,
* con 9 territorios interconectados entre sí. Es ejemplo, porque se puede
* reconstruir para hacer otros países
*/
Territorio *construir_lista_ejemplo(void)
{
	Territorio *cabeza = NULL;

	// Conexiones balanceadas (9 nodos):
	// Ciclo: 01-02-03-04-05-06-07-08-09-01
	// Extras: (01-04), (03-06), (07-09)
	// Grados: 01(3),02(2),03(3),04(3),05(2),06(3),07(3),08(2),09(3)
	const char c01[][3] = {"02", "09", "04"};
	const char c02[][3] = {"01", "03"};
	const char c03[][3] = {"02", "04", "06"};
	const char c04[][3] = {"03", "05", "01"};
	const char c05[][3] = {"04", "06"};
	const char c06[][3] = {"05", "07", "03"};
	const char c07[][3] = {"06", "08", "09"};
	const char c08[][3] = {"07", "09"};
	const char c09[][3] = {"08", "01", "07"};

	agregar_territorio(&cabeza, crear_territorio("01", "Dressrosa", c01, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("02", "Wano", c02, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("03", "Punk Hazard", c03, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("04", "Alabasta", c04, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("05", "Pisos Picados", c05, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("06", "Skypea", c06, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("07", "Somalia", c07, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("08", "Ba Sing Se", c08, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("09", "Pharloom", c09, 3, 0, 0, 0));

	return cabeza;
}

/*
* Funcion que incrementa una estadística (A, B o C) del territorio con el código dado.
* Si la estadística ya es 3, se incrementan en 1 las otras dos, sin pasarse de 3.
*/
int aumentar_estadistica(Territorio *cabeza, const char *codigo, char estadistica)
{
	// Buscar el territorio por su código
	// strcmp compara dos strings, si son iguales devuelve 0, si no son devuelve otro numero
	Territorio *actual = cabeza;
	while (strcmp(actual->codigo, codigo) != 0)
	{
		actual = actual->siguiente;
	}

	if (actual)
	{
		int *objetivo = NULL;
		int *otra1 = NULL;
		int *otra2 = NULL;

		if (estadistica == 'A')
		{
			objetivo = &actual->A;
			otra1 = &actual->B;
			otra2 = &actual->C;
		}
		else if (estadistica == 'B')
		{
			objetivo = &actual->B;
			otra1 = &actual->A;
			otra2 = &actual->C;
		}
		else
		{
			objetivo = &actual->C;
			otra1 = &actual->A;
			otra2 = &actual->B;
		}
		// Aumentar estadistica
		if (*objetivo < 3)
		{
			*objetivo = limitar_a_rango_0_a_3(*objetivo + 1);
		}
		else
		{
			*otra1 = limitar_a_rango_0_a_3(*otra1 + 1);
			*otra2 = limitar_a_rango_0_a_3(*otra2 + 1);
		}
	}
	return 0;
}

// Incrementa A/B/C del territorio dado; si esa estadística ya es 3, en lugar de subir otras del mismo territorio,
// aumenta esa MISMA estadística en todos los territorios vecinos (según su lista de conexiones).
void aumentar_estadistica_vecinos(Territorio *cabeza, char *codigo, char estadistica)
{
	// localizar el territorio base por codigo (string de 2 chars)
	Territorio *actual = cabeza;
	while (strcmp(actual->codigo, codigo) != 0)
	{
		actual = actual->siguiente;
	}

	int *objetivo = NULL;

	if (estadistica == 'A')
	{
		objetivo = &actual->A;
	}
	else if (estadistica == 'B')
	{
		objetivo = &actual->B;
	}
	else
	{
		objetivo = &actual->C;
	}

	// Si la estadística del territorio aún no está al máximo, simplemente súbela en el propio territorio
	if (*objetivo < 3)
	{
		*objetivo = *objetivo + 1;
		return;
	}

	// Si ya está en 3, propagar a todos los vecinos la misma estadística
	for (int i = 0; i < actual->cantidad_conexiones; i++)
	{
		const char *codigoVecino = actual->conexiones[i];

		// buscar vecino por codigo recorriendo la lista
		Territorio *vec = cabeza;
		while (strcmp(vec->codigo, codigoVecino) != 0)
		{
			vec = vec->siguiente;
		}
		int *dest = NULL;
		if (estadistica == 'A')
		{
			dest = &vec->A;
		}
		else if (estadistica == 'B')
		{
			dest = &vec->B;
		}
		else
		{
			dest = &vec->C;
		}
		if (*dest < 3)
		{
			*dest = *dest + 1;
			return;
		}
		if (comprobar_tres_todos(cabeza, estadistica) == 1)
		{
			return;
		}
		aumentar_estadistica_vecinos(cabeza, codigoVecino, estadistica);
	}
}

void eliminarTerritorio(Territorio *cabeza, char *codigo)
{
	Territorio *actual = cabeza;
	while (actual)
	{
		if (strcmp(actual->codigo, codigo) == 0)
		{ // strcmp retorna 0 si se encuentra el codigo

			if (actual->anterior == NULL)
			{
				actual->anterior->siguiente = actual->siguiente; // Modificar el puntero del nodo anterior
			}
			else
			{
				cabeza = actual->siguiente; // Si es el primer nodo, actualizar la cabeza
			}
			if (actual->siguiente == NULL)
			{
				actual->siguiente->anterior = actual->anterior; // Modificar el puntero del nodo siguiente
			}
			free(actual); // Liberar memoria (funciona independientemente si es el ultimo nodo o no)
			return;
		}
		actual = actual->siguiente;
	}
}

//Funcion usada en menu que permite seleccionar un territorio y estadistica random,
//realiza su respectiva conversion y llama a la funcion aumentar_estadistica
void seleccionar_territorio_estadistica_random(Territorio *cabeza){
	for(int i=0; i<3; i++){
        
        int codigoRandom = rand() % 10 + 1;
        int estadisticaRandom = rand() % 3; // 0=A, 1=B, 2=C

		while(1){
			Territorio* actual = buscarTerritorioPorNumero(codigoRandom, cabeza);
			if(actual != NULL){
				if(estadisticaRandom == 0){
					aumentar_estadistica_vecinos(cabeza, actual->codigo, 'A');

				}else if(estadisticaRandom == 1){
					aumentar_estadistica_vecinos(cabeza, actual->codigo, 'B');

				}else{
					aumentar_estadistica_vecinos(cabeza, actual->codigo, 'C');
				}
				break; //Si el territorio fue encontrado, si no, se repite el proceso
				}
			}
    }

}


