#include <stdio.h>
#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"


int main(void){
    MaquinaVirtual maquina;
    inicializarMaquina(&maquina);

    maquina.registros[OP1] = 0x0100000A; // Registro EAX 10
    maquina.registros[OP2] = 0x0100000B; // Registro EBX 11
    maquina.registros[EAX] = 10;
    

    printf("Valor en EAX: %d\n", maquina.registros[EAX]);

    maquina.Operaciones[MOV](&maquina);

    maquina.registros[OP1] = 0x0100000A; // Registro EAX 10
    maquina.registros[OP2] = 0x02000002; // inmediato 2



    printf("Valor en EAX después de MOV: %d\n", maquina.registros[EAX]);

    maquina.Operaciones[ADD](&maquina);

    printf("Valor en EAX después de ADD: %d\n", maquina.registros[EAX]);

    return 0;
}