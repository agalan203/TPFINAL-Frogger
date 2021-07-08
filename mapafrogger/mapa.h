/***************************************************************************//**
  @file     mapa.h
  @brief    Header del mapa del juego. 
 ******************************************************************************/
#ifndef _MAPA_H_
#define _MAPA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SIZE 16 //constantes para los lados del mapa
#define MAX_ELEM 4
#define IZQ_A_DER 0
#define DER_A_IZQ 1
#define DIST_MIN 1
//constantes para las distintas velocidades de los vehiculos/troncos//
//indica el tiempo que tarda un objeto en moverse un pixel//
#define MUY_LENTO 1.6
#define LENTO 1.4
#define NORMAL 1.2
#define RAPIDO 1
#define MUY_RAPIDO 0.75

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum cell_state {STREET=0, CAR, TRUCK, BUS, WATER, LOG, SAFE, DEAD, WIN};

typedef u_int8_t mapa_t[SIZE][SIZE];

typedef struct{
    bool direccion; //0 de izquierda a derecha, 1 viceversa
    bool maldad; //0 es un npc bueno(tronco), 1 es un npc malo(auto, camion)
    u_int8_t velocidad;
    u_int8_t size_obj;
    u_int8_t carril;
    u_int8_t objeto;
    u_int8_t cant_obj;
    int8_t pos_inic_objetos[MAX_ELEM]; //contiene la posicion inicial de cada objeto 
    clock_t tiempo_previo;
}carril_t;



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void creacion_mapa(void);
void inicia_mapa(u_int8_t nivel);
mapa_t* actualiza_mundo(void);
/*******************************************************************************
 ******************************************************************************/
#endif // _MAPA_H_