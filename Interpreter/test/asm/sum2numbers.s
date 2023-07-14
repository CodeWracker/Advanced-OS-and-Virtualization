pop ax 

dec ax
mov bx, ax 

mov cx, #0000
loop1:
pop ax
add cx, ax
dec bx
jnz loop1
hlt


