.globl _getc,_putc,_getebp
.globl _color 

_getebp: 
       mov ax, bp
       ret

_getc:
        xorb   ah,ah 
        int    0x16  
        ret 

_putc:           
        push   bp
        mov    bp,sp
        movb   al,4[bp]
        movb   ah,#14  
        mov    bx,_color   ! color is global set in C code by pid
        int    0x10    
        pop    bp
        ret

