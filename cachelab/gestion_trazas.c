#include "gestion_trazas.h"

#define MAX_LINEA 100
#define MAX_DIRECCION 11

bool obtener_linea_actual(FILE* archivo, char line[MAX_LINEA]) {
    if(fgets(line, MAX_LINEA, archivo) == NULL){
        return false;
    }
    return true;
}

void traza_trabajada(gestion_t* parametros, cache_t *cache, metricas_t* metricas, char linea_actual[MAX_LINEA]) {
    char mode;
    __uint8_t bytes_traza;
    char instruction_pointerS[MAX_DIRECCION];
    char mem_accedidaS[MAX_DIRECCION];
    char datos_l_o_e[MAX_DIRECCION];
    sscanf(linea_actual, "%10[^:]: %c %10s %hhu %10s", instruction_pointerS, &mode, mem_accedidaS, &bytes_traza, datos_l_o_e);
    __uint32_t mem_accedida = strtoul(mem_accedidaS, NULL, 16);
    __uint32_t E = parametros->E;
    __uint32_t C = parametros->C;
    __uint32_t S = parametros->S;
    __uint32_t tag = decodificar_direccion(mem_accedida, C, E, S, -1);
    __uint32_t set = decodificar_direccion(mem_accedida, C, E, S, 1);
    unsigned int indM = parametros->indexM;
    bool verbose = parametros->verboso;
    __uint8_t bytes = (C / (S * E));
    if (mode == 'R') {
        read(metricas, cache, tag, set, &(parametros->indexN), indM, bytes, verbose);
    } else if (mode == 'W') {
        write(metricas, cache, tag, set, &(parametros->indexN), indM, bytes, verbose);
    }
}
