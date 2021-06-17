// INCLUDES
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

// DISPLAY CONSTANTS
#define WIDTH 1120
#define HEIGHT 1120
#define ITEM 70     //ancho y alto de un 'pixel' de la matriz de 16x16

// STRUCTURES AND ENUMS
typedef uint8_t action_t; //PLAY/PAUSE/UP/DOWN/LEFT/RIGHT/EXITGAME/EXITPROGRAM

typedef uint8_t mundo_t[16][16];

typedef struct {
    uint8_t estado;
    uint8_t coordx;
    uint8_t coordy;
} rana_t;

typedef struct {
    uint8_t obtclsize;
    uint8_t spacebtwobtcl;
    uint8_t sentidofila;
    uint8_t velocidadfila;
} fila_t;

enum player_action {NONE=0, PLAY, EXIT, UP, DOWN, LEFT, RIGHT}; //play-pause y exitgame-exitprgram se infiere por contexto
enum cell_state {STREET=0, CAR, TRUCK, BUS, WATER, LOG, TORTOISE};

// FUNCTION PROTOTYPES
int init_allegro (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_DISPLAY * display); //recibe un puntero a la event queue y display; devuelve 0 si todo bien, -1 si fallo
void shutdown_allegro (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_DISPLAY * display);

action_t * get_input_all (ALLEGRO_EVENT_QUEUE * event_queue, action_t * accion); //devuelve la accion realizada, action_t es una estructura que guarda acciones
void output_world_all (mundo_t * mundo,rana_t * rana);  //muestra el mundo en un momento dado en el display
void output_initmenu_all (void); //muestra el menu de inicio en el display
void output_gamepaused_all (void);  //muestra el menu de pausa en el display

// FUNCION DEFINITIONS
int init_allegro (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_DISPLAY * display){
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
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }
    //display
    display = al_create_display(WIDTH, HEIGHT);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_event_queue(event_queue);
        return -1;
    }
    //primitivas
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        return -1;
    }
    //otras cosas que haya que agregar

    //Registra el display a la cola de eventos, los eventos del display se iran guardando en la cola a medida que vayan sucediendo 
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source()); //REGISTRAMOS EL TECLADO
    return 0;
}

void shutdown_allegro (ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_DISPLAY * display){
    //Destruir recursor empleados 
    al_shutdown_primitives_addon();
    al_destroy_display(display); 
    al_destroy_event_queue(event_queue);
    return 0;
}

action_t * get_input_all (ALLEGRO_EVENT_QUEUE * event_queue, action_t * accion){
    ALLEGRO_EVENT ev;

    //TIPOS DE EVENTOS POSIBLES: (USAMOS SOLO TECLADO????)

    if (al_get_next_event(event_queue, &ev)){ //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    * accion = UP;
                    break;
                case ALLEGRO_KEY_DOWN:
                    * accion = DOWN;
                    break;
                case ALLEGRO_KEY_LEFT:
                    * accion = LEFT;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    * accion = RIGHT;
                    break;
                case ALLEGRO_KEY_SPACE:
                    * accion = PLAY;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    * accion = EXIT;
                    break;
            }
        }
    }
    else {
        * accion = NONE; 
    }
    return accion;
}
