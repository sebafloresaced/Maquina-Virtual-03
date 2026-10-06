#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../cabeceras/maquina.h"
#include "../cabeceras/cargador.h"
#include "../cabeceras/desensamblador.h"

static int terminaEn(const char *texto, const char *extension) {
    size_t longitud = strlen(texto);
    size_t longitudExtension = strlen(extension);
    return longitud >= longitudExtension &&
           strcmp(texto + longitud - longitudExtension, extension) == 0;
}

static void leeArgumentos(int argc, char *argv[], const char **rutaArchivoVmx, const char **rutaArchivoVmi, uint32_t *memoria, int *disassemblerFlag, int *paramSegmentFlag, int *inicioParametros) {
    *rutaArchivoVmx = NULL;
    *rutaArchivoVmi = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            *disassemblerFlag = 1;
        else 
            if (strncmp(argv[i], "m=", 2) == 0) 
                *memoria = strtol(argv[i] + 2, NULL, 10) * 1024; //el parametro esta en KiB por eso se multiplica por 1024
            
            else 
                if (*rutaArchivoVmx != NULL && strcmp(argv[i], "-p") == 0) {
                    *paramSegmentFlag = 1;
                    *inicioParametros = i + 1; 
                    break;
                }
                else 
                    if (terminaEn(argv[i], ".vmx"))
                        *rutaArchivoVmx = argv[i];
                    else 
                        if (terminaEn(argv[i], ".vmi")) 
                            *rutaArchivoVmi = argv[i];
    }

}

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
