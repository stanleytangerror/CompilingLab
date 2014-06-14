.data
_ret: .asciiz "\n"
.globl main
.text

sort :
addi $sp, $sp, -2048

lw $t1 8($fp)
sw $t1 -4($fp)
li $t0 -8
sw $t0 -8($fp)
lw $t0 -8($fp)
addi $t1, $t0, -4
sw $t1 -412($fp)
lw $t0 -8($fp)
addi $t1, $t0, -4
sw $t1 -416($fp)
li $t0, 0
sw $t0 , -420($fp)
li $t0, 0
sw $t0 , -424($fp)
 label0 :
lw $t0 , -4($fp)
sw $t0 , -428($fp)
lw $t0 , -420($fp)
lw $t1 , -428($fp)
blt $t0, $t1, label1
j label2
 label1 :
lw $t0 , -420($fp)
sw $t0 , -432($fp)
lw $t0 , -432($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -432($fp)
lw $t0 , -412($fp)
lw $t1 , -432($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -436($fp)
lw $t0 , -420($fp)
li $t1 , 2
mul $t2, $t0, $t1
sw $t2 -440($fp)
li $t0 , 100
lw $t1 , -440($fp)
sub $t2, $t0, $t1
sw $t2 -448($fp)
lw $t0 -448($fp)
lw $t1 -436($fp)
add $t1, $t1, $fp
sw $t0 0($t1)
lw $t0 , -420($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -420($fp)
j label0
 label2 :
li $t0, 0
sw $t0 , -420($fp)
 label3 :
lw $t0 , -4($fp)
sw $t0 , -452($fp)
lw $t0 , -420($fp)
lw $t1 , -452($fp)
blt $t0, $t1, label4
j label5
 label4 :
li $t0, 0
sw $t0 , -424($fp)
 label6 :
lw $t0 , -424($fp)
lw $t1 , -420($fp)
blt $t0, $t1, label7
j label8
 label7 :
lw $t0 , -420($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -456($fp)
lw $t0 , -416($fp)
lw $t1 , -456($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -460($fp)
lw $t0 -460($fp)
add $t0, $t0, $fp
lw $t1 0($t0)
sw $t1 , -464($fp)
lw $t0 , -424($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -468($fp)
lw $t0 , -416($fp)
lw $t1 , -468($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -472($fp)
lw $t0 -472($fp)
add $t0, $t0, $fp
lw $t1 0($t0)
sw $t1 , -476($fp)
lw $t0 , -464($fp)
lw $t1 , -476($fp)
blt $t0, $t1, label9
j label10
 label9 :
lw $t0 , -420($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -480($fp)
lw $t0 , -416($fp)
lw $t1 , -480($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -484($fp)
lw $t0 -484($fp)
add $t0, $t0, $fp
lw $t1 0($t0)
sw $t1 , -488($fp)

lw $t0 , -420($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -492($fp)
lw $t0 , -416($fp)
lw $t1 , -492($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -496($fp)
lw $t0 , -424($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -500($fp)
lw $t0 , -416($fp)
lw $t1 , -500($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -504($fp)
lw $t0 -504($fp)
add $t0, $t0, $fp
lw $t1 0($t0)
sw $t1 , -508($fp)
lw $t0 -508($fp)
lw $t1 -496($fp)
add $t1, $t1, $fp
sw $t0 0($t1)
lw $t0 , -424($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -512($fp)
lw $t0 , -416($fp)
lw $t1 , -512($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -516($fp)
lw $t0 -520($fp)
lw $t1 -516($fp)
add $t1, $t1, $fp
sw $t0 0($t1)
 label10 :
lw $t0 , -424($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -424($fp)
j label6
 label8 :
lw $t0 , -420($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -420($fp)
j label3
 label5 :
li $t0, 0
sw $t0 , -420($fp)
 label11 :
lw $t0 , -4($fp)
sw $t0 , -524($fp)
lw $t0 , -420($fp)
lw $t1 , -524($fp)
blt $t0, $t1, label12
j label13
 label12 :
lw $t0 , -420($fp)
li $t1 , 4
mul $t2, $t0, $t1
sw $t2 -528($fp)
lw $t0 , -416($fp)
lw $t1 , -528($fp)
li $t3 -1
mul $t1 $t1 $t3
add $t2, $t0, $t1
sw $t2 -532($fp)
lw $t0 , -532($fp)
add $t0, $t0, $fp
lw $a0 , 0($t0)
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

lw $t0 , -420($fp)
li $t1 , 1
add $t2, $t0, $t1
sw $t2 -420($fp)
j label11
 label13 :
addi $sp, $sp, 2048
li $v0 , 0
jr $ra

main :

move $fp $sp
addi $sp, $sp, -2048

li $v0 5
syscall
sw $v0, -4($fp)
lw $t0 , -4($fp)
li $t1 , 100
bge $t0, $t1, label14
j label15
 label14 :
li $a0 , -1
li $v0 1
syscall
li $v0 4
la $a0 _ret
syscall

j label16
 label15 :
lw $t0 , -4($fp)

sw $t0 -8($fp)
sw $fp -12($fp)
sw $ra -16($fp)
addi $fp , $fp , -16
jal sort
lw $ra , 0($fp)
lw $fp , 4($fp)

 label16 :
addi $sp, $sp, 2048
li $v0 , 0
jr $ra
