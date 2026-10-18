#ifndef IMAGEN_H
#define IMAGEN_H

#include "../cabeceras/maquina.h"

void cargarImagen(const char *rutaArchivo, MaquinaVirtual *maquina, uint8_t *version);
void crearImagen(const char *rutaArchivo, const MaquinaVirtual *maquina);

#endif