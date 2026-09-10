#include "../cabeceras/memoria.h"

int DireccionFisica(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t *direccionFisica){
    uint16_t offset, segmento;
    uint32_t limiteSegmento, limiteAcceso, base;

    segmento = direccionLogica >> 16;
    base = maquina->segmentos[segmento].base;
    limiteSegmento = maquina->segmentos[segmento].tamanio + base;
    offset = direccionLogica & 0x0000FFFF;

    *direccionFisica = base + offset;
    limiteAcceso = DireccionFisica + (sizeof(uint32_t));

    if (*DireccionFisica >= base &&  limiteAcceso<= limiteSegmento)
        return 1;
    else
        return 0; //esto es error no esta en el segmento
}

uint32_t leerMemoria32(MaquinaVirtual *maquina, uint32_t direccionLogica)
{
    uint32_t valor, direccionFisica;

    if (DireccionFisica(maquina, direccionLogica, &direccionFisica)) {

        return maquina->memoria[direccionFisica];
    }
    else
       //Error de acceso a memoria
       return 0;
}

void escribeMemoria32(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t valor)
{
    uint32_t valor, direccionFisica;

    if (DireccionFisica(maquina, direccionLogica, &direccionFisica)) {

        return maquina->memoria[direccionFisica] = valor;
    }
    else
       //Error de acceso a memoria
       return 0;
}