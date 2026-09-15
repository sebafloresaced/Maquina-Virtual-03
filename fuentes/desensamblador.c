#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"

static char *nombreOperacion(uint8_t codigo) {
    switch (codigo) {
        case SYS: return "SYS";
        case JMP: return "JMP";
        case JP:  return "JP";
        case JN:  return "JN";
        case JZ:  return "JZ";
        case JC:  return "JC";
        case JV:  return "JV";
        case JNP: return "JNP";
        case JNN: return "JNN";
        case JNZ: return "JNZ";
        case NOT: return "NOT";
        case STOP: return "STOP";
        case MOV: return "MOV";
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case CMP: return "CMP";
        case AND: return "AND";
        case OR:  return "OR";
        case XOR: return "XOR";
        case SWAP: return "SWAP";
        case SHL: return "SHL";
        case SHR: return "SHR";
        case SAR: return "SAR";
        case LDL: return "LDL";
        case LDH: return "LDH";
        case RND: return "RND";
        default:  return "OP DESCONOCIDA";
    }
}

static char *nombreRegistro(uint8_t codigo) {
    switch (codigo) {
        case EAX: return "EAX";
        case EBX: return "EBX";
        case ECX: return "ECX"; 
        case EDX: return "EDX";
        case IP:   return "IP";
        case OPC:  return "OPC";
        case OP1:  return "OP1";
        case OP2:  return "OP2";
        case LAR:  return "LAR";
        case MAR:  return "MAR";
        case MBR:  return "MBR";
        case EEX:  return "EEX";
        case EFX:  return "EFX";
        case AC:   return "AC";
        case CC:   return "CC"; 
        case CS:   return "CS";
        case DS:   return "DS";
        default:   return "OPERANDO DESCONOCIDO";
    }
}

static char *obtieneOperando(MaquinaVirtual maquina, Operando OP) {
    
    char *nombreoperando = malloc(2 * sizeof(char)); // Ajustar el tamaño según sea necesario
    
    uint8_t tipo = OP >> 24; 

    switch (tipo) {
            case REGISTRO:
                char *registro = nombreRegistro(maquina.registros[OP] & 0x1F);
                strcpy(nombreoperando, registro);
                break;
            case INMEDIATO:
                int valor = maquina.registros[OP] & 0xFFFF;
                char valor_str[12];
                itoa(valor, valor_str, 10);
                strcpy(nombreoperando, valor_str);

                break;
            case MEMORIA:
                strcpy(nombreoperando, "[");

                strcat(nombreoperando, nombreRegistro(maquina.registros[OP] & 0x1F));
                
                uint16_t offset = (maquina.registros[OP] >> 8) & 0xFFFF;
                
                if (offset != 0) {
                    strcat(nombreoperando, "+");
                    char offset_str[12];
                    itoa(offset, offset_str, 10);
                    strcat(nombreoperando, offset_str);
                }

                strcat(nombreoperando, "]");

                break;
            default:
                strcpy(nombreoperando, "OPERANDO DESCONOCIDO");
                break;
        }

        return nombreoperando;
}

void desensamblador(MaquinaVirtual maquina) {
    
    int inicio = maquina.segmentos[CS].base;
    int fin = maquina.segmentos[CS].base + maquina.segmentos[CS].tamanio;

    int desplazamiento;
    char *operando1, *operando2, *operacion;

    printf("Desensamblador activado:\n\n");

    for (int i = inicio; i < fin; i++) {

        desensamblaInstruccion(maquina, maquina.memoria[maquina.registros[IP]], &desplazamiento);
        
        maquina.registros[IP] += desplazamiento + 1;

        strcpy(operacion, nombreOperacion(maquina.registros[OPC]));
        strcpy(operando1, nombreRegistro(maquina.registros[OP1]));
        strcpy(operando2, nombreRegistro(maquina.registros[OP2]));
        
        printf("0x%04X: %s %s, %s, %s\n", i, operacion, operando1, operando2);
        
    }
        
}
