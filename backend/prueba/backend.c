/***************************************************************************//**
  @file     backend.c
  @brief    
  @author   grupo 1
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "backend.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/*
 * @brief Esta funcion analiza el entorno de la rana, actualizando las vidas si muere,
 * 		las llegadas si llega a casa y la posicion si esta en un tronco.
 * 		Dependiendo el entorno se le actualizan los atributos de posicion. Si
 * 		murio vuelve al incio y se resetea contador, sino mantiene valores 
 * 		previos.
 * @param param1 prana: puntero a rana_be_t. Estructura del modulo rana, con 
 * 		informacion de la misma.
 * @param param2 pmapa: puntero a mapa_t. Matriz del modulo mapa, con ubicacion deobjetos
 * @return Si la rana murio, devuelve MUERE.
 * 	   Si la rana vive, devuelve VIVE.
 * 	   Si la rana llego a casa, devuelve LLEGO.
 * */
static int rana_contex(rana_be_t * prana,mapa_t * pmapa);

/*
 * @brief Cuando muere la rana, esta funcion se encarga de actualizar las vidas,
 * 		y dar una nueva posicion de origen a la rana. Tambien resetea su 
 * 		timer de vida.
 * @param param1 prana: puntero a rana_be_t. Estructura del modulo rana, con 
 * 		informacion de la misma.
 * */
static void rana_muere(rana_be_t * prana);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void juego_rana_init_b(uint8_t vidas,uint8_t nivel){
	rana_init(POSX_I,POSY_I,vidas,0);
	creacion_mapa();
	inicia_mapa(nivel);
}


int juego_rana_b(char evento,uint8_t nivel, rana_be_t ** rana,mapa_t ** mapa){
	int estado_rana;
 	int tm_max=MAX_TIEMPO-(nivel-1)*5;
	static int prev_nivel=1;
	static int pausa=0;
	static clock_t time_ps;
	if(nivel!=prev_nivel){
		inicia_mapa(nivel);
		prev_nivel=nivel;
	}
	if(evento=='p'){
		/*Pongo en pausa lo que corresponda*/
		*mapa=get_mapa();
		*rana=get_rana();
		if(pausa==0){
			time_ps=clock();
		}
		pausa=1;
		
	}
	else{
		/*Al salir de la pausa reanudo el juego desde el tiempo que corresponde*/
		if(pausa==1){
			/*Actualizo tiempos relativos*/
			int i;
			int cant_obj=SIZE-2-1-1;	
			carril_t * linea;
			for(i=0;i<cant_obj;i++){	/*Recorre las lineas donde hay objetos (SIZE-2*SAFE-HOME-DEAD)*/
				linea=get_carril(i);	
				linea->tiempo_previo+=(clock()-time_ps);
			}	
			pausa=0;
		}
		/*Sigue el juego*/
		/*Actualizo estado de la rana*/
		rana_be_t * prana;
		prana=rana_frogger(evento,tm_max);

		/*Actualizo mapa*/
		mapa_t * pmapa;
		pmapa=actualiza_mundo();

		/*Comparo Rana-Mundo*/
		if(prana->desborde==DESBR || prana->timeout==1){
			rana_muere(prana);
			estado_rana=MUERE;
		}
		else{	
			estado_rana=rana_contex(prana,pmapa);
		}
		
		/*Copio info de rana en puntero parametro*/
		*rana=prana;
		/*Copio direccion de mapa*/
		*mapa=pmapa;
	}
	return estado_rana;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static int rana_contex(rana_be_t * prana,mapa_t * pmapa){
	int contex_rana=(*pmapa)[prana -> pos_y ][prana -> pos_x];		
	int estado_rana;
	carril_t * linea;
	switch(contex_rana){
		case LOG:	
			/*Rana sobre TRONCO*/
			linea=get_carril(prana->pos_y-2);/*Linea de tronco en pos_y-DEAD-HOME*/
			if(linea->act_prev==1){
				if(linea->direccion==IZQ_A_DER){
					prana->pos_x++;	
				}
				else{	
					prana->pos_x--;	
				}
				if(prana->pos_x<0 || prana->pos_x >=SIZE){
					rana_muere(prana);
					estado_rana=MUERE;
				}
				else{
					estado_rana=VIVE;
				}
			}
			else{
				estado_rana=VIVE;
			}
			break;
		case CAR:
		case TRUCK:
		case WATER:
		case OCUPADO:
		case DEAD:	
			/*Rana en MUERE, en agua, auto y camion*/	
			rana_muere(prana);
			estado_rana=MUERE;
			break;
		case STREET: 
		case SAFE:	
			/*Rana VIVE, en calle o zona segura*/
			estado_rana=VIVE;
			break;
		case WIN:
			/*Rana en linea de llegada*/
			prana->llegadas++;
			prana->tiempo_res=prana->tiempo;
			rana_init(POSX_I,POSY_I,prana->vidas,prana->llegadas);
			(*pmapa)[prana->pos_y][prana->pos_x]=OCUPADO;
			estado_rana=LLEGO;
			break;
	}
	return estado_rana;
}

static void rana_muere(rana_be_t * prana){
	prana->vidas--;
	rana_init(POSX_I,POSY_I,prana->vidas,prana->llegadas);
}


 
