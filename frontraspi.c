/***************************************************************************//**
  @file     +frontraspi.c+
  @brief    +Front End de Frogger para un display matricial en RPI+
  @author   +Grupo 1: Albertina Galan, Lucia Ruiz+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "frontraspi.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static dcoord_t get_disp_coord (mundo_t * disp);   //trae la coordenada del display donde esta el jugador
static void cpytoworld(mundo_t * mundo,rpinr_t * nro, uint8_t xo, uint8_t yo ); //copia un nro en el display
static void cpytodisplay (mundo_t * mundo); //copia un 'mundo' en el display
void delay_falso(double); //delay no bloqueante de una cantidad de segundos que recibe

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum cell_state {STREET=0, CAR, TRUCK, WATER, LOG, SAFE, DEAD, WIN, OCUPADO};

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
//Numeros para imprimir en el display de la raspi
static rpinr_t uno = {
    {0,1,0},
    {1,1,0},
    {0,1,0},
    {0,1,0},
    {0,1,0}
    };
static rpinr_t dos = {
    {0,1,0},
    {1,0,1},
    {0,1,0},
    {1,0,0},
    {1,1,1}
    };
static rpinr_t tres = {
    {1,1,1},
    {0,0,1},
    {0,1,1},
    {0,0,1},
    {1,1,1}
    };
static rpinr_t cuatro = {
    {1,0,1},
    {1,0,1},
    {1,1,1},
    {0,0,1},
    {0,0,1}
    };
static rpinr_t cinco = {
    {1,1,1},
    {1,0,0},
    {1,1,1},
    {0,0,1},
    {1,1,1}
    };
static rpinr_t seis = {
    {1,1,1},
    {1,0,0},
    {1,1,1},
    {1,0,1},
    {1,1,1}
    };
static rpinr_t siete = {
    {1,1,1},
    {0,0,1},
    {0,1,1},
    {0,0,1},
    {0,0,1}
    };
static rpinr_t ocho = {
    {1,1,1},
    {1,0,1},
    {1,1,1},
    {1,0,1},
    {1,1,1}
    };
static rpinr_t nueve = {
    {1,1,1},
    {1,0,1},
    {1,1,1},
    {0,0,1},
    {0,0,1}
    };
static rpinr_t cero = {
    {1,1,1},
    {1,0,1},
    {1,0,1},
    {1,0,1},
    {1,1,1}
    };

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/********************************* INIT RPI **************************************/
int init_raspi (void) {

    disp_init();    //inicializa el display
    disp_clear();   //limpia todo el display
    joy_init();     //inicializo el joystick

    return 0;
}

/********************************* GET INPUT **************************************/
action_t get_input_raspi (void){ //devuelve la accion realizada, action_t es una estructura que guarda acciones
    
    jcoord_t myCoords;
    action_t accion = NONE;
    jswitch_t pressed;
    static int over = 1;
    static int nopress = 1;

    //primero actualizo las coordenadas medidas
    joy_update();
    //luego las guardo en myCoords
    myCoords = joy_get_coord();
    pressed = joy_get_switch();
    //evaluo si se presiono o no el joystick
    switch (pressed){
        case J_PRESS:
            if (nopress){
                accion = PLAY;
            }
            else {
                accion = NONE;
            }
            nopress = 0;
            break;
        case J_NOPRESS:
        //obtengo el sentido en que se movio el joystick
            if (over){
                if ( (myCoords.y < THRESHOLD) && (myCoords.y > -THRESHOLD) && (myCoords.x > THRESHOLD) ){
                    accion = RIGHT;
                }
                else if ( (myCoords.y < THRESHOLD) && (myCoords.y > -THRESHOLD) && (myCoords.x < -THRESHOLD) ){
                    accion = LEFT;
                }
                else if ( (myCoords.x < THRESHOLD) && (myCoords.x > -THRESHOLD) && (myCoords.y > THRESHOLD) ){
                    accion = UP;
                }
                else if ( (myCoords.x < THRESHOLD) && (myCoords.x > -THRESHOLD) && (myCoords.y < -THRESHOLD) ){
                    accion = DOWN;
                }
                else {
                    accion = NONE;
                }
            }
            else {
                accion = NONE;
            }
            over = 0;
            break;
    }

    //guarda contra la sensibilidad del joystick. asi, una misma accion no es interpretada mas de una vez seguida
    if ((myCoords.x == 0) && (myCoords.y == 0)){
        over = 1;
    }
    if (pressed == J_NOPRESS){
        nopress = 1;
    }

    return  accion;
}

/********************************* OUTPUT WORLD **************************************/
void output_world_raspi (mundo_t * mundo, rana_t * frog){  //muestra el mundo en un momento dado en el display
    
    dcoord_t myPoint = {};
    int i,j;

    //imprime los distintos elementos
    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            switch ((*mundo)[i][j]){
                case STREET:
                case WATER:
                case WIN:
                    disp_write(myPoint, D_OFF);
                    break;
                case CAR:
                case TRUCK:
                case SAFE:
                case LOG:
                case DEAD:
                case OCUPADO:
                    disp_write(myPoint, D_ON);
                    break;
            }
        }
    }
    //imprime como casillas bloqueadas si la rana llego a la llegada
    for (i=0;i<5;i++){
        if(frog->llego[i]){
            myPoint.x = (2+i*3);
            myPoint.y = 1;
            disp_write(myPoint,D_ON);        
        }
    }
    
    if (frog->estado == VIVA){
        if (frog->encendida == ENCENDIDA){
            disp_write(frog->coords, D_OFF);
        }
        else {
            disp_write(frog->coords, D_ON);
        }
        disp_update();
    }
    //disp_write(frog->coords, D_OFF);
    
    disp_update();
}

/********************************* MENU PAUSA **************************************/
action_t output_gamepaused_raspi (void){  //muestra el menu de pausa en el display y devuelve que quizo hacer el jugador
    mundo_t gamepausedrpi = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,1,0,0,1,1,1,0,1,0,1,0,0},
        {0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,0},
        {0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,0},
        {0,1,0,0,1,1,0,1,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,1,0,0,1,0,1,0,1,1,1,0,0},
        {0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,1,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
        {0,1,1,0,1,0,0,1,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    action_t accion = NONE;
    dcoord_t click = {0,0};
    int repeat=0;

    cpytodisplay(&gamepausedrpi);

    //espera a que se introduzca una de las acciones validas
    do{
        click = get_disp_coord (&gamepausedrpi);    //se fija que hay en el lugar donde se hizo click
        if ((click.x >= 3) && (click.x <= 5) && (click.y >= 6) && (click.y <= 8)){
            accion = PLAY;
            repeat = 1;
        }
        else if ((click.x >= 10) && (click.x <= 12) && (click.y >= 6) && (click.y <= 8)){
            accion = EXIT;
            repeat = 1;
        }
        else {
            repeat=0;
        }
        } while (repeat==0);

    return accion;
}

/********************************* MENU INICIO **************************************/
action_t output_initmenu_raspi (void){ //muestra el menu de inicio en el display y devuelve que quizo hacer el jugador

    mundo_t initmenurpi = {
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1},
        {1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1},
        {1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1},
        {1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1},
        {1,0,0,0,1,0,0,1,1,0,1,1,0,0,0,1},
        {1,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    };
    action_t accion = NONE;
    dcoord_t click = {0,0};
    int repeat=0;

    cpytodisplay(&initmenurpi);
    
    //espera a que se introduzca una de las acciones validas
    do{
        click = get_disp_coord (&initmenurpi);  //se fija que hay en el lugar donde se hizo click
        if ((click.x >= 4) && (click.x <= 6) && (click.y >= 8) && (click.y <= 10)){
            accion = PLAY;
            repeat = 1;
        }
        else if ((click.x >= 8) && (click.x <= 10) && (click.y >= 8) && (click.y <= 10)){
            accion = EXIT;
            repeat = 1;
        }
        else if ((click.x >= 3) && (click.x <= 11) && (click.y >= 12) && (click.y <= 14)){
            accion = TOPSCORES;
            repeat = 1;
        }
        else {
            repeat=0;
        }
    } while (repeat==0);
    
    return accion;
}

/********************************* DEAD FROG **************************************/
void output_dead_raspi (void){
    mundo_t deadfrog = {
        {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,0,0,1,1,0,1,1,1,0,1,1,0,0},
        {0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0},
        {0,1,1,0,0,1,1,0,1,0,1,0,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0},
        {0,0,0,0,0,0,1,1,0,1,1,1,1,1,0,0},
        {0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0}
    };

    cpytodisplay(&deadfrog);
    delay_falso(2.0);
}

/********************************* GAME OVER **************************************/
void output_gameover_raspi (void){
    mundo_t gameover = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0},
        {1,0,0,0,1,0,1,0,1,1,1,0,1,0,0,0},
        {1,0,1,0,1,1,1,0,1,0,1,0,1,1,0,0},
        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0},
        {1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0},
        {1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0},
        {1,0,1,0,1,0,1,0,1,1,0,0,1,1,1,0},
        {1,0,1,0,1,0,1,0,1,0,0,0,1,1,0,0},
        {1,1,1,0,0,1,0,0,1,1,1,0,1,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    mundo_t blanco = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    int j=0;

    cpytodisplay(&gameover);

    delay_falso(1.5);

    //Imprime el puntaje obtenido en el display
    for (j=0;j<4;j++){
        switch (puntajestring[j]){
            case '0':
                cpytoworld(&blanco,&cero,5,4*j);
                break;
            case '1':
                cpytoworld(&blanco,&uno,5,4*j);
                break;
            case '2':
                cpytoworld(&blanco,&dos,5,4*j);
                break;
            case '3':
                cpytoworld(&blanco,&tres,5,4*j);
                break;
            case '4':
                cpytoworld(&blanco,&cuatro,5,4*j);
                break;
            case '5':
                cpytoworld(&blanco,&cinco,5,4*j);
                break;
            case '6':
                cpytoworld(&blanco,&seis,5,4*j);
                break;
            case '7':
                cpytoworld(&blanco,&siete,5,4*j);
                break;
            case '8':
                cpytoworld(&blanco,&ocho,5,4*j);
                break;
            case '9':
                cpytoworld(&blanco,&nueve,5,4*j);
                break;
        }
    }

    cpytodisplay(&blanco);
    delay_falso(1.5);

}

/********************************* OUTPUT TOP SCORES RASPI **************************************/
void output_topscores_raspi (void){     //muestra los top scores en el display
    FILE * topscores;
    mundo_t scores = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    int i,j,repeat=0;
    char string[5];
    action_t accion = NONE;

    //abre el archivo top scores
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
    }

    //copia los primeros 2 top scores en el display
    for (i=0;i<2;i++){
        fgets(string,5,topscores);
        for (j=0;j<4;j++){
            switch (string[j]){
                case '0':
                    cpytoworld(&scores,&cero,6*i,4*j);
                    break;
                case '1':
                    cpytoworld(&scores,&uno,6*i,4*j);
                    break;
                case '2':
                    cpytoworld(&scores,&dos,6*i,4*j);
                    break;
                case '3':
                    cpytoworld(&scores,&tres,6*i,4*j);
                    break;
                case '4':
                    cpytoworld(&scores,&cuatro,6*i,4*j);
                    break;
                case '5':
                    cpytoworld(&scores,&cinco,6*i,4*j);
                    break;
                case '6':
                    cpytoworld(&scores,&seis,6*i,4*j);
                    break;
                case '7':
                    cpytoworld(&scores,&siete,6*i,4*j);
                    break;
                case '8':
                    cpytoworld(&scores,&ocho,6*i,4*j);
                    break;
                case '9':
                    cpytoworld(&scores,&nueve,6*i,4*j);
                    break;
            }
        }
    }

    cpytodisplay(&scores);

    //espera a que se pidan los siguientes dos top scores o salir
    do{
        accion = get_input_raspi();
        switch (accion){
            case PLAY:
                repeat = 1;
                break;
            default:
                repeat = 0;
        }
    } while (repeat == 0);

    //copia los segundos dos top scores en el display
    if (accion == PLAY){
        for (i=0;i<2;i++){
            fgets(string,5,topscores);
            for (j=0;j<4;j++){
                switch (string[j]){
                    case '0':
                        cpytoworld(&scores,&cero,6*i,4*j);
                        break;
                    case '1':
                        cpytoworld(&scores,&uno,6*i,4*j);
                        break;
                    case '2':
                        cpytoworld(&scores,&dos,6*i,4*j);
                        break;
                    case '3':
                        cpytoworld(&scores,&tres,6*i,4*j);
                        break;
                    case '4':
                        cpytoworld(&scores,&cuatro,6*i,4*j);
                        break;
                    case '5':
                        cpytoworld(&scores,&cinco,6*i,4*j);
                        break;
                    case '6':
                        cpytoworld(&scores,&seis,6*i,4*j);
                        break;
                    case '7':
                        cpytoworld(&scores,&siete,6*i,4*j);
                        break;
                    case '8':
                        cpytoworld(&scores,&ocho,6*i,4*j);
                        break;
                    case '9':
                        cpytoworld(&scores,&nueve,6*i,4*j);
                        break;
                }
            }
        }

        cpytodisplay(&scores);
        //espera a que se pida exit
        repeat = 0;
        do{
            accion = get_input_raspi();
            switch (accion){
                case PLAY:
                    repeat = 1;
                    break;
                default:
                    repeat = 0;
            }
        } while (repeat == 0);
    }
    fclose(topscores);
}

/********************************* OUTPUT LEVEL NR **************************************/
void output_level_raspi (rana_t * rana){    //muestro el numero del nivel y la cant de vidas
mundo_t level = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    //muestra el numero del nivel por display
    switch (rana->nivel){
        case '1':
            cpytoworld(&level,&uno,2,10);
            break;
        case '2':
            cpytoworld(&level,&dos,2,10);
            break;
        case '3':
            cpytoworld(&level,&tres,2,10);
            break;
        case '4':
            cpytoworld(&level,&cuatro,2,10);
            break;
        case '5':
            cpytoworld(&level,&cinco,2,10);
            break;
    }

    //muestra la cantidad de vidas
    switch (rana->vidas){
        case 1:
            cpytoworld(&level,&uno,9,4);
            break;
        case 2:
            cpytoworld(&level,&dos,9,4);
            break;
        case 3:
            cpytoworld(&level,&tres,9,4);
            break;
    }
    
    cpytodisplay(&level);
    delay_falso(2.0);

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/********************************* COPY NR TO WORLD **************************************/

//copia un numero en una pos dada del display
static void cpytoworld(mundo_t * mundo,rpinr_t * nro, uint8_t xo, uint8_t yo ){
    int i,j;
    for (i=0;i<5;i++){
        for (j=0;j<3;j++){
            (*mundo)[xo+i][yo+j]=(*nro)[i][j];
        }
    }
}

/********************************* GET DISPLAY COORD **************************************/
static dcoord_t get_disp_coord (mundo_t * disp){

    dcoord_t pos = {DISP_MAX_X>>1 , DISP_MAX_Y>>1};	//pos es la posici??n actual, empieza en el centro de la matriz
	dcoord_t npos = pos;                            //npos es la pr??xima posici??n
	jcoord_t coord = {0,0};							//coordenadas medidas del joystick
    dlevel_t aux = D_OFF;                           //para guardar el estado del display original

	do
	{
		disp_update();	//Actualiza el display con el contenido del buffer
		joy_update();	//Mide las coordenadas del joystick
		coord = joy_get_coord();	//Guarda las coordenadas medidas
		
		//Establece la pr??xima posici??n seg??n las coordenadas medidas
		if(coord.x > THRESHOLD && npos.x < DISP_MAX_X)
		{
			npos.x++;
		}
		if(coord.x < -THRESHOLD && npos.x > DISP_MIN)
		{
			npos.x--;
		}
		if(coord.y > THRESHOLD && npos.y > DISP_MIN)
		{
			npos.y--;
		}
		if(coord.y < -THRESHOLD && npos.y < DISP_MAX_Y)
		{
			npos.y++;
		}

        // trae lo que deberia haber en el led modificado del display matricial
        if ((*disp)[pos.y][pos.x]== 0){
            aux = D_OFF;
        }
        else if ((*disp)[pos.y][pos.x]== 1){
            aux = D_ON;
        }

        disp_write(pos,aux);    //reestablece lo que habia previamente en la posicion donde esta el mouse
		disp_write(npos,D_ON);	//enciende la posici??n nueva en el buffer
		pos = npos;				//actualiza la posici??n actual
		
	} while( joy_get_switch() == J_NOPRESS );	//termina si se presiona el switch

    return npos;
}

/********************************* COPY MAP TO DISPLAY **************************************/
static void cpytodisplay (mundo_t * mundo){ //copia una matriz de 16x16 en el buffer del display y la muestra
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            if ((*mundo)[i][j]==0){
                disp_write(myPoint, D_OFF);
            } 
            else if ((*mundo)[i][j]==1){
                disp_write(myPoint, D_ON);
            }
        }
    }
    disp_update();
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