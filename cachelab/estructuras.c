#include "estructuras.h"
#include <stdlib.h>

cache_t *crear_cache (__uint32_t s, __uint32_t e) {
    cache_t *cache = calloc(1, sizeof(cache_t));
    if (!cache){
        return NULL;
    }
    cache->sets = calloc(s, sizeof(linea_t**));
    if (!cache->sets) {
        free(cache);
        return NULL;
    }
    for (__uint32_t i = 0; i < s; i++) {
        cache->sets[i] = calloc(e, sizeof(linea_t*));
        if (!cache->sets[i]) {
            for (__uint32_t k = 0; k < i; k++) {
                free(cache->sets[k]);
            }
            free(cache->sets);
            free(cache);
            return NULL;
        }
        for (__uint32_t j = 0; j < e; j++) {
            cache->sets[i][j] = calloc(1, sizeof(linea_t));
            if (!cache->sets[i][j]) {
                for (__uint32_t k = 0; k <= i; k++) {
                    for (__uint32_t l = 0; l < (k == i ? j : e); l++) {
                        free(cache->sets[k][l]);
                    }
                    free(cache->sets[k]);
                }
                free(cache->sets);
                free(cache);
                return NULL;
            }
        }
    }
    cache->vias = e;
    return cache;
}

void liberar_cache(cache_t *cache, __uint32_t s) {
    for (__uint32_t i = 0; i < s; i++) {
        for (__uint32_t j = 0; j < cache->vias; j++) {
            free(cache->sets[i][j]);
        }
        free(cache->sets[i]);
    }
    free(cache->sets);
    free(cache);
}

metricas_t *crear_metricas() {
    return calloc(1, sizeof(metricas_t));
}

void liberar_metricas(metricas_t *metricas) {
    free(metricas);
}