/***************************************************************************//**
  @file     rana.c
  @brief    Declarancion del objeto rana y definiciones de funciones globales 
  		que modifican los atributos de la rana
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "rana.h"


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
/*
 * @brief: Declaracion del objeto rana_frg propia del modulo,
 * 		pero con persistencia global.	
 * */ 
static rana_be_t rana_frg;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void rana_init(uint8_t posx_in,uint8_t posy_in,uint8_t vidas_in,uint8_t llegadas_in){
	rana_frg.desborde=NDESBR;
	rana_frg.pos_x=posx_in;
	rana_frg.pos_y=posy_in;
	rana_frg.vidas=vidas_in;
	rana_frg.llegadas=llegadas_in;
	rana_frg.tiempo=clock();
	rana_frg.timeout=0;
	rana_frg.on_off=0;
	rana_frg.tiempo_onoff=clock();
}

rana_be_t * rana_frogger(char mov,uint8_t tiempo_max){
	switch(mov){
		case 'r':
			if(rana_frg.pos_x<(CANTCOLS-1)){/*si no hay posible desborde*/
				rana_frg.pos_x++;   /*muevo rana hacia la derecha*/
			}
			break;
		case 'l':	
			if(rana_frg.pos_x>0){	  /*si no hay posible desborde*/	
				rana_frg.pos_x--; /*muevo rana hacia la izquierda*/
			}
			break;
		case 'd':	
			if(rana_frg.pos_y<(CANTFILS-1)){ /*si no hay posible desborde*/
				rana_frg.pos_y++;        /*muevo rana hacia arriba*/
			}
			break;
		case 'u':	
			if(rana_frg.pos_y>0){	  /*si no hay posible desborde*/
				rana_frg.pos_y--; /*muevo rana hacia abajo*/
			}
			break;
		case '0':
			/*jugador aun no mueve rana*/
			break;
	}
	if((clock()-rana_frg.tiempo)/(double)CLOCKS_PER_SEC >= tiempo_max){
		rana_frg.timeout=1;
	}
	if((clock()-rana_frg.tiempo_onoff)/(double)CLOCKS_PER_SEC >= TITILE){
		rana_frg.on_off=(rana_frg.on_off==0)?1:0;
		rana_frg.tiempo_onoff = clock();
	}
	return &rana_frg;
}

rana_be_t * get_rana(void){
	return &rana_frg;
}




