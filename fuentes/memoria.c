#include "../cabeceras/memoria.h"
#include "../cabeceras/maquina.h"
#include <stdio.h>

void direccionFisica(MaquinaVirtual *maquina, uint32_t direccionLogica, uint32_t *direccionFisica, int16_t bytesAcceso)
{
    uint16_t offset, segmento;
    uint32_t limiteSegmento, direccionFisica, limiteAcceso, base;
    
    //compruebo que el segmento es valido y que el acceso a memoria no se sale de los limites del segmento

    segmento = direccionLogica >> 16;
    
    if ((segmento >= CANT_SEGMENTOS)  || (maquina->segmentos[segmento].base == -1 || maquina->segmentos[segmento].tamanio == -1)) // segmento invalido
       {
        printf("Error: Acceso a segmento invalido\n");
        exit();
       }

    base = maquina->segmentos[segmento].base;
    limiteSegmento = maquina->segmentos[segmento].tamanio + base;
    offset = maquina->registros[LAR] & 0x0000FFFF;

    *direccionFisica = base + offset;
    limiteAcceso = *direccionFisica + bytesAcceso;

    if (!(*direccionFisica >= base && limiteAcceso <= limiteSegmento)) {
        printf("Error: Acceso a memoria fuera de los limites del segmento\n");
        exit();
    }

}

void leerMemoria(MaquinaVirtual *maquina)
{
    int32_t valor;
    uint32_t direccionFisica;
    int16_t bytesAcceso = (maquina->registros[MAR] >> 16) & 0xFF;
    int i;

    direccionFisica(maquina, maquina->registros[LAR], &direccionFisica, bytesAcceso);
    
    for (i = 0; i < bytesAcceso; i++) { // leo tantos bytes como indique el registro MAR
        valor = (valor << 8) | maquina->memoria[direccionFisica + i];
    }
        
    maquina->registros[MBR] = valor;
    
}

void escribeMemoria(MaquinaVirtual *maquina)
{
    uint32_t direccionFisica;
    int16_t bytesAcceso = (maquina->registros[MAR] >> 16) & 0xFF;
    int i;

    direccionFisica(maquina, maquina->registros[LAR], &direccionFisica, bytesAcceso);

    maquina->registros[MAR] = (maquina->registros[MAR] | direccionFisica) // actualizo el registro MAR con la direccion fisica

    for (i = 0; i < bytesAcceso; i++) { // escribo tantos bytes como indique el registro MAR
        maquina->memoria[direccionFisica + i] = (maquina->registros[MBR] >> (24 - 8 * i)) & 0xFF;
    } 

}
