#include <stdio.h>
#include <string.h>
#include "../cabeceras/cargador.h"

static void leerEncabezado(FILE *archivo) {
  uint8_t encabezado[8];
  int tamanioCodigo;

  if (fread(encabezado, 1, 8, archivo) != 8) { // no tiene los 8 bytes del encabezado
    printf("Error: encabezado incompleto\n");
    exit(EXIT_FAILURE);
  }

  if (memcmp(encabezado, "VMX26", 5) != 0) { // no cumple el identificador del traductor
    printf("Error: identificador incorrecto\n");
    exit(EXIT_FAILURE);
  }

  if (encabezado[5] != 1) { // no cumple la version del traductor
    printf("Error: version incorrecta\n");
    exit(EXIT_FAILURE);
  }

  tamanioCodigo = ((uint16_t)encabezado[6] << 8) | encabezado[7];
  if (tamanioCodigo > TAM_MEMORIA) { // consultar si tirar error o cargar el codigo que entre en memoria
    printf("Error: el codigo no entra en la memoria\n");
    exit(EXIT_FAILURE);
  }

  return tamanioCodigo;
}

void cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina) {
  FILE *archivo;
  int tamanioCodigo;

  archivo = fopen(nombreArchivo, "rb");

  if (archivo == NULL) {
    printf("No se pudo abrir el archivo\n");
    exit(EXIT_FAILURE);
  }

  tamanioCodigo = leerEncabezado(archivo);
  if (tamanioCodigo == -1) { // hubo un error con el encabezado
    fclose(archivo);
    exit(EXIT_FAILURE);
  }
  if (fread(maquina->memoria, 1, tamanioCodigo, archivo) != (size_t)tamanioCodigo) { // no se pudieron leer la cantidad de bytes indicada
    printf("Error: el archivo contiene menos codigo del indicado\n");
    fclose(archivo);
    exit(EXIT_FAILURE);
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
}