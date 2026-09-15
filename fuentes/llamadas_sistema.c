#include "../cabeceras/maquina.h"
#include "../cabeceras/llamadas_sistema.h"
#include "../cabeceras/operaciones.h"
#include "../cabeceras/memoria.h"
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

    for (unsigned int i = 0; i < cantidad; i++) {
        maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
        leerMemoria(maquina);
        printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
        if (formato & HEXADECIMAL) {
            printf("0x%X ", maquina->registros[MBR]);
        }
        if (formato & DECIMAL) {
            printf("%d ", maquina->registros[MBR]);
        }
        if (formato & OCTAL) {
            printf("0o%o ", maquina->registros[MBR]);
        }
        if (formato & BINARIO) {
            printf("0b%b ", maquina->registros[MBR]);
        }
        if (formato & CARACTER) {
            uint8_t caracter = maquina->registros[MBR] & 0xFF;
            if (caracter >= 32 && caracter <= 126) { // rango de caracteres imprimibles
                printf("%c ", caracter);
            } else {
                printf(". "); // caracter no imprimible
            }
        }
        printf("\n");
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
            for (unsigned int i = 0; i < cantidad; i++) {
                scanf("%d", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[%04X]: %d \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]); // prompt
            }
            break;
        case CARACTER: 
            char caracter;
            for (unsigned int i = 0; i < cantidad; i++) {
                scanf(" %c", &caracter);
                maquina->registros[MBR] = (uint32_t)caracter;
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[%04X]: %c \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case OCTAL: 
            for (unsigned int i = 0; i < cantidad; i++) {
                scanf("%o", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[%04X]: 0%o \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case BINARIO: 
            for (unsigned int i = 0; i < cantidad; i++) {
                scanf("%b", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[%04X]: 0b%b \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
        case HEXADECIMAL: 
            for (unsigned int i = 0; i < cantidad; i++) {
                scanf("%x", &maquina->registros[MBR]);
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                escribirMemoria(maquina);
                printf("[%04X]: 0x%X \n", maquina->registros[MAR] & 0xFFFF, maquina->registros[MBR]);
            }
            break;
    }
}