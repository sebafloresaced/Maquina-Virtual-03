#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../cabeceras/maquina.h"
#include "../cabeceras/operaciones.h"
#include "../cabeceras/instrucciones.h"

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

static char *obtieneOperando(Operando OP) {
    
    char *nombreoperando = NULL;
    
    uint8_t tipo = OP >> 24; 

    switch (tipo) {
            case REGISTRO:
                char *registro = nombreRegistro(OP & 0x1F);
                strcpy(nombreoperando, registro);

                break;

            case INMEDIATO:
                int valor = OP & 0xFFFF;
                sprintf(nombreoperando, "%d", valor);

                break;

            case MEMORIA:
                strcpy(nombreoperando, "[");

                strcat(nombreoperando, nombreRegistro(OP & 0x1F));
                
                uint16_t offset = (OP >> 8) & 0xFFFF;
                
                if (offset != 0) {
                    strcat(nombreoperando, "+");
                    char offset_str[12];
                    sprintf(offset_str, "%d", offset);
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
    
    uint32_t inicio = maquina.segmentos[0].base;
    uint32_t fin = inicio + maquina.segmentos[0].tamanio;
    int desplazamiento;
    char *operando1 = NULL, *operando2 = NULL, *operacion = NULL;

    uint32_t direccionFisica = inicio;
    uint8_t instruccion;

    printf("Desensamblador activado:\n\n");

    while (direccionFisica < fin) {
        instruccion = maquina.memoria[direccionFisica];

        desensamblaInstruccion(&maquina, instruccion, &desplazamiento);

        direccionFisica += desplazamiento + 1;

        strcpy(operacion, nombreOperacion(maquina.registros[OPC]));
        strcpy(operando1, obtieneOperando(maquina.registros[OP1]));
        strcpy(operando2, obtieneOperando(maquina.registros[OP2]));
        
        printf("0x%08X: 0x%08X 0x%08X 0x%08X   ||  %s, %s, %s\n", direccionFisica, maquina.registros[OPC], maquina.registros[OP1], maquina.registros[OP2], operacion, operando1, operando2);
        
    }
        
}
