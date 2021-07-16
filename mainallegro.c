/***************************************************************************//**
  @file     +mainallegro.c+
  @brief    +programa principal de FROGGER para implementacion con librerias allegro+
  @author   +Grupo 1: Meichtry,Rodriguez,Ruiz,Galan+
 ******************************************************************************/

#include "globalstuff.h"
#include "frontall.h"
#include "backend.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void numTostring (void); //transforma el puntajeactual a puntajestring
uint16_t stringTonum (char * string); //transforma un string de 4 numeros a un entero
void istopscore (void); //evalua si el puntajeactual entra en los top 4 puntajes y los guarda en el file correspondiente
int comparescores (const void * puntaje1, const void * puntaje2); //compara dos puntajes para ordenarlos decrecientemente
void frogbcktofrnt (rana_be_t * prana , rana_t * rana, mapa_t * pmapa); //transforma la estructura rana del backend a la del frontend
void mapbcktofrnt (mapa_t * pmapa, mundo_t * mundo); //transforma el mapa del backend en el del frontend

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
                            istopscore();
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
                    if (rana.vidas == 0){
                        istopscore();
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
                        numTostring();
                        istopscore();
                        exit_game = 1;
                    }
                    numTostring();
                }
                if (estado == LLEGO){ //si llego del otro lado con tiempo del timer sobrante, se le suma como puntos el tiempo extra
                    puntajeactual += (int) prana->tiempo_res;
                    if (puntajeactual>=9999){
                        exit_game = 1;
                    }
                    numTostring();
                    maxfila = 15;
                }

                //veo si paso de nivel
                if (prana->llegadas == 5){ //para pasar de nivel debe completar las 5 casillas de llegada
                    nivel++;
                    if (nivel == 6){ //el nivel maximo es el nivel 5
                        exit_game = 1;
                        istopscore();
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

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

//Transformar puntajeactual a puntajestring
void numTostring (void){
    int aux = puntajeactual;

    puntajestring[0]=(aux/1000)+'0';
    aux = puntajeactual%1000;
    puntajestring[1]=(aux/100)+'0';
    aux = aux%100;
    puntajestring[2]=(aux/10)+'0';
    puntajestring[3]=aux%10+'0';
    puntajestring[4] = '\0';
}

//Transformar un string de 4 numeros a un entero
uint16_t stringTonum (char * string){
    uint16_t var=0, n=1000;
    int i;

    for(i=0;i<4;i++){
        var+=(string[i]-'0')*n;
        n=n/10;
    }
    return var;
}

//se fija si el puntaje obtenido es un topscore y lo coloca en el file topscore donde corresponde si lo es
void istopscore (void){
    int i,j;
    char newscores[5][5];

    FILE * topscores;

    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
    }

    for(i=0;i<4;i++){
        fgets(newscores[i],5,topscores);
    }
    for (j=0;j<5;j++){
        newscores[i][j]=puntajestring[j];
    }

    qsort(newscores[0], 5, sizeof(newscores)/sizeof(newscores[0]), comparescores);

    fclose(topscores);

    //se cierra y vuelve a abrir el archivo para que comienze a escribir desde el principio del file y se sobreescriba lo anterior
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
    }

    for(i=0;i<4;i++){
        fputs(newscores[i],topscores);
    }
    
    fclose(topscores);
}

//funcion para comparar puntajes para qsort
int comparescores (const void * puntaje1, const void * puntaje2){
    char * pp1 = (char *) puntaje1;
    char * pp2 = (char *) puntaje2;
    int i = 0;

    while ((pp1[i]== pp2[i])&& i<5){
        i++;
    }
    return (pp2[i]-pp1[i]);
}

//transforma mapa backend a mapa frontend
void mapbcktofrnt (mapa_t * pmapa, mundo_t * mundo){
    int i,j;
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            (*mundo)[i][j] = (*pmapa)[i][j];
        }
    }
}

//transforma rana backend a rana frontend
void frogbcktofrnt (rana_be_t * prana , rana_t * rana, mapa_t * pmapa){ //esta funcion no modifica: NIVEL
    int i,j;

    rana->coords.x = (uint8_t) prana->pos_x;
    rana->coords.y = (uint8_t) prana->pos_y;
    rana->vidas = (uint8_t) prana->vidas;
    rana->encendida = (uint8_t) prana->on_off;

    for(i = 2,j=0; i <= 14; i= i+3,j++){
        if((*pmapa)[1][i] == OCUPADO){
            rana->llego[j]=1;
        }
        else{
            rana->llego[j]=0;
        }
    }
}
