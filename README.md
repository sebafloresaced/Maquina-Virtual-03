# Maquina-Virtual-03

## Estructura del proyecto
```text
Maquina-Virtual-03/
├── cabeceras/
│   ├── maquina.h
│   ├── cargador.h
│   ├── memoria.h
│   ├── instrucciones.h
│   ├── operaciones.h
│   ├── llamadas_sistema.h
│   ├── desensamblador.h
|   └── imagen.h
├── fuentes/
│   ├── main.c
│   ├── maquina.c
│   ├── cargador.c
│   ├── memoria.c
│   ├── instrucciones.c
│   ├── operaciones.c
│   ├── llamadas_sistema.c
|   ├── imagen.c
│   └── desensamblador.c
└── README.md
```

### 1. Compilar la máquina virtual con GCC

```bash
gcc -std=c11 -Wall -Wextra fuentes/*.c -o vmx
```

### 2. Dar permisos de ejecución

El traductor `vmt` debe estar en la carpeta raíz del proyecto.

```bash
chmod +x vmt vmx vmg
```

### 3. Configurar PATH 

Permite escribir `vmt` y `vmx` sin el prefijo `./`.

```bash
export PATH="$PATH:$PWD"
```

Para conservar la configuración, ejecutar una sola vez desde la carpeta del proyecto:

```bash
printf '\nexport PATH="$PATH:%s"\n' "$PWD" >> ~/.bashrc
source ~/.bashrc
```

### 4. Traducir un programa

```bash
vmt filename.asm [filename.vmx] [-o]
```

- vmt es el programa ejecutable del Traductor.
- filename.asm (obligatorio) es la ruta y nombre del archivo de texto donde está escrito el código
fuente que será traducido (puede ser cualquier nombre con extensión .asm).
-  filename.vmx (opcional) es la ruta y nombre del archivo generado por el Traductor, que contiene
el programa en lenguaje máquina (puede ser cualquier nombre con extensión .vmx). Si se omite,
se crea un archivo con el mismo nombre que el .asm pero con extensión .vmx. Si el archivo ya
existe, se sobrescribe.
-  -o (opcional) es un flag o bandera opcional para indicar que se omita la salida por pantalla de la
traducción. Este flag no omite los mensajes de error producidos durante la traducción

### 5. Ejecutar la máquina virtual

```bash
vmx [filename.vmx] [filename.vmi] [m=M] [-d] [-p param1 param2 ... paramN]
```

Donde:
- `vmx` es el programa ejecutable del Ejecutor o Máquina Virtual.
- `filename.vmx` (opcional) es la ruta y nombre del archivo con el programa en lenguaje máquina
  (puede ser cualquier nombre con extensión `.vmx`).
- `filename.vmi` (opcional) es la ruta y nombre del archivo de imagen de la máquina virtual
  (puede ser cualquier nombre con extensión `.vmi`). Si se ejecuta un programa `.vmx`, esta ruta
  se utiliza para guardar la imagen de la máquina; si no se especifica un `.vmx`, se carga la
  máquina desde esta imagen.
- `m=M` (opcional) indica el tamaño de la memoria principal en KiB. Si se omite, el tamaño por
  defecto es 16 KiB.
- `-d` (opcional) fuerza a la máquina virtual a mostrar el código desensamblado, es decir, una
  representación en lenguaje assembler del código máquina cargado en la memoria principal.
- `-p` (opcional) indica que los argumentos siguientes (`param1 param2 ... paramN`) se pasan al
  programa. Debe escribirse al final del comando.

**Nota:** se debe especificar al menos uno de los archivos `.vmx` o `.vmi`. Si no se especifica
un archivo `.vmx`, los argumentos posteriores a `-p` no se utilizan.

### 6. Usar el debugger

El debugger permite visualizar en tiempo real el estado de la máquina virtual. Mientras la máquina
se encuentra en ejecución, monitorea el archivo de imagen y actualiza la salida por pantalla ante
cualquier cambio. Muestra el código ensamblador, la ubicación del registro IP, los valores de los
registros y la tabla de descriptores de segmentos.

```bash
vmg filename.vmi [-r] [-s] [w=W]
```

Donde:
- `vmg` es el programa ejecutable del debugger.
- `filename.vmi` (obligatorio) es la ruta y nombre del archivo de imagen de la máquina virtual.
- `-r` (opcional) indica al debugger que muestre los valores de los registros.
- `-s` (opcional) indica al debugger que muestre los valores de los segmentos.
- `w=W` (opcional) indica el tamaño de la ventana de código; `W` es la cantidad de líneas de código
  que se muestran alrededor de la instrucción apuntada por el registro IP.

Durante la ejecución, se puede ingresar un valor inmediato o un intervalo de dos valores para
visualizar el contenido de las celdas de memoria en esas direcciones físicas. Ingresar `q` finaliza
la ejecución del debugger.
