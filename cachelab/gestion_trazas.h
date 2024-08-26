#ifndef __GESTION__TRAZAS__
#define __GESTION__TRAZAS__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "estructuras.h"
#include "gestion_arg.h"
#include "operaciones.h"

bool obtener_linea_actual(FILE* archivo, char linea_actual[100]);

void traza_trabajada(gestion_t* parametros,cache_t *cache,metricas_t* metricas, char linea_actual[100]);

#endif