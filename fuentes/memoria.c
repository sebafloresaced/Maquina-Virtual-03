#include "../cabeceras/memoria.h"

int DireccionFisica(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t *direccionFisica, uint32_t bytesAcceso)
{
    uint16_t offset, segmento;
    uint32_t limiteSegmento, limiteAcceso, base;

    segmento = direccionLogica >> 16;
    base = maquina->segmentos[segmento].base;
    limiteSegmento = maquina->segmentos[segmento].tamanio + base;
    offset = direccionLogica & 0x0000FFFF;

    *direccionFisica = base + offset;
    limiteAcceso = *direccionFisica + bytesAcceso;

    if (*direccionFisica >= base && limiteAcceso <= limiteSegmento)
        return 1;
    else
        return 0; // esto es error: no está en el segmento
}

uint32_t leerMemoria32(MaquinaVirtual *maquina, uint32_t direccionLogica)
{
    uint32_t valor = 0;
    uint32_t direccionFisica;

    if (DireccionFisica(maquina, direccionLogica, &direccionFisica, sizeof(uint32_t))) {
        valor = maquina->memoria[direccionFisica];
    }

    return valor;
}

void escribeMemoria32(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t valor)
{
    uint32_t direccionFisica;

    if (DireccionFisica(maquina, direccionLogica, &direccionFisica, sizeof(uint32_t))) {
        maquina->memoria[direccionFisica] = valor;
    }
}