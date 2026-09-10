// Traduce las direcciones logicas según la segmentación y controla las lecturas y escrituras dentro de los límites válidos, retorna una direccion fisica VALIDA.
#include <stdint.h>
#include "../cabeceras/maquina.h"

int DireccionFisica(MaquinaVirtual *, uint32_t , uint32_t *);
uint32_t leerMemoria32(MaquinaVirtual *, uint32_t );
void escribeMemoria32(MaquinaVirtual *, uint32_t , uint32_t);