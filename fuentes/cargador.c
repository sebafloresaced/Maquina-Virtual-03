#include <stdio.h>
#include <string.h>
#include "../cabeceras/cargador.h"

static int leerEncabezado(FILE *archivo) {
  uint8_t encabezado[8];
  int tamanioCodigo;

  if (fread(encabezado, 1, 8, archivo) != 8) {
    printf("Error: encabezado incompleto\n");
    return -1;
  }
  if (memcmp(encabezado, "VMX26", 5) != 0) {
    printf("Error: identificador incorrecto\n");
    return -1;
  }
  if (encabezado[5] != 1) {
    printf("Error: version incorrecta\n");
    return -1;
  }
  tamanioCodigo = ((uint16_t)encabezado[6] << 8) | encabezado[7];
  if (tamanioCodigo > TAM_MEMORIA) {
    printf("Error: el codigo no entra en la memoria\n");
    return -1;
  }
  return tamanioCodigo;
}

int cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina) {
  FILE *archivo;
  int tamanioCodigo;

  archivo = fopen(nombreArchivo, "rb");
  if (archivo == NULL) {
    printf("No se pudo abrir el archivo\n");
    return 0;
  }
  tamanioCodigo = leerEncabezado(archivo);
  if (tamanioCodigo == -1) {
    fclose(archivo);
    return 0;
  }
  if (fread(maquina->memoria, 1, tamanioCodigo, archivo) != (size_t)tamanioCodigo) {
    printf("Error: el archivo contiene menos codigo del indicado\n");
    fclose(archivo);
    return 0;
  }

  /* Segmento de codigo */
  maquina->segmentos[0].base = 0;
  maquina->segmentos[0].tamanio = tamanioCodigo;
  /* Segmento de datos */
  maquina->segmentos[1].base = tamanioCodigo;
  maquina->segmentos[1].tamanio = TAM_MEMORIA - tamanioCodigo;
  /* Registros iniciales */
  maquina->registros[CS] = 0x00000000;
  maquina->registros[DS] = 0x00010000;
  maquina->registros[IP] = maquina->registros[CS];

  fclose(archivo);
  return 1;
}