// INCLUDES
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "disdrv.h"
#include "joydrv.h"
#include "termlib.h"
#include "globalstuff.h"

// FUNCTION PROTOTYPES
int init_raspi (void);

action_t get_input_raspi (void); //devuelve la accion realizada
void output_world_raspi (mundo_t * mundo, rana_t * rana);  //muestra el mundo en un momento dado en el display
void output_initmenu_raspi (void); //muestra el menu de inicio en el display
void output_gamepaused_raspi (void);  //muestra el menu de pausa en el display

// FUNCION DEFINITIONS
int init_raspi (void) {

    disp_init();    //inicializa el display
    disp_clear();   //limpia todo el display
    joy_init();     //inicializo el joystick

    return 0;
}

action_t get_input_raspi (void){ //devuelve la accion realizada, action_t es una estructura que guarda acciones
    
    jcoord_t myCoords;
    action_t accion;

    //primero actualizo las coordenadas medidas
    joy_update();
    //luego las guardo en myCoords
    myCoords = joy_get_coord();

    if (joy_get_switch()== J_PRESS) {
        accion = PLAY;
    }
    else if (joy_get_switch()== J_NOPRESS) {

        if ( (myCoords.y < 64) && (myCoords.y > -64) && (myCoords.x > 0) ){
            accion = RIGHT;
        }
        else if ( (myCoords.y < 64) && (myCoords.y > -64) && (myCoords.x < 0) ){
            accion = LEFT;
        }
        else if ( (myCoords.x < 64) && (myCoords.x > -64) && (myCoords.y > 0) ){
            accion = UP;
        }
        else if ( (myCoords.x < 64) && (myCoords.x > -64) && (myCoords.y < 0) ){
            accion = DOWN;
        }
        else if ( (myCoords.x == 0) && (myCoords.y == 0) ){
            accion = NONE;
        }
    }

    return  accion;
}
