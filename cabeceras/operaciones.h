// Se encarga de recibir los operandos y realizar la operacion correspondiente

#ifndef OPERACIONES_H
#define OPERACIONES_H

#include <stdint.h>
#include "maquina.h"

//tipos de operando
#define NINGUNO 0
#define REGISTRO 1
#define INMEDIATO 2
#define MEMORIA 3

//operaciones
#define MOV 0x10
#define ADD 0x11
#define SUB 0x12
#define MUL 0x13
#define DIV 0x14
#define CMP 0x15
#define AND 0x16
#define OR  0x17
#define XOR 0x18
#define SWAP 0x19
#define SHL 0x1A
#define SHR 0x1B
#define SAR 0x1C
#define LDL 0x1D
#define LDH 0x1E
#define RND 0x1F

#define SYS 0x00
#define JMP 0x01
#define JP  0x02
#define JN  0x03
#define JZ  0x04
#define JC  0x05
#define JV  0x06
#define JNP 0x07
#define JNN 0x08
#define JNZ 0x09
#define NOT 0x0A

#define STOP 0x0F

typedef uint32_t Operando;

void operacionMOV(MaquinaVirtual *);
void operacionADD(MaquinaVirtual *);
void operacionSUB(MaquinaVirtual *);
void operacionMUL(MaquinaVirtual *);
void operacionDIV(MaquinaVirtual *);
void operacionCMP(MaquinaVirtual *);
void operacionAND(MaquinaVirtual *);
void operacionOR(MaquinaVirtual *);
void operacionXOR(MaquinaVirtual *);
void operacionSWAP(MaquinaVirtual *);
void operacionSHL(MaquinaVirtual *);
void operacionSHR(MaquinaVirtual *);
void operacionSAR(MaquinaVirtual *);
void operacionLDL(MaquinaVirtual *);
void operacionLDH(MaquinaVirtual *);
void operacionRND(MaquinaVirtual *);
void operacionSYS(MaquinaVirtual *);
void operacionJMP(MaquinaVirtual *);
void operacionJP(MaquinaVirtual *);
void operacionJN(MaquinaVirtual *);
void operacionJZ(MaquinaVirtual *);
void operacionJC(MaquinaVirtual *);
void operacionJV(MaquinaVirtual *);
void operacionJNP(MaquinaVirtual *);
void operacionJNN(MaquinaVirtual *);
void operacionJNZ(MaquinaVirtual *);
void operacionNOT(MaquinaVirtual *);
void operacionSTOP(MaquinaVirtual *);

#endif