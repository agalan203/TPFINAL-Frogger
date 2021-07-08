#include <stdio.h>
#include "mapa.h"

int main(void){
    int i, j;
    
    mapa_t* testmapa;


    clock_t comienzo;
    int fin = 1;
    comienzo=clock();


    creacion_mapa();
    inicia_mapa(5);

    while(1){
        fin = 1;
        testmapa = actualiza_mundo();
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                printf("%d ", (*testmapa)[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        while(fin){
            if( (((clock()-comienzo)/(double)CLOCKS_PER_SEC)) >= 1 ){
                comienzo = clock();
                fin--;
            }
        }


    }

    return 0;
}