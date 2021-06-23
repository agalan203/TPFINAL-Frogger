// INCLUDES
#include "globalstuff.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

// DISPLAY CONSTANTS
#define WIDTH 1120
#define HEIGHT 1190
#define ITEM 70     //ancho y alto de un 'pixel' de la matriz de 16x16

//DISPLAY IMAGES
#define FRGGR_BCKGRD "all_images/frogger_bck.png"
#define FRGGR_TITLE "all_images/frogger_title.png"

// FUNCTION PROTOTYPES
int init_allegro (ALLEGRO_EVENT_QUEUE ** event_queue, ALLEGRO_DISPLAY ** display); //recibe un puntero a la event queue y display; devuelve 0 si todo bien, -1 si fallo
action_t get_input_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev); //devuelve la accion realizada, action_t es una estructura que guarda acciones
action_t output_initmenu_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev); //muestra el menu de inicio en el display

void output_world_all (mundo_t * mundo,rana_t * rana);  //muestra el mundo en un momento dado en el display
action_t output_gamepaused_all (ALLEGRO_EVENT_QUEUE * event_queue);  //muestra el menu de pausa en el display

// MAIN TEST
int main(void) {
    /****************************************************/
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT ev;
    action_t accion;
    
    init_allegro(&event_queue,&display);
    
    accion = output_initmenu_all (event_queue,&ev);

    //Destruir recursos empleados : SIEMPRE HACERLO EN MAIN
    al_shutdown_font_addon();
    al_shutdown_ttf_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_destroy_display(display);

    return 0;
}

// FUNCION DEFINITIONS
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
/********************************* OUTPUT INIT MENU ALLEGRO **************************************/
action_t output_initmenu_all (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT * ev){ //muestra el menu de inicio en el display
    ALLEGRO_BITMAP * background = NULL;
    ALLEGRO_FONT * font = al_load_ttf_font ("FreePixel.ttf",36,0);
    action_t accion = NONE;
    int exit = false;

    background = al_load_bitmap (FRGGR_TITLE);
    if(!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        return -1;
    }
    al_draw_bitmap(background,0,0,0);

    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,840,ALLEGRO_ALIGN_CENTRE,"Press spacebar to play");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,910,ALLEGRO_ALIGN_CENTRE,"Press escape to exit");
    al_draw_text(font,al_map_rgb(235, 238, 242),WIDTH/2,980,ALLEGRO_ALIGN_CENTRE,"Press T for top scores");

    al_flip_display();

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

    al_destroy_bitmap(background);
    al_destroy_font(font);

    return accion;
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