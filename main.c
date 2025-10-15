#include "implementacion_hashmap.h"
#include "generacion_terreno.h"
#include <stddef.h> //lo vamos a usar para poner NULL

int main() {
	Territorio* cabeza = construir_lista_ejemplo();

	imprimir_tabla(cabeza);

	hashmap* problematicas = crear_hashmap();

	agregar_h(problematicas, "P1", "Contaminación y cambio climático", "Reducir contaminación equilibrando desarrollo y sostenibilidad; malas decisiones aumentan temperatura y causan desastres.", paises("Dressrosa", "Wano", "Ba Sing Se", "Punk Hazard", NULL));

	agregar_h(problematicas, "P2", "Desigualdad social", "Gestionar recursos, empleo y educación entre clases; malas decisiones aumentan pobreza y protestas o colapsan la economía.", paises("Ba Sing Se", "Pisos Picados", "Dressrosa", NULL));

	agregar_h(problematicas, "P3", "Corrupción gubernamental", "Transparencia vs enriquecimiento ilícito; la corrupción reduce inversión y genera caos social.", paises("Alabasta", "Somalia", "Dressrosa", NULL));

	agregar_h(problematicas, "P4", "Migración y refugiados", "Decidir cómo integrar migrantes, administrar recursos y mantener orden; afecta economía, diversidad y estabilidad.", paises("Punk Hazard", "Alabasta", "Somalia", "Oceania", NULL));

	agregar_h(problematicas, "P5", "Violencia y crimen organizado", "Contener bandas y crimen; invertir en educación/empleo o usar fuerza con costo social.", paises("Somalia", "Pisos Picados", "Dressrosa", "Wano", NULL));

	eliminar_hashmap(problematicas);
	liberar_lista(cabeza);
	return 0;
}