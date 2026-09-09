#include "../cabeceras/memoria.h"

uint32_t DireccionFisica(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t cant_bytes_acceso){
    uint16_t offset, segmento;
    uint32_t DireccionFisica, limite, base;

    segmento = direccionLogica >> 16;
    base = maquina->segmentos[segmento].base;
    limite = maquina->segmentos[segmento].tamanio + base;
    offset = direccionLogica & 0x0000FFFF;

    DireccionFisica = base + offset;

    if (DireccionFisica >= base && DireccionFisica + cant_bytes_acceso <= limite)
        return DireccionFisica;
    else
        return 0; //esto es error no esta en el segmento
}
