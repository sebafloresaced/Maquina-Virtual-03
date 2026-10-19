\\DATA 4
\\EXTRA 0
\\STACK 64

main: MOV EAX, 10
    SYS 15

    ADD EAX, 5
    MUL EAX, 2
    SYS 15

    SYS 0

    MOV l[DS], EAX
    MOV EDX, DS
    LDL ECX, 1
    LDH ECX, 4
    MOV EAX, 1
    SYS 2
    STOP