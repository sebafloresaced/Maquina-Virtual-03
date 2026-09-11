// Traduce las direcciones logicas según la segmentación y controla las lecturas y escrituras dentro de los límites válidos.

#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdint.h>
#include "maquina.h"

int DireccionFisica(MaquinaVirtual *);
void leerMemoria32(MaquinaVirtual *);
void escribeMemoria32(MaquinaVirtual *);

#endif