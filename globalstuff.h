/***************************************************************************//**
  @file     +globalstuff.h+
  @brief    +Constantes, typedefs y otras cosas pertinentes a todo el programa+
  @author   +GRUPO 1+
 ******************************************************************************/

#ifndef _GLOBALSTUFF_H_
#define _GLOBALSTUFF_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "disdrv.h"
#include "joydrv.h"
#include "termlib.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define CANTFILS 16
#define CANTCOLS 16
#define VIVA 1
#define MUERTA 0
#define ENCENDIDA 1
#define APAGADA 0

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t action_t; //PLAY/PAUSE/UP/DOWN/LEFT/RIGHT/EXITGAME/EXITPROGRAM

typedef uint8_t mundo_t[16][16];

typedef uint8_t rpinr_t[5][3];

typedef struct {
    uint8_t estado; //1 si viva, 0 si muerta
    dcoord_t coords;
    uint8_t encendida; //1 si esta encendida, 0 si esta apagada
    uint8_t vidas;
    uint8_t nivel;
    uint8_t llego[5]; //0,1,2,3,4, dependiendo de a que casilla de llegada llego.
} rana_t;

enum player_action {NONE=0, PLAY, EXIT, UP, DOWN, LEFT, RIGHT, TOPSCORES}; //play-pause y exitgame-exitprgram se infiere por contexto
enum cell_state {STREET=0, CAR, TRUCK, WATER, LOG, SAFE, DEAD, HOME};

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

uint16_t puntajeactual = 0;
char puntajestring [5] = "0000";

// ESTO ESTA SOLO PARA PROBAR AHORA. DESPUES VUELA, no son variables globales

rana_t frog = {.estado=VIVA,.coords.x=8,.coords.y=15,.encendida=APAGADA,.vidas=2,.nivel='1',.llego={1,0,1,0,1}};
mundo_t world = {
    {DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
    {DEAD,DEAD,HOME,DEAD,DEAD,HOME,DEAD,DEAD,HOME,DEAD,DEAD,HOME,DEAD,DEAD,HOME,DEAD},
    {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},
    {LOG,LOG,LOG,WATER,WATER,WATER,WATER,WATER,LOG,LOG,WATER,WATER,WATER,WATER,WATER,WATER},
    {WATER,WATER,WATER,WATER,LOG,LOG,LOG,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},
    {WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},
    {WATER,LOG,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER,WATER},
    {WATER,WATER,WATER,WATER,WATER,WATER,WATER,LOG,LOG,LOG,LOG,WATER,WATER,WATER,WATER,WATER},
    {SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE},
    {STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET},
    {STREET,TRUCK,TRUCK,STREET,STREET,STREET,STREET,TRUCK,TRUCK,STREET,STREET,STREET,STREET,STREET,STREET,STREET},
    {STREET,STREET,STREET,STREET,CAR,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET},
    {STREET,STREET,CAR,STREET,STREET,STREET,STREET,STREET,CAR,STREET,STREET,STREET,STREET,STREET,STREET,STREET},
    {STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET},
    {STREET,STREET,STREET,STREET,CAR,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET,STREET},
    {SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE,SAFE}
};

#endif // _GLOBALSTUFF_H_