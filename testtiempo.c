#include <time.h>
#include <stdio.h>

#define CANT_SEG_LED 1

int main(void){
    clock_t comienzo;
    int fin = 1;
    comienzo=clock();
    int led_state = 0;
    while(1){
        fin = 1;
        led_state = (led_state == 0) ? 1 : 0;
        printf("LED: %d\n", led_state);

        while(fin){
            if( (((clock()-comienzo)/(double)CLOCKS_PER_SEC)) >= CANT_SEG_LED ){
                comienzo = clock();
                fin--;
            }
        }

    }

    return 0;
}

