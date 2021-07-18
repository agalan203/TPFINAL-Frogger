/***************************************************************************//**
  @file     +mainraspi.c+
  @brief    +programa principal de FROGGER para implementacion en rpi+
  @author   +Grupo 1: Meichtry,Rodriguez,Ruiz,Galan+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "globalstuff.h"
#include "frontraspi.h"
#include "backend.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void numTostring (void);    //transforma el puntajeactual a puntajestring
uint16_t stringTonum (char * string);   //transforma un string de 4 numeros a un entero
void istopscore (void);     //evalua si el puntajeactual entra en los top 4 puntajes y los guarda en el file correspondiente
int comparescores (const void * puntaje1, const void * puntaje2);   //compara dos puntajes para ordenarlos decrecientemente
void frogbcktofrnt (rana_be_t * prana , rana_t * rana, mapa_t * pmapa);     //transforma la estructura rana del backend a la del frontend
void mapbcktofrnt (mapa_t * pmapa, mundo_t * mundo);    //transforma el mapa del backend en el del frontend

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
            numTostring();
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
                    rana.estado = MUERTA;
                    output_dead_raspi();    //si fue atropellada lo muestro
                    if (rana.vidas == 0){
                        istopscore();
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
                        numTostring();
                        istopscore();
                        exit_game = 1;
                    }
                    numTostring();
                }
                if (estado == LLEGO){       //si llego del otro lado con tiempo del timer sobrante, se le suma como puntos el tiempo extra
                    puntajeactual += (int) prana->tiempo_res;
                    if (puntajeactual>=9999){
                        exit_game = 1;
                    }
                    numTostring();
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
                        istopscore();
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