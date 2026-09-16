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
    uint32_t direccionFisica;
    verificaDirFisica(maquina, maquina->registros[LAR], &direccionFisica, tamanio);
    maquina->registros[MAR] = (tamanio << 16) | direccionFisica;

    uint32_t formato = maquina->registros[EAX];
    uint32_t cantidad = maquina->registros[ECX] & 0xFFFF;

    for (unsigned int i = 0; i < cantidad; i++) {
        maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
        leerMemoria(maquina);
        printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt de direccion fisica
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
    uint32_t direccionFisica;
    uint32_t formato = maquina->registros[EAX];
    uint32_t cantidad = maquina->registros[ECX] & 0xFFFF;

    switch (formato) {
        case DECIMAL: 
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                verificaDirFisica(maquina, maquina->registros[LAR], &direccionFisica, tamanio);
                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;

                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf("%d", &maquina->registros[MBR]);
                escribirMemoria(maquina);
            }
            break;
        case CARACTER: 
            char caracter;
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                verificaDirFisica(maquina, maquina->registros[LAR], &direccionFisica, tamanio);
                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
                
                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf(" %c", &caracter);
                maquina->registros[MBR] = (uint32_t)caracter;
                escribirMemoria(maquina);
            }
            break;
        case OCTAL: 
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                verificaDirFisica(maquina, maquina->registros[LAR], &direccionFisica, tamanio);
                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
                
                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf("%o", &maquina->registros[MBR]);
                escribirMemoria(maquina);
            }
            break;
        case BINARIO: 
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                verificaDirFisica(maquina, maquina->registros[LAR], &direccionFisica, tamanio);
                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
                
                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf("%b", &maquina->registros[MBR]);
                escribirMemoria(maquina);
            }
            break;
        case HEXADECIMAL: 
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;
                verificaDirFisica(maquina, maquina->registros[LAR], &direccionFisica, tamanio);
                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
                
                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf("%x", &maquina->registros[MBR]);
                escribirMemoria(maquina);
            }
            break;
    }
}