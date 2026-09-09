// Decodifica las instrucciones; actualiza los registros y flags que correspondan.
// Obtiene el tipo de operandos y la operacion, y los carga en registros

#include <stdint.h>
#include "../cabeceras/maquina.h"

void desensamblaInstruccion(MaquinaVirtual *maquina, uint8_t instruccion, int *desplazamiento);