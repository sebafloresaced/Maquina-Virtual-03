MOV [0], 42       ; Guarda 42 al comienzo del segmento de datos
MOV EDX, CS      ; EDX = 0x00000000
LDL EDX, -1      ; EDX = 0x0000FFFF
MOV EAX, [EDX+1] ; Deberia detectar el cambio de segmento
STOP


