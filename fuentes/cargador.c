#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../cabeceras/cargador.h"

static uint16_t leer16(FILE *archivo) {
  uint8_t bytes[2];
  fread(bytes, 1, 2, archivo);
  return ((uint16_t)bytes[0] << 8) | bytes[1];
}

static void leerEncabezado(FILE *archivo, uint32_t memoria, uint16_t *tamanioCS, uint16_t *tamanioDS, uint16_t *tamanioES, uint16_t *tamanioSS, uint16_t *tamanioKS, uint16_t *offset_entry) {
  uint8_t version;
  uint8_t identificador[5];

  fread(identificador, 1, 5, archivo);
  fread(&version, 1, 1, archivo);
  *tamanioCS = leer16(archivo);

  if (version == 2){
    *tamanioDS = leer16(archivo);
    *tamanioES = leer16(archivo);
    *tamanioSS = leer16(archivo);
    *tamanioKS = leer16(archivo);
    *offset_entry = leer16(archivo);
  }
  else
     if (version == 1) 
        *tamanioDS = memoria - *tamanioCS;
}

void cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina) {
  FILE *archivo;
  uint16_t tamanioCS = 0, tamanioDS = 0, tamanioES = 0, tamanioSS = 0, tamanioKS = 0, offset_entry = 0; 

  archivo = fopen(nombreArchivo, "rb");

  if (archivo == NULL) {
    printf("No se pudo abrir el archivo\n");
    exit(EXIT_FAILURE);
  }

  leerEncabezado(archivo, maquina->tamanioMemoria, &tamanioCS, &tamanioDS, &tamanioES, &tamanioSS, &tamanioKS, &offset_entry);
  

  // ------ CONFIGURA SEGMENTOS Y REGISTROS ---------

  int segmentosUso = 0;

  // Si existe el Param Segment, ya fue configurado en la función cargaParamSegment, por lo que no se debe volver a inicializar.

  uint16_t tamañoPS = maquina->segmentos[0].tamanio;

  uint32_t tamanioTotal = tamañoPS + tamanioKS + tamanioCS + tamanioDS + tamanioES + tamanioSS;
  if (tamanioTotal > maquina->tamanioMemoria) {
    printf("Error: Memoria insuficiente\n");
    exit(EXIT_FAILURE);
  }

  if (tamañoPS > 0) {
    segmentosUso++;
  }

  // Si un segmento no existe, su tamaño quedo en 0 y no ocupa entradas en la tabla de segmentos.

  //Const Segment (puede no existir)
  if (tamanio.KS > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS;
    maquina->segmentos[segmentosUso].tamanio = tamanioKS;
    maquina->registros[KS] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento KS
    segmentosUso++;
  }

  //Code Segment (siempre existe)
  if (tamanioCS > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS + tamanioKS;
    maquina->segmentos[segmentosUso].tamanio = tamanioCS;
    maquina->registros[CS] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento CS
    segmentosUso++;
  }
      
  //Data Segment (puede no existir)
  if (tamanioDS > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS + tamanioKS + tamanioCS;
    maquina->segmentos[segmentosUso].tamanio = tamanioDS;
    maquina->registros[DS] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento DS
    segmentosUso++;
  }

  //Extra Segment (puede no existir)
  if (tamanioES > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS + tamanioKS + tamanioCS + tamanioDS;
    maquina->segmentos[segmentosUso].tamanio = tamanioES;
    maquina->registros[ES] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento ES
    segmentosUso++;
  }
    
  //Stack Segment (existe siempre)
  if (tamanioSS > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS + tamanioKS + tamanioCS + tamanioDS + tamanioES;
    maquina->segmentos[segmentosUso].tamanio = tamanioSS;
    maquina->registros[SS] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento SS
    maquina->registros[SP] = (segmentosUso << 16) | tamanioSS; // el puntero de pila apunta al final del segmento SS
    segmentosUso++;
  }

  fread(maquina->memoria + maquina->segmentos[2].base, 1, tamanioCS, archivo);

  maquina->registros[IP] = maquina->registros[CS] | offset_entry; //el puntero de instruccion apunta al entry point del programa

  fclose(archivo);
}

static int terminaEn(const char *texto, const char *extension) {
    size_t longitud = strlen(texto);
    size_t longitudExtension = strlen(extension);
    return longitud >= longitudExtension &&
           strcmp(texto + longitud - longitudExtension, extension) == 0;
}

void leeArgumentos(int argc, char *argv[], const char **rutaArchivoVmx, const char **rutaArchivoVmi, uint32_t *memoria, int *disassemblerFlag, int *paramSegmentFlag, int *inicioParametros) {
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