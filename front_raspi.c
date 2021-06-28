/***************************************************************************//**
  @file     +frontraspi.c+
  @brief    +Front End de Frogger para un display matricial en RPI+
  @author   +Grupo 1: Albertina Galan+
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


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

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


// MAIN TEST : EJEMPLO

int main (void){
    init_raspi();
    output_topscores_raspi();
    output_world_raspi(&world,&frog);
    return 0;
}


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

    //primero actualizo las coordenadas medidas
    joy_update();
    //luego las guardo en myCoords
    myCoords = joy_get_coord();

    if (joy_get_switch()== J_PRESS) {
        accion = PLAY;
    }
    else if (joy_get_switch()== J_NOPRESS) {

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

    return  accion;
}

/********************************* OUTPUT WORLD **************************************/
void output_world_raspi (mundo_t * mundo, rana_t * frog){  //muestra el mundo en un momento dado en el display
    
    dcoord_t myPoint = {};
    int i,j;

    for (i=0, myPoint.y = DISP_MIN ; i<CANTFILS; i++, myPoint.y++){
        for (j=0, myPoint.x = DISP_MIN ; j<CANTCOLS; j++, myPoint.x++){
            switch ((*mundo)[i][j]){
                case STREET:
                case WATER:
                case HOME:
                    disp_write(myPoint, D_OFF);
                    break;
                case CAR:
                case TRUCK:
                case SAFE:
                case LOG:
                case DEAD:
                    disp_write(myPoint, D_ON);
                    break;
            }
        }
    }
    for (i=0;i<5;i++){
        if(frog->llego[i]){
            myPoint.x = (2+i*3);
            myPoint.y = 1;
            disp_write(myPoint,D_ON);        
        }
    }
    disp_update();
}

/********************************* OUTPUT FROG **************************************/
void output_frog_raspi (rana_t * rana){ //se debe invocar cada una cantidad establecida de tiempo, menor que la de actualizacion del display
    if (rana->estado == VIVA){
        if (rana->encendida == ENCENDIDA){
            disp_write(rana->coords, D_OFF);
        }
        else {
            disp_write(rana->coords, D_ON);
        }
        disp_update();
    }
    else {
        output_dead_raspi();
    }
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

    do{
        click = get_disp_coord (&gamepausedrpi);
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
    
    do{
        click = get_disp_coord (&initmenurpi);
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

    //AHI VA UNA FUNCION PARA PERDER UN TOQUE DE TIEMPO

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

    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
    }

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

    do{
        accion = get_input_raspi();
        switch (accion){
            case PLAY:
            case RIGHT:
                repeat = 1;
                break;
            default:
                repeat = 0;
        }
    } while (repeat == 0);

    if (accion == RIGHT){
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
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/********************************* COPY NR TO WORLD **************************************/
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

    dcoord_t pos = {DISP_MAX_X>>1 , DISP_MAX_Y>>1};	//pos es la posición actual, empieza en el centro de la matriz
	dcoord_t npos = pos;                            //npos es la próxima posición
	jcoord_t coord = {0,0};							//coordenadas medidas del joystick
    dlevel_t aux = D_OFF;                           //para guardar el estado del display original

	do
	{
		disp_update();	//Actualiza el display con el contenido del buffer
		joy_update();	//Mide las coordenadas del joystick
		coord = joy_get_coord();	//Guarda las coordenadas medidas
		
		//Establece la próxima posición según las coordenadas medidas
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
		disp_write(npos,D_ON);	//enciende la posición nueva en el buffer
		pos = npos;				//actualiza la posición actual
		
	} while( joy_get_switch() == J_NOPRESS );	//termina si se presiona el switch

    return npos;
}

/********************************* COPY MAP TO DISPLAY **************************************/
static void cpytodisplay (mundo_t * mundo){
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