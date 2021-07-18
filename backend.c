/***************************************************************************//**
  @file     +backend.c+
  @brief    +funciones que administran el backend del juego+
  @author   +Grupo 1: Cristian Meichtry, Juan Martin Rodriguez+
 ******************************************************************************/

/*****************************************************************************/
//                               Header Files                                //
/*****************************************************************************/
#include "backend.h"

/*****************************************************************************/
//                      prototipos de funciones locales                      //
/*****************************************************************************/

 /*****************************************************************************
 * @brief rana_contex: analiza el entorno de la rana, actualiza las vidas y 
 *					   lleva la rana al inicio si esta muere, actualiza las
 * 					   llegadas si llega a casa y la posicion si esta en un tronco.
 * @param prana: puntero a estructura con informacion de la rana 
 * @param pmapa: puntero al mapa de juego con los objetos moviendose
 * @return: Devuelve el estado de la rana.
 *          la rana murio, devuelve MUERE
 *          la rana vive, devuelve VIVE
 *          la rana llego al otro lado, devuelve LLEGO
**/
static int rana_contex(rana_be_t * prana,mapa_t * pmapa);

/*
 * @brief Cuando muere la rana, esta funcion se encarga de actualizar las vidas,
 * 		y dar una nueva posicion de origen a la rana. Tambien resetea el 
 * 		tiempo de bonus.
 * @param param1 prana: puntero a rana_be_t. Estructura del modulo rana, con 
 * 		informacion de la misma.
 * */

 /*****************************************************************************
 * @brief rana_muere: Cuando muere la rana, se actualizan las vidas,
 * 					  y se lleva a la rana al lugar de origen. Tambien 
 *                    se resetea el tiempo de bonus.
 * @param prana: puntero a estructura con informacion de la rana
**/
static void rana_muere(rana_be_t * prana);

 /*****************************************************************************
 * @brief act_rana_tronco: Se invoca si la rana esta sobre un tronco previo a la 
 *						   actualizacion del mapa. Luego, cuando el mapa se actualiza,
 *						   tambien se actualiza la posicion de la rana. Ademas, 
 *						   luego de actualizar su posicion revisa que no este fuera 
 *						   del mapa. Si lo esta, muere. 
 * @param prana: puntero a estructura con informacion de la rana
**/
static int act_rana_tronco(rana_be_t * prana);

/*****************************************************************************/
//                     definicion de funciones globales                      //
/*****************************************************************************/
void juego_rana_init_b(uint8_t vidas,uint8_t nivel){
	rana_init(POSX_I,POSY_I,vidas,0);
	creacion_mapa();
	inicia_mapa(nivel);
}

int juego_rana_b(char evento,uint8_t nivel, rana_be_t ** rana,mapa_t ** mapa){
	int estado_rana=0;
 	int tm_max=MAX_TIEMPO-(nivel-1)*SEG_POR_NIV;
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
			rana_be_t * prana=get_rana();
			prana->tiempo+=(clock()-time_ps);	
			pausa=0;
		}
		/*Sigue el juego*/	
		rana_be_t * prana; 
		mapa_t * pmapa;

		/*Actualizo estado de la rana*/
		prana=rana_frogger(evento,tm_max);

		/*Antes de actualizar el mapa me fijo si la rana esta en un tronco*/
		pmapa=get_mapa();
		rana_tronco=( ((*pmapa)[prana->pos_y][prana->pos_x]) == LOG) ? 1 : 0;

		/*Actualizo mapa*/
		pmapa=actualiza_mundo();

		/*Comparo el caso Rana-Mundo, si la rana esta un tronco*/
		if(rana_tronco==1){
			estado_rana=act_rana_tronco(prana);
		}

		/*Comparo Rana-Mundo, si la rana no esta en un tronco*/
		if(prana->desborde==DESBR){
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

/*****************************************************************************/
//                      definicion de funciones locales                      //
/*****************************************************************************/
static int rana_contex(rana_be_t * prana,mapa_t * pmapa){
	int contex_rana=(*pmapa)[prana -> pos_y ][prana -> pos_x];		
	int estado_rana;
	switch(contex_rana){	
		case CAR:
		case TRUCK:
		case WATER:
		case OCUPADO:
		case DEAD:	
			//Rana MUERE si toca el agua, un auto o un camion
			rana_muere(prana);
			estado_rana=MUERE;
			break;
		case STREET: 
		case SAFE:	
			//Rana VIVE en calle o en zona segura
			estado_rana=VIVE;
			break;
		case WIN:
			//Rana en linea de llegada
			prana->llegadas++;
			if(prana->timeout==1){
				prana->tiempo_res=0;
			}
			else{
				prana->tiempo_res=(clock() - prana->tiempo)/(double)CLOCKS_PER_SEC;
			}
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
	linea=get_carril((prana->pos_y) - 2);//Linea de tronco en pos_y-DEAD-HOME
	if(linea->act_prev==1){
		//Si el tronco se movio, entonces la rana se mueve con el
		if(linea->direccion==IZQ_A_DER){
			prana->pos_x++;	
		}
		else{	
			prana->pos_x--;	
		}
		//Si la rana sale del mapa por estar en el tronco, muere
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
	prana->vidas--; //se decrementa la cantidad de vidas
	rana_init(POSX_I,POSY_I,prana->vidas,prana->llegadas); //la rana vuelve al inicio
}
