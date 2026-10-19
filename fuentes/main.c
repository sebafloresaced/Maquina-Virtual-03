#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../cabeceras/maquina.h"
#include "../cabeceras/cargador.h"
#include "../cabeceras/desensamblador.h"
#include "../cabeceras/imagen.h"

int main(int argc, char *argv[]) {

    MaquinaVirtual maquina;
    int disassemblerFlag = 0;
    int inicioParametros = argc;
    uint8_t version;
    uint32_t entry_point = 0;

    const char *rutaArchivoVmx;
    const char *rutaArchivoVmi;

    maquina.tamanioMemoria = TAM_MEMORIA_POR_DEFECTO;

    leeArgumentos(argc, argv, &rutaArchivoVmx, &rutaArchivoVmi, &maquina.tamanioMemoria, &disassemblerFlag, &inicioParametros);
    int cantidadParametros = argc - inicioParametros;

    srand((unsigned int)time(NULL));
    
    inicializarMaquina(&maquina);

    maquina.rutaImagen = rutaArchivoVmi;
    maquina.pasoAPaso = 0;
    maquina.detenerEjecucion = 0;

    if (rutaArchivoVmx != NULL) {
        cargarPrograma(rutaArchivoVmx, &maquina, &version, cantidadParametros, argv + inicioParametros, &entry_point);
    }
    else 
        if (rutaArchivoVmi != NULL)
            cargarImagen(rutaArchivoVmi, &maquina, &version);
        else {
            printf("Error: no se especifico un archivo .vmx o .vmi\n");
            exit(EXIT_FAILURE);
        }

    if (disassemblerFlag)
        desensamblador(maquina, version, entry_point);

    printf("Iniciando ejecucion del programa:\n");
    cicloPrincipal(&maquina);

    free(maquina.memoria);
    return EXIT_SUCCESS;
}
