.data
_ret: .asciiz "\n"
.globl main
.text

main :

move $fp $sp
addi $sp, $sp, -2048

li $t0, 10
sw $t0 , -4($fp)
li $t0, 20
sw $t0 , -8($fp)
li $t0, 3
sw $t0 , -12($fp)
lw $t0 , -8($fp)
lw $t1 , -12($fp)
mul $t2, $t0, $t1
sw $t2 -16($fp)
lw $t0 , -4($fp)
lw $t1 , -16($fp)
add $t2, $t0, $t1
sw $t2 -20($fp)
lw $t0 , -20($fp)
lw $t1 , -8($fp)
sub $t2, $t0, $t1
sw $t2 -24($fp)
lw $a0 ,  -24($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

lw $t0 , -4($fp)
lw $t1 , -8($fp)
add $t2, $t0, $t1
sw $t2 -28($fp)
lw $t0 , -28($fp)
lw $t1 , -12($fp)
div $t0 , $t1
mflo $t2
sw $t2 -24($fp)
lw $a0 ,  -24($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

lw $t0 , -4($fp)
lw $t1 , -8($fp)
add $t2, $t0, $t1
sw $t2 -4($fp)
lw $t0 , -4($fp)
lw $t1 , -8($fp)
sub $t2, $t0, $t1
sw $t2 -8($fp)
lw $t0 , -4($fp)
lw $t1 , -8($fp)
sub $t2, $t0, $t1
sw $t2 -4($fp)
lw $t0 , -4($fp)
lw $t1 , -8($fp)
sub $t2, $t0, $t1
sw $t2 -32($fp)
lw $t0 , -32($fp)
li $t1 , 2
div $t0 , $t1
mflo $t2
sw $t2 -36($fp)
lw $t0 , -4($fp)
lw $t1 , -8($fp)
div $t0 , $t1
mflo $t2
sw $t2 -40($fp)
lw $t0 , -40($fp)
li $t1 , 2
mul $t2, $t0, $t1
sw $t2 -44($fp)
lw $t0 , -36($fp)
lw $t1 , -44($fp)
add $t2, $t0, $t1
sw $t2 -8($fp)
lw $a0 ,  -8($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

addi $sp, $sp, 2048
li $v0 , 0
jr $ra
