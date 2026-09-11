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

uint32_t obtenerValor(MaquinaVirtual *maquina, Operando op);
void escribirValor(MaquinaVirtual *maquina, Operando op, int32_t valor);

void mov(MaquinaVirtual *);
void add(MaquinaVirtual *);
void sub(MaquinaVirtual *);
void mul(MaquinaVirtual *);
void divi(MaquinaVirtual *);
void cmp(MaquinaVirtual *);
void and(MaquinaVirtual *);
void or(MaquinaVirtual *);
void xor(MaquinaVirtual *);
void swap(MaquinaVirtual *);
void shl(MaquinaVirtual *);
void shr(MaquinaVirtual *);
void sar(MaquinaVirtual *);
void ldl(MaquinaVirtual *);
void ldh(MaquinaVirtual *);
void rnd(MaquinaVirtual *);
void sys(MaquinaVirtual *);
void jmp(MaquinaVirtual *);
void jp(MaquinaVirtual *);
void jN(MaquinaVirtual *);
void jz(MaquinaVirtual *);
void jc(MaquinaVirtual *);
void jv(MaquinaVirtual *);
void jnp(MaquinaVirtual *);
void jnn(MaquinaVirtual *);
void jnz(MaquinaVirtual *);
void not(MaquinaVirtual *);
void stop(MaquinaVirtual *);

#endif