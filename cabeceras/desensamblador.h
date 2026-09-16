#ifndef DESENSAMBLADOR_H
#define DESENSAMBLADOR_H

#include "maquina.h"

typedef char st15[30];
typedef char st6[6];

static void nombreOperacion(uint8_t codigo, st15 nombre);
static void nombreRegistro(uint8_t codigo, st15 nombre);
static void obtieneOperando(Operando OP, st15 nombre);
void desensamblador(MaquinaVirtual maquina);

#endif