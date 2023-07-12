pop ax ; get the qtt of args

sub ax, 1 ; sub 1 because im ignoring the name of the file
mov bx, ax ; save the qtt of args in bx

; loop to sum all the args
; it will pop the args and sum them
; until the bx is 0
mov cx, 0
loop1:
pop ax
sub ax, 48
add cx, ax
dec bx
jnz loop1
hlt



