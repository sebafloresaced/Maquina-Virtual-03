// Implementa las operaciones de SYS (leer del teclado y escribir en pantalla).

#ifndef LLAMADAS_SISTEMA_H
#define LLAMADAS_SISTEMA_H

#include "maquina.h"
#include <stdint.h>

// Operaciones SYS
#define LEER 1
#define ESCRIBIR 2

// Formatos
#define DECIMAL 1
#define CARACTER 2
#define OCTAL 4
#define HEXADECIMAL 8
#define BINARIO 16

void leerDatos(MaquinaVirtual *maquina);
void escribirDatos(MaquinaVirtual *maquina);

#endif