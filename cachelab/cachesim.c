#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "estructuras.h"
#include "gestion_arg.h"
#include "operaciones.h"
#include "gestion_trazas.h"

#define MAX_LINEA 100
#define EXITO 0
#define ERROR -1

int main (int argc, char* argv[]) {
    gestion_t parametros_cache;
    int retorno = transformar_arg_parametros_cache(&parametros_cache, argc, argv);
    if (retorno != 0) {
        if(parametros_cache.archivo != NULL) {
            fclose(parametros_cache.archivo);
        }
        return ERROR;
    }
    cache_t* cache = crear_cache(parametros_cache.S, parametros_cache.E);
    if (!cache) {
        return ERROR;
    }
    metricas_t* metricas = crear_metricas();
    if (!metricas) {
        return ERROR;
    }
    char linea_actual[MAX_LINEA];
    while (obtener_linea_actual(parametros_cache.archivo, linea_actual))
        traza_trabajada(&parametros_cache,cache,metricas, linea_actual);
    imprimir_resumen(metricas, parametros_cache);
    fclose(parametros_cache.archivo);
    liberar_metricas(metricas);
    liberar_cache(cache, parametros_cache.S);
    return EXITO;
}


