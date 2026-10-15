// Abre y valida el .vmx, carga el programa y configura los segmentos y registros iniciales.

#ifndef CARGADOR_H
#define CARGADOR_H

#include "maquina.h"

void cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina, uint8_t *version);
void leeArgumentos(int argc, char *argv[], const char **rutaArchivoVmx, const char **rutaArchivoVmi, uint32_t *memoria, int *disassemblerFlag, int *paramSegmentFlag, int *inicioParametros);
void cargarParamSegment(MaquinaVirtual *maquina, const char *nombrePrograma, int cantidadParametros, char *parametros[], uint32_t *cantidadArgumentos, uint32_t *direccionArgv);
void inicializarPila(MaquinaVirtual *maquina, uint32_t cantidadArgumentos, uint32_t direccionArgv);

#endif