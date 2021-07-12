#include <stdint.h>

int main(void){
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;
    action_t action;
    
    init_allegro(&event_queue,&display);        //
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
                error = output_world_all (&rana, &mundo);                       //si devuelve -1 hubo un error EXIT PROGRAM
                //si error esta en -1 prendo el flag exit program
                

                while(puntajeactual < 750*rana_t.nivel){    //esta dentro del mismo nivel --> actualizo estado de la rana 
                    mapa_t * Pmapa = actualiza_mundo(void);
                    action = get_input_all (event_queue, &ev);
                    action_t aux;
                    switch(action){             //en este contexto el play es pausa porque ya esta en play
                        case PLAY:
                            aux = output_gamepaused_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev);

                            break;
                        case UP:
                            rana_be_t * pRanab = rana_frogger(u);
                            int x_rana = (*pRanab).pos_x;
                            int y_rana = (*pRanab).pos_y;
                            analizar_estado_rana (&pRanaf, &Pmapa, x_rana, y_rana);
                            break;
                        case DOWN:
                            rana_be_t * pRanab = rana_frogger(d);
                            int x_rana = (*pRanab).pos_x;
                            int y_rana = (*pRanab).pos_y;
                            analizar_estado_rana (&pRanaf, &Pmapa, x_rana, y_rana);
                            break;
                        case LEFT:
                            rana_be_t * pRanab = rana_frogger(l);
                            int x_rana = (*pRanab).pos_x;
                            int y_rana = (*pRanab).pos_y;
                            analizar_estado_rana (&pRanaf, &Pmapa, x_rana, y_rana);
                            break;
                        case RIGHT:
                            rana_be_t * pRanab = rana_frogger(r);
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

                    puntajeactual = stringTonum();  //guardo el puntaje actual como decimal
                    asig_punt();                    //asigno puntaje
                    *puntajestring = numTostring(); //guardo puntaje actual como string para mandarlo al frontend

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
            top_scores = output_topscores_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev);

            break;
        case EXIT:
            // fin de programa
            break;
    }

    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_destroy_display(display);

    return 0;
}

// funciones

//Analizar estado de la rana
(void) analizar_estado_rana (rana_t * pRanaf, mapa_t * Pmapa, int x_rana, int y_rana){
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
    if((action == UP) && ((*pRanaf).estado=1) && (coord.x == maxfila){      //Si Se movio para adelante y la rana esta viva 
        puntajeactual+= 10;                                                 //y esta en la fila amxima que llego, entonces, 
    }                                                                       //Se suman 10 puntos
}

//Transformar de numero a string

void numTostring (void){
    int i=0, aux=0;
    aux = puntajeactual;
    for(i=1; aux>10; aux/=10){
        i *= 10;
    }
    for(; i>0; i/=10){
        *puntajestring++ = ('0'+puntajeactual)/i;
        puntajeacutal %= i;
    }
    *puntajestring = '\0';
}

//Transformar de string a numero

int stringTonum (void){
    int var=0, n=0;
    while(*puntajestring != '\0'){
        if(var>47&&var<58){
            var-=48;
            n+=var;
            n*=10;
        }
    }
    return n;
}
