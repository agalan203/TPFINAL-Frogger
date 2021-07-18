/***************************************************************************//**
  @file     +mainfun.h+
  @brief    +Funciones de main para el juego en librerias allegro o raspberry+ 
  @author   +Grupo 1: Meichtry,Rodriguez,Ruiz,Galan+
 ******************************************************************************/

#ifndef _MAINFUN_H_
#define _MAINFUN_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "globalstuff.h"
#include "rana.h"
#include "mapa.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief numTostring: transforma el puntajeactual a puntajestring
 * @param param1 puntajeactual: numero entero a convertir a string.
 * @param param2 puntajestring: string de 5 caracteres (char) donde guardar numero.
 **/
void numTostring (uint16_t puntajeactual,char puntajestring[5]);

/**
 * @brief istopscore:  evalua si el puntajeactual entra en los top 4 puntajes 
 * 		y los guarda en el file correspondiente
 * @param param1 puntajestring: string de 5 caracteres (char) a comparar con los 
 * 		puntajes guardados.
 **/
void istopscore (char puntajestring[5]); 

/**
 * @brief tmapbcktofrnt: transforma el mapa del backend en el del ffrontend. 
 * @param param1 pmapa: puntero a mapa_t. Este apunta al mapa del backend. 
 * @param param2 mundo: puntero a mundo_t. Este apunta al mapa del frontend.
 **/
void mapbcktofrnt (mapa_t * pmapa, mundo_t * mundo); //

/**
 * @brief frogbcktofrnt: Traduce informacion de la rana del backend a la rana del 
 * 		frontend.
 * @param param1 prana: puntero a rana_be_t. Este apunta a la estructura de la 
 * 		rana del backend. 
 * @param param2 rana: puntero a rana_t. Este apunta a la estructua de la rana del 
 * 		frontend.
 * @param param3 pmapa: puntero a mapa_t. Este apunta al mapa del backend.
 **/
void frogbcktofrnt (rana_be_t * prana , rana_t * rana, mapa_t * pmapa); 
/*******************************************************************************
 ******************************************************************************/

#endif // _MAINFUN_H_

