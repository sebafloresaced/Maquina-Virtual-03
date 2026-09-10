#include <stdio.h>
#include <string.h>
#include "../cabeceras/cargador.h"

static int leerEncabezado(FILE *archivo) {
  uint8_t encabezado[8];
  int tamanioCodigo;

  if (fread(encabezado, 1, 8, archivo) != 8) { // no tiene los 8 bytes del encabezado
    printf("Error: encabezado incompleto\n");
    return -1;
  }
  if (memcmp(encabezado, "VMX26", 5) != 0) { // no cumple el identificador del traductor
    printf("Error: identificador incorrecto\n");
    return -1;
  }
  if (encabezado[5] != 1) { // no cumple la version del traductor
    printf("Error: version incorrecta\n");
    return -1;
  }
  tamanioCodigo = ((uint16_t)encabezado[6] << 8) | encabezado[7];
  if (tamanioCodigo > TAM_MEMORIA) { // consultar si tirar error o cargar el codigo que entre en memoria
    printf("Error: el codigo no entra en la memoria\n");
    return -1;
  }
  return tamanioCodigo;
}

int cargarPrograma(const char *nombreArchivo, MaquinaVirtual *maquina) {
  FILE *archivo;
  int tamanioCodigo;

  archivo = fopen(nombreArchivo, add"rb");
  if (archivo == NULL) {
    printf("No se pudo abrir el archivo\n");
    return 0;
  }
  tamanioCodigo = leerEncabezado(archivo);
  if (tamanioCodigo == -1) { // hubo un error con el encabezado
    fclose(archivo);
    return 0;
  }
  if (fread(maquina->memoria, 1, tamanioCodigo, archivo) != (size_t)tamanioCodigo) { // no se pudieron leer la cantidad de bytes indicada
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