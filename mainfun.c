/***************************************************************************//**
  @file     +mainfun.h+
  @brief    +Funciones de main para el juego en librerias allegro o raspberry+ 
  @author   +Grupo 1: Meichtry,Rodriguez,Ruiz,Galan+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "mainfun.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief comparescore: compara dos puntajes para ordenarlos decrecientemente
 *
 * @param param1 puntaje1: puntero constante a void. Apunta al inicio de un puntaje 
 * 		en formato string.
 * 	  param2 puntaje2: puntero constante a void. Apunta al inicio de un puntaje 
 * 		en formato string.		
 * @return numero mayor a cero si puntaje2 es mayor a puntaje1
 * 	   numero igual a cero si los puntajes son iguales
 * 	   numero menor a cero si puntaje2 es menor a puntaje1 
 **/
static int comparescores (const void * puntaje1, const void * puntaje2);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void frogbcktofrnt (rana_be_t * prana , rana_t * rana, mapa_t * pmapa){ 
    //esta funcion no modifica: NIVEL
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

void mapbcktofrnt (mapa_t * pmapa, mundo_t * mundo){
    int i,j;
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            (*mundo)[i][j] = (*pmapa)[i][j];
        }
    }
}

void numTostring (uint16_t puntajeactual,char puntajestring[5]){
    int aux = puntajeactual;

    puntajestring[0]=(aux/1000)+'0';
    aux = puntajeactual%1000;
    puntajestring[1]=(aux/100)+'0';
    aux = aux%100;
    puntajestring[2]=(aux/10)+'0';
    puntajestring[3]=aux%10+'0';
    puntajestring[4] = '\0';
}

void istopscore (char puntajestring[5]){
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
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static int comparescores (const void * puntaje1, const void * puntaje2){
    char * pp1 = (char *) puntaje1;
    char * pp2 = (char *) puntaje2;
    int i = 0;

    while ((pp1[i]== pp2[i])&& i<5){
        i++;
    }
    return (pp2[i]-pp1[i]);
}



