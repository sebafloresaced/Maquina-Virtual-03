// Abre y valida el .vmx, carga el programa y configura los segmentos y registros iniciales.

#ifndef CARGADOR_H
#define CARGADOR_H

#include "maquina.h"

void cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina);
void leeArgumentos(int argc, char *argv[], const char **rutaArchivoVmx, const char **rutaArchivoVmi, uint32_t *memoria, int *disassemblerFlag, int *paramSegmentFlag, int *inicioParametros);

#endif