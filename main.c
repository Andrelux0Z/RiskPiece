#include "implementacion_hashmap.h"
#include "generacion_terreno.h"
#include <stddef.h> //lo vamos a usar para poner NULL


int main() {
	Territorio* cabeza = construir_lista_ejemplo();


	valoresProblematicas(cabeza);

	imprimir_tabla(cabeza);

	hashmap* problematicas = hashmap_crear();

	hashmap_insertar(problematicas, "P1", "Contaminación y cambio climático", "Reducir contaminación equilibrando desarrollo y sostenibilidad; malas decisiones aumentan temperatura y causan desastres.", cabeza);

	hashmap_insertar(problematicas, "P2", "Desigualdad social", "Gestionar recursos, empleo y educación entre clases; malas decisiones aumentan pobreza y protestas o colapsan la economía.", cabeza);

	hashmap_insertar(problematicas, "P3", "Corrupción gubernamental", "Transparencia vs enriquecimiento ilícito; la corrupción reduce inversión y genera caos social.", cabeza);

	hashmap_insertar(problematicas, "P4", "Migración y refugiados", "Decidir cómo integrar migrantes, administrar recursos y mantener orden; afecta economía, diversidad y estabilidad.", cabeza);

	hashmap_insertar(problematicas, "P5", "Violencia y crimen organizado", "Contener bandas y crimen; invertir en educación/empleo o usar fuerza con costo social.", cabeza);

	hashmap_eliminar(problematicas);
	liberar_lista(cabeza);
	return 0;
}