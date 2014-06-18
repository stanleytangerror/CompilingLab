.data
_ret: .asciiz "\n"
.globl main
.text

power :
addi $sp, $sp, -2048

lw $t1 12($fp)
sw $t1 -4($fp)
lw $t1 8($fp)
sw $t1 -8($fp)
li $t0, 1
sw $t0 , -12($fp)
 label0 :
lw $t0 , -8($fp)
sw $t0 , -16($fp)
lw $t0 , -16($fp)
li $t1 , 0
bgt $t0, $t1, label1
j label2
 label1 :
lw $t0 , -4($fp)
sw $t0 , -20($fp)
lw $t0 , -12($fp)
lw $t1 , -20($fp)
mul $t2, $t0, $t1
sw $t2 -12($fp)
lw $t0 , -8($fp)
sw $t0 , -24($fp)
lw $t0 , -24($fp)
li $t1 , 1
sub $t2, $t0, $t1
sw $t2 -8($fp)
j label0
 label2 :
addi $sp, $sp, 2048
lw $v0 -12($fp)
jr $ra

mod :
addi $sp, $sp, -2048

lw $t1 12($fp)
sw $t1 -4($fp)
lw $t1 8($fp)
sw $t1 -8($fp)
lw $t0 , -4($fp)
sw $t0 , -12($fp)
lw $t0 , -4($fp)
sw $t0 , -16($fp)
lw $t0 , -8($fp)
sw $t0 , -20($fp)
lw $t0 , -16($fp)
lw $t1 , -20($fp)
div $t0 , $t1
mflo $t2
sw $t2 -24($fp)
lw $t0 , -8($fp)
sw $t0 , -28($fp)
lw $t0 , -24($fp)
lw $t1 , -28($fp)
mul $t2, $t0, $t1
sw $t2 -32($fp)
lw $t0 , -12($fp)
lw $t1 , -32($fp)
sub $t2, $t0, $t1
sw $t2 -36($fp)
addi $sp, $sp, 2048
lw $v0 -36($fp)
jr $ra

getNumDigits :
addi $sp, $sp, -2048

lw $t1 8($fp)
sw $t1 -4($fp)
li $t0, 0
sw $t0 , -8($fp)
lw $t0 , -4($fp)
sw $t0 , -12($fp)
lw $t0 , -12($fp)
li $t1 , 0
blt $t0, $t1, label3
j label4
 label3 :
addi $sp, $sp, 2048
li $v0 , -1
jr $ra
 label4 :
 label5 :
lw $t0 , -4($fp)
sw $t0 , -16($fp)
lw $t0 , -16($fp)
li $t1 , 0
bgt $t0, $t1, label6
j label7
 label6 :
lw $t0 , -4($fp)
sw $t0 , -20($fp)
lw $t0 , -20($fp)
li $t1 , 10
div $t0 , $t1
mflo $t2
sw $t2 -4($fp)
lw $t0 , -8($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -8($fp)
j label5
 label7 :
addi $sp, $sp, 2048
lw $v0 -8($fp)
jr $ra

isNarcissistic :
addi $sp, $sp, -2048

lw $t1 8($fp)
sw $t1 -4($fp)
lw $t0 , -4($fp)
sw $t0 , -8($fp)
lw $t0 , -8($fp)

sw $t0 -12($fp)
sw $fp -16($fp)
sw $ra -20($fp)
addi $fp , $fp , -20
jal getNumDigits
lw $ra , 0($fp)
lw $fp , 4($fp)
sw $v0 -12($fp)
li $t0, 0
sw $t0 , -16($fp)
lw $t0 , -4($fp)
sw $t0 , -20($fp)
 label8 :
lw $t0 , -20($fp)
li $t1 , 0
bgt $t0, $t1, label9
j label10
 label9 :
li $t0 10, 
lw $t1 , -20($fp)

sw $t1 -24($fp)
sw $t0 -28($fp)
sw $fp -32($fp)
sw $ra -36($fp)
addi $fp , $fp , -36
jal mod
lw $ra , 0($fp)
lw $fp , 4($fp)
sw $v0 -24($fp)
lw $t0 , -20($fp)
lw $t1 , -24($fp)
sub $t2, $t0, $t1
sw $t2 -28($fp)
lw $t0 , -28($fp)
li $t1 , 10
div $t0 , $t1
mflo $t2
sw $t2 -20($fp)
lw $t0 , -12($fp)

lw $t1 , -24($fp)

sw $t1 -32($fp)
sw $t0 -36($fp)
sw $fp -40($fp)
sw $ra -44($fp)
addi $fp , $fp , -44
jal power
lw $ra , 0($fp)
lw $fp , 4($fp)
sw $v0 -32($fp)
lw $t0 , -16($fp)
lw $t1 , -32($fp)
add $t2, $t0, $t1
sw $t2 -16($fp)
j label8
 label10 :
lw $t0 , -4($fp)
sw $t0 , -36($fp)
lw $t0 , -16($fp)
lw $t1 , -36($fp)
beq $t0, $t1, label11
j label12
 label11 :
addi $sp, $sp, 2048
li $v0 , 1
jr $ra
j label13
 label12 :
addi $sp, $sp, 2048
li $v0 , 0
jr $ra
 label13 :

printHexDigit :
addi $sp, $sp, -2048

lw $t1 8($fp)
sw $t1 -4($fp)
lw $t0 , -4($fp)
sw $t0 , -8($fp)
lw $t0 , -8($fp)
li $t1 , 10
blt $t0, $t1, label14
j label15
 label14 :
lw $t0 , -4($fp)
sw $t0 , -12($fp)
lw $a0 ,  -12($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

j label16
 label15 :
lw $t0 , -4($fp)
sw $t0 , -16($fp)
li $t0 , 0
lw $t1 , -16($fp)
sub $t2, $t0, $t1
sw $t2 -24($fp)
lw $a0 ,  -24($fp)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

 label16 :
addi $sp, $sp, 2048
li $v0 , 0
jr $ra

printHex :
addi $sp, $sp, -2048

lw $t1 8($fp)
sw $t1 -4($fp)
li $t0 -8
sw $t0 -8($fp)
lw $t0 -8($fp)
addi $t1, $t0, -4
sw $t1 -28($fp)
lw $t0 -8($fp)
addi $t1, $t0, -4
sw $t1 -32($fp)
li $t0, 0
sw $t0 , -36($fp)
 label17 :
lw $t0 , -36($fp)
li $t1 , 4
blt $t0, $t1, label18
j label19
 label18 :
lw $t0 , -36($fp)
sw $t0 , -40($fp)
lw $t0 , -40($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -40($fp)
lw $t0 , -28($fp)
lw $t1 , -40($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -44($fp)
lw $t0 , -4($fp)
sw $t0 , -48($fp)
li $t0 16, 
lw $t1 , -48($fp)

sw $t1 -52($fp)
sw $t0 -56($fp)
sw $fp -60($fp)
sw $ra -64($fp)
addi $fp , $fp , -64
jal mod
lw $ra , 0($fp)
lw $fp , 4($fp)
sw $v0 -52($fp)
lw $t0 -52($fp)
lw $t1 -44($fp)
add $t1, $t1, $fp
sw $t0 0($t1)
lw $t0 , -4($fp)
sw $t0 , -56($fp)
lw $t0 , -56($fp)
li $t1 , 16
div $t0 , $t1
mflo $t2
sw $t2 -4($fp)
lw $t0 , -36($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -36($fp)
j label17
 label19 :
li $t0, 3
sw $t0 , -36($fp)
 label20 :
lw $t0 , -36($fp)
li $t1 , 0
bge $t0, $t1, label21
j label22
 label21 :
lw $t0 , -36($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -60($fp)
lw $t0 , -32($fp)
lw $t1 , -60($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -64($fp)
lw $t0 -64($fp)
add $t0, $t0, $fp
lw $t1 0($t0)
sw $t1 , -68($fp)
lw $t0 , -68($fp)

sw $t0 -72($fp)
sw $fp -76($fp)
sw $ra -80($fp)
addi $fp , $fp , -80
jal printHexDigit
lw $ra , 0($fp)
lw $fp , 4($fp)

lw $t0 , -36($fp)
li $t1 , 1
sub $t2, $t0, $t1
sw $t2 -36($fp)
j label20
 label22 :
addi $sp, $sp, 2048
li $v0 , 0
jr $ra

main :

move $fp $sp
addi $sp, $sp, -2048

li $t0, 0
sw $t0 , -4($fp)
li $t0, 9400
sw $t0 , -8($fp)
 label23 :
lw $t0 , -8($fp)
li $t1 , 9500
blt $t0, $t1, label24
j label25
 label24 :
lw $t0 , -8($fp)

sw $t0 -12($fp)
sw $fp -16($fp)
sw $ra -20($fp)
addi $fp , $fp , -20
jal isNarcissistic
lw $ra , 0($fp)
lw $fp , 4($fp)
sw $v0 -12($fp)
lw $t0 , -12($fp)
li $t1 , 1
beq $t0, $t1, label26
j label27
 label26 :
lw $t0 , -8($fp)

sw $t0 -16($fp)
sw $fp -20($fp)
sw $ra -24($fp)
addi $fp , $fp , -24
jal printHex
lw $ra , 0($fp)
lw $fp , 4($fp)

lw $t0 , -4($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -4($fp)
 label27 :
lw $t0 , -8($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -8($fp)
j label23
 label25 :
addi $sp, $sp, 2048
lw $v0 -4($fp)
jr $ra
