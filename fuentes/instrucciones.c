#include "../cabeceras/instrucciones.h"
#include "../cabeceras/maquina.h"
#include "../cabeceras/memoria.h"
#include "../cabeceras/operaciones.h"

static void cargarOperandos(MaquinaVirtual *maquina, uint8_t bytes[]) {
    int tamanioA, tamanioB;
    int posicion = 0;
    int i;
    uint32_t valorA = 0;
    uint32_t valorB = 0;

    tamanioA = maquina->registros[OP1] >> 24;
    tamanioB = maquina->registros[OP2] >> 24;

    for (i = 0; i < tamanioB; i++) { // cargo primero el OPB
        valorB = (valorB << 8) | bytes[posicion];
        posicion++;
    }

    for (i = 0; i < tamanioA; i++) {
        valorA = (valorA << 8) | bytes[posicion];
        posicion++;
    }

    maquina->registros[OP1] |= valorA;
    maquina->registros[OP2] |= valorB;
}

static void leerOperandos(MaquinaVirtual *maquina, int desplazamiento) {
    uint8_t bytes[6] = {0};
    uint32_t direccionFisica;
    int i;

    verificaDirFisica(maquina, maquina->registros[IP], &direccionFisica, desplazamiento + 1);

    for (i = 0; i < desplazamiento; i++) {
        bytes[i] = maquina->memoria[direccionFisica + i + 1];
    }
    
    cargarOperandos(maquina, bytes);
}


void desensamblaInstruccion(MaquinaVirtual *maquina, uint8_t instruccion, int *desplazamiento){ // desplazamiento tiene el tamanio total de los operandos
    uint8_t OPA, OPB, OPERACION;
    
    if (instruccion & 0x10) { //2 operandos
        OPB = (instruccion & 0xC0) >> 6;
        OPA = (instruccion & 0x30) >> 4;
    }
    else{ //1 operando o 0
        OPA = (instruccion & 0xC0) >> 6;
        OPB = 0;
    }

    *desplazamiento = OPA + OPB;
    maquina->registros[OP1] = OPA << 24; //lo copia en el byte mas significativo del registro
    maquina->registros[OP2] = OPB << 24;

    OPERACION = instruccion & 0x1F;
    maquina->registros[OPC] = OPERACION;

    leerOperandos(maquina, *desplazamiento);
}