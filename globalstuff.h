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

#endif // _GLOBALSTUFF_H_