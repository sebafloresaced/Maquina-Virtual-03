// Traduce las direcciones logicas según la segmentación y controla las lecturas y escrituras dentro de los límites válidos.

#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdint.h>
#include "maquina.h"

uint32_t calculaDirFisica(MaquinaVirtual *, uint32_t);
void verificaDirFisica(MaquinaVirtual *, uint32_t, uint16_t);
void leerMemoria(MaquinaVirtual *);
void escribirMemoria(MaquinaVirtual *);

#endif