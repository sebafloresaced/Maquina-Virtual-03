#include "../cabeceras/memoria.h"
#include "../cabeceras/maquina.h"
#include <stdio.h>
#include <stdlib.h>

uint32_t calculaDirFisica(MaquinaVirtual *maquina, uint32_t direccionLogica)
{
    uint32_t segmento = direccionLogica >> 16;
    uint16_t base = maquina->segmentos[segmento].base;
    uint16_t offset = direccionLogica & 0xFFFF;

    return base+offset;
}

void verificaDirFisica(MaquinaVirtual *maquina, uint32_t direccionFisica, uint16_t bytesAcceso)
{
    uint16_t segmento;
    uint32_t limiteSegmento, limiteAcceso, base;
    uint32_t direccionLogica = maquina->registros[LAR];
    
    //compruebo que el segmento es valido y que el acceso a memoria no se sale de los limites del segmento

    segmento = direccionLogica >> 16;
    
    if ((segmento >= CANT_SEGMENTOS)  || (maquina->segmentos[segmento].base == -1 || maquina->segmentos[segmento].tamanio == -1)) // segmento invalido
       {
        printf("Error: Fallo de segmento (acceso a segmento de memoria invalido)");
        exit(EXIT_FAILURE);
       }

    base = maquina->segmentos[segmento].base;
    limiteSegmento = maquina->segmentos[segmento].tamanio + base;

    limiteAcceso = direccionFisica + bytesAcceso;

    if (!(direccionFisica >= base && limiteAcceso <= limiteSegmento))
    {
        printf("Error: Fallo de segmento (fuera de los limites del segmento)");
        exit(EXIT_FAILURE);
    }
    
}

void leerMemoria(MaquinaVirtual *maquina)
{
    uint32_t valor = 0;
    uint32_t direccionFisica;
    uint16_t bytesAcceso = (maquina->registros[MAR] >> 16);
    int i;

    direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
    verificaDirFisica(maquina, direccionFisica, bytesAcceso);

    maquina->registros[MAR] = ((uint32_t)bytesAcceso << 16) | direccionFisica;
    
    for (i = 0; i < bytesAcceso; i++) { // leo tantos bytes como indique el registro MAR
        valor = (valor << 8) | maquina->memoria[direccionFisica + i];
    }
        
    maquina->registros[MBR] = valor;
    
}

void escribirMemoria(MaquinaVirtual *maquina)
{
    uint32_t direccionFisica;
    uint16_t bytesAcceso = (maquina->registros[MAR] >> 16);
    int i;

    direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
    verificaDirFisica(maquina, direccionFisica, bytesAcceso);

    maquina->registros[MAR] = ((uint32_t)bytesAcceso << 16) | direccionFisica; 

    for (i = 0; i < bytesAcceso; i++) { // escribo tantos bytes como indique el registro MAR
        maquina->memoria[direccionFisica + i] = (maquina->registros[MBR] >> (8 * (bytesAcceso - 1 - i))) & 0xFF;
    } 

}
