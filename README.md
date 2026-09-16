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
│   └── llamadas_sistema.h
│   └── desensamblador.h
├── fuentes/
│   ├── main.c
│   ├── maquina.c
│   ├── cargador.c
│   ├── memoria.c
│   ├── instrucciones.c
│   ├── operaciones.c
│   └── llamadas_sistema.c
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
chmod +x vmt vmx
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

vmt es el programa ejecutable del Traductor.
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
vmx filename.vmx [-d]
```
-  vmx es el programa ejecutable del Ejecutor o Máquina Virtual.
-  filename.vmx (obligatorio) es la ruta y nombre del archivo con el programa en lenguaje máquina
(puede ser cualquier nombre con extensión .vmx).
-  -d (opcional) es un flag que fuerza a la máquina virtual a mostrar el código desensamblado
(disassembler), es decir, un código Assembler que se corresponde con el código máquina cargado
en la memoria principal.



to-do list:
- Nombres de archivo en la estructura del README
- DIV funciona mal con numeros negativos
- SAR no conserva signo
- SYS 2 debe mostrar mas de un caracter
- El desensamblador no puede mostrar numeros negativos