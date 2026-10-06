// Implementa las operaciones de SYS (leer del teclado y escribir en pantalla).

#ifndef LLAMADAS_SISTEMA_H
#define LLAMADAS_SISTEMA_H

#include "maquina.h"
#include <stdint.h>

// Operaciones SYS
#define CLEAR 0
#define LEER 1
#define ESCRIBIR 2
#define STRING_READ 3
#define STRING_WRITE 4
#define BREAKPOINT 15

// Formatos
#define DECIMAL 1
#define CARACTER 2
#define OCTAL 4
#define HEXADECIMAL 8
#define BINARIO 16

void leerDatos(MaquinaVirtual *maquina);
void escribirDatos(MaquinaVirtual *maquina);

// funcion para limpiar pantalla
// funciones para leer y escribir datos en la memoria de la máquina virtual
// funcion para breakpoint, que detiene la ejecución de la máquina virtual y permite inspeccionar su estado

#endif