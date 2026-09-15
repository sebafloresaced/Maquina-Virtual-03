#include "../cabeceras/maquina.h"
#include "../cabeceras/llamadas_sistema.h"
#include "../cabeceras/operaciones.h"
#include <stdio.h>

// EDX -> posicion de memoria
// EAX -> formato
// ECX (2 bytes mas significativos) -> tamanio de valores
// ECX (2 bytes menos significativos) -> cantidad de valores

// Formatos:
// %#X -> hexadecimal
// %d -> decimal   
// %o -> octal
// %#b -> binario
// %c -> caracter

void escribirDatos(MaquinaVirtual *maquina) 
{
    uint32_t tamanio = maquina->registros[ECX] >> 16;
    maquina->registros[MAR] = tamanio << 16;

    uint32_t formato = maquina->registros[EAX];
    uint32_t cantidad = maquina->registros[ECX] & 0xFFFF;

    switch (formato) {
        case HEXADECIMAL:
            for (int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                leerMemoria(maquina);
                printf("[0x%X]: 0x%X \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]); // prompt
            }
            break;
        case DECIMAL: 
            for (int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                leerMemoria(maquina);
                printf("[0x%X]: %d \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case OCTAL: 
            for (int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                leerMemoria(maquina);
                printf("[0x%X]: 0%o \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case BINARIO:
            for (int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                leerMemoria(maquina);
                printf("[0x%X]: 0b%b \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case CARACTER: 
            for (int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                leerMemoria(maquina);
                printf("[0x%X]: %c \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
    }
}

void leerDatos(MaquinaVirtual *maquina) 
{
    uint32_t tamanio = maquina->registros[ECX] >> 16;
    maquina->registros[MAR] = tamanio << 16;

    uint32_t formato = maquina->registros[EAX];
    uint32_t cantidad = maquina->registros[ECX] & 0xFFFF;

    switch (formato) {
        case DECIMAL: 
            for (int i = 0; i < cantidad; i++) {
                scanf("%d", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[0x%X]: %d \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]); // prompt
            }
            break;
        case CARACTER: 
            for (int i = 0; i < cantidad; i++) {
                scanf("%c", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[0x%X]: %c \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case OCTAL: 
            for (int i = 0; i < cantidad; i++) {
                scanf("%o", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[0x%X]: 0%o \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case BINARIO: 
            for (int i = 0; i < cantidad; i++) {
                scanf("%b", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[0x%X]: 0b%b \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case HEXADECIMAL: 
            for (int i = 0; i < cantidad; i++) {
                scanf("%x", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[0x%X]: 0x%X \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
    }
}