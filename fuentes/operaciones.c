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
            uint16_t bytesAleer = sizeof(uint32_t); // 4 bytes a leer

            uint32_t direccionLogica = maquina->registros[registro] + offset; //direccion donde apunta el registro + el desplazamiento

            maquina->registros[MAR] = bytesAleer << 16;
            maquina->registros[LAR] = direccionLogica;
            
            leerMemoria(maquina);

            return maquina->registros[MBR];
        }

        default:
            return 0; //
    }
}

void escribirValor(MaquinaVirtual *maquina, Operando op, int32_t valor)
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
            uint16_t bytesAescribir = sizeof(uint32_t); // 4 bytes a escribir

            uint32_t direccionLogica = maquina->registros[registro] + offset;
            
            maquina->registros[MAR] = bytesAescribir << 16;
            maquina->registros[LAR] = direccionLogica;
            maquina->registros[MBR] = valor;

            escribeMemoria(maquina); //escribir 4 bytes de memoria desde MBR
            break;
        }
    }
}


void mov(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP2]);

    escribirValor(maquina, maquina->registros[OP1], valor);
}

void add(MaquinaVirtual *maquina)
{
    uint32_t valorDestino = obtenerValor(maquina, maquina->registros[OP2]);
    uint32_t valorOrigen  = obtenerValor(maquina, maquina->registros[OP1]);

    uint32_t resultado = valorDestino + valorOrigen;

    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void sub(MaquinaVirtual *maquina)
{

}

void mul(MaquinaVirtual *maquina)
{
    
}

void divi(MaquinaVirtual *maquina)
{
    
}

void cmp(MaquinaVirtual *maquina)
{
    
}

void and(MaquinaVirtual *maquina)
{
    
}

void or(MaquinaVirtual *maquina)
{
    
}

void xor(MaquinaVirtual *maquina)
{
    
}

void swap(MaquinaVirtual *maquina)
{
    
}

void shl(MaquinaVirtual *maquina)
{
    
}


void shr(MaquinaVirtual *maquina)
{
    
}

void sar(MaquinaVirtual *maquina)
{
    
}

void ldl(MaquinaVirtual *maquina)
{
    
}

void ldh(MaquinaVirtual *maquina)
{
    
}

void rnd(MaquinaVirtual *maquina)
{
    
}

void sys(MaquinaVirtual *maquina)
{
    
}

void jmp(MaquinaVirtual *maquina)
{
    
}

void jp(MaquinaVirtual *maquina)
{
    
}

void jN(MaquinaVirtual *maquina)
{
    
}

void jz(MaquinaVirtual *maquina)
{
    
}

void jc(MaquinaVirtual *maquina)
{
    
}

void jv(MaquinaVirtual *maquina)
{
    
}

void jnp(MaquinaVirtual *maquina)
{
    
}

void jnn(MaquinaVirtual *maquina)
{
    
}

void jnz(MaquinaVirtual *maquina)
{
    
}

void not(MaquinaVirtual *maquina)
{
    
}

void stop(MaquinaVirtual *maquina)
{
    
}