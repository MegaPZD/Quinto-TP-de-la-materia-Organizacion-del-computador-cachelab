#ifndef __OPERACIONES__
#define __OPERACIONES__
#include "estructuras.h"
#include "gestion_arg.h"

/**
 * Recibe una dirección de memoria para escribir o leer, los bytes (c),
 * cantidad de vías (e) y cantidad de sets (s) de la caché, y un número req (require).
 * Si req es 0, devuelve el offset de la dirección. Si es 1 devuelve el set, y si
 * es otro devuelve el tag.
*/
__uint32_t decodificar_direccion(__uint32_t direccion, __uint32_t c,
                                         __uint32_t e, __uint32_t s, __uint8_t req);

/**
 * Recibe unas métricas y una caché, un tag, set y offset para leer de la caché,
 * el índice de la operación actual, la cantidad de bytes que se quieren leer,
 * y un bool verbose, que controla la salida por consola de la información exhaustiva
 * de la lectura.
 * La función maneja los casos de hit y miss (dirty o clean), actualizando las métricas
 * e imprimiendo por pantalla la información solicitada.
*/
void read(metricas_t *metricas, cache_t *cache, __uint32_t tag, __uint32_t set,
                unsigned* indexN,unsigned indexM, __uint8_t bytes, bool verbose);

/**
 * Recibe unas métricas y una caché, un tag, set y offset para escribir de la caché,
 * el índice de la operación actual, la cantidad de bytes que se quieren escribir,
 * y un bool verbose, que controla la salida por consola de la información exhaustiva
 * de la lectura.
 * La función maneja los casos de hit y miss (dirty o clean), actualizando las métricas
 * e imprimiendo por pantalla la información solicitada.
*/
void write(metricas_t *metricas, cache_t *cache, __uint32_t tag, __uint32_t set,
                unsigned* indexN,unsigned indexM, __uint8_t bytes, bool verbose);

/**
 * Imprime por pantalla las métricas.
 */
void imprimir_resumen(metricas_t *metricas, gestion_t parametros);
#endif