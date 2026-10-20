#include "../cabeceras/maquina.h"
#include "../cabeceras/llamadas_sistema.h"
#include "../cabeceras/operaciones.h"
#include "../cabeceras/memoria.h"
#include "../cabeceras/imagen.h"
#include <stdlib.h>
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
    uint32_t tamanio = (uint32_t)maquina->registros[ECX] >> 16;
    uint32_t direccionFisica;

    uint32_t formato = maquina->registros[EAX];
    uint32_t cantidad = maquina->registros[ECX] & 0xFFFF;

    for (unsigned int i = 0; i < cantidad; i++) {
        maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;

        direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
        verificaDirFisica(maquina, direccionFisica, tamanio);
        
        maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
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
            uint32_t valor = (uint32_t)maquina->registros[MBR];
            for (uint32_t j = 0; j < tamanio; j++) {
                uint32_t desplazamiento = 8 * (tamanio - 1 - j);
                uint8_t caracter = (valor >> desplazamiento) & 0xFF;

                if (caracter >= 32 && caracter <= 126)
                    printf("%c", caracter);
                else
                    printf(".");
            }
        }
        printf("\n");
    }
}

void leerDatos(MaquinaVirtual *maquina) 
{
    uint32_t tamanio = (uint32_t)maquina->registros[ECX] >> 16;
    uint32_t direccionFisica;
    uint32_t formato = maquina->registros[EAX];
    uint32_t cantidad = maquina->registros[ECX] & 0xFFFF;

    switch (formato) {
        case DECIMAL: 
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;

                direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
                verificaDirFisica(maquina, direccionFisica, tamanio);

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

                direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
                verificaDirFisica(maquina, direccionFisica, tamanio);

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

                direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
                verificaDirFisica(maquina, direccionFisica, tamanio);
                
                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
                
                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf("%o", &maquina->registros[MBR]);
                escribirMemoria(maquina);
            }
            break;
        case BINARIO: 
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;

                direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
                verificaDirFisica(maquina, direccionFisica, tamanio);
                
                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
                
                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf("%b", &maquina->registros[MBR]);
                escribirMemoria(maquina);
            }
            break;
        case HEXADECIMAL: 
            for (unsigned int i = 0; i < cantidad; i++) {
                maquina->registros[LAR] = maquina->registros[EDX] + i * tamanio;

                direccionFisica = calculaDirFisica(maquina, maquina->registros[LAR]);
                verificaDirFisica(maquina, direccionFisica, tamanio);

                maquina->registros[MAR] = (tamanio << 16) | direccionFisica;
                
                printf("[%04X]: ", maquina->registros[MAR] & 0xFFFF); // prompt
                scanf("%x", &maquina->registros[MBR]);
                escribirMemoria(maquina);
            }
            break;
    }
}

static uint32_t posicionString(MaquinaVirtual *maquina, uint32_t inicio, uint32_t desplazamiento)
{
    uint32_t offsetInicial = inicio & 0xFFFF;

    // valido que el desplazamiento no cambie el segmento.
    if (desplazamiento > 0xFFFF - offsetInicial) {
        fprintf(stderr, "Error: string fuera del segmento\n");
        exit(EXIT_FAILURE);
    }

    uint32_t direccionLogica = inicio + desplazamiento;

    maquina->registros[LAR] = (int32_t)direccionLogica;

    uint32_t fisica = calculaDirFisica(maquina, direccionLogica);
    verificaDirFisica(maquina, fisica, 1);

    return fisica;
}

void escribirString(MaquinaVirtual *maquina)
{
    uint32_t inicio = (uint32_t)maquina->registros[EDX];

    for (uint32_t i = 0; ; i++) {
        uint32_t fisica = posicionString(maquina, inicio, i);
        uint8_t caracter = maquina->memoria[fisica];
        if (caracter == '\0')
            break;
        putchar(caracter);
    }

    fflush(stdout);
}

void leerString(MaquinaVirtual *maquina)
{
    int32_t limite = maquina->registros[ECX];
    uint32_t inicio = (uint32_t)maquina->registros[EDX];
    uint32_t cantidad = 0;
    int caracter;

    if (limite < -1) {
        printf("Error: limite de string invalido\n");
        exit(EXIT_FAILURE);
    }

    uint32_t fisica = posicionString(maquina, inicio, 0);

    fflush(stdout);

    while ((caracter = getchar()) != '\n' && caracter != EOF) {
        if (limite == -1 || cantidad < (uint32_t)limite) {
            // Reservar lugar para el terminador antes de guardar.
            uint32_t siguiente = posicionString(maquina, inicio, cantidad + 1);
            maquina->memoria[fisica] = (uint8_t)caracter;
            fisica = siguiente;
            cantidad++;
        }
        // Consumir el resto de caracteres sin guardarlos.
    }

    maquina->memoria[fisica] = '\0';
}

void limpiarPantalla(void)
{
    printf("\033[H\033[2J\033[3J");
    fflush(stdout);
}

void breakpoint(MaquinaVirtual *maquina)
{
    if (maquina->rutaImagen == NULL)
        return;

    crearImagen(maquina->rutaImagen, maquina);

    while (1) { // ciclo infinito
        fflush(stdout);
        int opcion = getchar();
        if (opcion != '\n' && opcion != EOF) {
            int resto;
            while ((resto = getchar()) != '\n' && resto != EOF) { // consume lo que queda de la linea
                // no hace nada
            }
        }

        if (opcion == '\n') {
            maquina->pasoAPaso = 1;
            return;
        }

        if (opcion == 'g') {
            maquina->pasoAPaso = 0;
            return;
        }

        if (opcion == 'q' || opcion == EOF) {
            maquina->detenerEjecucion = 1;
            return;
        }
    }
}