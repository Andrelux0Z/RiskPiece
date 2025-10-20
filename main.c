#include "implementacion_hashmap.h"
#include "generacion_terreno.h"
#include "menu.h"
#include <stdlib.h>
#include <time.h>

int main() {
    // Semilla unica por ejecucion para toda la app
    srand((unsigned)time(NULL) ^ (unsigned)clock());
	
	// Mostrar el menú principal, pasé toda la lógica aquí
	mostrar_menu_principal();
	
	return 0;
}