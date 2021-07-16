/***************************************************************************//**
  @file     +backend.c+
  @brief    +funciones que administran el backend del juego+
  @author   +Grupo 1: Cristian Meichtry, Juan Martin Rodriguez+
 ******************************************************************************/

#ifndef _BACKEND_H_
#define _BACKEND_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "rana.h"
#include "mapa.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define POSX_I SIZE/2
#define POSY_I SIZE-1
#define MAX_TIEMPO 11
#define SEG_POR_NIV 2
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum estado_juego {VIVE=0,MUERE,LLEGO,PAUSA};

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/*
 * @brief Esta funcion se encarga de preparar el juego. Creando el mapa, dando vidas 
 * 		iniciales e inicializando los objetos del primer nivel.
 * @param param1 vidas: Cantidad de vidas que va a tener la rana en el juego.
 * 	  param2 nivel: Nivel inicial 
 **/
void juego_rana_init_b(uint8_t vidas,uint8_t nivel);

/*
 * @brief Esta funcion se encarga de administrar el juego a lo largo del tiempo de juego.
 * 		Este tiempo de juego no incluye el menu de inicio, menu de pausa, o fin 
 * 		del juego.
 * 		Actualiza los estados de la rana y el mapa, realizando sus respectivas 
 * 		comparaciones.
 * 		Ademas, notifica a donde fue llamada con el estado del juego.
 * @param param1 evento: Se recibe un caracter con el evento proveniente del jugador.
 * 		Donde: evento='r' indica derecha.
 * 		       evento='l' indica izquierda.
 *		       evento='u' indica arriba.
 *		       evento='d' indica abajo.
 *		       evento='0' indica que la rana no se mueve por el jugador.
 *	 	       evento='p' indica que el jugador puso en pausa el juego. 
 * 	  param2 nivel: Se recibe el nivel que debe correr el juego. Si se cambia de 
 * 	  	nivel, la funcion reconoce la variacion y ajusta nuevas configuraciones
 * 	  	a los obejetos del mapa.
 * 	  param3 rana: Direccion de puntero a rana_main. En esta estructura se van 
 * 	  	actualizar la informacion de la rana. Y en este puntero se copia 
 * 	  	la direccion de	dicha estructura, permitiendo que se acceda a la 
 * 	  	informacion en otra parte el programa.
 * 	  param4 mapa: Direccion de puntero a mapa. En este puntero se copia la 
 * 	  direccion del mapa, para poder accederlo desde otra parte del programa.
 * @return Devuelve el estado del juego.
 * 		Si la rana murio, devuelve MUERE.
 * 		Si la rana llego a la casa, devuelve LLEGO.
 * 		Si la rana vive, devuelve VIVE.
 * 		Si se invoco la funcion con el evento pausa, devuelve PAUSA.
 **/
int juego_rana_b(char evento, uint8_t nivel, rana_be_t ** rana, mapa_t ** mapa);

/*******************************************************************************
 ******************************************************************************/

#endif // _BACKEND_H_
