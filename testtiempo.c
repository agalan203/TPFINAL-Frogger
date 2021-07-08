#include <time.h>
#include <stdio.h>

void delay_falso(double);

int main(void){

    printf("Hola Mundo!\n");

    delay_falso(5); //espera cinco segundos

    printf("Chau Mundo!\n");

    return 0;
}

/****************************************************************/
//                         delay_falso                          //
//     recibe la cantidad de segundos que tiene que esperar     //
/****************************************************************/

void delay_falso(double cant_seg){
    clock_t comienzo;
    int fin = 1;
    comienzo=clock();
    while(fin){
        if( (((clock()-comienzo)/(double)CLOCKS_PER_SEC)) >= cant_seg ){
            fin--;
        }
    }
}