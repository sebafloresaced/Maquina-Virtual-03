// Traduce las direcciones logicas según la segmentación y controla las lecturas y escrituras dentro de los límites válidos, retorna una direccion fisica VALIDA.
#include <stdint.h>
#include "../cabeceras/maquina.h"

uint32_t DireccionFisica(MaquinaVirtual *, uint32_t, uint32_t);
