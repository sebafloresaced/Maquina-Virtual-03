#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"
#include <stdlib.h>

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

void cicloPrincipal(MaquinaVirtual *maquina) {
    
    int desplazamiento;
    uint32_t direccionFisica;
    uint8_t instruccion;
    // IP esta inicializado en 0 en cargador.c

    while (maquina->registros[IP] != -1) {
        
        verificaDirFisica(maquina, maquina->registros[IP], &direccionFisica, 0);

        instruccion = maquina->memoria[direccionFisica];

        desensamblaInstruccion(maquina, instruccion, &desplazamiento);
        
        maquina->registros[IP] += desplazamiento + 1;

        if (maquina->registros[OPC] >= CANT_OPERACIONES || maquina->Operaciones[maquina->registros[OPC]] == NULL) {
            printf("Error: operación inválida\n");
            exit(EXIT_FAILURE);
        }

        maquina->Operaciones[maquina->registros[OPC]](maquina);
    }

    printf("Fin de la ejecución\n");

}