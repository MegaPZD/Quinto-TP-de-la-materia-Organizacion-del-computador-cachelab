#ifndef __ESTRUCTURAS__
#define __ESTRUCTURAS__

#include <math.h>
#include <stdbool.h>

typedef struct metricas {
        unsigned loads;
        unsigned stores;
        unsigned accesos;
        unsigned rmisses;
        unsigned wmisses;
        unsigned misses;
        unsigned drmisses;
        unsigned dwmisses;
        unsigned rbytes;
        unsigned wbytes;
        unsigned rtime;
        unsigned wtime;
        unsigned miss_rate;
} metricas_t;

typedef struct linea {
        unsigned index;
        bool v;
        bool d;
        __uint32_t tag;
} linea_t;

typedef struct cache {     // ejemplo de cache con 3 sets de 3 lineas en memoria:
        unsigned vias;     // [ [ [linea_t] [linea_t] [linea_t] ]   <- set 1
        linea_t ***sets;   //   [ [linea_t] [linea_t] [linea_t] ]   <- set 2
} cache_t;                 //   [ [linea_t] [linea_t] [linea_t] ] ] <- set 3

/**
 * Reserva la memoria necesaria para un cache_t.
 * Devuelve un puntero a la memoria reservada, o NULL en caso de error.
*/
cache_t *crear_cache(__uint32_t s, __uint32_t e);

/**
 * Libera la memoria reservada para la cache.
 */
void liberar_cache(cache_t *cache, __uint32_t s);

/**
 * Reserva la memoria necesaria para un metricas_t.
 * Devuelve un puntero a la memoria reservada, o NULL en caso de error.
*/
metricas_t *crear_metricas();

/**
 * Libera la memoria reservada para las metricas.
 */
void liberar_metricas(metricas_t *metricas);

#endif