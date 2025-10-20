#include "generacion_terreno.h"
#include "implementacion_hashmap.h"
#include "jugadores.h"
#include "comprobaciones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// La definición de Territorio está en generacion_terreno.h; no duplicar aquí.

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
Territorio *crear_territorio(const char *codigo, const char *nombre, const char conexiones[][3], const char problematicas[][3], int cantidad_conexiones, int A, int B, int C)
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

	// Copiar las claves de problematicas (A,B,C) al territorio
	for (int i = 0; i < 3; i++)
	{
		// cada key es de max 2 chars + nul
		strncpy(t->problematicas[i], problematicas[i], sizeof(t->problematicas[i]) - 1);
		t->problematicas[i][sizeof(t->problematicas[i]) - 1] = '\0';
	}
	// limpiar la cuarta posicion por si se usa en algun momento
	t->problematicas[3][0] = '\0';

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
	Territorio *actual = cabeza;

	for (int i = 0; i < n; i++)
	{
		indices[i] = i; // Llenar el arreglo con índices
	}

	barajar(indices, n); // Barajar los índices (Fisher-Yates shuffle)
	actual = cabeza;

	for (int i = 0; i < n && actual; i++)
	{
		// Guardar un indice barajado en el segundo caracter (temporalmente)
		actual->codigo[1] = (char)indices[i];
		actual = actual->siguiente;
	}

	actual = cabeza;

	while (actual)
	{
		// codigo[1] contiene un indice 0..8 temporal
		if ((unsigned char)actual->codigo[1] < 3)
		{
			// Caso 1: Valores de 1 en todos los aspectos
			actual->A = 1;
			actual->B = 1;
			actual->C = 1;
		}
		else if ((unsigned char)actual->codigo[1] < 6)
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

	for (int i = 0; i < n && actual; i++)
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

	const char c01[][3] = {"02", "09", "04"};
	const char c02[][3] = {"01", "03"};
	const char c03[][3] = {"02", "04", "06"};
	const char c04[][3] = {"03", "05", "01"};
	const char c05[][3] = {"04", "06"};
	const char c06[][3] = {"05", "07", "03"};
	const char c07[][3] = {"06", "08", "09"};
	const char c08[][3] = {"07", "09"};
	const char c09[][3] = {"08", "01", "07"};

	const char p01[][3] = {"P2", "P3", "12"};
	const char p02[][3] = {"P4", "11", "13"};
	const char p03[][3] = {"14", "15", "16"};
	const char p04[][3] = {"P1", "12", "P6"};
	const char p05[][3] = {"13", "P6", "15"};
	const char p06[][3] = {"P7", "13", "10"};
	const char p07[][3] = {"P5", "P6", "11"};
	const char p08[][3] = {"P7", "P8", "P9"};
	const char p09[][3] = {"10", "P4", "P7"};

	agregar_territorio(&cabeza, crear_territorio("01", "Dressrosa", c01, p01, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("02", "Wano", c02, p02, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("03", "Punk Hazard", c03, p03, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("04", "Alabasta", c04, p04, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("05", "Pisos Picados", c05, p05, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("06", "Skypea", c06, p06, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("07", "Somalia", c07, p07, 3, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("08", "Oceania", c08, p08, 2, 0, 0, 0));
	agregar_territorio(&cabeza, crear_territorio("09", "Pharloom", c09, p09, 3, 0, 0, 0));

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
	while (actual && strcmp(actual->codigo, codigo) != 0)
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
		else if (estadistica == 'C')
		{
			objetivo = &actual->C;
			otra1 = &actual->A;
			otra2 = &actual->B;
		}
		else
		{
			return -1; // estadistica invalida
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
		return 0;
	}
	return -1; // no encontrado
}

// Incrementa A/B/C del territorio dado; si esa estadística ya es 3, en lugar de subir otras del mismo territorio,
// aumenta esa MISMA estadística en todos los territorios vecinos (según su lista de conexiones).
void aumentar_estadistica_vecinos(Territorio *cabeza, const char *codigo, char estadistica)
{
	// localizar el territorio base por codigo (string de 2 chars)
	Territorio *actual = cabeza;
	while (actual && strcmp(actual->codigo, codigo) != 0)
	{
		actual = actual->siguiente;
	}
	if (!actual)
		return;

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

	// Si la estadística del territorio aún no está al máximo, simplemente se sube en 1
	if (*objetivo < 3)
	{
		*objetivo = *objetivo + 1;
		return;
	}

	// Si ya está en 3, se propaga al territorio anterior y siguiente en la lista
	if (actual->anterior)
	{
		int *cambio = NULL; // puntero al cambio
		if (estadistica == 'A') {
			cambio = &actual->anterior->A;
		} else if (estadistica == 'B') {
			cambio = &actual->anterior->B;
		} else if (estadistica == 'C') {
			cambio = &actual->anterior->C;
		}

		if (cambio && *cambio < 3)
		{
			*cambio = limitar_a_rango_0_a_3(*cambio + 1);
		}
	}
	
	if (actual->siguiente)
	{
		int *cambio = NULL;
		if (estadistica == 'A') {
			cambio = &actual->siguiente->A;
		} else if (estadistica == 'B') {
			cambio = &actual->siguiente->B;
		} else if (estadistica == 'C') {
			cambio = &actual->siguiente->C;
		}

		if (cambio && *cambio < 3)
		{
			*cambio = limitar_a_rango_0_a_3(*cambio + 1);
		}
	}
}

void eliminarTerritorio(Territorio **cabeza, const char *codigo)
{
	if (!cabeza || !*cabeza)
		return;
	Territorio *actual = *cabeza;
	while (actual)
	{
		if (strcmp(actual->codigo, codigo) == 0)
		{
			// Reconectar vecino anterior y siguiente
			if (actual->anterior)
			{
				actual->anterior->siguiente = actual->siguiente;
			}
			else
			{
				// Eliminando la cabeza
				*cabeza = actual->siguiente;
			}
			if (actual->siguiente)
			{
				actual->siguiente->anterior = actual->anterior;
			}
			// actualizar el puntero anterior del siguiente nodo
			if (actual->siguiente && actual->siguiente->anterior == actual)
			{
				actual->siguiente->anterior = actual->anterior;
			}
			free(actual);
			return;
		}
		actual = actual->siguiente;
	}
}

// Funcion usada en menu que permite seleccionar un territorio y estadistica random,
// realiza su respectiva conversion y llama a la funcion aumentar_estadistica
void seleccionar_territorio_estadistica_random(Territorio *cabeza)
{
	if (!cabeza)
		return;

	// Contar territorios disponibles
	int total_territorios = 0;
	Territorio *temp = cabeza;
	while (temp != NULL)
	{
		total_territorios++;
		temp = temp->siguiente;
	}

	if (total_territorios == 0)
		return;

	for (int i = 0; i < 3; i++)
	{
		int estadisticaRandom = rand() % 3; // 0=A, 1=B, 2=C

		// Seleccionar territorio aleatorio de los disponibles
		int indice_random = rand() % total_territorios;
		Territorio *actual = cabeza;

		for (int j = 0; j < indice_random && actual != NULL; j++)
		{
			actual = actual->siguiente;
		}

		if (actual != NULL && actual->codigo)
		{
			if (estadisticaRandom == 0)
			{
				aumentar_estadistica_vecinos(cabeza, actual->codigo, 'A');
			}
			else if (estadisticaRandom == 1)
			{
				aumentar_estadistica_vecinos(cabeza, actual->codigo, 'B');
			}
			else
			{
				aumentar_estadistica_vecinos(cabeza, actual->codigo, 'C');
			}
		}
	}
}

// Funcion para encontrar un territorio alternativo para los jugadores cuando se elimina un territorio (y no queden perdidos)
Territorio *encontrar_territorio_alternativo(Territorio *cabeza, const char *codigo_eliminado)
{
	if (!cabeza)
		return NULL;

	Territorio *actual = cabeza;
	Territorio *alternativa = NULL;

	// Buscar el primer territorio que no sea el que se va a eliminar
	while (actual != NULL)
	{
		if (strcmp(actual->codigo, codigo_eliminado) != 0)
		{
			alternativa = actual;
			break;
		}
		actual = actual->siguiente;
	}

	// Si no se encontró alternativa, usar la cabeza (último recurso)
	if (!alternativa && cabeza)
	{
		alternativa = cabeza;
	}

	return alternativa;
}

// Funcion para quitar las conexiones de los territorios vecinos cuando se elimina un territorio
void limpiar_conexiones_territorio(Territorio *cabeza, const char *codigo_eliminado)
{
	if (!cabeza || !codigo_eliminado)
		return;

	Territorio *actual = cabeza;
	while (actual != NULL)
	{
		// saltar el territorio que se va a eliminar
		if (strcmp(actual->codigo, codigo_eliminado) != 0)
		{
			// verificar todas las conexiones y quitar las referencias al territorio eliminado
			int i = 0;
			while (i < actual->cantidad_conexiones)
			{
				if (strcmp(actual->conexiones[i], codigo_eliminado) == 0)
				{
					for (int j = i; j < actual->cantidad_conexiones - 1; j++)
					{
						strcpy(actual->conexiones[j], actual->conexiones[j + 1]);
					}
					actual->conexiones[actual->cantidad_conexiones - 1][0] = '\0';
					actual->cantidad_conexiones--;
				}
				else
				{
					i++;
				}
			}
		}
		actual = actual->siguiente;
	}
}
