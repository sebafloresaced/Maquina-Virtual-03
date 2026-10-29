\\DATA 32
\\EXTRA 0
\\STACK 64

MENSAJE EQU "Texto leido: "
SALTO EQU "\n"

main: MOV EDX, DS
    MOV ECX, 0
    LDH ECX, 4
    LDL ECX, 1
    MOV EAX, 1
    SYS 1

    MOV EDX, DS
    ADD EDX, 4
    MOV ECX, 20
    SYS 3

    MOV EDX, KS
    ADD EDX, MENSAJE
    SYS 4

    MOV EDX, DS
    ADD EDX, 4
    SYS 4

    MOV EDX, KS
    ADD EDX, SALTO
    SYS 4

    MOV EDX, DS
    MOV ECX, 0
    LDH ECX, 4
    LDL ECX, 1
    MOV EAX, 1
    SYS 1

    MOV EBX, 0
    SYS 15
    MOV EBX, 777
    STOP