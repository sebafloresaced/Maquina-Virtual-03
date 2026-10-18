#include "../cabeceras/imagen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


// CARGAR VMI

static uint32_t leer32(FILE *archivo)
{
    uint8_t bytes[4];

    if (fread(bytes, 1, 4, archivo) != 4) {
        printf("Error: no se pudieron leer 4 bytes del archivo\n");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }

    return ((uint32_t)bytes[0] << 24) |
           ((uint32_t)bytes[1] << 16) |
           ((uint32_t)bytes[2] << 8)  |
           (uint32_t)bytes[3];
}

static uint16_t leer16(FILE *archivo)
{
    uint8_t bytes[2];

    if (fread(bytes, 1, 2, archivo) != 2) {
        printf("Error: no se pudieron leer 2 bytes del archivo\n");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }

    return ((uint16_t)bytes[0] << 8) | (uint16_t)bytes[1];
}


void cargarImagen(const char *rutaArchivo, MaquinaVirtual *maquina, uint8_t *version)
{
    FILE *archivo = fopen(rutaArchivo, "rb");

    if (archivo == NULL) {
        printf("Error: no se pudo abrir la imagen %s\n", rutaArchivo);
        exit(EXIT_FAILURE);
    }

    // Cabecera: identificador (5 bytes) + version (1 byte).
    uint8_t cabecera[6];

    if (fread(cabecera, 1, sizeof cabecera, archivo) != sizeof cabecera) {
        printf("Error: cabecera VMI incompleta\n");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }

    if (memcmp(cabecera, "VMI26", 5) != 0) {
        printf("Error: identificador VMI invalido\n");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }

    *version = cabecera[5];
    if (*version != 1 && *version != 2) {
        printf("Error: version VMI no soportada\n");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }


    // Los ultimos 2 bytes de la cabecera indican el tamaño en KiB.
    uint32_t tamanioMemoria = (uint32_t)leer16(archivo) * 1024;

    if (tamanioMemoria == 0) {
        printf("Error: tamaño de memoria invalido\n");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }

    // Restaurar los 32 registros.
    for (int i = 0; i < 32; i++) {
        maquina->registros[i] = (int32_t)leer32(archivo);
    }

    // Restaurar los 8 descriptores: base y tamaño, 2 bytes cada uno.
    for (int i = 0; i < 8; i++) {
        maquina->segmentos[i].base = leer16(archivo);
        maquina->segmentos[i].tamanio = leer16(archivo);
    }

    // Leer la memoria 
    uint8_t *memoria = malloc(tamanioMemoria);

    if (fread(memoria, 1, tamanioMemoria, archivo) != tamanioMemoria) {
        printf("Error: memoria incompleta en la VMI\n");
        free(memoria);
        fclose(archivo);
        exit(EXIT_FAILURE);
    }

    fclose(archivo);

    // Reemplazar la memoria anterior de la maquina.
    free(maquina->memoria);
    maquina->memoria = memoria;
    maquina->tamanioMemoria = tamanioMemoria;
}

// CREAR VMI

static void escribirBytes(FILE *archivo, const void *datos, size_t cantidad)
{
    if (fwrite(datos, 1, cantidad, archivo) != cantidad) {
        printf("Error: no se pudo escribir el archivo VMI\n");
        fclose(archivo);
        exit(EXIT_FAILURE);
    }
}

static void escribir16(FILE *archivo, uint16_t valor)
{
    uint8_t bytes[2] = {(uint8_t)(valor >> 8), (uint8_t)valor};
    escribirBytes(archivo, bytes, sizeof bytes);
}

static void escribir32(FILE *archivo, uint32_t valor)
{
    uint8_t bytes[4] = {
        (uint8_t)(valor >> 24),
        (uint8_t)(valor >> 16),
        (uint8_t)(valor >> 8),
        (uint8_t)valor
    };
    escribirBytes(archivo, bytes, sizeof bytes);
}

void crearImagen(const char *rutaArchivo, const MaquinaVirtual *maquina)
{
    // La cabecera guarda el tamaño en KiB usando 2 bytes.
    if (maquina->tamanioMemoria == 0 ||
        maquina->tamanioMemoria % 1024 != 0 ||
        maquina->tamanioMemoria / 1024 > UINT16_MAX) {
        printf("Error: tamaño de memoria no representable en VMI\n");
        exit(EXIT_FAILURE);
    }

    FILE *archivo = fopen(rutaArchivo, "wb");
    if (archivo == NULL) {
        printf("Error: no se pudo crear la imagen %s\n", rutaArchivo);
        exit(EXIT_FAILURE);
    }

    // Cabecera: identificador + version + tamaño de memoria en KiB.
    const uint8_t cabecera[6] = {'V', 'M', 'I', '2', '6', 1};

    escribirBytes(archivo, cabecera, sizeof cabecera);
    escribir16(archivo,(uint16_t)(maquina->tamanioMemoria / 1024));

    // Los 32 registros
    for (int i = 0; i < 32; i++) {
        escribir32(archivo, (uint32_t)maquina->registros[i]);
    }

    // Los 8 descriptores: base y tamaño.
    for (int i = 0; i < 8; i++) {
        escribir16(archivo, (uint16_t)maquina->segmentos[i].base);
        escribir16(archivo, (uint16_t)maquina->segmentos[i].tamanio);
    }

    // Toda la memoria
    escribirBytes(archivo, maquina->memoria, maquina->tamanioMemoria);

    fclose(archivo);
}