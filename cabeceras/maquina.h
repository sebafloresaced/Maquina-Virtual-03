// Define el estado de la máquina: memoria, registros y tabla de segmentos. Inicializa y coordina la ejecución.

#ifndef MAQUINA_H
#define MAQUINA_H

#include <stdint.h> // define tipos enteros de ancho fijo

#define TAM_MEMORIA 16384 // 16 Kib
#define CANT_REGISTROS 32
#define CANT_SEGMENTOS 8
#define CANT_OPERACIONES 32

// Indices de los registros
#define IP   0
#define OPC  1
#define OP1  2
#define OP2  3
#define LAR  4
#define MAR  5
#define MBR  6
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15
#define AC  16
#define CC  17
#define CS  26
#define DS  27

typedef struct {
    int16_t base;
    int16_t tamanio;
} DescriptorSegmento;

typedef struct MaquinaVirtual MaquinaVirtual;

typedef void(*Operacion)(MaquinaVirtual *);

struct MaquinaVirtual {
    uint8_t memoria[TAM_MEMORIA];
    int32_t registros[CANT_REGISTROS];
    DescriptorSegmento segmentos[CANT_SEGMENTOS];
    Operacion Operaciones[CANT_OPERACIONES];
};

void inicializarMaquina(MaquinaVirtual *maquina);

#endif