#include <stdio.h>
#include <stdlib.h>
#include "../cabeceras/operaciones.h"
#include "../cabeceras/memoria.h"
#include "../cabeceras/llamadas_sistema.h"

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
            
            if (maquina->registros[registro] >> 16 != (int32_t)(direccionLogica >> 16)) {
                printf("Error: el desplazamiento cambia el segmento\n");
                exit(EXIT_FAILURE);
            }

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
            
            if (maquina->registros[registro] >> 16 != (int32_t)(direccionLogica >> 16)) {
                printf("Error: el desplazamiento cambia el segmento\n");
                exit(EXIT_FAILURE);
            }

            maquina->registros[MAR] = bytesAescribir << 16;
            maquina->registros[LAR] = direccionLogica;
            maquina->registros[MBR] = valor;

            escribirMemoria(maquina); //escribir 4 bytes de memoria desde MBR
            break;
        }
    }
}

static void actualizarCC(MaquinaVirtual *maquina, uint64_t resultado64, int32_t v) { // V entra con 1/0 dependiendo de si se debe setear el bit
    uint32_t resultado = resultado64 & 0xFFFFFFFF; // Tomar solo los 32 bits menos significativos
    if (resultado == 0) { // Z
        maquina->registros[CC] |= 0x40000000;
    }
    else 
        maquina->registros[CC] &= ~0x40000000;

    if (resultado >> 31) { // N
        maquina->registros[CC] |= 0x80000000;
    }
    else {
        maquina->registros[CC] &= ~0x80000000;
    }
    
    if (resultado64 >> 32 != 0) { // C
        maquina->registros[CC] |= 0x20000000;
    }
    else {
        maquina->registros[CC] &= ~0x20000000;
    }

    if (v) { // V
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

    actualizarCC(maquina, valor, 0);
    escribirValor(maquina, maquina->registros[OP1], valor);
}

void operacionADD(MaquinaVirtual *maquina)
{
    int32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    int32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);
    uint64_t resultado64 = (uint64_t)valor1 + (uint64_t)valor2;
    int32_t resultado = (int32_t)resultado64;

    int signo1 = valor1 >> 31 & 0x80000000;
    int signo2 = valor2 >> 31 & 0x80000000;
    int signoResultado = resultado >> 31 & 0x80000000;

    int v = (signo1 == signo2) && (signoResultado != signo1);

    actualizarCC(maquina, resultado64, v);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSUB(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (uint64_t)valor1 + ~valor2 + 1;
    uint32_t resultado = (uint32_t)resultado64;

    int signo1 = valor1 >> 31;
    int signo2 = valor2 >> 31;
    int signoResultado = resultado >> 31;

    int v = (signo1 != signo2) && (signoResultado != signo1);

    actualizarCC(maquina, resultado64, v);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionMUL(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (uint64_t)valor1 * valor2;
    uint32_t resultado = (uint32_t)resultado64;

    // Producto con signo para comprobar V.
    int64_t productoConSigno = (int64_t)(int32_t)valor1 * (int64_t)(int32_t)valor2;
    int v = productoConSigno < INT32_MIN || productoConSigno > INT32_MAX; // verifica si entra en el rango de un int32_t

    actualizarCC(maquina, resultado64, v);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionDIV(MaquinaVirtual *maquina)
{
    int32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    int32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);
    
    if (valor2 != 0) {
        int64_t cociente = (int64_t)valor1 / (int64_t)valor2;
        uint32_t resultado = (uint32_t)cociente;
        int64_t resto = (int64_t)valor1 % (int64_t)valor2;

        int v = (valor1 == INT32_MIN && valor2 == -1); 
        // caso especial de overflow
        // Estaria intentando representar INT32_MAX + 1

        actualizarCC(maquina, (uint64_t)resultado, v); 
        escribirValor(maquina, maquina->registros[OP1], resultado);
        maquina->registros[AC] = (int32_t)resto;
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

    uint64_t resultado64 = (uint64_t)valor1 + ~valor2 + 1;
    uint32_t resultado = (uint32_t)resultado64;

    int signo1 = valor1 >> 31;
    int signo2 = valor2 >> 31;
    int signoResultado = resultado >> 31;

    int v = (signo1 != signo2) && (signoResultado != signo1);
    
    actualizarCC(maquina, resultado64, v);
}

void operacionAND(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (uint64_t)valor1 & valor2;
    uint32_t resultado = (uint32_t)resultado64;

    actualizarCC(maquina, resultado64, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionOR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (uint64_t)valor1 | valor2;
    uint32_t resultado = (uint32_t)resultado64;

    actualizarCC(maquina, resultado64, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionXOR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (uint64_t)valor1 ^ valor2;
    uint32_t resultado = (uint32_t)resultado64;

    actualizarCC(maquina, resultado64, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSWAP(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    valor1 ^= valor2;
    valor2 ^= valor1;
    valor1 ^= valor2;

    actualizarCC(maquina, (uint64_t)valor1, 0); // se actualiza con el resultado del ultimo operacionXOR
    escribirValor(maquina, maquina->registros[OP1], valor1);
    escribirValor(maquina, maquina->registros[OP2], valor2);
}

void operacionSHL(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (uint64_t)valor1 << valor2; // calculo el resultado de 64 bits para actualizarCC
    
    uint32_t resultado = valor1;
    int v = 0;
    for (uint32_t i = 0; i < valor2; i++) { // hago el desplazamiento de 1 bit a la izquierda valor2 veces para poder controlar el overflow (cambio de signo en alguna iteracion)
        int signoAnterior = resultado >> 31;

        resultado <<= 1;

        int signoNuevo = resultado >> 31;

        if (signoAnterior != signoNuevo)
            v = 1;
    }

    actualizarCC(maquina, resultado64, v); 
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSHR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (uint64_t)valor1 >> valor2;
    uint32_t resultado = (uint32_t)resultado64;

    actualizarCC(maquina, resultado64, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSAR(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint64_t resultado64 = (int64_t)(int32_t)valor1 >> valor2;
    uint32_t resultado = (uint32_t)resultado64;

    actualizarCC(maquina, (uint64_t)resultado, 0); // se que el carry va a ser 0, pasandole el resultado de 32 bits a 64 bits no se pierde informacion y no tengo problema con los 1's del signo
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionLDL(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (valor1 & 0xFFFF0000) | (valor2 & 0x0000FFFF);

    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionLDH(MaquinaVirtual *maquina)
{
    uint32_t valor1 = obtenerValor(maquina, maquina->registros[OP1]);
    uint32_t valor2  = obtenerValor(maquina, maquina->registros[OP2]);

    uint32_t resultado = (valor1 & 0x0000FFFF) | ((valor2 & 0x0000FFFF) << 16);

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
        case LEER:
            leerDatos(maquina);
            break;
        case ESCRIBIR:
            escribirDatos(maquina);
            break;
        default:
            printf("Error: llamada al sistema desconocida");
            exit(EXIT_FAILURE);
    }
}

void operacionJMP(MaquinaVirtual *maquina)
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

    uint32_t resultado = ~valor;

    actualizarCC(maquina, (uint64_t)resultado, 0);
    escribirValor(maquina, maquina->registros[OP1], resultado);
}

void operacionSTOP(MaquinaVirtual *maquina)
{
    maquina->registros[IP] = -1;
}