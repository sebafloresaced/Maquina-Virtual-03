#include "../cabeceras/maquina.h"

void inicializarMaquina(MaquinaVirtual *maquina) {
    int i;
    for (i = 0; i < TAM_MEMORIA; i++) { // consultar si hacer o no
        maquina->memoria[i] = 0;
    }
    for (i = 0; i < CANT_REGISTROS; i++) {
        maquina->registros[i] = 0;
    }
    for (i = 0; i < CANT_SEGMENTOS; i++) {
        maquina->segmentos[i].base = -1;
        maquina->segmentos[i].tamanio = -1;
    }

    // operaciones

    maquina->Operaciones[MOV] = mov;
    maquina->Operaciones[ADD] = add;
}