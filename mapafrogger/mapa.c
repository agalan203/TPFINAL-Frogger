#include "mapa.h"

static mapa_t mapa;

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

static void creacion_de_objetos(carril_t* arr_troncos[6], carril_t* arr_vehiculos[6], u_int8_t nivel);
static void convertir_velocidad(carril_t* arr[6]);
static void crear_vehiculos(carril_t* arr_vehiculos[6], int cant_vel);
static void crear_troncos(carril_t* arr_troncos[6], int cant_vel);
static void actualiza_linea(carril_t* carril);
static void carga_mapa(carril_t* linea);

void creacion_mapa(void){ //crea el mapa pero sin ningun vehiculo, mapa base
    int i, j; //variables para contadores

    for(i = 0; i < SIZE; i++){
        mapa[0][i] = DEAD;
        mapa[1][i] = DEAD;
    } 

    for(i = 1; i < SIZE; i= i+4){
        mapa[1][i] = WIN;
        mapa[1][i+1] = WIN;
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
    for(i = 0; i < 6; i++){
        carga_mapa(arr_troncos[i]);
        carga_mapa(arr_vehiculos[i]);
    }
}

static void creacion_de_objetos(carril_t* arr_troncos[6], carril_t* arr_vehiculos[6], u_int8_t nivel){
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

    convertir_velocidad(arr_vehiculos);
    convertir_velocidad(arr_troncos);

}

static void convertir_velocidad(carril_t* arr[6]){
    int i;
    for(i = 0; i < 6; i++){
    switch(arr[i] -> velocidad){
        case 0:
        case 1:
            arr[i] -> velocidad = MUY_LENTO;
            break;
        case 2:
            arr[i] -> velocidad = LENTO;
            break;
        case 3:
            arr[i] -> velocidad = NORMAL;
            break;
        case 4:
            arr[i] -> velocidad = RAPIDO;
            break;
        case 5:
            arr[i] -> velocidad = MUY_RAPIDO;
            break;
        }
    }
}

static void crear_vehiculos(carril_t* arr_vehiculos[6], int cant_vel){ //recibe la cantidad de velocidades a la que se mueven los vehiculos dependiendo del nivel 
    int i, j;
    int espacio_libre = 0;
    for(i = 0; i < 6; i++){
        arr_vehiculos[i] -> direccion = rand() % 2; //la direccion es 0 o 1            
        arr_vehiculos[i] -> velocidad = rand() % cant_vel + 1; 
        arr_vehiculos[i] -> cant_obj = (arr_vehiculos[i] -> size_obj == 1) ? rand() % 3 + 2: rand() % 3 + 1;
        espacio_libre = (arr_vehiculos[i] -> size_obj == 1) ? rand() % 3 + 1 : rand() % 3 + 3;
        if(arr_vehiculos[i] -> direccion == IZQ_A_DER){
            arr_vehiculos[i] -> pos_inic_objetos[j] = espacio_libre - 1;
            for(j = 1; j < arr_vehiculos[i] -> cant_obj; j++){
                arr_vehiculos[i] -> pos_inic_objetos[j] = arr_vehiculos[i] -> pos_inic_objetos[j-1] - espacio_libre - 1 - rand() % 2;
            }
        }
        else{
            arr_vehiculos[i] -> pos_inic_objetos[j] = (SIZE - 1) - (espacio_libre - 1);
            for(j = 1; j < arr_vehiculos[i] -> cant_obj; j++){
                arr_vehiculos[i] -> pos_inic_objetos[j] = arr_vehiculos[i] -> pos_inic_objetos[j-1] + espacio_libre + 1 + rand() % 2;
            }
        }
    }
}

static void crear_troncos(carril_t* arr_troncos[6], int cant_vel){
    int i,j;
    int espacio_libre = 0;
    for(i = 0; i < 6; i++){
        arr_troncos[i] -> direccion = rand() % 2; //la direccion es 0 o 1       
        arr_troncos[i] -> size_obj = rand() % 3 + 2; //los troncos pueden ser de 2, 3 o 4 cuadritos
        arr_troncos[i] -> velocidad = rand() % cant_vel + 1;
        switch(arr_troncos[i] -> size_obj){
            case 4:
                arr_troncos[i] -> cant_obj = rand() % 2 + 1;
                espacio_libre = (arr_troncos[i] -> cant_obj == 2) ? 3 : 0;
                break;
            case 3:
                arr_troncos[i] -> cant_obj = rand() % 3 + 1;
                espacio_libre = (arr_troncos[i] -> cant_obj == 3) ? 2 : 3;
                break;
            case 2:
                arr_troncos[i] -> cant_obj = rand() % 3 + 1;
                espacio_libre = rand() % 3 + 2;
                break;
        }

        if(arr_troncos[i] -> direccion == IZQ_A_DER){
            arr_troncos[i] -> pos_inic_objetos[j] = espacio_libre - 1;
            for(j = 1; j < arr_troncos[i] -> cant_obj; j++){
                arr_troncos[i] -> pos_inic_objetos[j] = arr_troncos[i] -> pos_inic_objetos[j-1] - espacio_libre - arr_troncos[i] -> size_obj - rand() % 2;
            }
        }
        else{
            arr_troncos[i] -> pos_inic_objetos[j] = (SIZE - 1) - (espacio_libre - 1);
            for(j = 1; j < arr_troncos[i] -> cant_obj; j++){
                arr_troncos[i] -> pos_inic_objetos[j] = arr_troncos[i] -> pos_inic_objetos[j-1] + espacio_libre + arr_troncos[i] -> size_obj + rand() % 2;
            }
        }
        
    }
}

mapa_t* actualiza_mundo(void){
    int i;
    carril_t* carriles[12] = {
        &tronco2, &tronco3, &tronco4, &tronco5, &tronco6, &tronco7,
        &vehiculo9, &vehiculo10, &vehiculo11, &vehiculo12, &vehiculo13, &vehiculo14
    };
    
    for(i = 0; i < 12; i++){
        actualiza_linea(carriles[i]);
    }

    return &mapa;
}

static void actualiza_linea(carril_t* linea){
    int i;
    if(((clock() - (linea -> tiempo_previo))/(double)CLOCKS_PER_SEC) >= linea -> velocidad){
        linea -> tiempo_previo = clock();
        for(i = 0; i < linea -> cant_obj; i++){
            linea -> pos_inic_objetos[i] += (linea -> direccion == IZQ_A_DER) ? 1 : -1;
        }        
        carga_mapa(linea);
    }
}

static void carga_mapa(carril_t* linea){
    int i, j;
    u_int8_t columna;
    if(linea -> direccion == IZQ_A_DER){
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
                    linea -> pos_inic_objetos[i] = -1;
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