extern handle_interrupt

section .text:

;Macro for defining an interrupt handler stub without error code
%macro def_int_stub 1
    
global int_stub_%1
int_stub_%1:
    push dword 0    ;Push zero as error code
    push dword %1
    jmp int_common
    
%endmacro

;Macro for defining an interrupt handler stub with error code
%macro def_int_stub_errcode 1
    
global int_stub_%1
int_stub_%1:
    push dword %1
    jmp int_common
    
%endmacro

;Interrupts 0 through 17
def_int_stub 0
def_int_stub 1
def_int_stub 2
def_int_stub 3
def_int_stub 4
def_int_stub 5
def_int_stub 6
def_int_stub 7
def_int_stub_errcode 8
def_int_stub 9
def_int_stub_errcode 10
def_int_stub_errcode 11
def_int_stub_errcode 12
def_int_stub_errcode 13
def_int_stub_errcode 14
def_int_stub 15
def_int_stub 16
def_int_stub_errcode 17

;Interrupts 18 through 48
%assign i 18
%rep 31
def_int_stub i
%assign i i+1
%endrep

;Common interrupt handler
;Calls the interrupt handler defined in C++ code
int_common:
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax
    
    push esp
    call handle_interrupt
    add esp, 4
    
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp
    
    add esp, 8
    iret
