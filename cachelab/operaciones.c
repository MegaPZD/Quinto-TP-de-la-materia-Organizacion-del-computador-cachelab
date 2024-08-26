#include <stdio.h>
#include <string.h>
#include "estructuras.h"
#include "operaciones.h"

#define TAG_LINEA_INVALIDA "-1"
#define FORMATO_RHIT "%u 1 %x %x %i %x 1 %i"
#define FORMATO_RMISS "%u %s %x %x %i %s %i %i"
#define FORMATO_WHIT "%u 1 %x %x %i %x 1 %i"
#define FORMATO_WMISS "%u %s %x %x %i %s %i %i"
#define CODIGO_DMISS "2b"
#define CODIGO_CMISS "2a"
#define PENALTY 100
#define MAX_TAG 10
#define FRMT_L1_METRICAS_MAPEO_DIRECTO "direct-mapped, %u sets, size = %uKB\n"
#define FRMT_L1_METRICAS "%u-way, %u sets, size = %uKB\n"
#define FRMT_L2_METRICAS "loads %u stores %u total %u\n"
#define FRMT_L3_METRICAS "rmiss %u wmiss %u total %u\n"
#define FRMT_L4_METRICAS "dirty rmiss %u dirty wmiss %u\n"
#define FRMT_L5_METRICAS "bytes read %u bytes written %u\n"
#define FRMT_L6_METRICAS "read time %u write time %u\n"
#define FRMT_L7_METRICAS "miss rate %f\n"


__uint32_t decodificar_direccion(__uint32_t direccion, __uint32_t c,
                                        __uint32_t e, __uint32_t s, __uint8_t req) {
        __uint32_t bytes_bloque = (c / (e * s));
        __uint32_t boffset = log2(bytes_bloque);
        __uint32_t bset = log2(s);
        __uint32_t btag = 32 - (boffset + bset);
        __uint32_t mask = 0xFFFFFFFF;
        __uint32_t offset = direccion & (mask >> (bset + btag));
        __uint32_t set = (direccion >> boffset) & (mask >> (boffset + btag));
        __uint32_t tag = (direccion >> (boffset + bset) & (mask >> (boffset + bset)));
        switch (req)
        {
        case 0:
                return offset;
        case 1:
                return set;
        default:
                return tag;
        }
}

linea_t *least_recently_used(cache_t *cache, __uint32_t set) {
        linea_t **lineas = cache->sets[set];
        linea_t *lru = lineas[0];
        for (__uint32_t i = 1; i < cache->vias; i++) {
                if (lru->v && !lineas[i]->v)
                        lru = lineas[i];
                else if (lru->v && lineas[i]->v && lineas[i]->index < lru->index)
                        lru = lineas[i];
                else if (!lru->v && !lineas[i]->v && lineas[i]->index < lru->index)
                        lru = lineas[i];
        }
        return lru;
}

char *tag_a_hexa(__uint32_t tag, char *hexa, bool valid) {
        if (!valid)
                return TAG_LINEA_INVALIDA;
        sprintf(hexa, "%x", tag);
        return hexa;
}

int index_linea(cache_t *cache, __uint32_t set, __uint32_t tag) {
        linea_t **lineas = cache->sets[set];
        for (__uint32_t i = 0; i < cache->vias; i++) {
                if (lineas[i]->tag == tag && lineas[i]->v)
                        return i;
        }
        return -1;
}

void rhit(metricas_t *metricas, cache_t *cache, int index_linea, __uint32_t set,
                                        unsigned* indexN, unsigned indexM, bool verbose) {
        metricas->loads++;
        metricas->accesos++;
        metricas->rtime++;
        linea_t *linea = cache->sets[set][index_linea];
        if (verbose && (*indexN <= indexM)) {
                printf(FORMATO_RHIT, *indexN, set, linea->tag, index_linea, linea->tag, linea->d);
                if (cache->vias > 1)
                        printf(" %u", linea->index);
                printf("\n");
        }
        linea->index = *indexN;
        (*indexN)++;
}

void rmiss(metricas_t *metricas, cache_t *cache, __uint32_t set, __uint32_t tag,
                        __uint8_t bytes, unsigned* indexN, unsigned indexM, bool verbose) {
        metricas->loads++;
        metricas->accesos++;
        metricas->rmisses++;
        metricas->misses++;
        metricas->rbytes += bytes;
        metricas->rtime++;
        linea_t *linea = least_recently_used(cache, set);
        int dirty = linea->d;
        int valid = linea->v;
        unsigned last_index = linea->index;
        __int32_t last_tag = linea->tag;
        char last_tag_hexa[MAX_TAG];
        char *tipo_miss = "";
        if (dirty) {
                metricas->rtime += PENALTY * 2;
                metricas->drmisses++;
                tipo_miss = CODIGO_DMISS;
                metricas->wbytes += bytes;
        }
        else {
                metricas->rtime += PENALTY;
                tipo_miss = CODIGO_CMISS;
        }
        linea->d = false;
        linea->index = *indexN;
        linea->v = true;
        linea->tag = tag;
        if (verbose && (*indexN <= indexM)) {
                printf(FORMATO_RMISS, *indexN, tipo_miss, set, tag,
                                                index_linea(cache, set, tag),
                                                tag_a_hexa(last_tag, last_tag_hexa, valid),
                                                valid, dirty);
                if (cache->vias > 1)
                        printf(" %u", last_index);
                printf("\n");
        }
        (*indexN)++;
}

void read(metricas_t *metricas, cache_t *cache, __uint32_t tag, __uint32_t set,
                                unsigned* indexN, unsigned indexM, __uint8_t bytes, bool verbose) {
        linea_t **cache_set = cache->sets[set];
        __uint32_t i = 0;
        while (i < cache->vias) {
                if (tag == cache_set[i]->tag && cache_set[i]->v) {
                        rhit(metricas, cache, i, set, indexN, indexM, verbose);
                        break;
                }
                i++;
        }
        if (i == cache->vias)
                rmiss(metricas, cache, set, tag, bytes, indexN, indexM, verbose);
}

void whit(metricas_t *metricas, cache_t *cache, int index_linea, __uint32_t set,
                                                unsigned* indexN, unsigned indexM, bool verbose) {
        metricas->stores++;
        metricas->accesos++;
        metricas->wtime++;
        linea_t *linea = cache->sets[set][index_linea];
        if (verbose && (*indexN <= indexM)) {
                printf(FORMATO_WHIT, *indexN, set, linea->tag, index_linea, linea->tag,
                                                                                        linea->d);
                if (cache->vias > 1)
                        printf(" %u", linea->index);
                printf("\n");
        }
        linea->d = true;
        linea->index = *indexN;
        (*indexN)++;
}

void wmiss(metricas_t *metricas, cache_t *cache, __uint32_t set, __uint32_t tag,
                                __uint8_t bytes, unsigned* indexN, unsigned indexM, bool verbose) {
        metricas->stores++;
        metricas->accesos++;
        metricas->wmisses++;
        metricas->rbytes += bytes;
        metricas->misses++;
        metricas->wtime++;
        linea_t *linea = least_recently_used(cache, set);
        bool dirty = linea->d;
        bool valid = linea->v;
        unsigned last_index = linea->index;
        __uint32_t last_tag = linea->tag;
        char last_tag_hexa[MAX_TAG];
        if (!linea->v)
                last_tag = 0;
        char *tipo_miss = "";
        if (dirty) {
                metricas->wtime += PENALTY * 2;
                metricas->dwmisses++;
                tipo_miss = CODIGO_DMISS;
                metricas->wbytes += bytes;
        }
        else {
                metricas->wtime += PENALTY;
                tipo_miss = CODIGO_CMISS;
        }
        linea->d = true;
        linea->index = *indexN;
        linea->v = true;
        linea->tag = tag;
        if (verbose && (*indexN <= indexM)) {
                printf(FORMATO_WMISS, *indexN, tipo_miss, set, linea->tag, 
                                                                index_linea(cache, set, tag),
                                                                tag_a_hexa(last_tag, last_tag_hexa, valid),
                                                                valid, dirty);
                if (cache->vias > 1)
                        printf(" %u", last_index);
                printf("\n");
        }
        (*indexN)++;
}

void write(metricas_t *metricas, cache_t *cache, __uint32_t tag, __uint32_t set,
                                        unsigned* indexN, unsigned indexM, __uint8_t bytes, bool verbose) {
        linea_t **cache_set = cache->sets[set];
        __uint32_t i = 0;
        while (i < cache->vias) {
                if (tag == cache_set[i]->tag && cache_set[i]->v) {
                        whit(metricas, cache, i, set, indexN, indexM, verbose);
                        break;
                }
                i++;
        }
        if (i == cache->vias)
                wmiss(metricas, cache, set, tag, bytes, indexN, indexM, verbose);
}

void imprimir_resumen(metricas_t *metricas, gestion_t parametros) {
        unsigned int tamanio_kb = (parametros.C) / 1024;
        if (parametros.E == 1)
                printf(FRMT_L1_METRICAS_MAPEO_DIRECTO, parametros.S, tamanio_kb);
        else
                printf(FRMT_L1_METRICAS, parametros.E, parametros.S, tamanio_kb);
        printf(FRMT_L2_METRICAS, metricas->loads, metricas->stores, metricas->accesos);
        printf(FRMT_L3_METRICAS, metricas->rmisses, metricas->wmisses, metricas->misses);
        printf(FRMT_L4_METRICAS, metricas->drmisses, metricas->dwmisses);
        printf(FRMT_L5_METRICAS, metricas->rbytes, metricas->wbytes);
        printf(FRMT_L6_METRICAS, metricas->rtime, metricas->wtime);
        printf(FRMT_L7_METRICAS, (float)metricas->misses / (float)metricas->accesos);
}
