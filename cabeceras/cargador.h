// Abre y valida el .vmx, carga el programa y configura los segmentos y registros iniciales.

#ifndef CARGADOR_H
#define CARGADOR_H

#include "maquina.h"

int cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina);

#endif