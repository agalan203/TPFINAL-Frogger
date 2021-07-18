/***************************************************************************//**
  @file     rana.h
  @brief    Control de movimientos y de vidas de la rana
  @author   Grupo 1: Cristian Meichtry, Juan Martin Rodriguez
 ******************************************************************************/
#ifndef _RANA_H_
#define _RANA_H_

/*****************************************************************************/
//                               Headers Files                               //
/*****************************************************************************/
#include <stdint.h>
#include <time.h>

/*****************************************************************************/
//                   constantes para los limites del mapa                    //
/*****************************************************************************/
#define CANTCOLS 16
#define CANTFILS 16

/*****************************************************************************/
//                           constantes del modulo                           //
/*****************************************************************************/
#define NDESBR 0 //indica que no hubo desborde
#define DESBR 1 //indica que hubo desborde
#define TITILE 0.03 //frecuencia con la que titila el led de la rana en Raspberry PI

/*****************************************************************************/
//                         Estructura del objeto rana                        //
/*****************************************************************************/
typedef struct{
	uint8_t pos_x; //posicion de la rana en coordenada x
	uint8_t pos_y; //posicion de la rana en coordenada y
	uint8_t desborde; //permite indicar desborde de la rana en el rio
	uint8_t vidas; //cantidad de vidas de la rana
	uint8_t llegadas; //cantidas de llegas a la meta de la rana
	uint8_t timeout; //indica si paso el limite de tiempo para sumar puntaje bonus 
	uint8_t on_off; //indica esta del led que representa a la rana en Raspberry PI
	clock_t tiempo; //tiempo de inicio en salida
	clock_t tiempo_res; // tiempo sobrante para sumar puntos extra
	clock_t tiempo_onoff; //frecuencia con la que titila el led de la rana en Raspberry PI
}rana_be_t;	

/*****************************************************************************/
//                      prototipos de funciones globales                     //
/*****************************************************************************/

/*****************************************************************************
 * @brief rana_frogger: Modifica la posicion de la rana dentro del mapa. Si la rana intenta salir
 * 		del mapa, se mantiene la posicion previa, evitando desborde del jugador.
 * 		Si la rana se encuentra fuera del mapa, lo indica en el campo desborde, 
 * 		actualizandolo a DESBR. 
 * @param mov: direccion hacia donde se tiene que mover. 
 * 			     'r' indica derecha.
 * 			     'l' indica izquierda.
 *			     'u' indica arriba.
 *			     'd' indica abajo.
 *			     '0' indica que la rana no se mueve por el jugador.
 * @param tiempo_max: tiempo maximo del nivel en el cual se puede recibir un bonus de puntaje.
 * @return: puntero a rana   
 */
rana_be_t * rana_frogger(char mov, uint8_t tiempo_max);

/*****************************************************************************
 * @brief rana_init: Inicializa los valores de la rana segun corresponda, a su vez puede cambiar
 * 					 los valores de la rana a lo largo del juego (por ejemplo nueva posicion 
 * 					 al cambiar de nivel). 
 * 					 El campo desborde se inicializa en sin desborde (con define NDESBR).
 * @param posx_in: posicion inicial en coordenada x.
 * @param posy_in: posicion inicial en coordenada y.
 * @param vidas_in: asigna la cantidad de vidas iniciales.
 * @param llegadas_in: contador de llegadas de la rana.
 */
void rana_init(uint8_t posx_in,uint8_t posy_in,uint8_t vidas_in,uint8_t llegadas_in);

/*****************************************************************************
 * @brief actualiza_mundo: devuelve el puntero a la rana
 * @return: puntero a rana 
**/
rana_be_t * get_rana(void);
/*****************************************************************************/
#endif // _RANA_H_