#include "implementacion_hashmap.h"
#include "generacion_terreno.h"

int main() {
	Territorio* cabeza = construir_lista_ejemplo();

	imprimir_tabla(cabeza);
	liberar_lista(cabeza);
	return 0;
}