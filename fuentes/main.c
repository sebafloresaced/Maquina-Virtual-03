#include <stdio.h>
#include <stdlib.h>

#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"
#include "../cabeceras/cargador.h"
#include "../cabeceras/desensamblador.h"

int main(int argc, char *argv[]){ 
    
    MaquinaVirtual maquina;
    
    char *rutaArchivo = (char *)malloc(strlen(argv[1]) + 1);
    strcpy(rutaArchivo, argv[1]);

    int disassemblerFlag = 0;

    if (argc == 3 && strcmp(argv[2], "-d") == 0)
        disassemblerFlag = 1;
    

    inicializarMaquina(&maquina);
    cargarPrograma(&maquina, rutaArchivo); 

    if (disassemblerFlag)
        desensamblador(maquina);
    
    cicloPrincipal(&maquina);

    return 0;
