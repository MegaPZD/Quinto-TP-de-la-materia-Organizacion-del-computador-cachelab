#ifndef __GESTION__CACHE__
#define __GESTION__CACHE__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define CUATRO_PARAMETROS 5
#define SIETE_PARAMETROS 8

typedef struct gestion_s
{
    FILE* archivo;
    unsigned int C;
    unsigned int E;
    unsigned int S;
    bool verboso;
    unsigned int indexN;
    unsigned int indexM;
} gestion_t;

/**
 * Recibe los parámetros del simulador de cache y chequea si son válidos.
 * Si lo son, los guarda en parametros_cache.
 */
int transformar_arg_parametros_cache(gestion_t* parametros_cache, int argc, char* argv[]);

#endif