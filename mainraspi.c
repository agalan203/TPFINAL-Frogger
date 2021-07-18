/***************************************************************************//**
  @file     +mainraspi.c+
  @brief    +programa principal de FROGGER para implementacion en rpi+
  @author   +Grupo 1: Meichtry,Rodriguez,Ruiz,Galan+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "mainfun.h"
#include "frontraspi.h"
#include "backend.h"

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

uint16_t puntajeactual = 0;     //puntaje numerico actual
char puntajestring [5] = "0000";    //puntaje actual en formato string

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int main (void){
    //variables locales al main
    int nivel = 1;
    int estado;
    int fila = 0;
    int maxfila = 15;
	char evento = '0';
    action_t accion;
	rana_be_t * prana = get_rana(); //puntero a la rana del backend
	mapa_t * pmapa = get_mapa();    //puntero al mapa del backend
    mundo_t mundo;      //mapa del frontend
    rana_t rana;        //rana del frontend

    //flags
    int exit_game = 0;
    int exit_prgm = 0;
    int strt_game = 0;

    //incializo la rpi
    init_raspi();

    //inicio el loop del programa
    do{
        //resetear las variables
        nivel = 1;
        evento = '0';
        fila = 0;
        maxfila = 15;
        prana = get_rana();
        pmapa = get_mapa();
        exit_game = 0;
        strt_game = 0;
        puntajeactual = 0;

        //muestro el menu de inicio
        accion = output_initmenu_raspi();
        //acciones posibles que se pudieron haber dado en el menu de inicio
        switch (accion){
            case TOPSCORES:
                output_topscores_raspi();   //muestra los 4 highscores
                break;
            case EXIT:
                exit_prgm = 1;              //indica que se debe cerrar el programa entero
                break;
            case PLAY:
                strt_game = 1;              //indica que se debe comenzar el juego
                break;
        }

        //inicio del juego
        if (strt_game){

            //incializo el backend
            juego_rana_init_b(3,nivel); 
            frogbcktofrnt (prana ,&rana, pmapa);
            rana.estado = VIVA;
            rana.nivel = nivel;
            mapbcktofrnt (pmapa, &mundo);
            //reseteo el puntaje actual
            puntajeactual = 0;
            numTostring(puntajeactual,puntajestring);
            //muestro el mundo inicial en el display
            output_world_raspi(&mundo,&rana);
            

            //incio el loop del juego
            while (!exit_game){

                //pido e interpreto una entrada (traduce las acciones del front para ser utilizadas por el back)
                accion = get_input_raspi();
                switch (accion){
                    case NONE:
                        evento = '0';
                        break;
                    case PLAY:
                        evento = 'p';
                        estado=juego_rana_b(evento,nivel,&prana,&pmapa);
                        accion = output_gamepaused_raspi ();    //si se puso pausa muestro el menu de pausa y veo que se eligio hacer
                        if (accion == EXIT){
                            istopscore(puntajestring);
                            exit_game = 1;
                        }
                        else {
                            evento = '0';
                        }  
                        break;
                    case UP:
                        evento = 'u';
                        break;
                    case DOWN:
                        evento = 'd';
                        break;
                    case LEFT:
                        evento = 'l';
                        break;
                    case RIGHT:
                        evento = 'r';
                        break;
                }

                //actualizo y muestro el mundo
                estado=juego_rana_b(evento,nivel,&prana,&pmapa);
                frogbcktofrnt (prana ,&rana, pmapa);
                //evaluo el estado de la rana
                if (estado == MUERE){
                    rana.estado = MUERTA;
                    output_dead_raspi();    //si fue atropellada lo muestro
                    if (rana.vidas == 0){
                        istopscore(puntajestring);
                        exit_game = 1;      //si perdio todas las vidas lo indico para mostrar gameover y salir
                    }
                }
                else if (estado == VIVE){
                    rana.estado = VIVA;
                }
                rana.nivel = (uint8_t) nivel + '0';
                mapbcktofrnt (pmapa, &mundo);
                //el mundo solo se actualiza y muestra si la rana esta viva y no se pidio salir del juego
                if((estado == VIVE) && (!exit_game)){
                    output_world_raspi(&mundo,&rana);
                }

                //actualizo el puntaje
                fila = rana.coords.y;
                if (fila < maxfila){    //se evalua si se encuentra en la mayor fila alcanzada
                    maxfila = fila;
                }
                if ((rana.estado == VIVA) && (rana.coords.y == maxfila) && (evento == 'u')){    //se fija si se debe sumar puntos
                    puntajeactual += 10;        //por cada fila que avanza se suman 10 puntos
                    if (puntajeactual>=9999){   //es el puntaje maximo obtenible, ganador absoluto del juego
                        puntajeactual = 9999;
                        numTostring(puntajeactual,puntajestring);
                        istopscore(puntajestring);
                        exit_game = 1;
                    }
                    numTostring(puntajeactual,puntajestring);
                }
                if (estado == LLEGO){       //si llego del otro lado con tiempo del timer sobrante, se le suma como puntos el tiempo extra
                    puntajeactual += (int) prana->tiempo_res;
                    if (puntajeactual>=9999){
                        exit_game = 1;
                    }
                    numTostring(puntajeactual,puntajestring);
                    maxfila = 15;
                }

                //veo si paso de nivel
                if (prana->llegadas == 5){  //para pasar de nivel debe completar las 5 casillas de llegada
                    nivel++;
                    if (nivel < 6){
                        rana.nivel++;
                        output_level_raspi(&rana);
                    }
                    if (nivel == 6){    //el nivel maximo es el nivel 5
                        exit_game = 1;
                        istopscore(puntajestring);
                    }           
                }
                //si se pidio salir, muestra gameover y el puntaje obtenido en la partida
                if(exit_game){
                    output_gameover_raspi();
                }
            }
        }
    } while (!exit_prgm);   //el loop se ejecuta hasta que se pida cerrar el programa entero

    //limpia y apaga el display de la raspi
    disp_clear();
    disp_update();

    return 0;
}

