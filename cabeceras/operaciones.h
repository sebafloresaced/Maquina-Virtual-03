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

uint32_t obtenerValor(MaquinaVirtual *maquina, Operando op);
void escribirValor(MaquinaVirtual *maquina, Operando op, uint32_t valor);

void mov(MaquinaVirtual *, uint32_t, uint32_t);
void add(MaquinaVirtual *, uint32_t, uint32_t);
void sub(MaquinaVirtual *, uint32_t, uint32_t);
void mul(MaquinaVirtual *, uint32_t, uint32_t);
void div(MaquinaVirtual *, uint32_t, uint32_t);
void cmp(MaquinaVirtual *, uint32_t, uint32_t);
void and(MaquinaVirtual *, uint32_t, uint32_t);
void or(MaquinaVirtual *, uint32_t, uint32_t);
void xor(MaquinaVirtual *, uint32_t, uint32_t);
void swap(MaquinaVirtual *, uint32_t, uint32_t);
void shl(MaquinaVirtual *, uint32_t, uint32_t);
void shr(MaquinaVirtual *, uint32_t, uint32_t);
void sar(MaquinaVirtual *, uint32_t, uint32_t);
void ldl(MaquinaVirtual *, uint32_t, uint32_t);
void ldh(MaquinaVirtual *, uint32_t, uint32_t);
void rnd(MaquinaVirtual *, uint32_t, uint32_t);
void sys(MaquinaVirtual *, uint32_t, uint32_t);
void jmp(MaquinaVirtual *, uint32_t, uint32_t);
void jp(MaquinaVirtual *, uint32_t, uint32_t);
void jn(MaquinaVirtual *, uint32_t, uint32_t);
void jz(MaquinaVirtual *, uint32_t, uint32_t);
void jc(MaquinaVirtual *, uint32_t, uint32_t);
void jv(MaquinaVirtual *, uint32_t, uint32_t);
void jnp(MaquinaVirtual *, uint32_t, uint32_t);
void jnn(MaquinaVirtual *, uint32_t, uint32_t);
void jnz(MaquinaVirtual *, uint32_t, uint32_t);
void not(MaquinaVirtual *, uint32_t, uint32_t);
void stop(MaquinaVirtual *, uint32_t, uint32_t);

#endif