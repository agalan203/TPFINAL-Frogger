/***************************************************************************//**
  @file     rana.h
  @brief    Header del modulo rana. Estas funciones  permiten dar valores 
  		iniciales a la rana y modificar los campos propios de la rana.
 ******************************************************************************/
#ifndef _RANA_H_
#define _RANA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
/*#include "globalstuff.h"*/
#include <stdint.h>
#include <time.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
/*constantes para probar modulo. Estas se encuentran en globalstuff.h*/
#define CANTCOLS 16
#define CANTFILS 16
/*Constantes del modulo*/
#define NDESBR 0
#define DESBR 1
#define TITILE 0.5 
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
/* 
 * Estructura de la rana del backend
 * @campos: campo1 pos_x: posicion de la rana en coordenada x.
 * 	    campo2 pos_y: posicion de la rana en coordenada y.
 * 	    campo3 desborde: permite indicar desborde de la rana en el rio.
 * 	    campo4 vidas: cantidad de vidas de la rana.
 * 	    campo5 llegadas: cantidas de llegas a la meta de la rana.
 * 	    campo6 timeout: indica si paso el tiempo maximo de vida por llegada
 * 	    campo6 tiempo: tiempo de inicio en salida.
 *
 * */
typedef struct{
	 uint8_t pos_x;
	 uint8_t pos_y;
	 uint8_t desborde;
	 uint8_t vidas;
	 uint8_t llegadas;
	 uint8_t timeout;
	 uint8_t on_off;
	 clock_t tiempo;
	 clock_t tiempo_res;
}rana_be_t;	

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief: Modifica la posicion de la rana dentro del mapa. Si la rana intenta salir
 * 		del mapa, se mantiene la posicion previa, evitando desborde del jugador.
 * 		Si la rana se encuentra fuera del mapa, lo indica en el campo desborde, 
 * 		actualizandolo a DESBR. 
 * @param: param1 mov: direccion hacia donde se tiene que mover. 
 * 			Donde: mov='r' indica derecha.
 * 			       mov='l' indica izquierda.
 *			       mov='u' indica arriba.
 *			       mov='d' indica abajo.
 *			       mov='0' indica que la rana no se mueve por el jugador.
 * @return: puntero a estructura rana_be_t,   
 */
rana_be_t * rana_frogger(char mov,uint8_t tiempo_max);

/**
 * @brief: Inicializa los valores de la rana segun corresponda, a su vez puede cambiar
 * 		los valores de la rana a lo largo del juego (por ejemplo nueva posicion 
 * 		al cambiar de nivel). 
 * 		El campo desborde se inicializa en sin desborde (con define NDESBR).
 * @param: param1 posx_in: posicion inicial en coordenada x.
 * 	   param2 posy_in: posicion inicial en coordenada y.
 * 	   param3 vidas_in: asigna la cantidad de vidas iniciales.
 * 	   param4 llegadas_in: contador de llegadas de la rana.
 */
void rana_init(uint8_t posx_in,uint8_t posy_in,uint8_t vidas_in,uint8_t llegadas_in);

rana_be_t * get_rana(void);
/*******************************************************************************
 ******************************************************************************/
#endif
