/*
TODO: Nombre colectivo anarquista
1. Colocar jugadores de forma aleatoria al inciar el juego
2. Turno de jugadores
    ->Moverse de territorio
    ->Implementar proyecto -> disminuir estadisticas del territorio
3. Aumentar de forma aleatoria las problematicas de los territorios
     ->Aumentar las estadisticas alrededor de los territorios que tienen 3 o mas en algun aspecto
4. Implementar pirata inicial
    ->Implementar paisos perdidos como nuevos piratas --Pensar lore al respecto a esto
*/
#include "generacion_terreno.h"
#include "jugadores.h"

#include "jugadores.h"
#include <stdlib.h>
#include <time.h>

//Funcion que permite crear los jugadores y los coloca de forma aleatoria en un territorio
void crearJugadoresInicial(Territorio* cabeza){
    srand(time(NULL));
    Territorio* ubicacionONU;
    Territorio* ubicacionAnarquista;

    while(1){ //Territorios disponiples
        int ubicacionAleatoria = rand() % 9 + 1;
        Territorio* ubicacionONU = buscarTerritorioPorCodigo(ubicacionAleatoria,cabeza);
        if(ubicacionONU != NULL){
            break;
        }
    }
    agregarJugador(cabeza,ubicacionONU,"ONU");

    while(1){ //Territorios disponiples
        int ubicacionAleatoria = rand() % 9 + 1;
        Territorio* ubicacionAnarquista = buscarTerritorioPorCodigo(ubicacionAleatoria,cabeza);
        if(ubicacionAnarquista != NULL || ubicacionAnarquista != ubicacionONU){
            break;
        }
    }
    agregarJugador(cabeza,ubicacionAnarquista,"Anarquista");
}


void agregarJugador(jugadorList* jugadorList,Territorio* ubicacion,char* nombre){

    //Crear nuevo jugador
    jugador* nuevoJugador = malloc(sizeof(jugador));
    if(!nuevoJugador){
        printf("Error: memoria insuficiente\n"); 
        exit(EXIT_FAILURE);
    }

    //Si la lista de jugadores esta vacia
    if(jugadorList->inicio == NULL){
        jugadorList->inicio = nuevoJugador;
        jugadorList->final = nuevoJugador;
        return;
    }

    //Si la lista de jugadores no esta vacia
    jugadorList->final->sigt = nuevoJugador;
    nuevoJugador->ant = jugadorList->final;
    jugadorList->final = nuevoJugador;
    nuevoJugador->sigt = NULL;   

    nuevoJugador->ubicacion = ubicacion; //Asignar ubicacion al nuevo jugador
    //Asignar nombre al nuevo jugador
    strncpy(nuevoJugador->nombre, nombre, sizeof(nuevoJugador->nombre) - 1);
    nuevoJugador->nombre[sizeof(nuevoJugador->nombre) - 1] = '\0';
}



/*Funcio que permite colocar a los jugadores de forma aleatoria y son creados en agregarPirata
Utilizar si se necesita crear y asignar territorio de forma aleatoria un pirata (en caso de pirata inicial) 
*/
void piratasInicial(Territorio* cabeza){
    srand(time(NULL));
    Territorio* ubicacion;

    //Numero aleatorio entre 1 y 9
    while(1){
        int ubicacionAleatoria = rand() % 9 + 1;
        Territorio* ubicacion = buscarTerritorioPorCodigo(ubicacionAleatoria,cabeza);
        if(ubicacion != NULL){
            break;
        }
    }
    agregarPirata(cabeza,ubicacion);
}

/*Funcion que permite agregar un pirata a la lista de piratas y asignar su ubicacion
Utilizar cuando muera un territorio y se cree un nuevo pirata en ese territorio (TODO O EL SIGUIENTE)
*/
void agregarPirata(pirataList* pirataList,Territorio* ubicacion){

    pirata* nuevoPirata = malloc(sizeof(pirata));
    if(!nuevoPirata){
		printf("Error: memoria insuficiente\n"); 
		exit(EXIT_FAILURE);
	}

    //Si la lista de piratas esta vacia
    if(pirataList->inicio == NULL){
        pirataList->inicio = nuevoPirata;
        pirataList->final = nuevoPirata;
        return;
    }

    //Si la lista de piratas no esta vacia
    pirataList->final->sigt = nuevoPirata;
    nuevoPirata->ant = pirataList->final;
    pirataList->final = nuevoPirata;
    nuevoPirata->sigt = NULL;   

    nuevoPirata->ubicacion = ubicacion;
}
