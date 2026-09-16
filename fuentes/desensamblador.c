#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"
#include "../cabeceras/instrucciones.h"
#include "../cabeceras/desensamblador.h"

static void nombreOperacion(uint8_t codigo, st15 nombre) {
    switch (codigo) {
        case SYS: strcpy(nombre, "SYS"); break;
        case JMP: strcpy(nombre, "JMP"); break;
        case JP:  strcpy(nombre, "JP"); break;
        case JN:  strcpy(nombre, "JN"); break;
        case JZ:  strcpy(nombre, "JZ"); break;
        case JC:  strcpy(nombre, "JC"); break;
        case JV:  strcpy(nombre, "JV"); break;
        case JNP: strcpy(nombre, "JNP"); break;
        case JNN: strcpy(nombre, "JNN"); break;
        case JNZ: strcpy(nombre, "JNZ"); break;
        case NOT: strcpy(nombre, "NOT"); break;
        case STOP: strcpy(nombre, "STOP"); break;
        case MOV: strcpy(nombre, "MOV"); break;
        case ADD: strcpy(nombre, "ADD"); break;
        case SUB: strcpy(nombre, "SUB"); break;
        case MUL: strcpy(nombre, "MUL"); break;
        case DIV: strcpy(nombre, "DIV"); break;
        case CMP: strcpy(nombre, "CMP"); break;
        case AND: strcpy(nombre, "AND"); break;
        case OR:  strcpy(nombre, "OR"); break;
        case XOR: strcpy(nombre, "XOR"); break;
        case SWAP: strcpy(nombre, "SWAP"); break;
        case SHL: strcpy(nombre, "SHL"); break;
        case SHR: strcpy(nombre, "SHR"); break;
        case SAR: strcpy(nombre, "SAR"); break;
        case LDL: strcpy(nombre, "LDL"); break;
        case LDH: strcpy(nombre, "LDH"); break;
        case RND: strcpy(nombre, "RND"); break;
        default:  strcpy(nombre, "");
    }
}

static void nombreRegistro(uint8_t codigo, st15 nombre) {
    switch (codigo) {
        case EAX: strcpy(nombre, "EAX"); break;
        case EBX: strcpy(nombre, "EBX"); break;
        case ECX: strcpy(nombre, "ECX"); break;
        case EDX: strcpy(nombre, "EDX"); break;
        case IP:   strcpy(nombre, "IP"); break;
        case OPC:  strcpy(nombre, "OPC"); break;
        case OP1:  strcpy(nombre, "OP1"); break;
        case OP2:  strcpy(nombre, "OP2"); break;
        case LAR:  strcpy(nombre, "LAR"); break;
        case MAR:  strcpy(nombre, "MAR"); break;
        case MBR:  strcpy(nombre, "MBR"); break;
        case EEX:  strcpy(nombre, "EEX"); break;
        case EFX:  strcpy(nombre, "EFX"); break;
        case AC:   strcpy(nombre, "AC"); break;
        case CC:   strcpy(nombre, "CC"); break;
        case CS:   strcpy(nombre, "CS"); break;
        case DS:   strcpy(nombre, "DS"); break;
        default:   strcpy(nombre, "");
    }
}

static void obtieneOperando(Operando OP, st15 nombre) {
    
    st15 nombreoperando;
    
    uint8_t tipo = OP >> 24; 

    switch (tipo) {
            case REGISTRO:
                st15 registro;
                nombreRegistro(OP & 0x1F, registro);
                strcpy(nombreoperando, registro);

                break;

            case INMEDIATO:
                int16_t valor = (int16_t)(OP & 0xFFFF);
                st6 valor_str;

                sprintf(valor_str, "%d", (int)valor);
                strcpy(nombreoperando, valor_str);

                break;

            case MEMORIA:
                strcpy(nombreoperando, "[");

                st15 registroMemoria;
                nombreRegistro(OP & 0x1F, registroMemoria);
                strcat(nombreoperando, registroMemoria);
                
                int16_t offset = (int16_t)((OP >> 8) & 0xFFFF);
                
                if (offset != 0) {
                    st7 offset_str;
                    sprintf(offset_str, "%+d", (int)offset);
                    strcat(nombreoperando, offset_str);
                }

                strcat(nombreoperando, "]");

                break;

            default:
                strcpy(nombreoperando, "");
                break;
        }

        strcpy(nombre, nombreoperando);
}

void desensamblador(MaquinaVirtual maquina) {
    
    uint32_t inicio = maquina.segmentos[0].base;
    uint32_t fin = inicio + maquina.segmentos[0].tamanio;
    int desplazamiento;
    st15 operando1, operando2, operacion;

    uint32_t direccionFisica = inicio;
    uint8_t instruccion;

    printf("Desensamblador activado:\n\n");

    maquina.registros[IP] = maquina.registros[CS];

    while (direccionFisica < fin) {

        instruccion = maquina.memoria[direccionFisica];
        desensamblaInstruccion(&maquina, instruccion, &desplazamiento);

        maquina.registros[IP] += desplazamiento + 1;
        
        
        nombreOperacion(maquina.registros[OPC], operacion);
        obtieneOperando(maquina.registros[OP1], operando1); 
        obtieneOperando(maquina.registros[OP2], operando2);

        int tamanio = desplazamiento + 1;

        printf("[%04X] ", (unsigned int)direccionFisica);
        for (int j = 0; j < tamanio; j++) {
            printf("%02X ", (unsigned int)maquina.memoria[direccionFisica + j]);
        }

        // Alinea la columna de las operaciones.
        for (int j = tamanio; j < 7; j++)
            printf("   ");

        printf("| %-5s", operacion);

        if (operando1[0] != '\0')
            printf(" %s", operando1);

        if (operando2[0] != '\0')
            printf(", %s", operando2);

        printf("\n");

        direccionFisica += tamanio;
        
    }
        
}
