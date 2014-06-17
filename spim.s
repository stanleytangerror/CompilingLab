.data
_ret: .asciiz "\n"
.globl main
.text

main :

move $fp $sp
addi $sp, $sp, -2048

li $t0 , 12
li $t1 , 11
sub $t2, $t0, $t1
sw $t2 -8($fp)
li $t0, 3
sw $t0 , -12($fp)
li $t0 , 17
li $t1 , 4
sub $t2, $t0, $t1
sw $t2 -20($fp)
lw $t0 , -8($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -24($fp)
lw $t0 , -8($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -28($fp)
lw $t0 , -28($fp)
lw $t1 , -20($fp)
add $t2, $t0, $t1
sw $t2 -32($fp)
lw $t0 , -8($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -36($fp)
lw $t0 , -36($fp)
lw $t1 , -20($fp)
add $t2, $t0, $t1
sw $t2 -40($fp)
li $t0, 42
sw $t0 , -44($fp)
li $t0, 0
sw $t0 , -48($fp)
lw $t0 , -8($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -52($fp)
lw $t0 , -52($fp)
lw $t1 , -20($fp)
add $t2, $t0, $t1
sw $t2 -56($fp)
lw $t0 , -56($fp)
lw $t1 , -24($fp)
sub $t2, $t0, $t1
sw $t2 -60($fp)
lw $t0 , -60($fp)
lw $t1 , -32($fp)
sub $t2, $t0, $t1
sw $t2 -64($fp)
lw $t0 , -64($fp)
lw $t1 , -40($fp)
add $t2, $t0, $t1
sw $t2 -40($fp)
 label0 :
lw $t0 , -12($fp)
lw $t1 , -8($fp)
sub $t2, $t0, $t1
sw $t2 -68($fp)
lw $t0 , -40($fp)
lw $t1 , -68($fp)
bgt $t0, $t1, label1
j label2
 label1 :
lw $t0 , -40($fp)
lw $t1 , -12($fp)
sub $t2, $t0, $t1
sw $t2 -72($fp)
lw $t0 , -8($fp)
li $t1 , 2
mul $t2, $t0, $t1
sw $t2 -76($fp)
lw $t0 , -72($fp)
lw $t1 , -76($fp)
add $t2, $t0, $t1
sw $t2 -80($fp)
lw $t0 , -20($fp)
lw $t1 , -24($fp)
mul $t2, $t0, $t1
sw $t2 -84($fp)
lw $t0 , -80($fp)
lw $t1 , -84($fp)
add $t2, $t0, $t1
sw $t2 -88($fp)
lw $t0 , -88($fp)
lw $t1 , -40($fp)
sub $t2, $t0, $t1
sw $t2 -92($fp)
lw $t0 , -48($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -96($fp)
lw $t0 , -44($fp)
lw $t1 , -96($fp)
add $t2, $t0, $t1
sw $t2 -100($fp)
lw $t0 , -100($fp)
li $t1 , 3
add $t2, $t0, $t1
sw $t2 -104($fp)
lw $t0 , -104($fp)
li $t1 , 4
add $t2, $t0, $t1
sw $t2 -108($fp)
lw $t0 , -108($fp)
li $t1 , 5
add $t2, $t0, $t1
sw $t2 -44($fp)
lw $t0 , -48($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -112($fp)
lw $t0 , -112($fp)
li $t1 , 0
add $t2, $t0, $t1
sw $t2 -48($fp)
lw $t0 , -48($fp)
li $t1 , 3
div $t0 , $t1
mflo $t2
sw $t2 -116($fp)
lw $t0 , -116($fp)
li $t1 , 3
mul $t2, $t0, $t1
sw $t2 -120($fp)
lw $t0 , -48($fp)
lw $t1 , -120($fp)
sub $t2, $t0, $t1
sw $t2 -124($fp)
lw $t0 , -8($fp)
lw $t1 , -8($fp)
sub $t2, $t0, $t1
sw $t2 -128($fp)
lw $t0 , -128($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -132($fp)
lw $t0 , -132($fp)
lw $t1 , -12($fp)
sub $t2, $t0, $t1
sw $t2 -136($fp)
lw $t0 , -124($fp)
lw $t1 , -136($fp)
beq $t0, $t1, label3
j label4
 label3 :
lw $t0 , -40($fp)
li $t1 , 2
add $t2, $t0, $t1
sw $t2 -40($fp)
 label4 :
lw $t0 , -40($fp)
li $t1 , 1
sub $t2, $t0, $t1
sw $t2 -40($fp)
j label0
 label2 :
lw $a0 ,  -44($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

lw $t0 , -44($fp)
sw $t0 , -48($fp)
 label5 :
lw $t0 , -44($fp)
li $t1 , 1222
bge $t0, $t1, label6
j label7
 label6 :
lw $t0 , -44($fp)
li $t1 , 1024
add $t2, $t0, $t1
sw $t2 -48($fp)
lw $t0 , -44($fp)
li $t1 , 1
sub $t2, $t0, $t1
sw $t2 -44($fp)
lw $t0 , -44($fp)
sw $t0 , -48($fp)
j label5
 label7 :
lw $a0 ,  -44($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

lw $t0 , -8($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -8($fp)
lw $t0 , -8($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -12($fp)
lw $t0 , -8($fp)
lw $t1 , -12($fp)
add $t2, $t0, $t1
sw $t2 -20($fp)
lw $a0 ,  -20($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

addi $sp, $sp, 2048
li $v0 , 0
jr $ra
