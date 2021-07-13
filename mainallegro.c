//MAIN ALLEGRO:
/*****************************************************************************/

//INCLUDES
#include "globalstuff.h"
#include "frontall.h"
#include "backend.h"

/*****************************************************************************/

//PROTOTIPOS DE FUNCIONES LOCALES
void numTostring (void);
uint16_t stringTonum (char * string);
void istopscore (void);
int comparescores (const void * puntaje1, const void * puntaje2);
void frogbcktofrnt (rana_be_t * prana , rana_t * rana, mapa_t * pmapa);
void mapbcktofrnt (mapa_t * pmapa, mundo_t * mundo);

/*****************************************************************************/
//VARIABLES GLOBALES
uint16_t puntajeactual = 0;
char puntajestring [5] = "0000";

//MAIN
int main (void){
    //variables
    int nivel = 1;
    int estado;
	char evento = '0';
	rana_be_t * prana = get_rana();
	mapa_t * pmapa = get_mapa();
    mundo_t mundo;
    rana_t rana;
    action_t accion;
    clock_t comienzo = clock();

    //flags
    int exit_game = 0;
    int exit_prgm = 0;
    int strt_game = 0;
    int fila = 0;
    int maxfila = 15;

    //incializo allegro

    //Creacion de variables para la utilizacion de allegro
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_BITMAP * background;
    ALLEGRO_BITMAP * automovil1;
    ALLEGRO_BITMAP * automovil2;
    ALLEGRO_BITMAP * camion;
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

    log2 = al_load_bitmap ("all_images/log2.png");
    if(!log2)
    {
        fprintf(stderr, "failed to load log2 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
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
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_bitmap(ranita);
        al_destroy_bitmap(ranamuerta);
        al_destroy_bitmap(lives);
        return -1;
    }
    //Finaliza la inicializacion de allegro

    //muestro el menu de inicio
    do{
        //resetear las variables
        nivel = 1;
        evento = '0';
        fila = 0;
        maxfila = 15;
        prana = get_rana();
        pmapa = get_mapa();
        comienzo = clock();
        exit_game = 0;
        strt_game = 0;
        puntajeactual = 0;

        //menu de inicio
        accion = output_initmenu_all(event_queue,&ev); 
        switch (accion){
            case TOPSCORES:
                output_topscores_all(event_queue,&ev);
                break;
            case EXIT:
                exit_prgm = 1;
                break;
            case PLAY:
                strt_game = 1;
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
            output_world_all (&rana, &mundo,background,automovil1,automovil2,camion,log2,log3,log4,ranita,ranamuerta,lives,llego);

            while (!exit_game){

                //pido e interpreto una entrada
                accion = get_input_all(event_queue,&ev);
                switch (accion){
                    case NONE:
                        evento = '0';
                        break;
                    case PLAY:
                        evento = 'p';
                        estado=juego_rana_b(evento,nivel,&prana,&pmapa);
                        accion = output_gamepaused_all (event_queue,&ev);
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
                if (estado == MUERE){
                    rana.estado = MUERTA;
                    output_world_all (&rana, &mundo,background,automovil1,automovil2,camion,log2,log3,log4,ranita,ranamuerta,lives,llego);
                    if (rana.vidas == 0){
                        output_gameover_all(event_queue,&ev);
                        istopscore();
                        exit_game = 1;
                    }
                }
                else if (estado == VIVE){
                    rana.estado = VIVA;
                }
                rana.nivel = (uint8_t) nivel + '0';
                mapbcktofrnt (pmapa, &mundo);
                output_world_all (&rana, &mundo,background,automovil1,automovil2,camion,log2,log3,log4,ranita,ranamuerta,lives,llego);

                //actualizo el puntaje
                fila = rana.coords.y;
                if (fila < maxfila){
                    maxfila = fila;
                }
                if ((rana.estado == VIVA) && (rana.coords.y == maxfila) && (evento == 'u')){
                    puntajeactual += 10;
                    if (puntajeactual>=9999){
                        puntajeactual = 9999;
                        istopscore();
                        exit_game = 1;
                    }
                    numTostring();
                }
                if (estado == LLEGO){
                    puntajeactual += (int) prana->tiempo_res;
                    if (puntajeactual>=9999){
                        exit_game = 1;
                    }
                    numTostring();
                }

                //veo si paso de nivel
                if (prana->llegadas == 5){
                    nivel++;
                    if (nivel == 6){
                        exit_game = 1;
                    }           
                }
            }
        }
    } while (!exit_prgm);

    //destruye allegro
    al_destroy_bitmap(background);
    al_destroy_bitmap(automovil1);
    al_destroy_bitmap(automovil2);
    al_destroy_bitmap(camion);
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

/*****************************************************************************/
//DEFINICIONES DE FUNCIONES LOCALES

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

//Transformar de string a numero
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
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
    }

    for(i=0;i<4;i++){
        fputs(newscores[i],topscores);
    }
    
    fclose(topscores);
}

//funcion para comparar puntajes
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
    }
}