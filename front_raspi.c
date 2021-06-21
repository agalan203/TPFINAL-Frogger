// INCLUDES
#include <stdio.h>
#include "globalstuff.h"

// FUNCTION PROTOTYPES
int init_raspi (void);
action_t get_input_raspi (void); //devuelve la accion realizada
void output_world_raspi (mundo_t * mundo);  //muestra el mundo en un momento dado en el display
void output_frog_raspi (rana_t * rana); //muestra la rana parpadeante en el display
void output_initmenu_raspi (void); //muestra el menu de inicio en el display
void output_gamepaused_raspi (void);  //muestra el menu de pausa en el display
void output_dead_raspi (void); //muestra cuando muere la rana en el display

// FUNCION DEFINITIONS

/********************************* INIT RPI **************************************/
int init_raspi (void) {

    disp_init();    //inicializa el display
    disp_clear();   //limpia todo el display
    joy_init();     //inicializo el joystick

    return 0;
}

/********************************* GET INPUT **************************************/
action_t get_input_raspi (void){ //devuelve la accion realizada, action_t es una estructura que guarda acciones
    
    jcoord_t myCoords;
    action_t accion = NONE;

    //primero actualizo las coordenadas medidas
    joy_update();
    //luego las guardo en myCoords
    myCoords = joy_get_coord();

    if (joy_get_switch()== J_PRESS) {
        accion = PLAY;
    }
    else if (joy_get_switch()== J_NOPRESS) {

        if ( (myCoords.y < 64) && (myCoords.y > -64) && (myCoords.x > 64) ){
            accion = RIGHT;
        }
        else if ( (myCoords.y < 64) && (myCoords.y > -64) && (myCoords.x < -64) ){
            accion = LEFT;
        }
        else if ( (myCoords.x < 64) && (myCoords.x > -64) && (myCoords.y > 64) ){
            accion = UP;
        }
        else if ( (myCoords.x < 64) && (myCoords.x > -64) && (myCoords.y < -64) ){
            accion = DOWN;
        }
        else {
            accion = NONE;
        }
    }

    return  accion;
}

/********************************* OUTPUT WORLD **************************************/
void output_world_raspi (mundo_t * mundo){  //muestra el mundo en un momento dado en el display
    
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            switch ((*mundo)[i][j]){
                case STREET:
                case WATER:
                    disp_write(myPoint, D_OFF);
                    break;
                case CAR:
                case TRUCK:
                case SAFE:
                case LOG:
                case TORTOISE:
                case DEAD:
                    disp_write(myPoint, D_ON);
                    break;
            }
        }
    }
    disp_update();
}

/********************************* OUTPUT FROG **************************************/
void output_frog_raspi (rana_t * rana){ //se debe invocar cada una cantidad establecida de tiempo, menor que la de actualizacion del display
    if (rana->estado == VIVA){
        if (rana->encendida == ENCENDIDA){
            disp_write(rana->coords, D_OFF);
        }
        else {
            disp_write(rana->coords, D_ON);
        }
        disp_update();
    }
    else {
        output_dead_raspi();
    }
}

/********************************* MENU PAUSA **************************************/
void output_gamepaused_raspi (void){  //muestra el menu de pausa en el display

    mundo_t gamepausedrpi = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,1,0,0,1,1,1,0,1,0,1,0,0},
        {0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,0},
        {0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,0},
        {0,1,0,0,1,1,0,1,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,1,0,0,1,0,1,0,1,1,1,0,0},
        {0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,1,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,1,0,1,0,0,1,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if (gamepausedrpi[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if (gamepausedrpi[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();
}

/********************************* MENU INICIO **************************************/
void output_initmenu_raspi (void){ //muestra el menu de inicio en el display

    mundo_t initmenurpi = {
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1},
        {1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1},
        {1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1},
        {1,0,0,0,1,0,0,1,1,0,1,1,0,0,0,1},
        {1,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    };
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if (initmenurpi[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if (initmenurpi[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();
}

/********************************* DEAD FROG **************************************/
void output_dead_raspi (void){
    mundo_t deadfrog = {
        {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,0,1,1,0,1,1,1,0,1,1,0,0},
        {0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0},
        {0,1,1,0,0,1,1,0,1,0,1,0,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0},
        {0,0,0,0,0,0,1,1,0,1,1,1,1,1,0,0},
        {0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0}
    };
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if (deadfrog[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if (deadfrog[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();
}