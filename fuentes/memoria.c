#include "../cabeceras/memoria.h"
#include "../cabeceras/maquina.h"
#include <stdio.h>

void cargaMAR(MaquinaVirtual *maquina)
{
    uint16_t offset, segmento;
    uint32_t limiteSegmento, direccionFisica, limiteAcceso, base;
    
    int16_t bytesAcceso = (maquina->registros[MAR] >> 16) & 0xFF;
    
    //compruebo que el segmento es valido y que el acceso a memoria no se sale de los limites del segmento

    segmento = maquina->registros[LAR] >> 16;
    
    if ((segmento >= CANT_SEGMENTOS)  || (maquina->segmentos[segmento].base == -1 || maquina->segmentos[segmento].tamanio == -1)) // segmento invalido
       {
        printf("Error: Acceso a segmento invalido\n");
        exit();
       }

    base = maquina->segmentos[segmento].base;
    limiteSegmento = maquina->segmentos[segmento].tamanio + base;
    offset = maquina->registros[LAR] & 0x0000FFFF;

    direccionFisica = base + offset;
    limiteAcceso = direccionFisica + bytesAcceso;

    if (direccionFisica >= base && limiteAcceso <= limiteSegmento) 
        maquina->registros[MAR] = (bytesAcceso << 16) | direccionFisica;
    else{
        printf("Error: Acceso a memoria fuera de los limites del segmento\n");
        exit();
    }

}

void leerMemoria(MaquinaVirtual *maquina)
{
    int32_t valor;
    uint32_t direccionFisica;
    int16_t bytesAcceso;
    int i;

    cargaMAR(maquina);

    direccionFisica = maquina->registros[MAR] & 0x0000FFFF; // obtengo la dirección física
    bytesAcceso = (maquina->registros[MAR] >> 16) & 0xFF;
    
    for (i = 0; i < bytesAcceso; i++) { // leo tantos bytes como indique el registro MAR
        valor = (valor << 8) | maquina->memoria[direccionFisica + i];
    }
        
    maquina->registros[MBR] = valor;
    
}

void escribeMemoria(MaquinaVirtual *maquina)
{
    uint32_t direccionFisica;
    int16_t bytesAcceso;
    int i;

    cargaMAR(maquina);

    direccionFisica = maquina->registros[MAR] & 0x0000FFFF; // obtengo la dirección física
    bytesAcceso = (maquina->registros[MAR] >> 16) & 0xFF;

    for (i = 0; i < bytesAcceso; i++) { // escribo tantos bytes como indique el registro MAR
        maquina->memoria[direccionFisica + i] = (maquina->registros[MBR] >> (24 - 8 * i)) & 0xFF;
    } 

}
