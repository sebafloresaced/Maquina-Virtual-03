#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../cabeceras/maquina.h"
#include "../cabeceras/cargador.h"
#include "../cabeceras/desensamblador.h"

int main(int argc, char *argv[]) {

    MaquinaVirtual maquina;
    int disassemblerFlag = 0;
    int paramSegmentFlag = 0;
    int inicioParametros = -1;

    const char *rutaArchivoVmx;
    const char *rutaArchivoVmi;

    maquina.tamanioMemoria = TAM_MEMORIA_POR_DEFECTO;

    leeArgumentos(argc, argv, &rutaArchivoVmx, &rutaArchivoVmi, &maquina.tamanioMemoria, &disassemblerFlag, &paramSegmentFlag, &inicioParametros);
    int cantidadParametros = argc - inicioParametros;

    srand((unsigned int)time(NULL));
    
    inicializarMaquina(&maquina);

   /* if (paramSegmentFlag)
        cargarParamSegment(argc,argv, &maquina);*/

    if (rutaArchivoVmx != NULL)
        cargarPrograma(rutaArchivoVmx, &maquina);
    /*else 
        if (rutaArchivoVmi != NULL)
            cargarImagen(rutaArchivoVmi, &maquina);
        */

    if (disassemblerFlag)
        desensamblador(maquina);

    printf("\nIniciando ejecucion del programa:\n");
    cicloPrincipal(&maquina);

    free(maquina.memoria);
    return EXIT_SUCCESS;
}
