/***************************************************************************//**
  @file     mapa.c
  @brief    Creacion y actualizacion constante del mapa de juego, de los troncos
            y de los vehiculos
  @author   Grupo 1: Cristian Meichtry, Juan Martin Rodriguez
 ******************************************************************************/

/*****************************************************************************/
//                               Header Files                                //
/*****************************************************************************/
#include "mapa.h"

/*****************************************************************************/
//                      definicion de variables locales                      //
/*****************************************************************************/
static mapa_t mapa; //declaracion del mapa de juego

//definicion e inicializacion de las estructuras de cada carril (no se inicializaron todos los campos)
static carril_t tronco2 = {0, 0, 1, 1, 2, LOG};
static carril_t tronco3 = {0, 0, 1, 1, 3, LOG};
static carril_t tronco4 = {0, 0, 1, 1, 4, LOG};
static carril_t tronco5 = {0, 0, 1, 1, 5, LOG};
static carril_t tronco6 = {0, 0, 1, 1, 6, LOG};
static carril_t tronco7 = {0, 0, 1, 1, 7, LOG};

static carril_t vehiculo9 = {0, 1, 1, 2, 9, TRUCK}; //en los ultimos dos carriles de los vehiculos solo hay camiones
static carril_t vehiculo10 = {0, 1, 1, 2, 10, TRUCK};
static carril_t vehiculo11 = {0, 1, 1, 1, 11, CAR};
static carril_t vehiculo12 = {0, 1, 1, 1, 12, CAR};
static carril_t vehiculo13 = {0, 1, 1, 1, 13, CAR};
static carril_t vehiculo14 = {0, 1, 1, 1, 14, CAR};

/*****************************************************************************/
//                      prototipos de funciones locales                      //
/*****************************************************************************/

/*****************************************************************************
 * @brief creacion_de_objetos: actualiza el mapa linea por linea constantemente
 * @param arr_troncos: arreglo de estructuras de troncos 
 * @param arr_vehiculos: arreglo de estructuras de vehiculos 
 * @param nivel: nivel de juego actual
**/
static void creacion_de_objetos(carril_t* arr_troncos[6], carril_t* arr_vehiculos[6], u_int8_t nivel);

/*****************************************************************************
 * @brief convertir_velocidad: actualiza el mapa linea por linea constantemente
 * @param arr: arreglo de estructuras de troncos o de vehiculos
**/
static void convertir_velocidad(carril_t* arr[6]);

/*****************************************************************************
 * @brief crear_vehiculos: actualiza el mapa linea por linea constantemente
 * @param arr_vehiculos: arreglo de estructuras de vehiculos 
 * @param cant_vel: cantidad de velocidades que va a haber en ese nivel 
**/
static void crear_vehiculos(carril_t* arr_vehiculos[6], int cant_vel);

/*****************************************************************************
 * @brief crear_troncos: actualiza el mapa linea por linea constantemente
 * @param arr_troncos: arreglo de estructuras de troncos 
 * @param cant_vel: cantidad de velocidades que va a haber en ese nivel 
**/
static void crear_troncos(carril_t* arr_troncos[6], int cant_vel);

/*****************************************************************************
 * @brief actualiza_linea: actualiza el mapa linea por linea constantemente
 * @param carril: carril que se quiere actualizar
**/
static void actualiza_linea(carril_t* carril);

/*****************************************************************************
 * @brief carga_mapa: actualiza el mapa linea por linea constantemente
 * @param linea: carril que se quiere cargar 
**/
static void carga_mapa(carril_t* linea);


/*****************************************************************************/
//                     definicion de funciones globales                      //
/*****************************************************************************/

void creacion_mapa(void){ //crea el mapa pero sin ningun vehiculo, mapa base
    int i, j; //variables para contadores

    for(i = 0; i < SIZE; i++){
        mapa[0][i] = DEAD;
        mapa[1][i] = DEAD;
    } 

    for(i = 2; i <= 14; i= i+3){
        mapa[1][i] = WIN;
    }

    for(i = 2; i <= 7; i++){  
        for(j = 0; j < SIZE; j++){
            mapa[i][j] = WATER;
        } 
    }

    for(i = 9; i <= 14; i++){  
        for(j = 0; j < SIZE; j++){
            mapa[i][j] = STREET;
        } 
    }

    for(i = 0; i < SIZE; i++){
        mapa[8][i] = SAFE;
        mapa[15][i] = SAFE;
    }
    
}

void inicia_mapa(u_int8_t nivel){ //se crean todos los objetos de un nivel, se mantienen a lo largo del nivel
    int i;
    carril_t* arr_troncos[6] = {&tronco2, &tronco3, &tronco4, &tronco5, &tronco6, &tronco7};
    carril_t* arr_vehiculos[6] = {&vehiculo9, &vehiculo10, &vehiculo11, &vehiculo12, &vehiculo13, &vehiculo14};
    creacion_de_objetos(arr_troncos, arr_vehiculos, nivel);
    //Limpia celda donde llega la rana, si se pasa de nivel pasa de OCUPADO a WIN
    for(i = 2; i <= 14; i= i+3){
        mapa[1][i] = WIN;
    }
    //Carga al mapa los nuevos objetos del nivel
    for(i = 0; i < 6; i++){
        carga_mapa(arr_troncos[i]);
        carga_mapa(arr_vehiculos[i]);
    }
}

carril_t * get_carril(uint8_t linea){
	 carril_t* carriles[12] = {
		&tronco2, &tronco3, &tronco4, &tronco5, &tronco6, &tronco7,
		&vehiculo9, &vehiculo10, &vehiculo11, &vehiculo12, &vehiculo13, &vehiculo14
	 };
	 return carriles[linea];
}

mapa_t * get_mapa(void){
	return &mapa;
}

mapa_t* actualiza_mundo(void){ //actualiza el mapa constantemente
    int i;
    carril_t* carriles[12] = {
        &tronco2, &tronco3, &tronco4, &tronco5, &tronco6, &tronco7,
        &vehiculo9, &vehiculo10, &vehiculo11, &vehiculo12, &vehiculo13, &vehiculo14
    };
    
    for(i = 0; i < 12; i++){
        actualiza_linea(carriles[i]); //actualiza linea por linea
    }

    return &mapa;
}

/*****************************************************************************/
//                      definicion de funciones locales                      //
/*****************************************************************************/

static void creacion_de_objetos(carril_t* arr_troncos[6], carril_t* arr_vehiculos[6], u_int8_t nivel){ //se crea cada objeto con sus atributos correspondientes
    int i;
    srand(time(NULL));
    switch(nivel){
        case 1:
            crear_vehiculos(arr_vehiculos, 2); //en nivel 1 solo hay dos velocidades posibles (velocidad 1 y 2)
            crear_troncos(arr_troncos, 2);
            break;
        case 2:
            crear_vehiculos(arr_vehiculos, 3); //en nivel 2 hay tres velocidades posibles (velocidad 1, 2 y 3)
            crear_troncos(arr_troncos, 3);
            break;     
        case 3:
            crear_vehiculos(arr_vehiculos, 3); //en nivel 3 hay tres velocidades posibles (velocidad 2, 3 y 4)
            crear_troncos(arr_troncos, 3);
            for(i = 0; i < 6; i++){
                (arr_vehiculos[i] -> velocidad)++;
                (arr_troncos[i] -> velocidad)++;
            }
            break;
        case 4:
            crear_vehiculos(arr_vehiculos, 2); //en nivel 4 solo hay dos velocidades posibles (3 y 4)
            crear_troncos(arr_troncos, 2);
            for(i = 0; i < 6; i++){
                (arr_vehiculos[i] -> velocidad) += 2;
                (arr_troncos[i] -> velocidad) +=2;
            }
            break;
        case 5:
            crear_vehiculos(arr_vehiculos, 2); //en nivel 5 solo hay dos velocidades posibles (4 y 5)
            crear_troncos(arr_troncos, 2);
            for(i = 0; i < 6; i++){
                (arr_vehiculos[i] -> velocidad) += 3;
                (arr_troncos[i] -> velocidad) += 3;
            }
            break;
    }

    for(i = 0; i < 6; i++){
        (arr_vehiculos[i] -> tiempo_previo) = clock();
        (arr_troncos[i] -> tiempo_previo) = clock();
    }

    convertir_velocidad(arr_vehiculos); //se convierte las velocidades 1, 2, 3, 4, 5
    convertir_velocidad(arr_troncos);   //a los valores constantes definidos 

}

static void convertir_velocidad(carril_t* arr[6]){ //"traduce" los valores enteros que se asignaron aleatoriamente a los valores constates definidos
    int i;
    for(i = 0; i < 6; i++){
    switch(arr[i] -> velocidad){
        case 0:
        case 1:
            arr[i] -> tm_cell = MUY_LENTO;
            break;
        case 2:
            arr[i] -> tm_cell = LENTO;
            break;
        case 3:
            arr[i] -> tm_cell = NORMAL;
            break;
        case 4:
            arr[i] -> tm_cell = RAPIDO;
            break;
        case 5:
            arr[i] -> tm_cell = MUY_RAPIDO;
            break;
        }
    }
}

static void crear_vehiculos(carril_t* arr_vehiculos[6], int cant_vel){ //recibe la cantidad de velocidades a la que se mueven los vehiculos dependiendo del nivel y el arreglo de vehiculos
    int i, j;
    int espacio_libre = 0;
    for(i = 0; i < 6; i++){
        arr_vehiculos[i] -> direccion = rand() % 2; //la direccion es 0 o 1            
        arr_vehiculos[i] -> velocidad = rand() % cant_vel + 1; //asigna velocidades random dependiendo de la cantidad que correspondan para el nivel
        arr_vehiculos[i] -> cant_obj = (arr_vehiculos[i] -> size_obj == 1) ? rand() % 3 + 2: rand() % 3 + 1; //asigna la cantidad de elementos que pueden aparecer
                                                                                                             //depende del tamaño del vehiculo (auto/camion)
        espacio_libre = (arr_vehiculos[i] -> size_obj == 1) ? rand() % 3 + 1 : rand() % 3 + 3; //define el espacio libre que puede haber entre los vehiculos
        if(arr_vehiculos[i] -> direccion == IZQ_A_DER){ //se crean los vehiculos dependiendo del sentido, cantidad y tamaño asignados para cada carril 
            arr_vehiculos[i] -> pos_inic_objetos[0] = espacio_libre - 1;
            for(j = 1; j < arr_vehiculos[i] -> cant_obj; j++){
                arr_vehiculos[i] -> pos_inic_objetos[j] = arr_vehiculos[i] -> pos_inic_objetos[j-1] - espacio_libre - 1 - rand() % 2;
            }
        }
        else{
            arr_vehiculos[i] -> pos_inic_objetos[0] = (SIZE - 1) - (espacio_libre - 1);
            for(j = 1; j < arr_vehiculos[i] -> cant_obj; j++){
                arr_vehiculos[i] -> pos_inic_objetos[j] = arr_vehiculos[i] -> pos_inic_objetos[j-1] + espacio_libre + 1 + rand() % 2;
            }
        }
    }
}

static void crear_troncos(carril_t* arr_troncos[6], int cant_vel){ //recibe la cantidad de velocidades a la que se mueven los troncos dependiendo del nivel y el arreglo de troncos
    int i,j;
    int espacio_libre = 0;
    for(i = 0; i < 6; i++){
        arr_troncos[i] -> direccion = rand() % 2; //la direccion es 0 o 1       
        arr_troncos[i] -> size_obj = rand() % 3 + 2; //los troncos pueden ser de 2, 3 o 4 cuadritos
        arr_troncos[i] -> velocidad = rand() % cant_vel + 1;
        switch(arr_troncos[i] -> size_obj){ //define el espacio libre que puede haber entre los troncos dependiendo del size de los mismos
            case 4:
                arr_troncos[i] -> cant_obj = rand() % 2 + 2;
                espacio_libre = (arr_troncos[i] -> cant_obj == 3) ? 2 : 3;
                break;
            case 3:
                arr_troncos[i] -> cant_obj = rand() % 3 + 2;
                espacio_libre = (arr_troncos[i] -> cant_obj == 4) ? 1 : ((arr_troncos[i] -> cant_obj == 3 ) ? 2 : 4);
                break;
            case 2:
                arr_troncos[i] -> cant_obj = rand() % 3 + 3;
                espacio_libre = (arr_troncos[i] -> cant_obj == 5) ? 1 : ((arr_troncos[i] -> cant_obj == 4 ) ? 2 : 4);
                break;
        }

        if(arr_troncos[i] -> direccion == IZQ_A_DER){ //funcionamiento analogo a crear_vehiculos
            arr_troncos[i] -> pos_inic_objetos[0] = espacio_libre - 1;
            for(j = 1; j < arr_troncos[i] -> cant_obj; j++){
                arr_troncos[i] -> pos_inic_objetos[j] = arr_troncos[i] -> pos_inic_objetos[j-1] - espacio_libre - arr_troncos[i] -> size_obj;
            }
        }
        else{
            arr_troncos[i] -> pos_inic_objetos[0] = (SIZE - 1) - (espacio_libre - 1);
            for(j = 1; j < arr_troncos[i] -> cant_obj; j++){
                arr_troncos[i] -> pos_inic_objetos[j] = arr_troncos[i] -> pos_inic_objetos[j-1] + espacio_libre + arr_troncos[i] -> size_obj;
            }
        }
        
    }
}

static void actualiza_linea(carril_t* linea){ //actualiza cada carril por saparado segun la velocidad de los objetos
    int i;
    if(((clock() - (linea -> tiempo_previo))/(double)CLOCKS_PER_SEC) >= linea -> tm_cell){
        linea -> tiempo_previo = clock();
	linea -> act_prev = 1;
        for(i = 0; i < linea -> cant_obj; i++){
            linea -> pos_inic_objetos[i] += (linea -> direccion == IZQ_A_DER) ? 1 : -1;
        }        
        carga_mapa(linea); //carga el mapa actualizado
    }
    else{
    	linea -> act_prev = 0;
    }
}

static void carga_mapa(carril_t* linea){ //carga el mapa despues de cada actualizacion 
    int i, j;
    u_int8_t columna;
    if(linea -> direccion == IZQ_A_DER){ //chequeo sentido y le asigno posicion por posicion el valor que corresponda 
        for(i = 0; i < linea -> cant_obj; i++){
            if(linea -> pos_inic_objetos[i] >= 0 && linea -> pos_inic_objetos[i] < SIZE){
                for(j = 0; j < linea -> size_obj; j++){
                    columna = linea -> pos_inic_objetos[i] - j;
                    if(columna >= 0){
                        mapa[linea -> carril][columna] = linea -> objeto;
                    }
                }
            }
            else if(linea -> pos_inic_objetos[i] >= SIZE){
                if(linea -> pos_inic_objetos[i] - linea -> size_obj + 1 < SIZE){
                    for(j = 1; j < linea -> size_obj; j++){
                        columna = linea -> pos_inic_objetos[i] - j;
                        if(columna < SIZE){
                            mapa[linea -> carril][columna] = linea -> objeto;
                        }
                    }
                }
                else{
                    mapa[linea -> carril][SIZE - 1] = (linea -> objeto == LOG) ? WATER : STREET;
                    linea -> pos_inic_objetos[i] = -2;
                }
            }
            if(linea -> pos_inic_objetos[i] - linea -> size_obj >= 0){
                mapa[linea -> carril][linea -> pos_inic_objetos[i] - linea -> size_obj] = (linea -> objeto == LOG) ? WATER : STREET;
            }
        }
        
    }
    else{
        for(i = 0; i < linea -> cant_obj; i++){
            if(linea -> pos_inic_objetos[i] >= 0 && linea -> pos_inic_objetos[i] < SIZE){
                for(j = 0; j < linea -> size_obj; j++){
                    columna = linea -> pos_inic_objetos[i] + j;
                    if(columna < SIZE){
                        mapa[linea -> carril][columna] = linea -> objeto;
                    }
                }
            }
            else if(linea -> pos_inic_objetos[i] < 0){
                if(linea -> pos_inic_objetos[i] + linea -> size_obj - 1 >= 0){
                    for(j = 1; j < linea -> size_obj; j++){
                        columna = linea -> pos_inic_objetos[i] + j;
                        if(columna >= 0){
                            mapa[linea -> carril][columna] = linea -> objeto;
                        }
                    }
                }
                else{
                    mapa[linea -> carril][0] = (linea -> objeto == LOG) ? WATER : STREET;
                    linea -> pos_inic_objetos[i] = SIZE;
                }
            }
            if(linea -> pos_inic_objetos[i] + linea -> size_obj < SIZE){
                mapa[linea -> carril][linea -> pos_inic_objetos[i] + linea -> size_obj] = (linea -> objeto == LOG) ? WATER : STREET;
            }
        }
    }
}
