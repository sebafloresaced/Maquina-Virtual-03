\\DATA 64
\\EXTRA 0
\\STACK 128

MENSAJE EQU "Ingrese un texto: "

main: SYS 0
    MOV EDX, KS
    ADD EDX, MENSAJE
    SYS 4

    MOV EDX, DS
    MOV ECX, 3
    SYS 3
    SYS 4
    STOP