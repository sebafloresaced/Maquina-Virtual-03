#include "../cabeceras/maquina.h"

void desensamblaInstruccion(MaquinaVirtual *maquina, uint8_t instruccion, int *desplazamiento){
    uint8_t OPA, OPB, OPERACION;
    if (instruccion & 0x10) { //2 operandos
        OPB = (instruccion & 0xC0) >> 6;
        OPA = (instruccion & 0x30) >> 4;

        maquina->registros[OP1] = OPA << 24; //lo copia en el byte mas significativo del registro
        maquina->registros[OP2] = OPB << 24;
        
    }
    else{ //1 operando o 0
        OPA = (instruccion & 0xC0) >> 6;
        OPB = 0;

        maquina->registros[OP1] = OPA << 24;
        maquina->registros[OP2] = OPB;

    }
    
    *desplazamiento = OPA + OPB;

    OPERACION = instruccion & 0x1F;
    maquina->registros[OPC] = OPERACION;

}