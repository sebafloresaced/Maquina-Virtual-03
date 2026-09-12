#include <stdio.h>
#include <stdlib.h>
#include "../cabeceras/operaciones.h"
#include "../cabeceras/memoria.h"

static uint32_t obtenerValor(MaquinaVirtual *maquina, Operando op)
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

static void escribirValor(MaquinaVirtual *maquina, Operando op, int32_t valor)
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

// OP1 = Operando A
// OP2 = Operando B

void mov(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP2]);

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], valor);
}

void add(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 + valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void sub(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 - valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void mul(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 * valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void divi(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);
    
    if (valor2 != 0) {
        uint32_t resultado = valor1 / valor2;
        uint32_t resto = valor1 % valor2;

        actualizarCC(maquina, resultado);
        escribirValor(maquina, maquina->registros[OP1], resultado);
        escribirValor(maquina, maquina->registros[AC], resto);
    }
    else {
        stop(maquina);
    }
}

void cmp(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 - valor2;
    
    actualizarCC(maquina, resultado);
}

void and(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 & valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void or(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 | valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void xor(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 ^ valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void swap(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    valor1 ^= valor2;
    valor2 ^= valor1;
    valor1 ^= valor2;

    actualizarCC(maquina, valor1); // se actualiza con el resultado del ultimo xor
    escribirValor(maquina, maquina->registros[OP1], valor1);
    escribirValor(maquina, maquina->registros[OP2], valor2);
}

void shl(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 << valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void shr(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 >> valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void sar(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (int32_t)valor1 >> valor2;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void ldl(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (valor1 & 0xFF00) | (valor2 & 0x00FF);

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void ldh(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (valor1 & 0x00FF) | (valor2 & 0xFF00);

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void rnd(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = rand() % (valor + 1);

    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void sys(MaquinaVirtual *maquina)
{
    
}

void jmp(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    maquina->registros[IP] = valor;
}

void jp(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (!(maquina->registros[CC] & 0xC0000000)) // si el bit N == 0 y el bit Z == 0
        maquina->registros[IP] = valor;
}

void jN(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x80000000) // si el bit N == 1 
        maquina->registros[IP] = valor;
}

void jz(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x40000000) // si el bit Z == 1 
        maquina->registros[IP] = valor;
}

void jc(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x20000000) // si el bit C == 1 
        maquina->registros[IP] = valor;
}

void jv(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x10000000) // si el bit V == 1 
        maquina->registros[IP] = valor;
}

void jnp(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0xC0000000) // si el bit N == 1 o el bit Z == 1
        maquina->registros[IP] = valor;
}

void jnn(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (!(maquina->registros[CC] & 0x80000000)) // si el bit N == 0
        maquina->registros[IP] = valor;
}

void jnz(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (!(maquina->registros[CC] & 0x40000000)) // si el bit Z == 0
        maquina->registros[IP] = valor;
}

void not(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    resultado = ~valor;

    actualizarCC(maquina, resultado);
    escribirValor(maquina, maquina->registros[OP1], valor);
}

void stop(MaquinaVirtual *maquina)
{
    maquina->registros[IP] = -1;
}