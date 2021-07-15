#include "backend.h"

#include <stdio.h>

void mostrar(mapa_t * pmapa, rana_be_t * prana);

int main(void){
	/*Backend inicia*/
	int nivel=1;
	char evento;
	rana_be_t * prana=NULL;
	mapa_t * pmapa=NULL;
	juego_rana_init_b(3,nivel);	
	clock_t comienzo=clock();
	int fin=1;
	int cont=0;
	do{
		/*Leo evento de fronted*/
		evento='u';
		prana=get_rana();
		switch(cont){
			case 1:
				prana->pos_x=2;
				prana->pos_y=2;
				break;
			case 2:
				prana->pos_x=5;
				prana->pos_y=2;
				break;
			case 3:
				prana->pos_x=8;
				prana->pos_y=2;
				break;
			case 4:
				prana->pos_x=11;
				prana->pos_y=2;
				break;
			case 5:
				prana->pos_x=14;
				prana->pos_y=2;
				break;
		}
		if(prana->llegadas==5){
			nivel++;
		}
		int estado=juego_rana_b(evento,nivel,&prana,&pmapa);
		printf("ESTADO: %d ; \n",estado);
		mostrar(pmapa,prana);
		
		while(fin){
		    if( (((clock()-comienzo)/(double)CLOCKS_PER_SEC)) >= 1 ){
			comienzo = clock();
			fin=0;
			cont++;
		    }
        	}
		fin=1;
	}while(1);

	return 0;
}

void mostrar(mapa_t * pmapa,rana_be_t * prana){
	int j,i;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			printf("%d ", (*pmapa)[i][j]);
		}
		printf("\n");
	}
	printf("\n");	
	printf("POSX: %d ; POSY: %d ; vidas: %d ; llegadas: %d\n",prana->pos_x,prana->pos_y,prana->vidas, prana->llegadas);
}
