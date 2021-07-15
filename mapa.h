/****************************************************************************
  @file     mapa.h
  @brief    Header del mapa del juego. 
*****************************************************************************/
#ifndef _MAPA_H_
#define _MAPA_H_

/*****************************************************************************/
//                               Headers FIles                               //
/*****************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

/*****************************************************************************/
//                     constante para los lados del mapa                     //
/*****************************************************************************/
#define SIZE 16 //constante para los lados del mapa
#define MAX_ELEM 4
/*****************************************************************************/
//                constantes para los sentidos de los carriles               //
/*****************************************************************************/
#define IZQ_A_DER 0 
#define DER_A_IZQ 1

/*****************************************************************************/
//    constantes para las distintas velocidades de los vehiculos/troncos     //
//         indican el tiempo que tarda un objeto en moverse un pixel         //
/*****************************************************************************/
#define MUY_LENTO 0.35
#define LENTO 0.3
#define NORMAL 0.25
#define RAPIDO 0.2
#define MUY_RAPIDO 0.15

/*****************************************************************************/
//                 constantes los distintos objetos del mapa                 //
/*****************************************************************************/
enum cell_state {STREET=0, CAR, TRUCK, WATER, LOG, SAFE, DEAD, WIN, OCUPADO};

/*****************************************************************************/
//   definicion del tipo de dato del mapa y de la estructura de cada carril  //
/*****************************************************************************/

typedef u_int8_t mapa_t[SIZE][SIZE];

typedef struct{
    bool direccion; //0 de izquierda a derecha, 1 viceversa
    bool maldad;
    u_int8_t velocidad; //velocidad a la que se mueven los objetos
    u_int8_t size_obj; //size del objeto que esta en el carril
    u_int8_t carril; //numero de carril
    u_int8_t objeto; //que objeto es: tronco/auto/camion
    u_int8_t cant_obj; //cantidad de objetos en el carril
    int8_t pos_inic_objetos[MAX_ELEM]; //coordenada inicial de cada objeto en el carril
    clock_t tiempo_previo;
    uint8_t act_prev;
    double tm_cell; //tiempo que espera en la celda para luego pasar a la siguiente
}carril_t;

/*****************************************************************************/
//                      prototipos de funciones publicas                     //
/*****************************************************************************/

/*****************************************************************************
 * @brief creacion_mapa: Crea un mapa de 16x16 con agua, calle y zona segura, pero sin vehiculos
 * ni obstaculos 
**/
void creacion_mapa(void);
/*****************************************************************************/

/*****************************************************************************
 * @brief inicia_mapa: Debe invocarse al comenzar un nivel. Inicia el mapa del
 * nivel con una configuracion de autos y obstaculos aleatoria
 * @param nivel: El nivel que se va a ejecutar en el momento del llamado 
**/
void inicia_mapa(u_int8_t nivel);
/*****************************************************************************/

/*****************************************************************************
 * @brief actualiza_mundo: actualiza el mapa linea por linea constantemente
 * @return: puntero al mapa 
**/
mapa_t * actualiza_mundo(void);

/*****************************************************************************
 * @brief get_carril: busca un puntero a la estructura de un carril y lo devuelve 
 * @param param1 linea: numero de linea a obtener 
 * @return: puntero a carril
**/
carril_t * get_carril(uint8_t linea);

/*****************************************************************************
 * @brief get_mapa Devuelve el puntero al mapa
 * @return: puntero al mapa
**/
mapa_t * get_mapa(void);

/*****************************************************************************/

#endif // _MAPA_H_
