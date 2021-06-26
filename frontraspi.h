/***************************************************************************//**
  @file     +frontraspi.h+
  @brief    +Front End de Frogger para un display matricial en RPI+
  @author   +Grupo 1: Albertina Galan+
 ******************************************************************************/

#ifndef _FRONTRASPI_H_
#define _FRONTRASPI_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "globalstuff.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define THRESHOLD 40

/*******************************************************************************
 * TYPEDEFS AND STRUCTURES
 ******************************************************************************/

typedef uint8_t rpinr_t[5][3];

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief INIT_RASPI: inicializa la RPI
 * @return devuelve 0 si todo bien, -1 si fallo
*/
int init_raspi (void);
/**
 * @brief GET_INPUT_RASPI: analiza que se introdujo por joystick
 * @return devuelve la accion realizada
*/
action_t get_input_raspi (void);
/**
 * @brief OUTPUT_WORLD_RASPI: muestra el mundo en un momento dado por display
 * @param mundo Puntero al mapa del mundo
*/
void output_world_raspi (mundo_t * mundo);
/**
 * @brief OUTPUT_WORLD_RASPI: muestra la rana en un momento dado por display. La rana titila. Debe ser invocada periodicamente con el rana.encendida cambiado
 * @param rana Puntero a la rana (jugador)
*/
void output_frog_raspi (rana_t * rana);
/**
 * @brief OUTPUT_INITMENU_RASPI: muestra el menu de inicio del juego por display
 * @return devuelve la accion a realizar (PLAY AGAIN- EXIT GAME)
*/
action_t output_initmenu_raspi (void);
/**
 * @brief OUTPUT_GAMEPAUSED_RASPI: muestra el menu de pausa por display
 * @return devuelve la accion a realizar (PLAY- EXIT GAME)
*/
action_t output_gamepaused_raspi (void);
/**
 * @brief OUTPUT_TOPSCORES_RASPI: muestra los highscores por display
*/
void output_topscores_raspi (void);
/**
 * @brief OUTPUT_DEAD_RASPI: muestra la imagen de rana muerta por display
*/
void output_dead_raspi (void);
/**
 * @brief OUTPUT_GAMEOVER_RASPI: muestra la imagen de game over y el puntaje obtenido
*/
void output_gameover_raspi (void);
/**
 * @brief OUTPUT_GAMEOVER_RASPI: muestra el numero del nivel y la cant de vidas
 * @param rana Puntero a la rana (jugador)
*/
void output_level_raspi (rana_t * rana);

/*******************************************************************************
 ******************************************************************************/

#endif // _FRONTRASPI_H_