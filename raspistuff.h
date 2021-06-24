// CONSTANTES, VARIABLES Y COSAS DE LA RASPI

//INCLUDES
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

//STRUCTURES

typedef uint8_t rpinr_t[5][3];

//VARIABLES GLOBALES

rpinr_t uno = {
    {0,1,0},
    {1,1,0},
    {0,1,0},
    {0,1,0},
    {0,1,0}
    };
rpinr_t dos = {
    {0,1,0},
    {1,0,1},
    {0,1,0},
    {1,0,0},
    {1,1,1}
    };
rpinr_t tres = {
    {1,1,1},
    {0,0,1},
    {0,1,1},
    {0,0,1},
    {1,1,1}
    };
rpinr_t cuatro = {
    {1,0,1},
    {1,0,1},
    {1,1,1},
    {0,0,1},
    {0,0,1}
    };
rpinr_t cinco = {
    {1,1,1},
    {1,0,0},
    {1,1,1},
    {0,0,1},
    {1,1,1}
    };
rpinr_t seis = {
    {1,1,1},
    {1,0,0},
    {1,1,1},
    {1,0,1},
    {1,1,1}
    };
rpinr_t siete = {
    {1,1,1},
    {0,0,1},
    {0,1,1},
    {0,0,1},
    {0,0,1}
    };
rpinr_t ocho = {
    {1,1,1},
    {1,0,1},
    {1,1,1},
    {1,0,1},
    {1,1,1}
    };
rpinr_t nueve = {
    {1,1,1},
    {1,0,1},
    {1,1,1},
    {0,0,1},
    {0,0,1}
    };
rpinr_t cero = {
    {1,1,1},
    {1,0,1},
    {1,0,1},
    {1,0,1},
    {1,1,1}
    };
