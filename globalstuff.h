//CONSTANTES, TYPEDEFS, ENUMS GLOBALES

// INCLUDES
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

// STRUCTURES AND ENUMS
typedef uint8_t action_t; //PLAY/PAUSE/UP/DOWN/LEFT/RIGHT/EXITGAME/EXITPROGRAM

typedef uint8_t mundo_t[16][16];

typedef struct {
    uint8_t estado;
    uint8_t coordx;
    uint8_t coordy;
} rana_t;

typedef struct {
    uint8_t obtclsize;
    uint8_t spacebtwobtcl;
    uint8_t sentidofila;
    uint8_t velocidadfila;
} fila_t;

enum player_action {NONE=0, PLAY, EXIT, UP, DOWN, LEFT, RIGHT}; //play-pause y exitgame-exitprgram se infiere por contexto
enum cell_state {STREET=0, CAR, TRUCK, BUS, WATER, LOG, TORTOISE};
