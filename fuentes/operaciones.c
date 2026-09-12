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
            uint32_t direccionFisica = verificaDirFisica(maquina, direccionLogica, &direccionFisica, bytesAleer);
            maquina->registros[MAR] |= direccionFisica;

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
            uint32_t direccionFisica = verificaDirFisica(maquina, direccionLogica, &direccionFisica, bytesAescribir);
            maquina->registros[MAR] |= direccionFisica;
            maquina->registros[MBR] = valor;

            escribeMemoria(maquina); //escribir 4 bytes de memoria desde MBR
            break;
        }
    }
}

static void actualizarCC(MaquinaVirtual *maquina, int32_t resultado, int32_t C, int32_t V) { // C y V entran con 1/0 dependiendo de si se debe setear el bit
    if (resultado == 0) {
        maquina->registros[CC] |= 0x40000000;
    }
    else {
        maquina->registros[CC] &= ~0x40000000;

        if (resultado < 0) {
            maquina->registros[CC] |= 0x80000000;
        }
        else {
            maquina->registros[CC] &= ~0x80000000;
        }
    }
    
    if (C) {
        maquina->registros[CC] |= 0x20000000;
    }
    else {
        maquina->registros[CC] &= ~0x20000000;
    }

    if (V) {
        maquina->registros[CC] |= 0x10000000;
    }
    else {
        maquina->registros[CC] &= ~0x10000000;
    }
}

// OP1 = Operando A
// OP2 = Operando B

void operacionMOV(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP2]);

    actualizarCC(maquina, valor, 0, 0);
    escribirValor(maquina, maquina->registros[OP1], valor);
}

void operacionADD(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);
    uint64_t completo = (uint64_t)valor1 + valor2;
    uint32_t resultado = (uint32_t)completo;

    int c = resultado < completo;

    int signo1 = valor1 >> 31;
    int signo2 = valor2 >> 31;
    int signoResultado = resultado >> 31;

    int v = (signo1 == signo2) && (signoResultado != signo1);

    actualizarCC(maquina, resultado, c, v);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSUB(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t completo = (uint64_t)valor1 - valor2;
    uint32_t resultado = (uint32_t)completo;

    int c = valor1 >= valor2; // A - B = A + (~B + 1) => si A >= B no hay carry

    int signo1 = valor1 >> 31;
    int signo2 = valor2 >> 31;
    int signoResultado = resultado >> 31;

    int v = (signo1 != signo2) && (signoResultado != signo1);

    actualizarCC(maquina, resultado, c, v);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionMUL(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 * valor2;

    actualizarCC(maquina, resultado, c, v); // consultar como setear C y V
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionDIV(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);
    
    if (valor2 != 0) {
        uint32_t resultado = valor1 / valor2;
        uint32_t resto = valor1 % valor2;

        actualizarCC(maquina, resultado, c, v); // consultar como setear C y V
        escribirValor(maquina, maquina->registros[OP1], resultado);
        escribirValor(maquina, maquina->registros[AC], resto);
    }
    else {
        printf("Error: division por cero");
        exit(EXIT_FAILURE);
    }
}

void operacionCMP(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 - valor2;
    
    int c = valor1 >= valor2; // A - B = A + (~B + 1) => si A >= B no hay carry

    int signo1 = valor1 >> 31;
    int signo2 = valor2 >> 31;
    int signoResultado = resultado >> 31;

    int v = (signo1 != signo2) && (signoResultado != signo1);
    
    actualizarCC(maquina, resultado, c, v);
}

void operacionAND(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 & valor2;

    actualizarCC(maquina, resultado, 0, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionOR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 | valor2;

    actualizarCC(maquina, resultado, 0, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionXOR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 ^ valor2;

    actualizarCC(maquina, resultado, 0, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSWAP(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    valor1 ^= valor2;
    valor2 ^= valor1;
    valor1 ^= valor2;

    actualizarCC(maquina, valor1, 0, 0); // se actualiza con el resultado del ultimo operacionXOR
    escribirValor(maquina, maquina->registros[OP1], valor1);
    escribirValor(maquina, maquina->registros[OP2], valor2);
}

void operacionSHL(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 << valor2;

    actualizarCC(maquina, resultado, c, v); // consultar como setear C y V
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSHR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = valor1 >> valor2;

    actualizarCC(maquina, resultado, C, V); // consultar como setear C y V
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSAR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (int32_t)valor1 >> valor2;

    actualizarCC(maquina, resultado, C, V); // consultar como setear C y V
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionLDL(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (valor1 & 0xFF00) | (valor2 & 0x00FF);

    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionLDH(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (valor1 & 0x00FF) | (valor2 & 0xFF00);

    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionRND(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = rand() % (valor + 1);

    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSYS(MaquinaVirtual *maquina)
{
    uint32_t operacion = obtenerValor(maquina,maquina->registros[OP1]);

    switch (operacion) {
        case 1:
            leerDatos(maquina);
            break;
        case 2:
            escribirDatos(maquina);
            break;
        default:
            printf("Error: llamada al sistema desconocida");
            exit(EXIT_FAILURE);
    }
}

void operacionJPM(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    maquina->registros[IP] = valor;
}

void operacionJP(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (!(maquina->registros[CC] & 0xC0000000)) // si el bit N == 0 y el bit Z == 0
        maquina->registros[IP] = valor;
}

void operacionJN(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x80000000) // si el bit N == 1 
        maquina->registros[IP] = valor;
}

void operacionJZ(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x40000000) // si el bit Z == 1 
        maquina->registros[IP] = valor;
}

void operacionJC(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x20000000) // si el bit C == 1 
        maquina->registros[IP] = valor;
}

void operacionJV(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0x10000000) // si el bit V == 1 
        maquina->registros[IP] = valor;
}

void operacionJNP(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (maquina->registros[CC] & 0xC0000000) // si el bit N == 1 o el bit Z == 1
        maquina->registros[IP] = valor;
}

void operacionJNN(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (!(maquina->registros[CC] & 0x80000000)) // si el bit N == 0
        maquina->registros[IP] = valor;
}

void operacionJNZ(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    if (!(maquina->registros[CC] & 0x40000000)) // si el bit Z == 0
        maquina->registros[IP] = valor;
}

void operacionNOT(MaquinaVirtual *maquina)
{
    uint32_t valor = obtenerValor(maquina, maquina->registros[OP1]);

    resultado = ~valor;

    actualizarCC(maquina, resultado, 0, 0);
    escribirValor(maquina, maquina->registros[OP1], valor);
}

void operacionSTOP(MaquinaVirtual *maquina)
{
    maquina->registros[IP] = -1;
    exit(EXIT_SUCCESS);
}