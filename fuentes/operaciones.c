#include <stdio.h>
#include "operaciones.h"
#include "memoria.h"
#include "maquina.h"

uint32_t obtenerValor(MaquinaVirtual *maquina, Operando op)
{
    uint8_t tipo = op >> 24;

    switch (tipo)
    {
        case REGISTRO:
        {
            uint8_t nroregistro = op & 0x1F;
            return maquina->registros[nroregistro];
        }

        case INMEDIATO:
        {
            int16_t inmediato = op & 0xFFFF; //int para conservar el signo
            return (uint32_t)inmediato;
        }

        case MEMORIA:
        {
            int16_t offset = (op >> 8) & 0xFFFF;
            uint8_t registro = op & 0x1F;

            uint32_t direccionLogica = maquina->registros[registro] + offset; //direccion donde apunta el registro + el desplazamiento

            // acá leerías memoria

            return leerMemoria32(maquina, direccionLogica);
        }

        default:
            return 0; //
    }
}

void escribirValor(MaquinaVirtual *maquina, Operando op, uint32_t valor)
{ 
    uint32_t tipo = op >> 24;

    switch (tipo)
    {
        case REGISTRO:
        {
            uint8_t nroregistro = op & 0x1F;
            maquina->registros[nroregistro] = valor;
            break;
        }

        case MEMORIA:
        {
            int16_t offset = (op >> 8) & 0xFFFF;
            uint8_t registro = op & 0x1F;

            uint32_t direccionLogica = maquina->registros[registro] + offset;
            
            escribeMemoria32(maquina, direccionLogica, valor);
            break;
        }
    }
}


void mov(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    uint32_t valor = obtenerValor(maquina, OPB);

    escribirValor(maquina, OPA, valor);
}

void add(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    uint32_t valorDestino = obtenerValor(maquina, OPB);
    uint32_t valorOrigen  = obtenerValor(maquina, OPA);

    uint32_t resultado = valorDestino + valorOrigen;

    escribirValor(maquina, OPA, resultado);
}