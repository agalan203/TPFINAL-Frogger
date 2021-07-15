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

/*
 * @brief Se invoca si la rana esta en un tronco previo a la actualizacion del mapa.
 * 		Luego, si el mapa se actualiza, particularmente la linea donde esta la
 * 		rana, entonces esta funcion actualiza la posicion de la rana.
 * 		Ademas, luego de actualizar su posicion revisa que no este fuera del 
 * 		mapa. Si lo esta, muere. 
 * @param param1 prana: puntero a rana_be_t. Estructura del modulo rana, con 
 * 		informacion de la misma.
 * */
static int act_rana_tronco(rana_be_t * prana);
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
	int estado_rana=0;
 	int tm_max=MAX_TIEMPO-(nivel-1)*5;
	uint8_t rana_tronco=0;
	static uint8_t prev_nivel=1;
	static uint8_t pausa=0;
	static clock_t time_ps;
	
	/*Cambio de nivel*/
	if(nivel!=prev_nivel){
		rana_be_t * prana = get_rana();
		juego_rana_init_b(prana->vidas,nivel);
		prev_nivel=nivel;
	}

	/*Juego en pausa*/
	if(evento=='p'){
		/*Pongo en pausa lo que corresponda*/
		*mapa=get_mapa();
		*rana=get_rana();
		if(pausa==0){
			time_ps=clock();
		}
		pausa=1;
		estado_rana=PAUSA;		
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
		rana_be_t * prana; 
		mapa_t * pmapa;

		/*Actualizo estado de la rana*/
		prana=rana_frogger(evento,tm_max);

		/*Antes de actualizar el mapa me fijo si la rana esta en un tronco*/
		pmapa=get_mapa();
		rana_tronco=((*pmapa)[prana->pos_y][prana->pos_x]==LOG)?1:0;

		/*Actualizo mapa*/
		pmapa=actualiza_mundo();

		/*Comparo el caso Rana-Mundo, si la rana esta un tronco*/
		if(rana_tronco==1){
			estado_rana=act_rana_tronco(prana);
		}

		/*Comparo Rana-Mundo, si la rana no esta en un tronco*/
		if(prana->desborde==DESBR || prana->timeout==1){
			rana_muere(prana);
			estado_rana=MUERE;
		}
		else if(rana_tronco==0){		
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
	switch(contex_rana){	
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
			prana->tiempo_res=clock() - prana->tiempo;
			(*pmapa)[prana->pos_y][prana->pos_x]=OCUPADO;
			rana_init(POSX_I,POSY_I,prana->vidas,prana->llegadas);
			estado_rana=LLEGO;
			break;
	}
	return estado_rana;
}

static int act_rana_tronco(rana_be_t * prana){
	int estado_rana=0;
	carril_t * linea;
	linea=get_carril(prana->pos_y-2);/*Linea de tronco en pos_y-DEAD-HOME*/
	if(linea->act_prev==1){
		/*Si el tronco se movio, entonces la rana se mueve con el*/
		if(linea->direccion==IZQ_A_DER){
			prana->pos_x++;	
		}
		else{	
			prana->pos_x--;	
		}
		/*Si la rana sale del mapa por estar en el tronco, muere*/
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
	return estado_rana;
}

static void rana_muere(rana_be_t * prana){
	prana->vidas--;
	rana_init(POSX_I,POSY_I,prana->vidas,prana->llegadas);
}


 
