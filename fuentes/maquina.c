#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"

void inicializarMaquina(MaquinaVirtual *maquina) {
    int i;
    
    for (i = 0; i < CANT_REGISTROS; i++) {
        maquina->registros[i] = 0;
    }
    for (i = 0; i < CANT_SEGMENTOS; i++) {
        maquina->segmentos[i].base = -1;
        maquina->segmentos[i].tamanio = -1;
    }
    for (i = 0; i < CANT_OPERACIONES; i++) {
        maquina->Operaciones[i] = NULL;
    }

    // operaciones
    maquina->Operaciones[MOV] = operacionMOV;
    maquina->Operaciones[ADD] = operacionADD;
    maquina->Operaciones[SUB] = operacionSUB;
    maquina->Operaciones[MUL] = operacionMUL;
    maquina->Operaciones[DIV] = operacionDIV;
    maquina->Operaciones[CMP] = operacionCMP;
    maquina->Operaciones[AND] = operacionAND;
    maquina->Operaciones[OR] = operacionOR;
    maquina->Operaciones[XOR] = operacionXOR;
    maquina->Operaciones[SWAP] = operacionSWAP;
    maquina->Operaciones[SHL] = operacionSHL;
    maquina->Operaciones[SHR] = operacionSHR;
    maquina->Operaciones[SAR] = operacionSAR;
    maquina->Operaciones[LDL] = operacionLDL;
    maquina->Operaciones[LDH] = operacionLDH;
    maquina->Operaciones[RND] = operacionRND;

    maquina->Operaciones[SYS] = operacionSYS;
    maquina->Operaciones[JMP] = operacionJMP;
    maquina->Operaciones[JP] = operacionJP;
    maquina->Operaciones[JN] = operacionJN;
    maquina->Operaciones[JZ] = operacionJZ;
    maquina->Operaciones[JC] = operacionJC;
    maquina->Operaciones[JV] = operacionJV;
    maquina->Operaciones[JNP] = operacionJNP;
    maquina->Operaciones[JNN] = operacionJNN;
    maquina->Operaciones[JNZ] = operacionJNZ;
    maquina->Operaciones[NOT] = operacionNOT;

    maquina->Operaciones[STOP] = operacionSTOP;
}