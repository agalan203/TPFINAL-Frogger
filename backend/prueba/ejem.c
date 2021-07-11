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
		if(cont>=5){
			if(cont==10){
				cont=0;
			}
			evento='p';
			printf("pausa\n");
		}
		else{
			evento='u';
			printf("Reanuda\n");
		}
		int estado=juego_rana_b(evento,1,&prana,&pmapa);
		mostrar(pmapa,prana);
		while(fin){
		    if( (((clock()-comienzo)/(double)CLOCKS_PER_SEC)) >= 1 ){
			comienzo = clock();
			fin--;
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
	printf("POSX: %d ; POSY: %d ; vidas: %d\n",prana->pos_x,prana->pos_y,prana->vidas);
}
