#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"

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
    maquina->Operaciones[SUB] = sub;
    maquina->Operaciones[MUL] = mul;
    maquina->Operaciones[DIV] = divi;
    maquina->Operaciones[CMP] = cmp;
    maquina->Operaciones[AND] = and;
    maquina->Operaciones[OR] = or;
    maquina->Operaciones[XOR] = xor;
    maquina->Operaciones[SWAP] = swap;
    maquina->Operaciones[SHL] = shl;
    maquina->Operaciones[SHR] = shr;
    maquina->Operaciones[SAR] = sar;
    maquina->Operaciones[LDL] = ldl;
    maquina->Operaciones[LDH] = ldh;
    maquina->Operaciones[RND] = rnd;

    maquina->Operaciones[SYS] = sys;
    maquina->Operaciones[JMP] = jmp;
    maquina->Operaciones[JP] = jp;
    maquina->Operaciones[JN] = jN;
    maquina->Operaciones[JZ] = jz;
    maquina->Operaciones[JC] = jc;
    maquina->Operaciones[JV] = jv;
    maquina->Operaciones[JNP] = jnp;
    maquina->Operaciones[JNN] = jnn;
    maquina->Operaciones[JNZ] = jnz;
    maquina->Operaciones[NOT] = not;

    maquina->Operaciones[STOP] = stop;
}