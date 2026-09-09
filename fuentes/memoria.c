#include "../cabeceras/memoria.h"
#include "../cabeceras/maquina.h"

uint32_t DireccionFisica(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t cant_bytes_acceso){
    uint16_t offset, segmento;
    uint32_t DireccionFisica, limiteSegmento, limiteAcceso, base;

    segmento = direccionLogica >> 16;
    
    if (segmento >= CANT_SEGMENTOS) // segmento invalido
        return 0; // consultar valor de error

    if (maquina->segmentos[segmento].base == -1 || maquina->segmentos[segmento].tamanio == -1) // segmento sin uso
        return 0;

    base = maquina->segmentos[segmento].base;
    limiteSegmento = maquina->segmentos[segmento].tamanio + base;
    offset = direccionLogica & 0x0000FFFF;

    DireccionFisica = base + offset;
    limiteAcceso = DireccionFisica + cant_bytes_acceso;

    if (DireccionFisica < base ||  limiteAcceso > limiteSegmento) // fuera del segmento
        return 0;

    return DireccionFisica;
}
