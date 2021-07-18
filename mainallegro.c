/***************************************************************************//**
  @file     +mainallegro.c+
  @brief    +programa principal de FROGGER para implementacion con librerias allegro+
  @author   +Grupo 1: Meichtry,Rodriguez,Ruiz,Galan+
 ******************************************************************************/


#include "frontall.h"
#include "backend.h"
#include "mainfun.h"


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

uint16_t puntajeactual = 0; //puntaje numerico actual
char puntajestring [5] = "0000"; //puntaje actual en formato string

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
	mapa_t * pmapa = get_mapa(); //puntero al mapa del backend
    mundo_t mundo; //mapa del frontend
    rana_t rana; //rana del frontend
    rana_t auxfrog; //rana auxiliar para guardar las coordenadas anteriores y mostrar cuando muere

    //flags
    int exit_game = 0;
    int exit_prgm = 0;
    int strt_game = 0;
    
    //Inicializacion para la implementacion de librerias allegro

    //Creacion de variables para la utilizacion de allegro
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_BITMAP * background;
    ALLEGRO_BITMAP * automovil1;
    ALLEGRO_BITMAP * automovil2;
    ALLEGRO_BITMAP * camion;
    ALLEGRO_BITMAP * log1;
    ALLEGRO_BITMAP * log2;
    ALLEGRO_BITMAP * log3;
    ALLEGRO_BITMAP * log4;
    ALLEGRO_BITMAP * ranita;
    ALLEGRO_BITMAP * ranamuerta; 
    ALLEGRO_BITMAP * lives;
    ALLEGRO_BITMAP * llego;

    //inicializacion de allegro
    init_allegro(&event_queue,&display);

    background = al_load_bitmap ("all_images/frogger_bck.png");
    if(!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        return -1;
    }

    automovil1 = al_load_bitmap ("all_images/car3.png");
    if(!automovil1)
    {
        fprintf(stderr, "failed to load automovil1 bitmap!\n");
        al_destroy_bitmap(background);
        return -1;
    }

    automovil2 = al_load_bitmap ("all_images/car4.png");
    if(!automovil2)
    {
        fprintf(stderr, "failed to load automovil2 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        return -1;
    }

    camion = al_load_bitmap ("all_images/truck1.png");
    if(!camion)
    {
        fprintf(stderr, "failed to load truck bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        return -1;
    }

    log1 = al_load_bitmap ("all_images/log1.png");
    if(!log1)
    {
        fprintf(stderr, "failed to load log1 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        return -1;
    }

    log2 = al_load_bitmap ("all_images/log2.png");
    if(!log2)
    {
        fprintf(stderr, "failed to load log2 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log1);
        return -1;
    }

    log3 = al_load_bitmap ("all_images/log3.png");
    if(!log3)
    {
        fprintf(stderr, "failed to load log3 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log1);
        al_destroy_bitmap(log2);
        return -1;
    }

    log4 = al_load_bitmap ("all_images/log4.png");
    if(!log4)
    {
        fprintf(stderr, "failed to load log4 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log1);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        return -1;
    }

    ranita = al_load_bitmap ("all_images/frog.png");
    if(!ranita)
    {
        fprintf(stderr, "failed to load ranita bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log1);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        return -1;
    }

    ranamuerta = al_load_bitmap ("all_images/deadfrog.png");
    if(!ranamuerta)
    {
        fprintf(stderr, "failed to load ranamuerta bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log1);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_bitmap(ranita);
        return -1;
    }

    lives = al_load_bitmap ("all_images/lives.png");
    if(!lives)
    {
        fprintf(stderr, "failed to load lives bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log1);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_bitmap(ranita);
        al_destroy_bitmap(ranamuerta);
        return -1;
    }

    llego = al_load_bitmap ("all_images/llego.png");
    if(!llego)
    {
        fprintf(stderr, "failed to load lives bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log1);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_bitmap(ranita);
        al_destroy_bitmap(ranamuerta);
        al_destroy_bitmap(lives);
        return -1;
    }
    //Finaliza la inicializacion de allegro

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
        accion = output_initmenu_all(event_queue,&ev); 
        //acciones posibles que se pudieron haber dado en el menu de inicio
        switch (accion){
            case TOPSCORES:
                output_topscores_all(event_queue,&ev); //muestra los 4 highscores
                break;
            case EXIT:
                exit_prgm = 1; //indica que se debe cerrar el programa entero
                break;
            case PLAY:
                strt_game = 1; //indica que se debe comenzar el juego
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
            output_world_all (&rana, &mundo,background,automovil1,automovil2,camion,log1,log2,log3,log4,ranita,ranamuerta,lives,llego);

            //incio el loop del juego
            while (!exit_game){

                //obtengo las coordenadas de la rana en el instante anterior a actualizarlas
                frogbcktofrnt (prana,&auxfrog,pmapa);

                //pido e interpreto una entrada (traduce las acciones del front para ser utilizadas por el back)
                accion = get_input_all(event_queue,&ev);
                switch (accion){
                    case NONE:
                        evento = '0';
                        break;
                    case PLAY:
                        evento = 'p';
                        estado=juego_rana_b(evento,nivel,&prana,&pmapa);
                        accion = output_gamepaused_all (event_queue,&ev); //si se puso pausa muestro el menu de pausa y veo que se eligio hacer
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
                    auxfrog.estado = MUERTA; //si fue atropellada lo muestro
                    output_world_all (&auxfrog, &mundo,background,automovil1,automovil2,camion,log1,log2,log3,log4,ranita,ranamuerta,lives,llego);
                    al_flush_event_queue(event_queue);
                    if (rana.vidas == 0){
                        istopscore(puntajestring);
                        exit_game = 1; //si perdio todas las vidas lo indico para mostrar gameover y salir
                    }
                }
                else if (estado == VIVE){
                    rana.estado = VIVA;
                }
                rana.nivel = (uint8_t) nivel + '0';
                mapbcktofrnt (pmapa, &mundo);
                //el mundo se actualiza solo si no se pidio salir
                if (!exit_game){
                    output_world_all (&rana, &mundo,background,automovil1,automovil2,camion,log1,log2,log3,log4,ranita,ranamuerta,lives,llego);
                }

                //actualizo el puntaje
                fila = rana.coords.y;
                if (fila < maxfila){ //se evalua si se encuentra en la mayor fila alcanzada
                    maxfila = fila;
                }
                if ((rana.estado == VIVA) && (rana.coords.y == maxfila) && (evento == 'u')){ //se fija si se debe sumar puntos
                    puntajeactual += 10; //por cada fila que avanza se suman 10 puntos
                    if (puntajeactual>=9999){ //es el puntaje maximo obtenible, ganador absoluto del juego
                        puntajeactual = 9999;
                        numTostring(puntajeactual,puntajestring);
                        istopscore(puntajestring);
                        exit_game = 1;
                    }
                    numTostring(puntajeactual,puntajestring);
                }
                if (estado == LLEGO){ //si llego del otro lado con tiempo del timer sobrante, se le suma como puntos el tiempo extra
                    puntajeactual += (int) prana->tiempo_res;
                    if (puntajeactual>=9999){
                        exit_game = 1;
                    }
                    numTostring(puntajeactual,puntajestring);
                    maxfila = 15;
                }

                //veo si paso de nivel
                if (prana->llegadas == 5){ //para pasar de nivel debe completar las 5 casillas de llegada
                    nivel++;
                    if (nivel == 6){ //el nivel maximo es el nivel 5
                        exit_game = 1;
                        istopscore(puntajestring);
                    }           
                }
                //si perdio muestra gameover
                if (exit_game && (accion!= EXIT)){
                    output_gameover_all(event_queue,&ev);
                }
            }
        }
    } while (!exit_prgm); //el loop se ejecuta hasta que se pida cerrar el programa entero

    //destruye todo lo utilizado de allegro
    al_destroy_bitmap(background);
    al_destroy_bitmap(automovil1);
    al_destroy_bitmap(automovil2);
    al_destroy_bitmap(camion);
    al_destroy_bitmap(log1);
    al_destroy_bitmap(log2);
    al_destroy_bitmap(log3);
    al_destroy_bitmap(log4);
    al_destroy_bitmap(ranita);
    al_destroy_bitmap(ranamuerta);
    al_destroy_bitmap(llego);
    al_destroy_bitmap(lives);
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_destroy_display(display);

    return 0;
}


