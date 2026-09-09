// Traduce las direcciones según la segmentación y controla las lecturas y escrituras dentro de los límites válidos, retorna una direccion fisica VALIDA.
#include <stdint.h>
#include "../cabeceras/maquina.h"

uint32_t direccionFisica(MaquinaVirtual *, uint32_t, int);
