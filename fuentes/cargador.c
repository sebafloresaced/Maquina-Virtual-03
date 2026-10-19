#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../cabeceras/cargador.h"
#include "../cabeceras/maquina.h"
#include "../cabeceras/memoria.h"

static uint16_t leer16(FILE *archivo) {
  uint8_t bytes[2];
  fread(bytes, 1, 2, archivo);
  return ((uint16_t)bytes[0] << 8) | bytes[1];
}

static void leerEncabezado(FILE *archivo, uint32_t memoria, uint16_t *tamanioCS, uint16_t *tamanioDS, uint16_t *tamanioES, uint16_t *tamanioSS, uint16_t *tamanioKS, uint16_t *offset_entry, uint8_t *version) {
  uint8_t identificador[5];

  fread(identificador, 1, 5, archivo);
  fread(version, 1, 1, archivo);
  *tamanioCS = leer16(archivo);

  if (*version == 2){
    *tamanioDS = leer16(archivo);
    *tamanioES = leer16(archivo);
    *tamanioSS = leer16(archivo);
    *tamanioKS = leer16(archivo);
    *offset_entry = leer16(archivo);
  }
  else
    if (*version == 1)
      *tamanioDS = memoria - *tamanioCS;
    else {
      printf("Error: Version invalida");
      exit(EXIT_FAILURE);
    }
}

static void cargarParamSegment(MaquinaVirtual *maquina, const char *nombrePrograma, int cantidadParametros, char *parametros[], uint32_t *cantidadArgumentos, uint32_t *direccionArgv) {
    maquina->registros[PS] = -1;
    *cantidadArgumentos = 0;
    *direccionArgv = UINT32_MAX;

    if (cantidadParametros == 0)
        return;

    // Incluye el nombre del programa como argumento 0.
    size_t cantidad = (size_t)cantidadParametros + 1;

    size_t bytesPunteros = cantidad * 4;
    size_t bytesStrings = 0;

    // Calcula el espacio necesario incluyendo cada '\0'.
    for (size_t i = 0; i < cantidad; i++) {
        const char *texto = (i == 0) ? nombrePrograma : parametros[i - 1];
        size_t longitud = strlen(texto);
        if (longitud >= UINT16_MAX - bytesPunteros - bytesStrings) {
            printf("Error: Param Segment demasiado grande\n");
            exit(EXIT_FAILURE);
        }
        bytesStrings += longitud + 1;
    }

    size_t tamanioPS = bytesStrings + bytesPunteros;

    if (tamanioPS > maquina->tamanioMemoria) {
        printf("Error: memoria insuficiente para parametros\n");
        exit(EXIT_FAILURE);
    }

    // PS siempre es el primer segmento cuando existe.
    maquina->segmentos[0].base = 0;
    maquina->segmentos[0].tamanio = (uint16_t)tamanioPS;
    maquina->registros[PS] = 0;

    uint32_t offsetString = 0;
    uint32_t offsetArgv = (uint32_t)bytesStrings;

    for (size_t i = 0; i < cantidad; i++) {
        const char *texto = (i == 0) ? nombrePrograma : parametros[i - 1];
        size_t longitud = strlen(texto) + 1;

        memcpy(maquina->memoria + offsetString, texto, longitud); // copia una cantidad de bytes de un lugar de memoria a otro

        // Direccion logica: segmento 0 + offset del string.
        uint32_t puntero = offsetString;
        uint32_t posicion = offsetArgv + (uint32_t)i * 4;

        // Guarda el puntero en big endian.
        maquina->memoria[posicion] = (uint8_t)(puntero >> 24);
        maquina->memoria[posicion + 1] = (uint8_t)(puntero >> 16);
        maquina->memoria[posicion + 2] = (uint8_t)(puntero >> 8);
        maquina->memoria[posicion + 3] = (uint8_t)puntero;

        offsetString += (uint32_t)longitud;
    }

    *cantidadArgumentos = (uint32_t)cantidad;
    *direccionArgv = offsetArgv;
}

static void inicializarPila(MaquinaVirtual *maquina, uint32_t cantidadArgumentos, uint32_t direccionArgv)
{
    uint32_t indiceSS = (uint32_t)maquina->registros[SS] >> 16;

    if (indiceSS >= CANT_SEGMENTOS || maquina->segmentos[indiceSS].tamanio < 12) {
        printf("Error: pila insuficiente para iniciar el programa\n");
        exit(EXIT_FAILURE);
    }

    uint32_t valores[3] = { direccionArgv, cantidadArgumentos, -1 }; // argv, argc, return

    for (int i = 0; i < 3; i++) {
        maquina->registros[SP] -= 4;

        maquina->registros[LAR] = maquina->registros[SP];
        maquina->registros[MAR] = 4 << 16;
        maquina->registros[MBR] = (int32_t)valores[i];

        escribirMemoria(maquina);
    }
}

void cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina, uint8_t *version, int cantidadParametros, char *parametros[], uint32_t *entry_point) {
  FILE *archivo;
  uint16_t tamanioCS = 0, tamanioDS = 0, tamanioES = 0, tamanioSS = 0, tamanioKS = 0, offset_entry = 0; 

  archivo = fopen(nombreArchivo, "rb");

  if (archivo == NULL) {
    printf("No se pudo abrir el archivo\n");
    exit(EXIT_FAILURE);
  }

  leerEncabezado(archivo, maquina->tamanioMemoria, &tamanioCS, &tamanioDS, &tamanioES, &tamanioSS, &tamanioKS, &offset_entry, version);
  
  uint32_t cantidadArgumentos = 0;
  uint32_t direccionArgv = UINT32_MAX;

  if (*version == 2) {
    cargarParamSegment(maquina, nombreArchivo, cantidadParametros, parametros, &cantidadArgumentos, &direccionArgv);
  }

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
  else {
    maquina->registros[PS] = -1;
  }

  // Si un segmento no existe, su tamaño quedo en 0 y no ocupa entradas en la tabla de segmentos.

  //Const Segment (puede no existir)
  if (tamanioKS > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS;
    maquina->segmentos[segmentosUso].tamanio = tamanioKS;
    maquina->registros[KS] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento KS
    segmentosUso++;
  }
  else {
    maquina->registros[KS] = -1;
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
  else {
    maquina->registros[DS] = -1;
  }

  //Extra Segment (puede no existir)
  if (tamanioES > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS + tamanioKS + tamanioCS + tamanioDS;
    maquina->segmentos[segmentosUso].tamanio = tamanioES;
    maquina->registros[ES] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento ES
    segmentosUso++;
  }
  else {
    maquina->registros[ES] = -1;
  }
    
  //Stack Segment (existe siempre)
  if (tamanioSS > 0) {
    maquina->segmentos[segmentosUso].base = tamañoPS + tamanioKS + tamanioCS + tamanioDS + tamanioES;
    maquina->segmentos[segmentosUso].tamanio = tamanioSS;
    maquina->registros[SS] = (segmentosUso << 16); // el puntero de segmento apunta al inicio del segmento SS
    maquina->registros[SP] = (segmentosUso << 16) | tamanioSS; // el puntero de pila apunta al final del segmento SS
    segmentosUso++;
  }

  // Cargo el segmento de código en memoria
  uint32_t indiceCS = (uint32_t)maquina->registros[CS] >> 16;
  if (indiceCS >= CANT_SEGMENTOS || maquina->segmentos[indiceCS].tamanio == 0) {
    printf("Error: descriptor de segmento de codigo invalido\n");
    fclose(archivo);
    exit(EXIT_FAILURE);
  }

  if (fread(maquina->memoria + maquina->segmentos[indiceCS].base, 1, tamanioCS, archivo) != tamanioCS) {
    printf("Error: no se pudo leer el segmento de codigo completo\n");
    fclose(archivo);
    exit(EXIT_FAILURE);
  }

  // Cargo el segmento de constantes en memoria
  if (tamanioKS > 0) {
    uint32_t indiceKS = (uint32_t)maquina->registros[KS] >> 16;
    fread(maquina->memoria + maquina->segmentos[indiceKS].base, 1, tamanioKS, archivo);
  }

  *entry_point = maquina->registros[CS] | offset_entry;
  maquina->registros[IP] = *entry_point; // el puntero de instruccion apunta al entry point del programa

  if (*version == 2) {
    inicializarPila(maquina, cantidadArgumentos, direccionArgv);
  }

  fclose(archivo);
}

static int terminaEn(const char *texto, const char *extension) {
    size_t longitud = strlen(texto);
    size_t longitudExtension = strlen(extension);
    return longitud >= longitudExtension &&
           strcmp(texto + longitud - longitudExtension, extension) == 0;
}

void leeArgumentos(int argc, char *argv[], const char **rutaArchivoVmx, const char **rutaArchivoVmi, uint32_t *memoria, int *disassemblerFlag, int *inicioParametros) {
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