/***************************************************************************//**
  @file     +frontall.h+
  @brief    +Front End de Frogger para un display grafico con librerias Allegro+
  @author   +Grupo 1: Albertina Galan, Lucia Ruiz+
 ******************************************************************************/

#ifndef _FRONTALL_H_
#define _FRONTALL_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "globalstuff.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define WIDTH 1120    //ancho del display
#define HEIGHT 1190  //alto del display 
#define ITEM 70     //ancho y alto de un 'pixel' de la matriz de 16x16

/*******************************************************************************
 * EXTERNAL VARIABLES
 ******************************************************************************/

extern uint16_t puntajeactual;
extern char puntajestring [5];

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief INIT_ALLEGRO: inicializa allegro
 * @param event_queue Puntero a la cola de eventos
 * @param display Puntero al display
 * @return devuelve 0 si todo bien, -1 si fallo
*/
int init_allegro (ALLEGRO_EVENT_QUEUE ** event_queue, ALLEGRO_DISPLAY ** display);
/**
 * @brief GET_INPUT_ALL: analiza que se introdujo por teclado
 * @param event_queue Puntero a la cola de eventos
 * @param ev Puntero a la variable que guarda los eventos
 * @return devuelve la accion realizada
*/
action_t get_input_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev);
/**
 * @brief OUTPUT_INITMENU_ALL: muestra el menu de inicio por display
 * @param event_queue Puntero a la cola de eventos
 * @param ev Puntero a la variable que guarda los eventos
 * @return devuelve la accion a realizar (PLAY- EXIT PROGRAM -TOPSCORES )
*/
action_t output_initmenu_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev);
/**
 * @brief OUTPUT_GAMEPAUSED_ALL: muestra el menu de pausa por display
 * @param event_queue Puntero a la cola de eventos
 * @param ev Puntero a la variable que guarda los eventos
 * @return devuelve la accion a realizar (PLAY- EXIT GAME)
*/
action_t output_gamepaused_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev);
/**
 * @brief OUTPUT_TOPSCORES_ALL: muestra los highscores por display
 * @param event_queue Puntero a la cola de eventos
 * @param ev Puntero a la variable que guarda los eventos
 * @return devuelve EXIT cuando elige salir el jugador
*/
action_t output_topscores_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev);
/**
 * @brief OUTPUT_INITMENU_ALL: muestra el menu de inicio del juego por display
 * @param event_queue Puntero a la cola de eventos
 * @param ev Puntero a la variable que guarda los eventos
 * @return devuelve la accion a realizar (EXIT GAME)
*/
action_t output_gameover_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev);
/**
 * @brief OUTPUT_WORLD_ALL: muestra el mundo en un momento dado por display
 * @param rana Puntero a la rana (jugador)
 * @param mundo Puntero al mapa del mundo
 * @return devuelve -1 si fallo algo
*/
int output_world_all (rana_t * rana, mundo_t * mundo, ALLEGRO_BITMAP * background, ALLEGRO_BITMAP * automovil1, ALLEGRO_BITMAP * automovil2, ALLEGRO_BITMAP * camion, ALLEGRO_BITMAP * log1, ALLEGRO_BITMAP * log2, ALLEGRO_BITMAP * log3, ALLEGRO_BITMAP * log4, ALLEGRO_BITMAP * ranita, ALLEGRO_BITMAP * ranamuerta, ALLEGRO_BITMAP * lives, ALLEGRO_BITMAP * llego);

/*******************************************************************************
 ******************************************************************************/

#endif // _FRONTALL_H_
