/***************************************************************************//**
  @file     +backend.h+
  @brief    +funciones que administran el backend del juego+
  @author   +Grupo 1: Cristian Meichtry, Juan Martin Rodriguez+
 ******************************************************************************/
#ifndef _BACKEND_H_
#define _BACKEND_H_

/*****************************************************************************/
//                               Headers Files                               //
/*****************************************************************************/
#include "rana.h"
#include "mapa.h"

/*****************************************************************************/
//                 constantes para los posiciones iniciales                  //
/*****************************************************************************/
#define POSX_I SIZE/2 //coordenada x inicial
#define POSY_I SIZE-1 //coordenada y inicial

/*****************************************************************************/
//                        constantes el tiempo de bonus                      //
/*****************************************************************************/
#define MAX_TIEMPO 11 //tiempo maximo para conseguir bonus
#define SEG_POR_NIV 2 //segundos que se restan al tiempo maximo para bonus

/*****************************************************************************/
//                  constantes para el desarrollo del juego                  //
/*****************************************************************************/

enum estado_juego {VIVE=0,MUERE,LLEGO,PAUSA};

/*****************************************************************************/
//                      prototipos de funciones globales                     //
/*****************************************************************************/

 /*****************************************************************************
 * @brief juego_rana_init_b: prepara el juego. Crea el mapa, asigna las vidas 
 * 		                       iniciales e inicializa los objetos del primer nivel
 * @param vidas: Cantidad de vidas que va a tener la rana en el juego.
 * @param nivel: nivel inicial
 */
void juego_rana_init_b(uint8_t vidas,uint8_t nivel);

 /*****************************************************************************
 * @brief juego_rana_b: administrar el juego a lo largo del tiempo de juego real
 *                      (no se incluye tiempo de pausa, ni de inicio, ni de fin)
 *                      Actualiza los estados de la rana y del mapa (segun el nivel)
 *                      Y devuelve el estado actual de juego.
 * @param evento: evento proveniente del jugador. Donde:
 *                'r' indica derecha.
 * 		            'l' indica izquierda.
 *		            'u' indica arriba.
 *		            'd' indica abajo.
 *		            '0' indica que la rana no se mueve por el jugador.
 *	 	            'p' indica que el jugador puso en pausa el juego. 
 * @param nivel: nivel que debe correr el juego
 * @param rana: direccion del puntero a la estructura que contiene informacion sobre la rana
 * @param mapa: direccion de puntero a mapa
 * @return Devuelve el estado actual del juego.
 * 		     Si la rana murio, devuelve MUERE.
 * 		     Si la rana llego al otro lado, devuelve LLEGO.
 * 		     Si la rana vive, devuelve VIVE.
 * 		     Si se invoco la funcion con el evento pausa, devuelve PAUSA.
 */
int juego_rana_b(char evento, uint8_t nivel, rana_be_t ** rana, mapa_t ** mapa);

/*****************************************************************************/

#endif // _BACKEND_H_
