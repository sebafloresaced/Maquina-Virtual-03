#include "../cabeceras/memoria.h"
#include "../cabeceras/maquina.h"

static void actualizarRegistrosMemoria(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t direccionFisica, uint32_t bytesAcceso, uint32_t valor) 
{
    maquina->registros[LAR] = direccionLogica;
    maquina->registros[MAR] = (bytesAcceso << 16) | direccionFisica;
    maquina->registros[MBR] = valor;
}

int DireccionFisica(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t *direccionFisica, uint32_t bytesAcceso)
{
    uint16_t offset, segmento;
    uint32_t limiteSegmento, limiteAcceso, base;

    segmento = direccionLogica >> 16;
    
    if (segmento >= CANT_SEGMENTOS) // segmento invalido
        return 0; // consultar valor de error

    if (maquina->segmentos[segmento].base == -1 || maquina->segmentos[segmento].tamanio == -1) // segmento sin uso
        return 0;

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
    int i;

    if (DireccionFisica(maquina, direccionLogica, &direccionFisica, sizeof(uint32_t)) != 0) { // no da error
        for (i = 0; i < 4; i++) { // leo cuatro bytes
            valor = (valor << 8) | maquina->memoria[direccionFisica + i];
        }
        actualizarRegistrosMemoria(maquina,direccionLogica,*direccionFisica,4,valor);
    }

    return valor;
}

void escribeMemoria32(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t valor)
{
    uint32_t direccionFisica;

    if (DireccionFisica(maquina, direccionLogica, &direccionFisica, sizeof(uint32_t)) != 0) { // no da error
        for (i = 0; i < 4; i++) { // escribo cuatro bytes
            maquina->memoria[direccionFisica + i] = (valor >> (24 - 8 * i)) & 0xFF;
        } 
        actualizarRegistrosMemoria(maquina,direccionLogica,*direccionFisica,4,valor);
    }
    else {
        printf("Error");
    }
}
