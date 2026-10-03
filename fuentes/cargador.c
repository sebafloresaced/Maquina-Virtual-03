#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../cabeceras/cargador.h"

static uint16_t leer16(FILE *archivo) {
  uint8_t bytes[2];
  fread(bytes, 1, 2, archivo);
  return ((uint16_t)bytes[0] << 8) | bytes[1];
}

static void leerEncabezado(FILE *archivo, uint32_t memoria, uint16_t *tamanioCS, uint16_t *tamanioDS, uint16_t *tamanioES, uint16_t *tamanioSS, uint16_t *tamanioConstS, uint16_t *offset_entry) {
  uint8_t version;
  uint8_t identificador[5];

  fread(identificador, 1, 5, archivo);
  fread(&version, 1, 1, archivo);
  *tamanioCS = leer16(archivo);

  if (version == 2){
    *tamanioDS = leer16(archivo);
    *tamanioES = leer16(archivo);
    *tamanioSS = leer16(archivo);
    *tamanioConstS = leer16(archivo);
    *offset_entry = leer16(archivo);
  }
  else
     if (version == 1) 
        *tamanioDS = memoria - *tamanioCS;
}

void cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina) {
  FILE *archivo;
  uint16_t tamanioCS = 0, tamanioDS = 0, tamanioES = 0, tamanioSS = 0, tamanioConstS = 0, offset_entry = 0; 

  archivo = fopen(nombreArchivo, "rb");

  if (archivo == NULL) {
    printf("No se pudo abrir el archivo\n");
    exit(EXIT_FAILURE);
  }

  leerEncabezado(archivo, maquina->tamanioMemoria, &tamanioCS, &tamanioDS, &tamanioES, &tamanioSS, &tamanioConstS, &offset_entry);
  

  // ------ CONFIGURA SEGMENTOS ---------


  // Si existe el Param Segment, ya fue configurado en la función cargaParamSegment, por lo que no se debe volver a inicializar.
  

  uint8_t tamañoPS = maquina->segmentos[0].tamanio;

  // Si un segmento no existe, su tamaño quedo en 0. (se reserva su espacio en la tabla de segmentos pero no va a afectar al acceso sobre la memoria principal)

  //Const Segment (puede no existir)
  maquina->segmentos[1].base = tamañoPS;
  maquina->segmentos[1].tamanio = tamanioConstS;

  //Code Segment (siempre existe)
  maquina->segmentos[2].base = tamañoPS + tamanioConstS; 
  maquina->segmentos[2].tamanio = tamanioCS;
      
  //Data Segment (puede no existir)

  maquina->segmentos[3].base = tamañoPS + tamanioConstS + tamanioCS;
  maquina->segmentos[3].tamanio = tamanioDS;

  //Extra Segment (puede no existir)
  maquina->segmentos[4].base = tamañoPS + tamanioConstS + tamanioCS + tamanioDS;
  maquina->segmentos[4].tamanio = tamanioES;
    
  //Stack Segment (existe siempre)
  maquina->segmentos[5].base = tamañoPS + tamanioConstS + tamanioCS + tamanioDS + tamanioES;
  maquina->segmentos[5].tamanio = tamanioSS;


  if ((uint32_t) (tamanioCS + tamanioDS + tamanioES + tamanioSS + tamanioConstS + tamañoPS) > maquina->tamanioMemoria) {
    printf("Error: Memoria insuficiente\n");
    exit(EXIT_FAILURE);
  }

  fread(maquina->memoria + maquina->segmentos[2].base, 1, tamanioCS, archivo);
  
  // ---- REGISTROS ----

  //KS
  if(tamanioConstS > 0)        
    maquina->registros[KS] = 0x00010000;
  else
    maquina->registros[KS] = 0xFFFFFFFF;
 
  //CS
  maquina->registros[CS] = 0x00020000;
 
  //DS
  if(tamanioDS > 0)
    maquina->registros[DS] = 0x00030000;
  else
    maquina->registros[DS] = 0xFFFFFFFF;

  //ES
  if(tamanioES > 0)
    maquina->registros[ES] = 0x00040000;
  else
    maquina->registros[ES] = 0xFFFFFFFF;

  //SS
  maquina->registros[SS] = 0x00050000 || (tamanioSS + sizeof(uint8_t)); //el puntero a pila apunta fuera del segmento (le sumo un byte) 

  maquina->registros[IP] = maquina->registros[CS] | offset_entry; //el puntero de instruccion apunta al entry point del programa

  fclose(archivo);
}
