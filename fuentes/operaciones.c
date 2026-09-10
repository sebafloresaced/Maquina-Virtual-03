#include <stdio.h>
#include "../cabeceras/operaciones.h"
#include "../cabeceras/memoria.h"

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

void sub(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{

}

void mul(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void div(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void cmp(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void and(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void or(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void xor(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void swap(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void shl(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void shr(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void sar(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void ldl(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void ldh(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void rnd(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void sys(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jmp(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jp(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jn(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jz(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jc(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jv(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jnp(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jnn(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void jnz(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void not(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}

void stop(MaquinaVirtual *maquina, Operando OPA, Operando OPB)
{
    
}