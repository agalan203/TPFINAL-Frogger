//includes
#include "globalstuff.h"
#include "frontall.h"
#include "backend/backend.h"
#include "rana/rana.h"
#include "mapafrogger/mapa.h"

//funciones locales
void analizar_estado_rana (rana_t * pRanaf, mapa_t * Pmapa, int x_rana, int y_rana);
void numTostring (void);
uint16_t stringTonum (char * string);
void istopscore (void);
int comparescores (const void * puntaje1, const void * puntaje2);

int main(void){
    //variables del juego
    mundo_t mundo;
    rana_t rana;
    int error;

    //Creacion de variables para la utilizacion de allegro
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;
    action_t action;
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

    action = output_initmenu_all(event_queue,&ev);       //muestro menu de inicio y recibo action(play, exitprogram)
    
    switch(action){
        case PLAY:
            int llego[5] = {0};
            int maxfila = 15;               //fila maxima que llego EMPIEZA EN LA FILA 15 Y TERMINA EN 0
            creacion_mapa();                //crea mapa base del backend
            rana_init(8,15,3,0);            //inicializo rana del backend
            coord_t coord = {rana_frg.pos_x, rana_frg.pos_y};
            rana_t *pRanaf = {1, coord, 0, 3, 1, llego[5]};                     //inicializo rana del frontend

            for((*pRanaf).nivel=1; (*pRanaf).nivel<6; (*pRanaf).nivel++){       //se sale del loop cuando se termina el nivel
                inicia_mapa((*pRanaf).nivel);                                   //inicia mapa en nivel correspondiente
                error = output_world_all (&rana, &mundo,background,automovil1,automovil2,camion,log2,log3,log4,ranita,ranamuerta,lives,llego);                       //si devuelve -1 hubo un error EXIT PROGRAM
                //si error esta en -1 prendo el flag exit program
                

                while(puntajeactual < 750*rana_t.nivel){    //esta dentro del mismo nivel --> actualizo estado de la rana 
                    mapa_t * Pmapa = actualiza_mundo();
                    action = get_input_all (event_queue, &ev);
                    action_t aux;
                    switch(action){             //en este contexto el play es pausa porque ya esta en play
                        case PLAY:
                            aux = output_gamepaused_all (event_queue, &ev);

                            break;
                        case UP:
                            rana_be_t * pRanab = rana_frogger('u');
                            int x_rana = (*pRanab).pos_x;
                            int y_rana = (*pRanab).pos_y;
                            analizar_estado_rana (&pRanaf, &Pmapa, x_rana, y_rana);
                            break;
                        case DOWN:
                            rana_be_t * pRanab = rana_frogger('d');
                            int x_rana = (*pRanab).pos_x;
                            int y_rana = (*pRanab).pos_y;
                            analizar_estado_rana (&pRanaf, &Pmapa, x_rana, y_rana);
                            break;
                        case LEFT:
                            rana_be_t * pRanab = rana_frogger('l');
                            int x_rana = (*pRanab).pos_x;
                            int y_rana = (*pRanab).pos_y;
                            analizar_estado_rana (&pRanaf, &Pmapa, x_rana, y_rana);
                            break;
                        case RIGHT:
                            rana_be_t * pRanab = rana_frogger('r');
                            int x_rana = (*pRanab).pos_x;
                            int y_rana = (*pRanab).pos_y;
                            analizar_estado_rana (&pRanaf, &Pmapa, x_rana, y_rana);         //parametros mandarle al back: accion rana y mundo
                            break;
                    }
                    
                    if((*pRanaf).estado == 0){     //rana murio
                        (*pRanaf).vidas = --(*pRanaf).vidas; //le resto 1 a la cantidad de vidas actuales
                        if((*pRanaf).vidas == 0){  //si la rana se queda sin vidas --> fin del juego
                            //fin del juego
                        }
                        else{                   //si sigue teniendo vidas, pongo a la rana al principio del juego y borro puntos
                            (*pRanaf).coord.x = 8;
                            (*pRanaf).coord.y = 15;
                            //no es necesario borrar puntis de esa ronda porque no se van a sumar ya la la rana va a estar en la fila 15
                            //se empieza a sumar puntos una vez que pasa la fila que llego la ultima vez
                        }
                    }

                    // actualizacion del puntaje 

                    int fila = coord.x;             //numero de fila en la que esta
                    if(fila<maxfila){               //la fila de mas adelante es la 1, se empieza en la 15 
                        maxfila = fila;             //asigno el numero de fila al que voy a llegar a max fila
                    }

                    puntajeactual = stringTonum(puntajestring);  //guardo el puntaje actual como decimal
                    asig_punt();                    //asigno puntaje
                    numTostring(); //guardo puntaje actual como string para mandarlo al frontend

                   /// if(puntajeactual = 750*(*pRanaf).nivel){       //750 es el puntaje con el que se pasa de nivel (150 puntos al cruzar, y hay que cruzar 5 veces para que la rana llene esos 5 lugares)
                   //      (*pRanaf).nivel++;                         //muestro el incremento en nivel
                   // }
                   // if(puntajeactual = 3750){
                   //     //termino el juego... como le mando eso al frontend????
                   // } USAR ARREGLO DE RANAF (LLEGO[5])
                }
            }
            break;
        case TOPSCORES:
            action_t top_scores;
            top_scores = output_topscores_all (event_queue, &ev);

            break;
        case EXIT:
            // fin de programa
            break;
    }

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

// funciones

//Analizar estado de la rana
void analizar_estado_rana (rana_t * pRanaf, mapa_t * Pmapa, int x_rana, int y_rana){
    if((*pRanab).desborde == 1){    //si desborda-->muere SIEMPRE
        (*pRanaf).estado = 0;
    }
    switch (y_rana){                //analizo si la rana vive o muere y si llego al final
        case 0:
            (*pRanaf).estado = 0;   //fila 0 es zona de muerte
            break;
        case 8:                     //fila 8 es zona segura
            (*pRanaf).estado = 1;
            break;
        case 15:                    //fila 15 es zona segura
            (*pRanaf).estado = 1;
            break;
        default:
            if(8<y_rana<15){        //STREET --> fijarse si ESTA EN LA CALLE 
                if(*Pmapa[y_rana][x_rana] == STREET){
                    (*pRanaf).estado = 1;
                }
                else{
                    (*pRanaf).estado = 0;
                }
            }
            if(1<y_rana<8){        //WATER --> fijarse si ESTA EN TRONCO SOLUCIONAR QUE SE MUEVA CON EL TRONCO
                if(*Pmapa[y_rana][x_rana] == LOG){
                    (*pRanaf).estado = 1;
                    if(action=NONE){
                        //mandarle a cristian un flag para mostrarle que la rana se tiene que mover con el tronco
                    }
                    }
                else{
                    (*pRanaf).estado = 0;
                }

            }
            if(y_rana == 1){                //zona de casillas del final, si cae en la casilla hay que mostrar que la rana esta viva
                switch(x_rana){             //y pasar en el arreglo llego[] en que casilla cayo 
                    case 2:                         
                        (*pRanaf).estado = 1;
                        (*pRanaf).llego[0] = 1;
                        break;
                    case 5:
                        (*pRanaf).estado = 1;
                        (*pRanaf).llego[1] = 1;
                        break;
                    case 8:
                        (*pRanaf).estado = 1;
                        (*pRanaf).llego[2] = 1;
                        break;
                    case 11:
                        (*pRanaf).estado = 1;
                        (*pRanaf).llego[3] = 1;
                        break;
                    case 14:
                        (*pRanaf).estado = 1;
                        (*pRanaf).llego[4] = 1;
                        break;
                    default:
                        (*pRanaf).estado = 0;
                        break;
                }
            }
            break;
    }
}

//Asignar puntaje

void asig_punt (void){                                                      //no devuelve nada porque puntajeactual es una variable global
    if((action == UP) && ((*pRanaf).estado=1) && (coord.x == maxfila)){      //Si Se movio para adelante y la rana esta viva 
        puntajeactual+= 10;                                                 //y esta en la fila amxima que llego, entonces, 
    }                                                                       //Se suman 10 puntos
}

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

//Transformar de puntajestring a numero

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
