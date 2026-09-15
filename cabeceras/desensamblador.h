#ifndef DESENSAMBLADOR_H
#define DESENSAMBLADOR_H

#include "maquina.h"

static char *nombreOperacion(uint8_t codigo);
static char *nombreRegistro(uint8_t codigo);
static char *obtieneOperando(MaquinaVirtual maquina, Operando OP);

void desensamblador(MaquinaVirtual maquina);

#endif