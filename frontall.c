/***************************************************************************//**
  @file     +frontall.c+
  @brief    +Front End de Frogger para un display grafico con librerias Allegro+
  @author   +Grupo 1: Albertina Galan+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "frontall.h"

// MAIN TEST : EJEMPLO
/*
int main(void) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;
    action_t accion;
    
    init_allegro(&event_queue,&display);
    accion = output_initmenu_all(event_queue,&ev);
    output_world_all(&frog,&world);

    //Destruir recursos empleados : SIEMPRE HACERLO EN MAIN
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_destroy_display(display);

    return 0;
}
*/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/********************************* GET IMPUT ALLEGRO **************************************/
action_t get_input_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev){
    
    action_t accion = NONE;

    //TIPOS DE EVENTOS POSIBLES: (USAMOS SOLO TECLADO)

    if (al_get_next_event(event_queue, ev)){   //Toma un evento de la cola
        if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev->keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    accion = UP;
                    break;
                case ALLEGRO_KEY_DOWN:
                    accion = DOWN;
                    break;
                case ALLEGRO_KEY_LEFT:
                    accion = LEFT;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    accion = RIGHT;
                    break;
                case ALLEGRO_KEY_SPACE:
                    accion = PLAY;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    accion = EXIT;
                    break;
                case ALLEGRO_KEY_T:
                    accion = TOPSCORES;
                    break;
            }
        }
    }
    else {
        accion = NONE; 
    }
    return accion;
}

/********************************* INIT ALLEGRO **************************************/
int init_allegro (ALLEGRO_EVENT_QUEUE ** event_queue, ALLEGRO_DISPLAY ** display){
    //incializo allegro
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
    //instalo el teclado
    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }
    //cola de eventos
    *event_queue = al_create_event_queue();
    if (!*event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }
    //display
    *display = al_create_display(WIDTH, HEIGHT);
    if (!*display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_event_queue(*event_queue);
        return -1;
    }
    //primitivas
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        al_destroy_event_queue(*event_queue);
        al_destroy_display(*display);
        return -1;
    }
    //imagenes
    if (!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize images addon!\n");
        al_destroy_event_queue(*event_queue);
        al_destroy_display(*display);
        al_shutdown_primitives_addon();
        return -1;
    }
    //fonts
    if (!al_init_font_addon()) {
        fprintf(stderr, "failed to initialize fonts addon!\n");
        al_destroy_event_queue(*event_queue);
        al_destroy_display(*display);
        al_shutdown_primitives_addon();
        al_shutdown_image_addon();
        return -1;
    }
    //true types fonts
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "failed to initialize ttfonts addon!\n");
        al_destroy_event_queue(*event_queue);
        al_destroy_display(*display);
        al_shutdown_primitives_addon();
        al_shutdown_image_addon();
        al_shutdown_font_addon();
        return -1;
    }
    //otras cosas que haya que agregar

    al_register_event_source(*event_queue, al_get_keyboard_event_source()); //REGISTRAMOS EL TECLADO

    return 0;
}
/********************************* DESTROY ALLEGRO **************************************/
    // NO funciona, hay que hacerlo en main.
    
    /*
    //Destruir recursos empleados 
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_destroy_display(display); 
    al_destroy_event_queue(event_queue);
    */

/********************************* OUTPUT INIT MENU ALLEGRO **************************************/
action_t output_initmenu_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev){ //muestra el menu de inicio en el display
    ALLEGRO_BITMAP * background = NULL;
    ALLEGRO_FONT * font = al_load_ttf_font ("FreePixel.ttf",36,0);
    action_t accion = NONE;
    int exit = false;
    FILE * topscores;
    char string[5];

    //crea lo que usa
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
        al_destroy_font(font);
        return -1;
    }

    background = al_load_bitmap ("all_images/frogger_title.png");
    if(!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    al_draw_bitmap(background,0,0,0);

    //imprime el high score y las instrucciones 
    fgets(string,5,topscores);
    al_draw_textf(font,al_map_rgb(235, 238, 242),WIDTH/2,45,ALLEGRO_ALIGN_CENTRE,"%s",string);

    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,840,ALLEGRO_ALIGN_CENTRE,"Press spacebar to play");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,910,ALLEGRO_ALIGN_CENTRE,"Press escape to exit");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,980,ALLEGRO_ALIGN_CENTRE,"Press T for top scores");

    al_flip_display();

    //espera a que se introduzca una operacion valida
    do{
        accion = get_input_all(event_queue,ev);
        switch (accion){
            case PLAY:
            case EXIT:
            case TOPSCORES:
                exit = true;
                break;
            default:
                exit = false;
                break;
        }
    } while (exit == false);

    //destruye los recursos utilizados
    al_destroy_bitmap(background);
    al_destroy_font(font);
    fclose(topscores);

    return accion;
}

/********************************* OUTPUT PAUSED MENU ALLEGRO **************************************/
action_t output_gamepaused_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev){  //muestra el menu de pausa en el display
    ALLEGRO_BITMAP * background = NULL;
    ALLEGRO_BITMAP * pause = NULL;
    ALLEGRO_FONT * font = al_load_ttf_font ("FreePixel.ttf",36,0);
    action_t accion = NONE;
    int exit = false;
    FILE * topscores;
    char string[5];

    //crea lo que usa
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
        al_destroy_font(font);
        return -1;
    }

    background = al_load_bitmap ("all_images/frogger_title.png");
    if(!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    al_draw_bitmap(background,0,0,0);

    pause = al_load_bitmap ("all_images/frogger_pause.png");
    if(!pause)
    {
        fprintf(stderr, "failed to load pause bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    al_draw_bitmap(pause,WIDTH/2-55,490,0);

    //imprime el high score y las instrucciones 
    fgets(string,5,topscores);
    al_draw_textf(font,al_map_rgb(235, 238, 242),WIDTH/2,45,ALLEGRO_ALIGN_CENTRE,"%s",string);

    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,840,ALLEGRO_ALIGN_CENTRE,"GAME PAUSED");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,910,ALLEGRO_ALIGN_CENTRE,"Press spacebar to resume");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,980,ALLEGRO_ALIGN_CENTRE,"Press escape to exit game");

    al_flip_display();

    //espera a que se introduzca una operacion valida
    do{
        accion = get_input_all(event_queue,ev);
        switch (accion){
            case PLAY:
            case EXIT:
                exit = true;
                break;
            default:
                exit = false;
                break;
        }
    } while (exit == false);

    //destruye los recursos utilizados
    al_destroy_bitmap(background);
    al_destroy_bitmap(pause);
    al_destroy_font(font);
    fclose(topscores);

    return accion;
}

/********************************* OUTPUT GAME OVER ALLEGRO **************************************/
action_t output_gameover_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev){  //muestra el fin del juego en el display
    FILE * topscores;
    char string[5];
    ALLEGRO_BITMAP * background = NULL;
    ALLEGRO_BITMAP * square = NULL;
    ALLEGRO_FONT * font = al_load_ttf_font ("FreePixel.ttf",36,0);
    action_t accion = NONE;
    int exit = false;

    //crea lo que usa
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
        al_destroy_font(font);
        return -1;
    }

    background = al_load_bitmap ("all_images/frogger_bck.png");
    if(!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    al_draw_bitmap(background,0,0,0);

    square = al_load_bitmap ("all_images/blackblock.png");
    if(!square)
    {
        fprintf(stderr, "failed to load square bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    al_draw_bitmap(square,WIDTH/4,300,0);

    //imprime el high score y las instrucciones 
    fgets(string,5,topscores);
    al_draw_textf(font,al_map_rgb(235, 238, 242),WIDTH/2,45,ALLEGRO_ALIGN_CENTRE,"%s",string);

    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,370,ALLEGRO_ALIGN_CENTRE,"GAME OVER");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,530,ALLEGRO_ALIGN_CENTRE,"Press spacebar");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,580,ALLEGRO_ALIGN_CENTRE,"to play again");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,670,ALLEGRO_ALIGN_CENTRE,"Press escape to exit");

    al_flip_display();

    //espera a que se introduzca una operacion valida
    do{
        accion = get_input_all(event_queue,ev);
        switch (accion){
            case PLAY:
            case EXIT:
                exit = true;
                break;
            default:
                exit = false;
                break;
        }
    } while (exit == false);

    //destruye los recursos utilizados
    al_destroy_bitmap(background);
    al_destroy_bitmap(square);
    al_destroy_font(font);
    fclose(topscores);

    return accion;

}

/********************************* OUTPUT TOP SCORES ALLEGRO **************************************/
action_t output_topscores_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev){  //muestra los top scores en el display
    FILE * topscores;
    ALLEGRO_BITMAP * background = NULL;
    ALLEGRO_BITMAP * square = NULL;
    ALLEGRO_FONT * font = al_load_ttf_font ("FreePixel.ttf",36,0);
    action_t accion = NONE;
    int exit = false;
    int i;
    char string[5];

    //crea lo que usa
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
        al_destroy_font(font);
        return -1;
    }

    background = al_load_bitmap ("all_images/frogger_bck.png");
    if(!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    al_draw_bitmap(background,0,0,0);

    square = al_load_bitmap ("all_images/blackblock.png");
    if(!square)
    {
        fprintf(stderr, "failed to load square bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    al_draw_bitmap(square,WIDTH/4,300,0);

    //imprime los top scores y las instrucciones 
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,370,ALLEGRO_ALIGN_CENTRE,"TOP SCORES");
    for (i=0;i<4;i++){
        fgets(string,5,topscores);
        al_draw_textf(font,al_map_rgb(235, 238, 242),WIDTH/2,490+i*40,ALLEGRO_ALIGN_CENTRE,"%s",string);
    }
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,750,ALLEGRO_ALIGN_CENTRE,"Press escape to return");

    al_flip_display();

    //espera a que se introduzca una operacion valida
    do{
        accion = get_input_all(event_queue,ev);
        switch (accion){
            case EXIT:
                exit = true;
                break;
            default:
                exit = false;
                break;
        }
    } while (exit == false);

    //destruye los recursos utilizados
    al_destroy_bitmap(background);
    al_destroy_bitmap(square);
    al_destroy_font(font);
    fclose(topscores);

    return accion;
}

/********************************* OUTPUT WORLD ALLEGRO **************************************/
int output_world_all (rana_t * rana, mundo_t * mundo, ALLEGRO_BITMAP * background, ALLEGRO_BITMAP * automovil1, ALLEGRO_BITMAP * automovil2, ALLEGRO_BITMAP * camion, ALLEGRO_BITMAP * log2, ALLEGRO_BITMAP * log3, ALLEGRO_BITMAP * log4, ALLEGRO_BITMAP * ranita, ALLEGRO_BITMAP * ranamuerta, ALLEGRO_BITMAP * lives, ALLEGRO_BITMAP * llego){  //muestra el mundo en un momento dado en el display
    FILE * topscores;
    char string[5];
    int i,j;
    int contlogs;

    ALLEGRO_FONT * font = al_load_ttf_font ("FreePixel.ttf",36,0);

    //inicializa todo lo requerido
    /* copiar esto en MAIN
    topscores = fopen("topscores.txt","r+");
    if(!topscores){
        fprintf(stderr, "failed to open topscores file!\n");
        al_destroy_font(font);
        return -1;
    }

    background = al_load_bitmap ("all_images/frogger_bck.png");
    if(!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    automovil1 = al_load_bitmap ("all_images/car3.png");
    if(!automovil1)
    {
        fprintf(stderr, "failed to load automovil1 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    automovil2 = al_load_bitmap ("all_images/car4.png");
    if(!automovil2)
    {
        fprintf(stderr, "failed to load automovil2 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    camion = al_load_bitmap ("all_images/truck1.png");
    if(!camion)
    {
        fprintf(stderr, "failed to load truck bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    log2 = al_load_bitmap ("all_images/log2.png");
    if(!log2)
    {
        fprintf(stderr, "failed to load log2 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    log3 = al_load_bitmap ("all_images/log3.png");
    if(!log3)
    {
        fprintf(stderr, "failed to load log3 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log2);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    log4 = al_load_bitmap ("all_images/log4.png");
    if(!log4)
    {
        fprintf(stderr, "failed to load log4 bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    ranita = al_load_bitmap ("all_images/frog.png");
    if(!ranita)
    {
        fprintf(stderr, "failed to load ranita bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    ranamuerta = al_load_bitmap ("all_images/deadfrog.png");
    if(!ranamuerta)
    {
        fprintf(stderr, "failed to load ranamuerta bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_bitmap(ranita);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    lives = al_load_bitmap ("all_images/lives.png");
    if(!lives)
    {
        fprintf(stderr, "failed to load lives bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_bitmap(ranita);
        al_destroy_bitmap(ranamuerta);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }

    llego = al_load_bitmap ("all_images/llego.png");
    if(!llego)
    {
        fprintf(stderr, "failed to load lives bitmap!\n");
        al_destroy_bitmap(background);
        al_destroy_bitmap(automovil1);
        al_destroy_bitmap(automovil2);
        al_destroy_bitmap(camion);
        al_destroy_bitmap(log2);
        al_destroy_bitmap(log3);
        al_destroy_bitmap(log4);
        al_destroy_bitmap(ranita);
        al_destroy_bitmap(ranamuerta);
        al_destroy_bitmap(lives);
        al_destroy_font(font);
        fclose(topscores);
        return -1;
    }
    HASTA ACA COPIAR EN MAIN */

    al_draw_bitmap(background,0,0,0);

    //muestra el highscore y el nivel y el puntaje actual
    fgets(string,5,topscores);
    al_draw_textf(font,al_map_rgb(235, 238, 242),WIDTH/2,45,ALLEGRO_ALIGN_CENTRE,"%s",string);
    al_draw_textf(font,al_map_rgb(235, 238, 242),20,45,0,"%s",puntajestring);
    al_draw_textf(font,al_map_rgb(235, 238, 242),980,45,0,"LEVEL %c",rana->nivel);
    
    //muestra los elementos del mundo donde estan
    for (i=0; i<CANTFILS; i++){
        for (j=0; j<CANTCOLS; j++){
            switch ((*mundo)[i][j]){
                case STREET:
                case WATER:
                case SAFE:
                case DEAD:
                    break;                    
                case CAR:
                    if (i%2){
                        al_draw_bitmap(automovil1,j*ITEM,(i+1)*ITEM,0);
                    }
                    else {
                        al_draw_bitmap(automovil2,j*ITEM,(i+1)*ITEM,0);
                    }
                    break;
                case TRUCK:
                    al_draw_bitmap(camion,j*ITEM,(i+1)*ITEM,0);
                    if (j<(CANTCOLS-1)){
                        j++;
                    }
                    break;
                case LOG:
                    contlogs = 0;
                    while (((*mundo)[i][j+contlogs] == LOG) && ((j+contlogs)< CANTCOLS)){
                        ++contlogs;
                    }
                    switch (contlogs){
                        case 1:
                        case 2:
                            al_draw_bitmap(log2,j*ITEM,(i+1)*ITEM,0);
                            break;
                        case 3:
                            al_draw_bitmap(log3,j*ITEM,(i+1)*ITEM,0);
                            break;
                        case 4:
                            al_draw_bitmap(log4,j*ITEM,(i+1)*ITEM,0);
                            break;
                    }
                    j+=contlogs;
                    break;
            }
        }
    }

    //muestra la rana
    if (rana->estado == VIVA){
        al_draw_bitmap(ranita,rana->coords.x*ITEM,(rana->coords.y+1)*ITEM,0);
    }
    else if (rana->estado == MUERTA){
        al_draw_bitmap(ranamuerta,rana->coords.x*ITEM,(rana->coords.y+1)*ITEM,0);
    }

    //muestra la cantidad de vidas de la rana
    for (i=0;i<rana->vidas;i++){
        al_draw_bitmap(lives,1075-i*50,5,0);
    }

    //muestra los casilleros a donde ya llego la rana
    for (i=0;i<5;i++){
        if(rana->llego[i]){
            al_draw_bitmap(llego,82+2*116*i,ITEM*2-10,0);
        }
    }

    al_flip_display();
    
    /* ESTO COPIAR AL FINAL DE MAIN
    al_destroy_bitmap(background);
    al_destroy_bitmap(automovil1);
    al_destroy_bitmap(automovil2);
    al_destroy_bitmap(camion);
    al_destroy_bitmap(log2);
    al_destroy_bitmap(log3);
    al_destroy_bitmap(log4);
    al_destroy_bitmap(ranita);
    al_destroy_bitmap(ranamuerta);
    al_destroy_bitmap(llego);
    al_destroy_bitmap(lives);
    HASTA ACA COPIAR EN MAIN */
    
    al_destroy_font(font);
    fclose(topscores);
    return 0;
}