#include "gestion_arg.h"

#define ERROR_CANT_ARGS "Error: Cantidad incorrecta de argumentos.\n"
#define ERROR_ARCHIVO "Error: No se pudo abrir el archivo.\n"
#define ERROR_ESPECS_CACHE "Error: Especificaciones de la caché incorrectas.\n"
#define ERROR_MODO_VERBOSO "Error: Argumento de modo verboso incorrecto. ¿Quiso decir -v?\n"
#define ERROR_SIZE_CACHE "Error: Tamaño de cache inválido.\n"
#define ERROR_RANGO_VERBOSO "Error: Rango inválido para el modo verboso.\n"
#define ERROR -1
#define EXITO 0
#define POSICION_ARCHIVO 1
#define POSICION_C 2
#define POSICION_E 3
#define POSICION_S 4
#define POSICION_V 5
#define POSICION_N 6
#define POSICION_M 7
#define MODO_VERBOSO_COMANDO "-v"
#define INICIO_INDICES -1

void iniciar_C_E_S(gestion_t* parametros_cache, char* argv[]) {
    parametros_cache->C = atoi(argv[POSICION_C]);
    parametros_cache->E = atoi(argv[POSICION_E]);
    parametros_cache->S = atoi(argv[POSICION_S]);
}

void iniciar_n_m(gestion_t* parametros_cache, char* argv[]) {
    parametros_cache->indexN = atoi(argv[POSICION_N]);
    parametros_cache->indexM = atoi(argv[POSICION_M]);
}

bool parametros_potencias_de_2(int c, int e, int s) {
    if (c <= 0 || e <= 0 || s <= 0) return false;
    return (c & (c - 1) || e & (e - 1) || c & (c - 1)) == 0;
}

int transformar_arg_parametros_cache(gestion_t* parametros_cache, int argc, char* argv[]) {
    if (!(argc == CUATRO_PARAMETROS || argc == SIETE_PARAMETROS)) {
        fprintf(stderr, ERROR_CANT_ARGS);
        return ERROR;
    } else {
        parametros_cache->archivo = fopen(argv[POSICION_ARCHIVO], "r");
        if (parametros_cache->archivo == NULL) {
            fprintf(stderr, ERROR_ARCHIVO);
            return ERROR;
        }
        iniciar_C_E_S(parametros_cache, argv);
        if (!parametros_potencias_de_2(parametros_cache->C, parametros_cache->E, parametros_cache->S)) {
            fprintf(stderr, ERROR_ESPECS_CACHE);
            return ERROR;
        }
        int tamanio_bloque = parametros_cache->C / (parametros_cache->E * parametros_cache->S);
        if (!(tamanio_bloque >= 4)){
            fprintf(stderr, ERROR_SIZE_CACHE);
            return ERROR;
        }
        if (argc == SIETE_PARAMETROS) {
            if(strcmp(argv[POSICION_V], MODO_VERBOSO_COMANDO) != 0) {
                fprintf(stderr, ERROR_MODO_VERBOSO);
                return ERROR;
            }
            parametros_cache->verboso = true;
            iniciar_n_m(parametros_cache, argv);
            if(parametros_cache->indexN > parametros_cache->indexM) {
                fprintf(stderr, ERROR_RANGO_VERBOSO);
                return ERROR;
            }
        } else if(argc == CUATRO_PARAMETROS) {
            parametros_cache->verboso = false;
            parametros_cache->indexN = INICIO_INDICES;
            parametros_cache->indexM = INICIO_INDICES;
        }
    }
    return EXITO;
}